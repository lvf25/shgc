/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#ifndef APMTuningComponent_H
#define APMTuningComponent_H

#include "VehicleSetup/VehicleComponent.h"

class APMTuningComponent : public VehicleComponent
{
    Q_OBJECT
    
public:
    APMTuningComponent(Vehicle* vehicle, AutoPilotPlugin* autopilot, QObject* parent = NULL);
    
    // Virtuals from VehicleComponent
    QStringList setupCompleteChangedTriggerList(void) const final;
    
    // Virtuals from VehicleComponent
    QString name(void) const final;
    QString description(void) const final;
    QString iconResource(void) const final;
    bool requiresSetup(void) const final;
    bool setupComplete(void) const final;
    QUrl setupSource(void) const final;
    QUrl summaryQmlSource(void) const final;
    bool allowSetupWhileArmed(void) const final { return true; }

private:
    const QString   _name;
    QVariantList    _summaryItems;
};

#endif
