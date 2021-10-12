#include "FactSystem.h"
#include "FactGroup.h"
#include "FactPanelController.h"
#include <QQmlEngine>


const char* FactSystem::_factSystemQmlUri = "QGroundControl.FactSystem";

FactSystem::FactSystem(QGCApplication* app, QGCToolbox* toolbox)
    : QGCTool(app, toolbox)
{

}

void FactSystem::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);

    qmlRegisterType<Fact>               (_factSystemQmlUri, 1, 0, "Fact");
    qmlRegisterType<FactMetaData>       (_factSystemQmlUri, 1, 0, "FactMetaData");
    qmlRegisterType<FactPanelController>(_factSystemQmlUri, 1, 0, "FactPanelController");

    qmlRegisterUncreatableType<FactGroup>(_factSystemQmlUri, 1, 0, "FactGroup", "ReferenceOnly");
}
