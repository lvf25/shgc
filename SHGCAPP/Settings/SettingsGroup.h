#ifndef SETTINGSGROUP_H
#define SETTINGSGROUP_H

#include "QGCLoggingCategory.h"
#include <QVariantList>
#include "QGCToolbox.h"



#include "FactSystem/SettingsFact.h"
class FactMetaData;

#define DEFINE_SETTING_NAME_GROUP() \
    static const char* name; \
    static const char* settingsGroup;

#define DECLARE_SETTINGGROUP(NAME, GROUP) \
    const char* NAME ## Settings::name = #NAME; \
    const char* NAME ## Settings::settingsGroup = GROUP; \
    NAME ## Settings::NAME ## Settings(QObject* parent) \
        : SettingsGroup(name, settingsGroup, parent)

#define DECLARE_SETTINGSFACT(CLASS, NAME) \
    const char* CLASS::NAME ## Name = #NAME; \
    Fact* CLASS::NAME() \
    { \
        if (!_ ## NAME ## Fact) { \
            _ ## NAME ## Fact = _createSettingsFact(NAME ## Name); \
        } \
        return _ ## NAME ## Fact; \
    }


#define DECLARE_SETTINGSFACT_NO_FUNC(CLASS, NAME) \
    const char* CLASS::NAME ## Name = #NAME; \
    Fact* CLASS::NAME()


#define DEFINE_SETTINGFACT(NAME) \
    private: \
    SettingsFact* _ ## NAME ## Fact = nullptr; \
    public: \
    Q_PROPERTY(Fact* NAME READ NAME CONSTANT) \
    Fact* NAME(); \
    static const char* NAME ## Name;



class SettingsGroup : public QObject
{

    Q_OBJECT
public:
    SettingsGroup(const QString &name, const QString &settingsGroup, QObject* parent = nullptr);

    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)

    virtual bool    visible             () { return _visible; }
    virtual void    setVisible          (bool vis) { _visible = vis; emit visibleChanged(); }

signals:
    void            visibleChanged      ();

protected:
    SettingsFact*   _createSettingsFact(const QString& factName);
    bool            _visible;
    QString         _name;
    QString         _settingsGroup;

    QMap<QString, FactMetaData*> _nameToMetaDataMap;
};

#endif // SETTINGSGROUP_H
