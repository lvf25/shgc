/****************************************************************************
 *
 *   (c) 2009-2018 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#ifndef APMMotorComponent_H
#define APMMotorComponent_H

#include "AutoPilotPlugins/Common/MotorComponent.h"

class APMMotorComponent : public MotorComponent
{
    Q_OBJECT

public:
    APMMotorComponent(Vehicle* vehicle, AutoPilotPlugin* autopilot, QObject* parent = NULL);

    QUrl setupSource(void) const final;

private:
    const QString   _name;
};

#endif
