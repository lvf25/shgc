#ifndef SHGCDATAMODEL_H
#define SHGCDATAMODEL_H


#include <QGCToolbox.h>


class QmlObjectTableModel;
class QmlObjectListModel;

class SHGCDataModel : public QGCTool
{
    Q_OBJECT
public:
    SHGCDataModel(QGCApplication* app, QGCToolbox* toolbox);
    ~SHGCDataModel();

    // Override from QGCTool
    virtual void setToolbox(QGCToolbox *toolbox);

    QmlObjectTableModel*        uploadtabelmodel              (){return _uploadtabelmodel; }



private:
    QmlObjectTableModel*        _uploadtabelmodel             = nullptr;


};

#endif // SHGCDATAMODEL_H
