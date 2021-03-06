/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "GeoFenceManager.h"
#include "Vehicle/Vehicle.h"
#include "ui/QmlObjectListModel.h"
#include "FactSystem/ParameterManager.h"
#include "QGCApplication.h"
#include "QGCMapPolygon.h"
#include "QGCMapCircle.h"

QGC_LOGGING_CATEGORY(GeoFenceManagerLog, "GeoFenceManagerLog")

GeoFenceManager::GeoFenceManager(Vehicle* vehicle)
    : _vehicle                  (vehicle)
    , _planManager              (vehicle, MAV_MISSION_TYPE_FENCE)
    , _firstParamLoadComplete   (false)
#if defined(QGC_AIRMAP_ENABLED)
    , _airspaceManager            (qgcApp()->toolbox()->airspaceManager())
#endif
{
    connect(&_planManager, &PlanManager::inProgressChanged,         this, &GeoFenceManager::inProgressChanged);
    connect(&_planManager, &PlanManager::error,                     this, &GeoFenceManager::error);
    connect(&_planManager, &PlanManager::removeAllComplete,         this, &GeoFenceManager::removeAllComplete);
    connect(&_planManager, &PlanManager::sendComplete,              this, &GeoFenceManager::_sendComplete);
    connect(&_planManager, &PlanManager::newMissionItemsAvailable,  this, &GeoFenceManager::_planManagerLoadComplete);
}

GeoFenceManager::~GeoFenceManager()
{

}

bool GeoFenceManager::inProgress(void) const
{
    return _planManager.inProgress();
}

void GeoFenceManager::loadFromVehicle(void)
{
    _planManager.loadFromVehicle();
}

void GeoFenceManager::sendToVehicle(const QGeoCoordinate&   breachReturn,
                                    QmlObjectListModel&     polygons,
                                    QmlObjectListModel&     circles)
{
    Q_UNUSED(breachReturn);

    QList<MissionItem*> fenceItems;

    _sendPolygons.clear();
    _sendCircles.clear();

    for (int i=0; i<polygons.count(); i++) {
        _sendPolygons.append(*polygons.value<QGCFencePolygon*>(i));
    }
    for (int i=0; i<circles.count(); i++) {
        _sendCircles.append(*circles.value<QGCFenceCircle*>(i));
    }

    for (int i=0; i<_sendPolygons.count(); i++) {
        const QGCFencePolygon& polygon = _sendPolygons[i];

        for (int j=0; j<polygon.count(); j++) {
            const QGeoCoordinate& vertex = polygon.path()[j].value<QGeoCoordinate>();

            MissionItem* item = new MissionItem(0,
                                                polygon.inclusion() ? MAV_CMD_NAV_FENCE_POLYGON_VERTEX_INCLUSION : MAV_CMD_NAV_FENCE_POLYGON_VERTEX_EXCLUSION,
                                                MAV_FRAME_GLOBAL,
                                                polygon.count(),    // vertex count
                                                0, 0, 0,            // param 2-4 unused
                                                vertex.latitude(),
                                                vertex.longitude(),
                                                0,                  // param 7 unused
                                                false,              // autocontinue
                                                false,              // isCurrentItem
                                                this);              // parent
            fenceItems.append(item);
        }
    }

    for (int i=0; i<_sendCircles.count(); i++) {
        QGCFenceCircle& circle = _sendCircles[i];

        MissionItem* item = new MissionItem(0,
                                            circle.inclusion() ? MAV_CMD_NAV_FENCE_CIRCLE_INCLUSION : MAV_CMD_NAV_FENCE_CIRCLE_EXCLUSION,
                                            MAV_FRAME_GLOBAL,
                                            circle.radius()->rawValue().toDouble(),
                                            0, 0, 0,                    // param 2-4 unused
                                            circle.center().latitude(),
                                            circle.center().longitude(),
                                            0,                          // param 7 unused
                                            false,                      // autocontinue
                                            false,                      // isCurrentItem
                                            this);                      // parent
        fenceItems.append(item);
    }

    // Plan manager takes control of MissionItems, so no need to delete
    _planManager.writeMissionItems(fenceItems);
}

void GeoFenceManager::removeAll(void)
{
    _planManager.removeAll();
}

void GeoFenceManager::_sendComplete(bool error)
{
    if (error) {
        _polygons.clear();
        _circles.clear();
    } else {
        _polygons = _sendPolygons;
        _circles = _sendCircles;
    }
    _sendPolygons.clear();
    _sendCircles.clear();
    emit sendComplete(error);
}

void GeoFenceManager::_planManagerLoadComplete(bool removeAllRequested)
{
    bool loadFailed = false;

    Q_UNUSED(removeAllRequested);

    _polygons.clear();
    _circles.clear();

    MAV_CMD expectedCommand = (MAV_CMD)0;
    int expectedVertexCount = 0;
    QGCFencePolygon nextPolygon(true /* inclusion */);
    const QList<MissionItem*>& fenceItems = _planManager.missionItems();

    for (int i=0; i<fenceItems.count(); i++) {
        MissionItem* item = fenceItems[i];

        MAV_CMD command = item->command();

        if (command == MAV_CMD_NAV_FENCE_POLYGON_VERTEX_INCLUSION || command == MAV_CMD_NAV_FENCE_POLYGON_VERTEX_EXCLUSION) {
            if (nextPolygon.count() == 0) {
                // Starting a new polygon
                expectedVertexCount = item->param1();
                expectedCommand = command;
            } else if (expectedVertexCount != item->param1()){
                // In the middle of a polygon, but count suddenly changed
                emit error(BadPolygonItemFormat, tr("GeoFence load: Vertex count change mid-polygon - actual:expected").arg(item->param1()).arg(expectedVertexCount));
                break;
            } if (expectedCommand != command) {
                // Command changed before last polygon was completely loaded
                emit error(BadPolygonItemFormat, tr("GeoFence load: Polygon type changed before last load complete - actual:expected").arg(command).arg(expectedCommand));
                break;
            }
            nextPolygon.appendVertex(QGeoCoordinate(item->param5(), item->param6()));
            if (nextPolygon.count() == expectedVertexCount) {
                // Polygon is complete
                nextPolygon.setInclusion(command == MAV_CMD_NAV_FENCE_POLYGON_VERTEX_INCLUSION);
                _polygons.append(nextPolygon);
                nextPolygon.clear();
            }
        } else if (command == MAV_CMD_NAV_FENCE_CIRCLE_INCLUSION || command == MAV_CMD_NAV_FENCE_CIRCLE_EXCLUSION) {
            if (nextPolygon.count() != 0) {
                // Incomplete polygon
                emit error(IncompletePolygonLoad, tr("GeoFence load: Incomplete polygon loaded"));
                break;
            }
            QGCFenceCircle circle(QGeoCoordinate(item->param5(), item->param6()), item->param1(), command == MAV_CMD_NAV_FENCE_CIRCLE_INCLUSION /* inclusion */);
            _circles.append(circle);
        } else {
            emit error(UnsupportedCommand, tr("GeoFence load: Unsupported command %1").arg(item->command()));
            break;
        }
    }

    if (loadFailed) {
        _polygons.clear();
        _circles.clear();
    }

    emit loadComplete();
}

bool GeoFenceManager::supported(void) const
{
    return (_vehicle->capabilityBits() & MAV_PROTOCOL_CAPABILITY_MISSION_FENCE) && (_vehicle->maxProtoVersion() >= 200);
}
