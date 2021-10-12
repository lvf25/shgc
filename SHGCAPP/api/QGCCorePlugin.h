#ifndef QGCCOREPLUGIN_H
#define QGCCOREPLUGIN_H
#include "QGCToolbox.h"
#include "comm/QGCMAVLink.h"

#include <QObject>
#include <QVariantList>

class QGCApplication;
class QGCOptions;
class QGCSettings;
class QGCCorePlugin_p;
class FactMetaData;
class QGeoPositionInfoSource;
class QmlObjectListModel;
class Vehicle;
class LinkInterface;
class PlanMasterController;

class QmlObjectTableModel;


class QGCCorePlugin : public QGCTool
{
    Q_OBJECT
public:
    QGCCorePlugin(QGCApplication* app, QGCToolbox* toolbox);
    ~QGCCorePlugin();

    bool showTouchAreas(void) const { return _showTouchAreas; }
    bool showAdvancedUI(void) const { return _showAdvancedUI; }

    QmlObjectTableModel*        shgcTableModel          () {return _shgcTableModel; }


    /// Allows the core plugin to override the visibility for a settings group
    ///     @param name - SettingsGroup name
    /// @return true: Show settings ui, false: Hide settings ui
    virtual bool overrideSettingsGroupVisibility(QString name);

    // Override from QGCTool
    void                            setToolbox              (QGCToolbox* toolbox);


    /// Allows the core plugin to override the setting meta data before the setting fact is created.
    ///     @param settingsGroup - QSettings group which contains this item
    ///     @param metaData - MetaData for setting fact
    /// @return true: Setting should be visible in ui, false: Setting should not be shown in ui
    virtual bool adjustSettingMetaData(const QString& settingsGroup, FactMetaData& metaData);

    virtual QGeoPositionInfoSource* createPositionSource(QObject* parent) { Q_UNUSED(parent); return nullptr; }

    /// Allows the plugin to see all mavlink traffic to a vehicle
    /// @return true: Allow vehicle to continue processing, false: Vehicle should not process message

    virtual bool mavlinkMessage(Vehicle* vehicle, LinkInterface* link, mavlink_message_t message);

    /// @return An instance of QGCOptions
    virtual QGCOptions* options(void);


    /// Allows custom builds to load custom items from the plan file. Either before the document is
    /// parsed or after.
    virtual void    preLoadFromJson     (PlanMasterController* pController, QJsonObject& json) { Q_UNUSED(pController); Q_UNUSED(json); }
    virtual void    postLoadFromJson    (PlanMasterController* pController, QJsonObject& json) { Q_UNUSED(pController); Q_UNUSED(json); }


    /// Allows custom builds to add custom items to the plan file. Either before the document is
    /// created or after.
    virtual void    preSaveToJson           (PlanMasterController* pController, QJsonObject& json) { Q_UNUSED(pController); Q_UNUSED(json); }
    virtual void    postSaveToJson          (PlanMasterController* pController, QJsonObject& json) { Q_UNUSED(pController); Q_UNUSED(json); }

    /// Same for the specific "mission" portion
    virtual void    preSaveToMissionJson    (PlanMasterController* pController, QJsonObject& missionJson) { Q_UNUSED(pController); Q_UNUSED(missionJson); }
    virtual void    postSaveToMissionJson   (PlanMasterController* pController, QJsonObject& missionJson) { Q_UNUSED(pController); Q_UNUSED(missionJson); }


protected:
    bool                _showTouchAreas;
    bool                _showAdvancedUI;

private:
    QGCCorePlugin_p*    _p;

    QmlObjectTableModel* _shgcTableModel;


};
#endif // QGCCOREPLUGIN_H

