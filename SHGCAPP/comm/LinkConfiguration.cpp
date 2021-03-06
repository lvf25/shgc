#include "LinkConfiguration.h"


#ifndef NO_SERIAL_LINK
#include "SerialLink.h"
#endif
#include "UDPLink.h"
#include "TCPLink.h"
#if !defined(__mobile__)
#include "LogReplayLink.h"
#endif

#define LINK_SETTING_ROOT "LinkConfigurations"

LinkConfiguration::LinkConfiguration(const QString& name)
    : _link(nullptr)
    , _name(name)
    , _dynamic(false)
    , _autoConnect(false)
    , _highLatency(false)
{
    _name = name;
    if (_name.isEmpty()) {
        qWarning() << "Internal error";
    }
}

LinkConfiguration::LinkConfiguration(LinkConfiguration* copy)
{
    _link       = copy->link();
    _name       = copy->name();
    _dynamic    = copy->isDynamic();
    _autoConnect= copy->isAutoConnect();
    _highLatency= copy->isHighLatency();
    Q_ASSERT(!_name.isEmpty());
}

void LinkConfiguration::copyFrom(LinkConfiguration* source)
{
    Q_ASSERT(source != NULL);
    _link       = source->link();
    _name       = source->name();
    _dynamic    = source->isDynamic();
    _autoConnect= source->isAutoConnect();
    _highLatency= source->isHighLatency();
}

/*!
  Where the settings are saved
  @return The root path of the setting.
*/
const QString LinkConfiguration::settingsRoot()
{
    return QString(LINK_SETTING_ROOT);
}

/*!
  Configuration Factory
  @return A new instance of the given type
*/
LinkConfiguration* LinkConfiguration::createSettings(int type, const QString& name)
{
    LinkConfiguration* config = nullptr;
    switch(type) {
#ifndef NO_SERIAL_LINK
        case LinkConfiguration::TypeSerial:
            config = new SerialConfiguration(name);
            break;
#endif
        case LinkConfiguration::TypeUdp:
            config = new UDPConfiguration(name);
            break;
        case LinkConfiguration::TypeTcp:
            config = new TCPConfiguration(name);
            break;
#ifdef QGC_ENABLE_BLUETOOTH
    case LinkConfiguration::TypeBluetooth:
//        config = new BluetoothConfiguration(name);
        break;
#endif
#ifndef __mobile__
        case LinkConfiguration::TypeLogReplay:
            config = new LogReplayLinkConfiguration(name);
            break;
#endif
#ifdef QT_DEBUG
        case LinkConfiguration::TypeMock:
//            config = new MockConfiguration(name);
            break;
#endif
    }
    return config;
}

/*!
  Duplicate link settings
  @return A new copy of the given settings instance
*/
LinkConfiguration* LinkConfiguration::duplicateSettings(LinkConfiguration* source)
{
    LinkConfiguration* dupe = nullptr;
    switch(source->type()) {
#ifndef NO_SERIAL_LINK
        case TypeSerial:
            dupe = new SerialConfiguration(dynamic_cast<SerialConfiguration*>(source));
            break;
#endif
        case TypeUdp:
            dupe = new UDPConfiguration(dynamic_cast<UDPConfiguration*>(source));
            break;
        case TypeTcp:
            dupe = new TCPConfiguration(dynamic_cast<TCPConfiguration*>(source));
            break;
#ifdef QGC_ENABLE_BLUETOOTH
        case TypeBluetooth:
//            dupe = new BluetoothConfiguration(dynamic_cast<BluetoothConfiguration*>(source));
            break;
#endif
#ifndef __mobile__
        case TypeLogReplay:
            dupe = new LogReplayLinkConfiguration(dynamic_cast<LogReplayLinkConfiguration*>(source));
            break;
#endif
#ifdef QT_DEBUG
        case TypeMock:
//            dupe = new MockConfiguration(dynamic_cast<MockConfiguration*>(source));
            break;
#endif
        case TypeLast:
            break;
    }
    return dupe;
}

void LinkConfiguration::setName(const QString name)
{
    _name = name;
    emit nameChanged(name);
}

void LinkConfiguration::setLink(LinkInterface* link)
{
    if(_link != link) {
        _link = link;
        emit linkChanged(link);
    }
}
