/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#ifndef APMAirframeComponent_H
#define APMAirframeComponent_H

#include "VehicleSetup/VehicleComponent.h"

class APMAirframeComponent : public VehicleComponent
{
    Q_OBJECT
    
public:
    APMAirframeComponent(Vehicle* vehicle, AutoPilotPlugin* autopilot, QObject* parent = NULL);
    
    // Virtuals from VehicleComponent
    QStringList setupCompleteChangedTriggerList(void) const override;

    // Virtuals from VehicleComponent
    QString name(void) const override;
    QString description(void) const override;
    QString iconResource(void) const override;
    bool requiresSetup(void) const override;
    bool setupComplete(void) const override;
    QUrl setupSource(void) const override;
    QUrl summaryQmlSource(void) const override;

private:
    bool            _requiresFrameSetup; ///< true: FRAME parameter must be set
    const QString   _name;
    Fact*           _frameParamFact;
    bool            _useNewFrameParam;

    static const char* _oldFrameParam;
    static const char* _newFrameParam;
};

#endif
