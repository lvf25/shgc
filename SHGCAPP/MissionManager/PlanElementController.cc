/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "PlanElementController.h"
#include "PlanMasterController.h"
#include "QGCApplication.h"
#include "Vehicle/MultiVehicleManager.h"
#include "Settings/SettingsManager.h"
#include "Settings/AppSettings.h"

PlanElementController::PlanElementController(PlanMasterController* masterController, QObject* parent)
    : QObject           (parent)
    , _masterController (masterController)
    , _controllerVehicle(masterController->controllerVehicle())
    , _managerVehicle   (masterController->managerVehicle())
    , _flyView          (false)
{

}

PlanElementController::~PlanElementController()
{

}

void PlanElementController::start(bool flyView)
{
    _flyView = flyView;
}

void PlanElementController::managerVehicleChanged(Vehicle* managerVehicle)
{
    _managerVehicle = managerVehicle;
}
