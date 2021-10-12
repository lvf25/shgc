#include "SHGCMountManager.h"


#include "UDPLink.h"
#include "TCPLink.h"

#include "QGCApplication.h"
#ifndef NO_SERIAL_LINK
#include "QGCSerialPortInfo.h"
#endif

#include "FlowVelocimeterLink.h"


SHGCMountFv_WLGManager::SHGCMountFv_WLGManager(QGCApplication* app, QGCToolbox* toolbox)
    :QGCTool(app, toolbox)
{

}

void SHGCMountFv_WLGManager::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);
}

LinkInterface* SHGCMountFv_WLGManager::createConnectedLink(SharedLinkConfigurationPointer& config, bool isPX4Flow)
{
    if (!config) {
        qWarning() << "LinkManager::createConnectedLink called with nullptr config";
        return nullptr;
    }

    LinkInterface* pLink = nullptr;
    switch(config->type()) {
#ifndef NO_SERIAL_LINK
    case LinkConfiguration::TypeSerial:
    {
        SerialConfiguration* serialConfig = dynamic_cast<SerialConfiguration*>(config.data());
        if (serialConfig) {
            pLink = new SerialLink(config, isPX4Flow);
            if (serialConfig->usbDirect()) {
                _activeLinkCheckList.append(dynamic_cast<SerialLink*>(pLink));
                if (!_activeLinkCheckTimer.isActive()) {
                    _activeLinkCheckTimer.start();
                }
            }
        }
    }
        break;
#else
    Q_UNUSED(isPX4Flow)
#endif
    case LinkConfiguration::TypeUdp:
        pLink = new UDPLink(config);
        break;
    case LinkConfiguration::TypeTcp:
        pLink = new TCPLink(config);
        break;
#ifdef QGC_ENABLE_BLUETOOTH
    case LinkConfiguration::TypeBluetooth:
        //        pLink = new BluetoothLink(config);
        break;
#endif

    case LinkConfiguration::TypeLast:
        break;
    }
    if (pLink) {
        _addLink(pLink);
        connectLink(pLink);
    }
    return pLink;
}


void SHGCMountFv_WLGManager::_addLink(LinkInterface* link)
{
    if (thread() != QThread::currentThread()) {
        qWarning() << "_addLink called from incorrect thread";
        return;
    }

    if (!link) {
        return;
    }

//    if (!containsLink(link)) {
//        _sharedLinks.append(SharedLinkInterfacePointer(link));
//        emit newLink(link);
//    }
//    connect(link, &LinkInterface::bytesReceived,        _mavlinkProtocol,   &MAVLinkProtocol::receiveBytes);

}


//bool SHGCMountFv_WLGManager::containsLink(LinkInterface* link)
//{
//    for (int i=0; i<_sharedLinks.count(); i++) {
//        if (_sharedLinks[i].data() == link) {
//            return true;
//        }
//    }
//    return false;
//}


bool SHGCMountFv_WLGManager::connectLink(LinkInterface* link)
{
    if (link) {
        return link->_connect();
    } else {
        qWarning() << "Internal error";
        return false;
    }
}

