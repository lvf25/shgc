#ifndef APMParameterMetaData_H
#define APMParameterMetaData_H

#include <QObject>
#include <QMap>
#include <QPointer>
#include <QXmlStreamReader>
#include <QLoggingCategory>

#include "FactSystem/FactSystem.h"
#include "AutoPilotPlugins/AutoPilotPlugin.h"
#include "Vehicle/Vehicle.h"

Q_DECLARE_LOGGING_CATEGORY(APMParameterMetaDataLog)
Q_DECLARE_LOGGING_CATEGORY(APMParameterMetaDataVerboseLog)

class APMFactMetaDataRaw
{
public:
    APMFactMetaDataRaw(void)
        : rebootRequired(false)
    { }

    QString name;
    QString category;
    QString group;
    QString shortDescription;
    QString longDescription;
    QString min;
    QString max;
    QString incrementSize;
    QString units;
    bool    rebootRequired;
    QList<QPair<QString, QString> > values;
    QList<QPair<QString, QString> > bitmask;
};


/// Collection of Parameter Facts for PX4 AutoPilot

typedef QMap<QString, APMFactMetaDataRaw*> ParameterNametoFactMetaDataMap;

class APMParameterMetaData : public QObject
{
    Q_OBJECT

public:
    APMParameterMetaData(void);

    void addMetaDataToFact(Fact* fact, MAV_TYPE vehicleType);
    void loadParameterFactMetaDataFile(const QString& metaDataFile);

    static void getParameterMetaDataVersionInfo(const QString& metaDataFile, int& majorVersion, int& minorVersion);

private:
    enum {
        XmlStateNone,
        XmlstateParamFileFound,
        XmlStateFoundVehicles,
        XmlStateFoundLibraries,
        XmlStateFoundParameters,
        XmlStateFoundVersion,
        XmlStateFoundGroup,
        XmlStateFoundParameter,
        XmlStateDone
    };

    QVariant _stringToTypedVariant(const QString& string, FactMetaData::ValueType_t type, bool* convertOk);
    bool skipXMLBlock(QXmlStreamReader& xml, const QString& blockName);
    bool parseParameterAttributes(QXmlStreamReader& xml, APMFactMetaDataRaw *rawMetaData);
    void correctGroupMemberships(ParameterNametoFactMetaDataMap& parameterToFactMetaDataMap, QMap<QString,QStringList>& groupMembers);
    QString mavTypeToString(MAV_TYPE vehicleTypeEnum);

    bool _parameterMetaDataLoaded;   ///< true: parameter meta data already loaded
    QMap<QString, ParameterNametoFactMetaDataMap> _vehicleTypeToParametersMap; ///< Maps from a vehicle type to paramametertoFactMeta map>
};

#endif
