#include "SHGCUpLoadWidget.h"
#include "ui_SHGCUpLoadWidget.h"
#include "MissionController.h"
#include "PlanElementController.h"
#include "QGCCorePlugin.h"
#include "QGCApplication.h"
#include "SHGCDataModel.h"

static SHGCUpLoadWidget* _instance = nullptr;   ///< @brief MainWindow singleton

SHGCUpLoadWidget* SHGCUpLoadWidget::_create(QWidget *parent)
{
    new SHGCUpLoadWidget(parent);
    return _instance;
}

SHGCUpLoadWidget* SHGCUpLoadWidget::instance(void)
{
    return _instance;
}

void SHGCUpLoadWidget::deleteInstance(void)
{
    delete this;
}

SHGCUpLoadWidget::SHGCUpLoadWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SHGCUpLoadWidget)
{
    ui->setupUi(this);
    _instance = this;
    this->setStyleSheet("QWidget{background-color: rgba(244, 248, 249, 1);}");
    ui->reupload->hide();

    SHGCDataModel *datamodel = qgcApp()->toolbox()->Shgcdatamodel();
    uploadtablemodel = datamodel->uploadtabelmodel();
    ui->tableView->verticalHeader()->setVisible(false);

    connect(uploadtablemodel,&QmlObjectTableModel::updateCount,this,&SHGCUpLoadWidget::updateCount);

    ui->tableView->setModel(uploadtablemodel);
    ui->tableView->setColumnHidden(6,true);
    ui->tableView->setColumnHidden(7,true);
    ui->tableView->setColumnHidden(8,true);
    ui->tableView->setColumnHidden(9,true);


}

SHGCUpLoadWidget::~SHGCUpLoadWidget()
{
    _instance = nullptr;
    delete ui;
}

void SHGCUpLoadWidget::on_upload_clicked()
{
    emit uploadToUAV();
}


void SHGCUpLoadWidget::updateCount(int count)
{

}

void SHGCUpLoadWidget::on_cancel_clicked()
{
    qDebug() << "-----------------------------uploadtablemodel----------------------" << uploadtablemodel << endl;
    this->hide();
}
