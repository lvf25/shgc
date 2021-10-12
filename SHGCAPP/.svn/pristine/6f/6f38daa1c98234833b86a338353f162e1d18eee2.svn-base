#ifndef SETTINGSFACT_H
#define SETTINGSFACT_H
#include "Fact.h"


class SettingsFact : public Fact
{
    Q_OBJECT
public:
    SettingsFact(QObject* parent = nullptr);
    SettingsFact(QString settingsGroup, FactMetaData* metaData, QObject* parent = nullptr);
    SettingsFact(const SettingsFact& other, QObject* parent = nullptr);

    const SettingsFact& operator=(const SettingsFact& other);

    Q_PROPERTY(bool visible MEMBER _visible CONSTANT)

    // Must be called before any references to fact
    void setVisible(bool visible) { _visible = visible; }
private:
    QString _settingsGroup;
    bool    _visible;

private slots:
    void _rawValueChanged(QVariant value);

};

#endif // SETTINGSFACT_H
