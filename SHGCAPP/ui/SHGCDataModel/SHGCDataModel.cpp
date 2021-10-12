#include "SHGCDataModel.h"
#include "QGCApplication.h"
#include "qmlobjecttablemodel.h"
#include "QmlObjectListModel.h"


SHGCDataModel::SHGCDataModel(QGCApplication* app, QGCToolbox* toolbox)
    : QGCTool(app, toolbox)
{

}

SHGCDataModel::~SHGCDataModel()
{

}


void SHGCDataModel::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);

    _uploadtabelmodel             = new QmlObjectTableModel(this);
}
