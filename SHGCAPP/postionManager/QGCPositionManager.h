#ifndef QGCPOSITIONMANAGER_H
#define QGCPOSITIONMANAGER_H

#include <QGeoPositionInfoSource>
#include <QNmeaPositionInfoSource>

#include <QVariant>
#include "QGCToolbox.h"
#include "SimulatedPosition.h"


class QGCPositionManager : public QGCTool
{
    Q_OBJECT

public:
    QGCPositionManager(QGCApplication* app, QGCToolbox* toolbox);
    ~QGCPositionManager();

    enum QGCPositionSource {
        Simulated,
        InternalGPS,
        Log,
        NmeaGPS
    };

    QGeoCoordinate gcsPosition(void) { return _gcsPosition; }

    void setToolbox(QGCToolbox* toolbox);

    void setPositionSource(QGCPositionSource source);

    void setNmeaSourceDevice(QIODevice* device);

    int updateInterval() const;

private slots:
    void _positionUpdated(const QGeoPositionInfo &update);
    void _error(QGeoPositionInfoSource::Error positioningError);

signals:
    void gcsPositionChanged(QGeoCoordinate gcsPosition);
    void positionInfoUpdated(QGeoPositionInfo update);


private:
    int             _updateInterval;
    QGeoCoordinate  _gcsPosition;

    QGeoPositionInfoSource*     _currentSource;
    QGeoPositionInfoSource*     _defaultSource;
    QNmeaPositionInfoSource*    _nmeaSource;
    QGeoPositionInfoSource*     _simulatedSource;

};

#endif // QGCPOSITIONMANAGER_H
