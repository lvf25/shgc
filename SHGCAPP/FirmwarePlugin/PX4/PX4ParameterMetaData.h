#ifndef PX4PARAMETERMETADATA_H
#define PX4PARAMETERMETADATA_H

#include <QObject>
#include <QMap>
#include <QXmlStreamReader>
#include <QLoggingCategory>

#include "FactSystem.h"
#include "AutoPilotPlugin.h"
#include "Vehicle/Vehicle.h"

Q_DECLARE_LOGGING_CATEGORY(PX4ParameterMetaDataLog)


class PX4ParameterMetaData : public QObject
{
    Q_OBJECT

public:
    PX4ParameterMetaData(void);

    void            loadParameterFactMetaDataFile   (const QString& metaDataFile);
    FactMetaData*   getMetaDataForFact              (const QString& name, MAV_TYPE vehicleType);
    void            addMetaDataToFact               (Fact* fact, MAV_TYPE vehicleType);

    static void getParameterMetaDataVersionInfo(const QString& metaDataFile, int& majorVersion, int& minorVersion);

private:
    enum {
        XmlStateNone,
        XmlStateFoundParameters,
        XmlStateFoundVersion,
        XmlStateFoundGroup,
        XmlStateFoundParameter,
        XmlStateDone
    };


    QVariant _stringToTypedVariant(const QString& string, FactMetaData::ValueType_t type, bool* convertOk);

    bool _parameterMetaDataLoaded;   ///< true: parameter meta data already loaded
    QMap<QString, FactMetaData*> _mapParameterName2FactMetaData; ///< Maps from a parameter name to FactMetaData
};

#endif // PX4PARAMETERMETADATA_H
