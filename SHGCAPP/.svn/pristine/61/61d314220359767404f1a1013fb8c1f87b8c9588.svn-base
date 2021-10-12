#ifndef UnitsSettings_H
#define UnitsSettings_H

#include "SettingsGroup.h"

class UnitsSettings : public SettingsGroup
{
    Q_OBJECT

public:
    UnitsSettings(QObject* parent = nullptr);

    enum DistanceUnits {
        DistanceUnitsFeet = 0,
        DistanceUnitsMeters
    };

    enum AreaUnits {
        AreaUnitsSquareFeet = 0,
        AreaUnitsSquareMeters,
        AreaUnitsSquareKilometers,
        AreaUnitsHectares,
        AreaUnitsAcres,
        AreaUnitsSquareMiles,
    };

    enum SpeedUnits {
        SpeedUnitsFeetPerSecond = 0,
        SpeedUnitsMetersPerSecond,
        SpeedUnitsMilesPerHour,
        SpeedUnitsKilometersPerHour,
        SpeedUnitsKnots,
    };

    enum TemperatureUnits {
        TemperatureUnitsCelsius = 0,
        TemperatureUnitsFarenheit,
    };

    Q_ENUM(DistanceUnits)
    Q_ENUM(AreaUnits)
    Q_ENUM(SpeedUnits)
    Q_ENUM(TemperatureUnits)

    DEFINE_SETTING_NAME_GROUP()

    DEFINE_SETTINGFACT(distanceUnits)
    DEFINE_SETTINGFACT(areaUnits)
    DEFINE_SETTINGFACT(speedUnits)
    DEFINE_SETTINGFACT(temperatureUnits)
};

#endif
