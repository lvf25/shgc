#pragma once


#include "QGCToolbox.h"

#include "LinkInterface.h"
#include "LinkConfiguration.h"

#ifndef NO_SERIAL_LINK
    #include "SerialLink.h"
#endif


class SHGCMountFv_WLGManager : public QGCTool
{
    Q_OBJECT
public:
    SHGCMountFv_WLGManager(QGCApplication* app, QGCToolbox* toolbox);

    // Override from QGCTool
    virtual void setToolbox(QGCToolbox *toolbox);

    LinkInterface* createConnectedLink(SharedLinkConfigurationPointer& config, bool isPX4Flow = false);


private:
    void _addLink(LinkInterface* link);
    bool containsLink(LinkInterface* link);
    bool connectLink(LinkInterface* link);

#ifndef NO_SERIAL_LINK
    QTimer              _activeLinkCheckTimer;                  ///< Timer which checks for a vehicle showing up on a usb direct link
    QList<SerialLink*>  _activeLinkCheckList;                   ///< List of links we are waiting for a vehicle to show up on
    static const int    _activeLinkCheckTimeoutMSecs = 15000;   ///< Amount of time to wait for a heatbeat. Keep in mind ArduPilot stack heartbeat is slow to come.
#endif
};

