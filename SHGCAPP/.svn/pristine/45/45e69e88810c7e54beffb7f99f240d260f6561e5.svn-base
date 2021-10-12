#include "point_edit_widget.h"
#include "ui_point_edit_widget.h"
#include <QDebug>
#include <QMouseEvent>
#include "SHGCDataModel.h"
#include "QGCApplication.h"
#include "SimpleMissionItem.h"
#include "MissionSettingsItem.h"

#pragma execution_character_set("utf-8")

static point_Edit_Widget* _instance = nullptr;   ///< @brief MainWindow singleton
bool point_Edit_Widget::pressStatu = false;

point_Edit_Widget* point_Edit_Widget::_create(QWidget *parent)
{
    _instance = new point_Edit_Widget(parent);
    return _instance;
}

point_Edit_Widget* point_Edit_Widget::instance(void)
{
    return _instance;
}

void point_Edit_Widget::deleteInstance(void)
{
    delete this;
}

point_Edit_Widget::point_Edit_Widget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::point_Edit_Widget)
{
    ui->setupUi(this);

//    w_desktop = QGCMain::w_QGCWidget;
//    h_desktop = QGCMain::h_QGCWidget;

    initPointEditing_widget_LineEdit();//初始化"单点编辑"LineEdit输入框
    initReturnPoint_widget_LineEdit();//初始化"返航点"LineEdit输入框
    initGlobalSettings_widget_LineEdit();//初始化"全局设置"LineEdit编辑框
    initSwitchButton();//初始化断点续飞按钮开关

    //初始化"航线编辑"为选定状态
    ui->btn_Edit->setChecked(true);
    ui->btn_allPoint_Edit->setChecked(true);

    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setMouseTracking(true);

    tableViewInit();

    //    ui->pointEditTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);//设置表头属性
//    ui->pointEditTableView->resizeRowsToContents();//所有行高度自适应
//    ui->pointEditTableView->resizeColumnsToContents();//所有行宽度自适应

    ui->singlePointEditing_scrollArea->hide();
    ui->globalSettings_scrollArea->hide();
    ui->returnPoint_widget->hide();
    ui->all_points_Edit_ScrollArea->show();

    ui->angleExplain_widget_GS->hide();//默认全局设置 "角度说明" widget 不显示
    ui->cycle_widget_GS->hide();//默认全局设置 "循环设置"项 widget 不显示

}

point_Edit_Widget::~point_Edit_Widget()
{
    if(pModel) {
        delete pModel;
        pModel = NULL;
    }

//    if(upLoadWidget) {
//        delete upLoadWidget;
//        upLoadWidget = NULL;
//    }

    _instance = nullptr;
    delete ui;
}


//=======================================批量编辑========================*/

void point_Edit_Widget::initTableView()
{
    drag_widget = true;

    ui->pointEditTableView->resize(this->width()/3, this->height());//设置初始化时的表格大小

//    //隐藏列表头
//    ui->pointEditTableView->verticalHeader()->setVisible(false);
//    //设置行表头及其字体的颜色、网格线颜色
//    ui->pointEditTableView->horizontalHeader()->setStyleSheet( "QHeaderView::section {background-color: rgb(230,248,255);"
//                                                      "font:13pt 'Source Han Sans CN';color: rgb(51,51,51);"
//                                                      "border:0.5px solid rgb(209,230,239);}");

////    ui->pointEditTableView->horizontalHeader()->setStretchLastSection(true); //设置表头充满表格的宽度
//    ui->pointEditTableView->horizontalHeader()->setHighlightSections(false);
//    ui->pointEditTableView->horizontalHeader()->setFixedHeight(50); //设置表头的高度为50

//    //设置相邻行颜色交替显示
//    ui->pointEditTableView->setAlternatingRowColors(true);
//    ui->widget->setStyleSheet("QTableView {background-color:  rgb(255, 255, 255);"
//                              " alternate-background-color: rgb(246, 252, 255); "
//                              "color: rgb(51, 51, 51);}");

//    ui->pointEditTableView->setStyleSheet("gridline-color: rgb(209,230,239)");//设置网格线的颜色

//    ui->pointEditTableView->verticalHeader()->setDefaultSectionSize(50); //设置行高为50

}

void point_Edit_Widget::tableViewInit()
{
    SHGCDataModel *datamodel = qgcApp()->toolbox()->Shgcdatamodel();
    pModel = new QmlObjectTableModel;
    pModel = datamodel->uploadtabelmodel();

    //添加航点功能
    CPushButtonItemDelegate *addDelegate = new CPushButtonItemDelegate(QIcon(":/images/resources/images/ui/add_point.png"));
    ui->pointEditTableView->setItemDelegateForColumn(6,addDelegate);
    connect(addDelegate,&CPushButtonItemDelegate::BtnClicked,this,&point_Edit_Widget::_insertRows);

    //删除航点功能
    CPushButtonItemDelegate *deleteDelegate = new CPushButtonItemDelegate(QIcon(":/images/resources/images/ui/delete_point.png"));
    ui->pointEditTableView->setItemDelegateForColumn(7,deleteDelegate);
    connect(deleteDelegate,&CPushButtonItemDelegate::BtnClicked,this,&point_Edit_Widget::_removeRows);


    //上移航点
    CPushButtonItemDelegate *upDelegate = new CPushButtonItemDelegate(QIcon(":/images/resources/images/ui/up_point.png"));
    ui->pointEditTableView->setItemDelegateForColumn(8,upDelegate);
    connect(upDelegate,&CPushButtonItemDelegate::BtnClicked,this,&point_Edit_Widget::_upRows);


    //下移航点
    CPushButtonItemDelegate *downDelegate = new CPushButtonItemDelegate(QIcon(":/images/resources/images/ui/down_point.png"));
    ui->pointEditTableView->setItemDelegateForColumn(9,downDelegate);
    connect(downDelegate,&CPushButtonItemDelegate::BtnClicked,this,&point_Edit_Widget::_downRows);

    ui->pointEditTableView->verticalHeader()->setVisible(false);

//    pModel->setSort(1,Qt::AscendingOrder);  //递增排序

    ui->pointEditTableView->setModel(pModel);
    ui->pointEditTableView->setFont(QFont("Source Han Sans CN",12,3));//设置字体


    //打开之后默认隐藏这些列
    ui->pointEditTableView->setColumnHidden(0,true);
    ui->pointEditTableView->setColumnHidden(1,true);
    ui->pointEditTableView->setColumnHidden(2,true);
    ui->pointEditTableView->setColumnHidden(3,true);

    //编辑航点动作
//    CComboBoxItemDelegate *pointAction = new CComboBoxItemDelegate();
//    connect(pointAction,SIGNAL(comboBoxClicked(QModelIndex,int)),this,SLOT(changeAction(QModelIndex,int)));
//    ui->tableView->setItemDelegateForColumn(2,pointAction);


    ui->pointEditTableView->setSelectionBehavior(QAbstractItemView::SelectRows);   //设置整行选择
    ui->pointEditTableView->setSelectionMode (QAbstractItemView::SingleSelection);
    ui->pointEditTableView->setMouseTracking(true);    //开启捕鼠功能


    initTableView();//初始化批量编辑表格属性

    //    ui->tableView->setStyleSheet("selection-background-color:rgba(0,0,0,50)");  //设置选中行颜色 //设置选中行颜色
    //    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));
    //行数改变
//    connect(ui->tableView->selectionModel(),SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),  this, SLOT(showOperationDetails(const QModelIndex &, const QModelIndex &)));
    //单元格内容改变
//    connect(ui->tableView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(itemDataChanged(QModelIndex,QModelIndex)));
}

void point_Edit_Widget::_insertRows(QModelIndex index)
{
    if(index.row() == 0)
    {
        return;
    }

    SimpleMissionItem* item = qobject_cast<SimpleMissionItem*>(pModel->get(index.row()));
    QGeoCoordinate coordinate = item->coordinate();

    emit addPointItem(coordinate.latitude(),coordinate.longitude(),index.row());
    pModel->refrushModel();

}

void point_Edit_Widget::_removeRows(QModelIndex index)
{
    if(index.row() >= pModel->rowCount())
    {
        return;
    }

    emit removePointItem(index.row());
    pModel->refrushModel();

}

void point_Edit_Widget::_upRows(QModelIndex index)
{
    if(index.row() >= pModel->rowCount())
    {
        return;
    }

    emit upPointItem(index.row());
    pModel->refrushModel();

}

void point_Edit_Widget::_downRows(QModelIndex index)
{
    if(index.row() >= pModel->rowCount())
    {
        return;
    }

    emit downPointItem(index.row());
    pModel->refrushModel();

}

//点击空白关闭窗口
bool point_Edit_Widget::event(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        if(QApplication::activeWindow() != this)
        {
            this->close();
        }
    }
    return QWidget::event(event);
}

void point_Edit_Widget::on_btn_Drag_widget_pressed()
{
    pressStatu = true;
    start_x = QCursor::pos().x();
}

void point_Edit_Widget::on_btn_Drag_widget_released()
{
    pressStatu = false;
}

void point_Edit_Widget::mouseMoveEvent(QMouseEvent *event)
{

    QPoint n = QCursor::pos();
    int w = this->width() - n.x();



    if(drag_widget && w > 680) //point_Edit_Widget宽度为   680
    {
        this->setGeometry(n.x(),0,this->width(),this->height());

        ui->pointEditTableView->resize(this->width() - 70, this->height());

        //拖动后显示整张表
        ui->pointEditTableView->setColumnHidden(0,false);
        ui->pointEditTableView->setColumnHidden(1,false);
        ui->pointEditTableView->setColumnHidden(2,false);
        ui->pointEditTableView->setColumnHidden(3,false);

    }

    if(drag_widget && w <= 700)
    {
        ui->pointEditTableView->resize(600, this->height());

        ui->pointEditTableView->setColumnHidden(0,true);
        ui->pointEditTableView->setColumnHidden(1,true);
        ui->pointEditTableView->setColumnHidden(2,true);
        ui->pointEditTableView->setColumnHidden(3,true);
    }
    pModel->refrushModel();
}

void point_Edit_Widget::on_btn_allPoint_Edit_clicked()
{
    initTableView();//初始化表格属性

    ui->singlePointEditing_scrollArea->hide();
    ui->globalSettings_scrollArea->hide();
    ui->returnPoint_widget->hide();

    ui->btn_onePoint_Edit->setStyleSheet("background-color: rgb(255, 255, 255); outline:0px; font: 22pt \"思源黑体\";"
                                         "color: rgb(168, 184, 191); border-width:0; border-style:outset;");
    ui->btn_allPoint_Edit->setStyleSheet("background-image: url(:/images/singlePointEditing_blue.png); outline:0px; font: 22pt \"思源黑体\";"
                                         "color: rgb(168, 184, 191); border-width:0; border-style:outset;");
    ui->btn_Drag_widget->show();

    ui->all_points_Edit_ScrollArea->show();

}

//void point_Edit_Widget::OnAddPointClicked(QModelIndex index)
//{

//}

//void point_Edit_Widget::OnDeletePointClicked(QModelIndex index)
//{

//}

//void point_Edit_Widget::OnUpPointClicked(QModelIndex index)
//{

//}

//void point_Edit_Widget::OnDownPointClicked(QModelIndex index)
//{

//}

//void point_Edit_Widget::changeAction(QModelIndex index,int i)
//{
//    qDebug() << "行数" << index << endl;
//    qDebug() << "下拉框" << i << endl;
//}


//void point_Edit_Widget::onTableClicked(const QModelIndex &index)
//{

//}

//void point_Edit_Widget::itemDataChanged(QModelIndex index01,QModelIndex)
//{
//    QModelIndex index_lng;
//    QModelIndex index_lat;

//    if(index01.column() == 4 || index01.column() == 5){
//        index_lng = pModel->index(index01.row(),4);
//        index_lat = pModel->index(index01.row(),5);
//    }

//    QModelIndex index_no = pModel->index(index01.row(),1);
//    QVariant seq_no = pModel->data(index_no).toInt();
//    QVariant _lng = pModel->data(index_lng).toDouble();
//    QVariant _lat = pModel->data(index_lat).toDouble();
//    emit upPointData(seq_no,_lng,_lat);
//    pModel->submitAll();
//    pModel->select();


//}

//void point_Edit_Widget::showOperationDetails(const QModelIndex &current, const QModelIndex &previous)
//{
//    //    qDebug() << "sssssss"<< endl;
//    //    pModel->submitAll();

//}

/*============================================================================*/

//当点击航点时，清空输入框
//void point_Edit_Widget::clearPointEditing_widget_LineEdit(QVariant,QVariant)
//{
//    //单点编辑
//    ui->serialNumber_lineEdit_SPE->clear();
//    ui->longitude_lineEdit_SPE->clear();
//    ui->latitude_lineEdit_SPE->clear();
//    ui->takeoffAltitude_lineEdit_SPE->clear();
//    ui->relativeDistance_lineEdit_SPE->clear();
//    ui->absoluteAngle_lineEdit_SPE->clear();
//    ui->horizontalVelocity_lineEdit_SPE->clear();
//    ui->verticalVelocity_lineEdit_SPE->clear();
//    ui->slope_lineEdit_SPE->clear();
//    ui->slopeAngle_lineEdit_SPE->clear();
//    ui->noseOrientation_lineEdit_SPE->clear();
//    ui->residenceTime_lineEdit_SPE->clear();


//    //全局设置


//    //返航点

//    //批量编辑，重新载入航点数据


//}

void point_Edit_Widget::on_btn_Edit_clicked()
{
    ui->widget_point_edit->show();
    ui->widget_edit->show();

    ui->btn_allPoint_Edit->setStyleSheet("background-image: url(:/images/singlePointEditing_blue.png); outline:0px; font: 22pt \"思源黑体\";"
                                         "color: rgb(168, 184, 191); border-width:0; border-style:outset;");
    ui->btn_onePoint_Edit->setStyleSheet("background-color: transparent; outline:0px; font: 22pt \"思源黑体\";"
                                         "color: rgb(168, 184, 191); border-width:0; border-style:outset;");
    ui->btn_Edit->setStyleSheet("background-image: url(:/images/pointEditor_blue.png); outline:0px; font: 22pt \"思源黑体\";"
                                "color: rgb(168, 184, 191); border-width:0; border-style:outset;");
    ui->btn_setting->setStyleSheet("background-color: rgb(255, 255, 255); outline:0px; font: 22pt \"思源黑体\";"
                                   "color: rgb(168, 184, 191); border-width:0; border-style:outset;");
    ui->btn_Back_point->setStyleSheet("background-color: rgb(255, 255, 255); outline:0px; font: 22pt \"思源黑体\";"
                                      "color: rgb(168, 184, 191); border-width:0; border-style:outset;");
    on_btn_allPoint_Edit_clicked();
}

void point_Edit_Widget::on_btn_setting_clicked()
{
    drag_widget = false;
    ui->btn_Drag_widget->hide();
    ui->singlePointEditing_scrollArea->hide();
    ui->all_points_Edit_ScrollArea->hide();
    ui->returnPoint_widget->hide();
    ui->widget_point_edit->hide();

    ui->btn_setting->setStyleSheet("background-image: url(:/images/pointEditor_blue.png); outline:0px; font: 22pt \"思源黑体\";"
                                   "color: rgb(168, 184, 191); border-width:0; border-style:outset;");
    ui->btn_Edit->setStyleSheet("background-color: rgb(255, 255, 255); outline:0px; font: 22pt \"思源黑体\";"
                                "color: rgb(168, 184, 191); border-width:0; border-style:outset;");
    ui->btn_Back_point->setStyleSheet("background-color: rgb(255, 255, 255); outline:0px; font: 22pt \"思源黑体\";"
                                      "color: rgb(168, 184, 191); border-width:0; border-style:outset;");

    ui->widget_point_edit->hide();
    ui->widget_edit->hide();

    ui->globalSettings_scrollArea->show();
}

void point_Edit_Widget::on_btn_Back_point_clicked()
{
    drag_widget = false;
    ui->btn_Drag_widget->hide();
    ui->singlePointEditing_scrollArea->hide();
    ui->all_points_Edit_ScrollArea->hide();
    ui->globalSettings_scrollArea->hide();
    ui->widget_point_edit->hide();

    ui->btn_setting->setStyleSheet("background-color: rgb(255, 255, 255); outline:0px; font: 22pt \"思源黑体\";"
                                   "color: rgb(168, 184, 191); border-width:0; border-style:outset;");
    ui->btn_Edit->setStyleSheet("background-color: rgb(255, 255, 255); outline:0px; font: 22pt \"思源黑体\";"
                                "color: rgb(168, 184, 191); border-width:0; border-style:outset;");
    ui->btn_Back_point->setStyleSheet("background-image: url(:/images/pointEditor_blue.png); outline:0px; font: 22pt \"思源黑体\";"
                                      "color: rgb(168, 184, 191); border-width:0; border-style:outset;");

    ui->widget_point_edit->hide();
    ui->widget_edit->hide();

    ui->returnPoint_widget->show();
}

void point_Edit_Widget::on_btn_onePoint_Edit_clicked()
{
    drag_widget = false;
    ui->btn_Drag_widget->hide();
    ui->all_points_Edit_ScrollArea->hide();
    ui->returnPoint_widget->hide();
    ui->globalSettings_scrollArea->hide();

    ui->btn_onePoint_Edit->setStyleSheet("background-image: url(:/images/singlePointEditing_blue.png); outline:0px; font: 22pt \"思源黑体\";"
                                         "color: rgb(168, 184, 191); border-width:0; border-style:outset;");
    ui->btn_allPoint_Edit->setStyleSheet("background-color: rgb(255, 255, 255); outline:0px; font: 22pt \"思源黑体\";"
                                         "color: rgb(168, 184, 191); border-width:0; border-style:outset;");

    ui->singlePointEditing_scrollArea->show();
}

/*===============================返航点======================================*/

//void point_Edit_Widget::recvHomeData(QString home_lng, QString home_lat)
//{
//    QString rp_lng = home_lng;
//    QString rp_lat = home_lat;
//    ui->longitude_lineEdit_RP->setText(rp_lng +" E");
//    ui->latitude_lineEdit_RP->setText(rp_lat +" N");
//}

void point_Edit_Widget::initReturnPoint_widget_LineEdit()
{
    ui->longitude_lineEdit_RP->setAlignment(Qt::AlignCenter);
    ui->longitude_lineEdit_RP->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");

    ui->latitude_lineEdit_RP->setAlignment(Qt::AlignCenter);
    ui->latitude_lineEdit_RP->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");

    ui->setTakeoffPoint_widget_RP->setStyleSheet("background-image: url(:/images/returnPoint_setUp_blueBackground.png); outline:0px;"
                                                 "border-width:0; border-style:outset;");
    ui->setTakeoffPoint_pushButton_RP->setStyleSheet("font: 16pt \"思源黑体\";color: rgb(254,254,254);");

    ui->widget_3->hide();
}

//void point_Edit_Widget::on_setTakeoffPoint_pushButton_RP_clicked()//设置为起飞点
//{
//    ui->setMyPosition_widget_RP->setStyleSheet("background-image: url(:/images/returnPoint_setUp_blueFrame.png); outline:0px;"
//                                               "border-width:0; border-style:outset;");
//    ui->setMyPosition_pushButton_RP->setStyleSheet("font: 16pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->manualSettings_widget_RP->setStyleSheet("background-image: url(:/images/returnPoint_setUp_blueFrame.png); outline:0px;"
//                                                "border-width:0; border-style:outset;");
//    ui->manualSettings_pushButton_RP->setStyleSheet("font: 16pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->setTakeoffPoint_widget_RP->setStyleSheet("background-image: url(:/images/returnPoint_setUp_blueBackground.png); outline:0px;"
//                                                 "border-width:0; border-style:outset;");
//    ui->setTakeoffPoint_pushButton_RP->setStyleSheet("font: 16pt \"思源黑体\";color: rgb(254,254,254);");

//    ui->widget_3->hide();

//    emit rpToStart();
//    ui->longitude_lineEdit_RP->clear();
//    ui->latitude_lineEdit_RP->clear();
//}

//void point_Edit_Widget::on_setMyPosition_pushButton_RP_clicked()//设置为我的位置
//{
//    ui->setTakeoffPoint_widget_RP->setStyleSheet("background-image: url(:/images/returnPoint_setUp_blueFrame.png); outline:0px;"
//                                                 "border-width:0; border-style:outset;");
//    ui->setTakeoffPoint_pushButton_RP->setStyleSheet("font: 16pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->manualSettings_widget_RP->setStyleSheet("background-image: url(:/images/returnPoint_setUp_blueFrame.png); outline:0px;"
//                                                "border-width:0; border-style:outset;");
//    ui->manualSettings_pushButton_RP->setStyleSheet("font: 16pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->setMyPosition_widget_RP->setStyleSheet("background-image: url(:/images/returnPoint_setUp_blueBackground.png); outline:0px;"
//                                               "border-width:0; border-style:outset;");
//    ui->setMyPosition_pushButton_RP->setStyleSheet("font: 16pt \"思源黑体\";color: rgb(254,254,254);");

//    ui->widget_3->hide();
//    emit rpToCurrent();
//    ui->longitude_lineEdit_RP->clear();
//    ui->latitude_lineEdit_RP->clear();
//}

//void point_Edit_Widget::on_manualSettings_pushButton_RP_clicked()//手动设置
//{
//    ui->setTakeoffPoint_widget_RP->setStyleSheet("background-image: url(:/images/returnPoint_setUp_blueFrame.png); outline:0px;"
//                                                 "border-width:0; border-style:outset;");
//    ui->setTakeoffPoint_pushButton_RP->setStyleSheet("font: 16pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->setMyPosition_widget_RP->setStyleSheet("background-image: url(:/images/returnPoint_setUp_blueFrame.png); outline:0px;"
//                                               "border-width:0; border-style:outset;");
//    ui->setMyPosition_pushButton_RP->setStyleSheet("font: 16pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->manualSettings_widget_RP->setStyleSheet("background-image: url(:/images/returnPoint_setUp_blueBackground.png); outline:0px;"
//                                                "border-width:0; border-style:outset;");
//    ui->manualSettings_pushButton_RP->setStyleSheet("font: 16pt \"思源黑体\";color: rgb(254,254,254);");

//    ui->widget_3->show();
//    emit rpToSelf();


//}

/*=========================================单点编辑=========================================*/
void point_Edit_Widget::initPointEditing_widget_LineEdit()//初始化"单点编辑"LineEdit编辑框
{
    ui->serialNumber_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->serialNumber_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
    ui->serialNumber_lineEdit_SPE->setEnabled(false);//该编辑框不可编辑

    ui->longitude_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->longitude_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
    // 这个行编辑只接受非负浮点数
    //QRegExp regExp_Lo("^(([1-9]+)|([0-9]+\\.[0-9]{1,2}))$");
    QRegExp regExp_Lo("^\\d+(\\.\\d+)?[E,W]$");
    ui->longitude_lineEdit_SPE->setValidator(new QRegExpValidator( regExp_Lo, this ));

    ui->latitude_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->latitude_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
    // 这个行编辑只接受非负浮点数
    //QRegExp regExp_La("^(([1-9]+)|([0-9]+\\.[0-9]{1,2}))$");
    QRegExp regExp_La("^\\d+(\\.\\d+)?[N,S]$");
    ui->latitude_lineEdit_SPE->setValidator(new QRegExpValidator( regExp_La, this ));

    ui->takeoffAltitude_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->takeoffAltitude_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
    // 这个行编辑只接受非负浮点数
    //QRegExp regExp_Ta("^(([1-9]+)|([0-9]+\\.[0-9]{1,2}))$");
    QRegExp regExp_Ta("^\\d+(\\.\\d+)?$");
    ui->takeoffAltitude_lineEdit_SPE->setValidator(new QRegExpValidator( regExp_Ta, this ));

    ui->relativeDistance_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->relativeDistance_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
    // 这个行编辑只接受非负浮点数
    //QRegExp regExp_Rd("^(([1-9]+)|([0-9]+\\.[0-9]{1,2}))$");
    QRegExp regExp_Rd("^\\d+(\\.\\d+)?$");
    ui->relativeDistance_lineEdit_SPE->setValidator(new QRegExpValidator( regExp_Rd, this ));

    ui->absoluteAngle_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->absoluteAngle_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
    // 这个行编辑只接受非负浮点数
    //QRegExp regExp_Aa("^(([1-9]+)|([0-9]+\\.[0-9]{1,2}))$");
    QRegExp regExp_Aa("^\\d+(\\.\\d+)?$");
    ui->absoluteAngle_lineEdit_SPE->setValidator(new QRegExpValidator( regExp_Aa, this ));

    ui->horizontalVelocity_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->horizontalVelocity_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
    // 这个行编辑只接受非负浮点数
    //QRegExp regExp_Hv("^(([1-9]+)|([0-9]+\\.[0-9]{1,2}))$");
    QRegExp regExp_Hv("^\\d+(\\.\\d+)?$");
    ui->horizontalVelocity_lineEdit_SPE->setValidator(new QRegExpValidator( regExp_Hv, this ));

    ui->verticalVelocity_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->verticalVelocity_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
    // 这个行编辑只接受非负浮点数
    //QRegExp regExp_Vv("^(([1-9]+)|([0-9]+\\.[0-9]{1,2}))$");
    QRegExp regExp_Vv("^\\d+(\\.\\d+)?$");
    ui->verticalVelocity_lineEdit_SPE->setValidator(new QRegExpValidator( regExp_Vv, this ));

    ui->slope_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->slope_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
    // 这个行编辑只接受非负浮点数
    //QRegExp regExp_S("^(([1-9]+)|([0-9]+\\.[0-9]{1,2}))$");
    QRegExp regExp_S("^\\d+(\\.\\d+)?$");
    ui->slope_lineEdit_SPE->setValidator(new QRegExpValidator( regExp_S, this ));

    ui->slopeAngle_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->slopeAngle_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
    // 这个行编辑只接受非负浮点数
    //QRegExp regExp_Sa("^(([1-9]+)|([0-9]+\\.[0-9]{1,2}))$");
    QRegExp regExp_Sa("^\\d+(\\.\\d+)?$");
    ui->slopeAngle_lineEdit_SPE->setValidator(new QRegExpValidator( regExp_Sa, this ));

    ui->noseOrientation_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->noseOrientation_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
    // 这个行编辑只接受非负浮点数
    //QRegExp regExp_No("^(([1-9]+)|([0-9]+\\.[0-9]{1,2}))$");
    QRegExp regExp_No("^\\d+(\\.\\d+)?$");
    ui->noseOrientation_lineEdit_SPE->setValidator(new QRegExpValidator( regExp_No, this ));

    ui->residenceTime_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->residenceTime_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
    // 这个行编辑只接受非负浮点数
    //QRegExp regExp_Rt"^(([1-9]+)|([0-9]+\\.[0-9]{1,2}))$");
    QRegExp regExp_Rt("^\\d+(\\.\\d+)?$");
    ui->noseOrientation_lineEdit_SPE->setValidator(new QRegExpValidator( regExp_Rt, this ));

    ui->action_lineEdit_SPE->setAlignment(Qt::AlignCenter);
    ui->action_lineEdit_SPE->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");
}

//void point_Edit_Widget::getWaypointDataDB_setLineEdit(QVariant no)
//{
//    SPE_waypointID = no.toString();

//}

//void point_Edit_Widget::on_US_serialNumber_pushButton_SPE_clicked()//序号前移
//{
//    QString strOut = ui->serialNumber_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 2)
//    {
//        length = 2;
//    }

//    QString strIn = QString::number(length);
//    ui->serialNumber_lineEdit_SPE->setText(strIn);
//}

//void point_Edit_Widget::on_DS_serialNumber_pushButton_SPE_clicked()//序号后移
//{
//    QString strOut = ui->serialNumber_lineEdit_SPE->text();
//    int length = strOut.toInt();

//}

//序号 LineEdit内的值发生改变
//void point_Edit_Widget::on_serialNumber_lineEdit_SPE_textChanged(const QString &arg1)
//{
//    emit serialNumber_SPE_textChanged(arg1);//值发生改变时，刷新航点

//    seq_no_LineEdit_isChanged = true;
//    SPE_seq_no_changed = arg1;//保存更改后的seq_no

//    QString SPE_waypointID_temp = SPE_waypointID;

//    /***值发生改变，更新数据库***/
//    //把当前序号对应的seq_no改成修改后的值
//    bool isOk=query.exec("update sys_waypoint set seq_no = " + arg1 + " where seq_no = " + SPE_waypointID);
//    if(!isOk)
//    {
//        qDebug()<<"更新 seq_no 数据失败！";
//    }
//    //把之前修改后的值所对应的seq_no改为 未修改之前航点对应的seq_no
//    isOk=query.exec("update sys_waypoint set seq_no = " + SPE_waypointID_temp + " where seq_no = " + SPE_seq_no_changed);
//    if(!isOk)
//    {
//        qDebug()<<"更新 seq_no 数据失败！";
//    }
//}

////经度 LineEdit内的值发生改变
//void point_Edit_Widget::on_longitude_lineEdit_SPE_textChanged(const QString &arg1)
//{
//    emit longitude_SPE_textChanged(arg1);//值发生改变时，刷新航点

//    //值发生改变，更新数据库
//    bool isOk=query.exec("update sys_waypoint set var_lng = " + arg1 + " where seq_no = " + SPE_seq_no_changed);
//    if(!isOk)
//    {
//        qDebug()<<"更新 var_lng 数据失败！";
//    }
//}

////纬度 LineEdit内的值发生改变
//void point_Edit_Widget::on_latitude_lineEdit_SPE_textChanged(const QString &arg1)
//{
//    emit latitude_SPE_textChanged(arg1);//值发生改变时，刷新航点

//    //若序号seq_no LineEdit编辑框内的值 没有 发生改变  则更新时根据原来的SPE_waypointID进行过滤
//    if(seq_no_LineEdit_isChanged == false)
//    {
//        SPE_seq_no_changed = SPE_waypointID;
//    }

//    qDebug()<<"SPE_seq_no_changed :"<<SPE_seq_no_changed;

//    //值发生改变，更新数据库
//    bool isOk=query.exec("update sys_waypoint set var_lat = " + arg1 + " where seq_no = " + SPE_seq_no_changed);
//    if(!isOk)
//    {
//        qDebug()<<"更新 var_lat 数据失败！";
//    }
//}

//void point_Edit_Widget::on_MS_takeoffAltitude_pushButton_SPE_clicked()//相对起飞点高度 减
//{
//    QString strOut = ui->takeoffAltitude_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->takeoffAltitude_lineEdit_SPE->setText(strIn);
//}

//void point_Edit_Widget::on_PS_takeoffAltitude_pushButton_SPE_clicked()//相对起飞点高度 加
//{
//    QString strOut = ui->takeoffAltitude_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->takeoffAltitude_lineEdit_SPE->setText(strIn);
//}

////相对起飞点高度 LineEdit内的值发生改变
//void point_Edit_Widget::on_takeoffAltitude_lineEdit_SPE_textChanged(const QString &arg1)
//{
//    emit takeoffAltitude_SPE_textChanged(arg1);//值发生改变时，刷新航点

//    //值发生改变，更新数据库
//    bool isOk=query.exec("update sys_waypoint set rel_height = " + arg1 + " where seq_no = " + SPE_waypointID);
//    if(!isOk)
//    {
//        qDebug()<<"更新 rel_height 数据失败！";
//    }
//}


//void point_Edit_Widget::on_MS_relativeDistance_pushButton_SPE_clicked()//相对上一航点距离 减
//{
//    QString strOut = ui->relativeDistance_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->relativeDistance_lineEdit_SPE->setText(strIn);
//}

//void point_Edit_Widget::on_PS_relativeDistance_pushButton_SPE_clicked()//相对上一航点距离 加
//{
//    QString strOut = ui->relativeDistance_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->relativeDistance_lineEdit_SPE->setText(strIn);
//}

////相对上一航点距离 LineEdit内的值发生改变
//void point_Edit_Widget::on_relativeDistance_lineEdit_SPE_textChanged(const QString &arg1)
//{
//    emit relativeDistance_SPE_textChanged(arg1);//值发生改变时，刷新航点

//    //若序号seq_no LineEdit编辑框内的值 没有 发生改变  则更新时根据原来的SPE_waypointID进行过滤
//    if(seq_no_LineEdit_isChanged == false)
//    {
//        SPE_seq_no_changed = SPE_waypointID;
//    }

//    //值发生改变，更新数据库
//    bool isOk=query.exec("update sys_waypoint set prev_distance = " + arg1 + " where seq_no = " + SPE_seq_no_changed);
//    if(!isOk)
//    {
//        qDebug()<<"更新 prev_distance 数据失败！";
//    }
//}

//void point_Edit_Widget::on_MS_absoluteAngle_pushButton_SPE_clicked()//绝对角度 减
//{
//    QString strOut = ui->absoluteAngle_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->absoluteAngle_lineEdit_SPE->setText(strIn);
//}

//void point_Edit_Widget::on_PS_absoluteAngle_pushButton_SPE_clicked()//绝对角度 加
//{
//    QString strOut = ui->absoluteAngle_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->absoluteAngle_lineEdit_SPE->setText(strIn);
//}

////绝对角度 LineEdit内的值发生改变
//void point_Edit_Widget::on_absoluteAngle_lineEdit_SPE_textChanged(const QString &arg1)
//{
//    emit absoluteAngle_SPE_textChanged(arg1);//值发生改变时，刷新航点

//    //若序号seq_no LineEdit编辑框内的值 没有 发生改变  则更新时根据原来的SPE_waypointID进行过滤
//    if(seq_no_LineEdit_isChanged == false)
//    {
//        SPE_seq_no_changed = SPE_waypointID;
//    }

//    //值发生改变，更新数据库
//    bool isOk=query.exec("update sys_waypoint set abs_angle = " + arg1 + " where seq_no = " + SPE_seq_no_changed);
//    if(!isOk)
//    {
//        qDebug()<<"更新 abs_angle 数据失败！";
//    }
//}

//void point_Edit_Widget::on_MS_horizontalVelocity_pushButton_SPE_clicked()//水平速度 减
//{
//    QString strOut = ui->horizontalVelocity_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->horizontalVelocity_lineEdit_SPE->setText(strIn);
//}

//void point_Edit_Widget::on_PS_horizontalVelocity_pushButton_SPE_clicked()//水平速度 加
//{
//    QString strOut = ui->horizontalVelocity_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->horizontalVelocity_lineEdit_SPE->setText(strIn);
//}

////水平速度 LineEdit内的值发生改变
//void point_Edit_Widget::on_horizontalVelocity_lineEdit_SPE_textChanged(const QString &arg1)
//{
//    emit horizontalVelocity_SPE_textChanged(arg1);//值发生改变时，刷新航点

//    //若序号seq_no LineEdit编辑框内的值 没有 发生改变  则更新时根据原来的SPE_waypointID进行过滤
//    if(seq_no_LineEdit_isChanged == false)
//    {
//        SPE_seq_no_changed = SPE_waypointID;
//    }

//    //值发生改变，更新数据库
//    bool isOk=query.exec("update sys_waypoint set h_speed = " + arg1 + " where seq_no = " + SPE_seq_no_changed);
//    if(!isOk)
//    {
//        qDebug()<<"更新 h_speed 数据失败！";
//    }
//}

//void point_Edit_Widget::on_MS_verticalVelocity__pushButton_SPE_clicked()//垂直速度 减
//{
//    QString strOut = ui->verticalVelocity_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->verticalVelocity_lineEdit_SPE->setText(strIn);
//}

//void point_Edit_Widget::on_PS_verticalVelocity__pushButton_SPE_clicked()//垂直速度 加
//{
//    QString strOut = ui->verticalVelocity_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->verticalVelocity_lineEdit_SPE->setText(strIn);
//}

////垂直速度 LineEdit内的值发生改变
//void point_Edit_Widget::on_verticalVelocity_lineEdit_SPE_textChanged(const QString &arg1)
//{
//    emit verticalVelocity_SPE_textChanged(arg1);//值发生改变时，刷新航点

//    //若序号seq_no LineEdit编辑框内的值 没有 发生改变  则更新时根据原来的SPE_waypointID进行过滤
//    if(seq_no_LineEdit_isChanged == false)
//    {
//        SPE_seq_no_changed = SPE_waypointID;
//    }

//    //值发生改变，更新数据库
//    bool isOk=query.exec("update sys_waypoint set v_speed = " + arg1 + " where seq_no = " + SPE_seq_no_changed);
//    if(!isOk)
//    {
//        qDebug()<<"更新 v_speed 数据失败！";
//    }
//}

//void point_Edit_Widget::on_MS_slope_pushButton_SPE_clicked()//坡度 减
//{
//    QString strOut = ui->slope_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->slope_lineEdit_SPE->setText(strIn);
//}

//void point_Edit_Widget::on_PS_slope_pushButton_SPE_clicked()//坡度 加
//{
//    QString strOut = ui->slope_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->slope_lineEdit_SPE->setText(strIn);
//}

////坡度 LineEdit内的值发生改变
//void point_Edit_Widget::on_slope_lineEdit_SPE_textChanged(const QString &arg1)
//{
//    emit slope_SPE_textChanged(arg1);//值发生改变时，刷新航点

//    //若序号seq_no LineEdit编辑框内的值 没有 发生改变  则更新时根据原来的SPE_waypointID进行过滤
//    if(seq_no_LineEdit_isChanged == false)
//    {
//        SPE_seq_no_changed = SPE_waypointID;
//    }

//    //值发生改变，更新数据库
//    bool isOk=query.exec("update sys_waypoint set fly_gradient = " + arg1 + " where seq_no = " + SPE_seq_no_changed);
//    if(!isOk)
//    {
//        qDebug()<<"更新 fly_gradient 数据失败！";
//    }
//}

//void point_Edit_Widget::on_MS_slopeAngle_pushButton_SPE_clicked()//坡角 减
//{
//    QString strOut = ui->slopeAngle_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->slopeAngle_lineEdit_SPE->setText(strIn);
//}

//void point_Edit_Widget::on_PS_slopeAngle_pushButton_SPE_clicked()//坡角 加
//{
//    QString strOut = ui->slopeAngle_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->slopeAngle_lineEdit_SPE->setText(strIn);
//}

////坡角 LineEdit内的值发生改变
//void point_Edit_Widget::on_slopeAngle_lineEdit_SPE_textChanged(const QString &arg1)
//{
//    emit slopeAngle_SPE_textChanged(arg1);//值发生改变时，刷新航点

//    //若序号seq_no LineEdit编辑框内的值 没有 发生改变  则更新时根据原来的SPE_waypointID进行过滤
//    if(seq_no_LineEdit_isChanged == false)
//    {
//        SPE_seq_no_changed = SPE_waypointID;
//    }

//    //值发生改变，更新数据库
//    bool isOk=query.exec("update sys_waypoint set fly_angle = " + arg1 + " where seq_no = " + SPE_seq_no_changed);
//    if(!isOk)
//    {
//        qDebug()<<"更新 fly_angle 数据失败！";
//    }
//}

//void point_Edit_Widget::on_coordinatedTurn_pushButton_SPE_clicked()//转弯模式 协调转弯
//{
//    ui->rollTurn_widget_SPE->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                           "border-width:0; border-style:outset;");
//    ui->rollTurn_pushButton_SPE->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->stopTurning_widget_SPE->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                              "border-width:0; border-style:outset;");
//    ui->stopTurning_pushButton_SPE->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->coordinatedTurn_widget_SPE->setStyleSheet("background-image: url(:/images/turningMode_bluebackground.png); outline:0px;"
//                                                  "border-width:0; border-style:outset;");
//    ui->coordinatedTurn_pushButton_SPE->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(254,254,254);");
//}

//void point_Edit_Widget::on_rollTurn_pushButton_SPE_clicked()//转弯模式 侧倾转弯
//{
//    ui->coordinatedTurn_widget_SPE->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                  "border-width:0; border-style:outset;");
//    ui->coordinatedTurn_pushButton_SPE->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->stopTurning_widget_SPE->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                              "border-width:0; border-style:outset;");
//    ui->stopTurning_pushButton_SPE->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->rollTurn_widget_SPE->setStyleSheet("background-image: url(:/images/turningMode_bluebackground.png); outline:0px;"
//                                           "border-width:0; border-style:outset;");
//    ui->rollTurn_pushButton_SPE->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(254,254,254);");
//}

//void point_Edit_Widget::on_stopTurning_pushButton_SPE_clicked()//转弯模式 停止转弯
//{
//    ui->rollTurn_widget_SPE->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                           "border-width:0; border-style:outset;");
//    ui->rollTurn_pushButton_SPE->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->coordinatedTurn_widget_SPE->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                  "border-width:0; border-style:outset;");
//    ui->coordinatedTurn_pushButton_SPE->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->stopTurning_widget_SPE->setStyleSheet("background-image: url(:/images/turningMode_bluebackground.png); outline:0px;"
//                                              "border-width:0; border-style:outset;");
//    ui->stopTurning_pushButton_SPE->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(254,254,254);");
//}

//机头朝向 LineEdit内的值发生改变
//void point_Edit_Widget::on_noseOrientation_lineEdit_SPE_textChanged(const QString &arg1)
//{
//    emit noseOrientation_SPE_textChanged(arg1);//值发生改变时，刷新航点

//    //若序号seq_no LineEdit编辑框内的值 没有 发生改变  则更新时根据原来的SPE_waypointID进行过滤
//    if(seq_no_LineEdit_isChanged == false)
//    {
//        SPE_seq_no_changed = SPE_waypointID;
//    }

//    //值发生改变，更新数据库
//    bool isOk=query.exec("update sys_waypoint set plane_toward = " + arg1 + " where seq_no = " + SPE_seq_no_changed);
//    if(!isOk)
//    {
//        qDebug()<<"更新 plane_toward 数据失败！";
//    }
//}

//void point_Edit_Widget::on_MS_residenceTime_pushButton_SPE_clicked()//停留时间 减
//{
//    QString strOut = ui->residenceTime_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->residenceTime_lineEdit_SPE->setText(strIn);
//}

//void point_Edit_Widget::on_PS_residenceTime_pushButton_SPE_clicked()//停留时间 加
//{
//    QString strOut = ui->residenceTime_lineEdit_SPE->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->residenceTime_lineEdit_SPE->setText(strIn);
//}

//停留时间 LineEdit内的值发生改变
//void point_Edit_Widget::on_residenceTime_lineEdit_SPE_textChanged(const QString &arg1)
//{
//    emit residenceTime_SPE_textChanged(arg1);//值发生改变时，刷新航点

//    //若序号seq_no LineEdit编辑框内的值 没有 发生改变  则更新时根据原来的SPE_waypointID进行过滤
//    if(seq_no_LineEdit_isChanged == false)
//    {
//        SPE_seq_no_changed = SPE_waypointID;
//    }

//    //值发生改变，更新数据库
//    bool isOk=query.exec("update sys_waypoint set stay_times = " + arg1 + " where seq_no = " + SPE_seq_no_changed);
//    if(!isOk)
//    {
//        qDebug()<<"更新 stay_times 数据失败！";
//    }
//}

//===================================全局设置=====================================*/

void point_Edit_Widget::initGlobalSettings_widget_LineEdit()//初始化"全局设置"LineEdit编辑框
{
    ui->takeoffAltitude_lineEdit_GS->setAlignment(Qt::AlignCenter);
    ui->takeoffAltitude_lineEdit_GS->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");

    ui->relativeDistance_lineEdit_GS->setAlignment(Qt::AlignCenter);
    ui->relativeDistance_lineEdit_GS->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");

    ui->horizontalVelocity_lineEdit_GS->setAlignment(Qt::AlignCenter);
    ui->horizontalVelocity_lineEdit_GS->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");

    ui->verticalVelocity_lineEdit_GS->setAlignment(Qt::AlignCenter);
    ui->verticalVelocity_lineEdit_GS->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");

    ui->noseOrientation_lineEdit_GS->setAlignment(Qt::AlignCenter);
    ui->noseOrientation_lineEdit_GS->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");

    ui->residenceTime_lineEdit_GS->setAlignment(Qt::AlignCenter);
    ui->residenceTime_lineEdit_GS->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");

    ui->theCycleNumber_lineEdit_GS->setAlignment(Qt::AlignCenter);
    ui->theCycleNumber_lineEdit_GS->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");

    ui->angleSelection_lineEdit_GS->setAlignment(Qt::AlignCenter);
    ui->angleSelection_lineEdit_GS->setStyleSheet("font: 16pt \"Source Han Sans CN\";color: rgb(51,51,51);");

}

void point_Edit_Widget::initSwitchButton()//初始化断点续飞按钮开关
{
    ui->breakpointContinueToFly_SwitchButton->setBgColorOn(QColor(3, 168, 247));
    ui->breakpointContinueToFly_SwitchButton->setShowText(false);
    ui->breakpointContinueToFly_SwitchButton->setAnimation(true);
    ui->breakpointContinueToFly_SwitchButton->setChecked(true);
}

//void point_Edit_Widget::on_MS_takeoffAltitude_pushButton_GS_clicked()//相对起飞点高度 减
//{
//    QString strOut = ui->takeoffAltitude_lineEdit_GS->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->takeoffAltitude_lineEdit_GS->setText(strIn);

//    //数据处理
//}

//void point_Edit_Widget::on_PS_takeoffAltitude_pushButton_GS_clicked()//相对起飞点高度 加
//{
//    QString strOut = ui->takeoffAltitude_lineEdit_GS->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->takeoffAltitude_lineEdit_GS->setText(strIn);


//    //数据处理
//}

//void point_Edit_Widget::on_MS_relativeDistance_pushButton_GS_clicked()//相对上一航点距离 减
//{
//    QString strOut = ui->relativeDistance_lineEdit_GS->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->relativeDistance_lineEdit_GS->setText(strIn);

//    //数据处理
//}

//void point_Edit_Widget::on_PS_relativeDistance_pushButton_GS_clicked()//相对上一航点距离 加
//{
//    QString strOut = ui->relativeDistance_lineEdit_GS->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->relativeDistance_lineEdit_GS->setText(strIn);


//    //数据处理
//}

//void point_Edit_Widget::on_MS_horizontalVelocity_pushButton_GS_clicked()//水平速度 减
//{
//    QString strOut = ui->horizontalVelocity_lineEdit_GS->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->horizontalVelocity_lineEdit_GS->setText(strIn);

//    //数据处理
//}

//void point_Edit_Widget::on_PS_horizontalVelocity_pushButton_Gs_clicked()//水平速度 加
//{
//    QString strOut = ui->horizontalVelocity_lineEdit_GS->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->horizontalVelocity_lineEdit_GS->setText(strIn);


//    //数据处理
//}

//void point_Edit_Widget::on_MS_verticalVelocity__pushButton_GS_clicked()//垂直速度 减
//{
//    QString strOut = ui->verticalVelocity_lineEdit_GS->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->verticalVelocity_lineEdit_GS->setText(strIn);

//    //数据处理
//}

//void point_Edit_Widget::on_PS_verticalVelocity__pushButton_GS_clicked()//垂直速度 加
//{
//    QString strOut = ui->verticalVelocity_lineEdit_GS->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->verticalVelocity_lineEdit_GS->setText(strIn);


//    //数据处理
//}

//void point_Edit_Widget::on_coordinatedTurn_pushButton_GS_clicked()//转弯模式 协调转弯
//{
//    ui->rollTurn_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                          "border-width:0; border-style:outset;");
//    ui->rollTurn_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->stopTurning_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                             "border-width:0; border-style:outset;");
//    ui->stopTurning_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->coordinatedTurn_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_bluebackground.png); outline:0px;"
//                                                 "border-width:0; border-style:outset;");
//    ui->coordinatedTurn_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(254,254,254);");
//}

//void point_Edit_Widget::on_rollTurn_pushButton_GS_clicked()//转弯模式 侧倾转弯
//{
//    ui->coordinatedTurn_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                 "border-width:0; border-style:outset;");
//    ui->coordinatedTurn_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->stopTurning_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                             "border-width:0; border-style:outset;");
//    ui->stopTurning_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->rollTurn_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_bluebackground.png); outline:0px;"
//                                          "border-width:0; border-style:outset;");
//    ui->rollTurn_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(254,254,254);");
//}

//void point_Edit_Widget::on_stopTurning_pushButton_GS_clicked()//转弯模式 停止转弯
//{
//    ui->rollTurn_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                          "border-width:0; border-style:outset;");
//    ui->rollTurn_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->coordinatedTurn_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                 "border-width:0; border-style:outset;");
//    ui->coordinatedTurn_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->stopTurning_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_bluebackground.png); outline:0px;"
//                                             "border-width:0; border-style:outset;");
//    ui->stopTurning_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(254,254,254);");
//}

//void point_Edit_Widget::on_MS_residenceTime_pushButton_GS_clicked()//停留时间 减
//{
//    QString strOut = ui->residenceTime_lineEdit_GS->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->residenceTime_lineEdit_GS->setText(strIn);

//    //数据处理
//}

//void point_Edit_Widget::on_PS_residenceTime_pushButton_GS_clicked()//停留时间 加
//{
//    QString strOut = ui->residenceTime_lineEdit_GS->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->residenceTime_lineEdit_GS->setText(strIn);


//    //数据处理
//}

//void point_Edit_Widget::on_endOfMission_turnBack_pushButton_GS_clicked()//任务结束后动作 返航
//{
//    ui->endOfMission_hover_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                    "border-width:0; border-style:outset;");
//    ui->endOfMission_hover_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->endOfMission_cycle_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                    "border-width:0; border-style:outset;");
//    ui->endOfMission_cycle_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->endOfMission_turnBack_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_bluebackground.png); outline:0px;"
//                                                       "border-width:0; border-style:outset;");
//    ui->endOfMission_turnBack_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(254,254,254);");

//    //如果此时"循环设置"项在显示中，则隐藏
//    if(endOfMission_cycle_pushButton_GS == true)
//    {
//        ui->cycle_widget_GS->hide();
//        endOfMission_cycle_pushButton_GS = false;
//    }
//}

//void point_Edit_Widget::on_endOfMission_hover_pushButton_GS_clicked()//任务结束后动作 悬停
//{
//    ui->endOfMission_turnBack_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                       "border-width:0; border-style:outset;");
//    ui->endOfMission_turnBack_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->endOfMission_cycle_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                    "border-width:0; border-style:outset;");
//    ui->endOfMission_cycle_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->endOfMission_hover_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_bluebackground.png); outline:0px;"
//                                                    "border-width:0; border-style:outset;");
//    ui->endOfMission_hover_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(254,254,254);");

//    //如果此时"循环设置"项在显示中，则隐藏
//    if(endOfMission_cycle_pushButton_GS == true)
//    {
//        ui->cycle_widget_GS->hide();
//        endOfMission_cycle_pushButton_GS = false;


//    }
//}

//void point_Edit_Widget::on_endOfMission_cycle_pushButton_GS_clicked()//任务结束后动作 循环
//{
//    ui->endOfMission_turnBack_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                       "border-width:0; border-style:outset;");
//    ui->endOfMission_turnBack_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->endOfMission_hover_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                    "border-width:0; border-style:outset;");
//    ui->endOfMission_hover_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->endOfMission_cycle_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_bluebackground.png); outline:0px;"
//                                                    "border-width:0; border-style:outset;");
//    ui->endOfMission_cycle_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(254,254,254);");

//    //点击后弹出"循环设置"项
//    if(endOfMission_cycle_pushButton_GS == false)
//    {
//        ui->cycle_widget_GS->show();
//        endOfMission_cycle_pushButton_GS = true;
//    }
//    else
//    {
//        ui->cycle_widget_GS->hide();
//        endOfMission_cycle_pushButton_GS = false;

//        ui->endOfMission_cycle_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                        "border-width:0; border-style:outset;");
//        ui->endOfMission_cycle_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");
//    }

//}

//void point_Edit_Widget::on_MS_theCycleNumberpushButton_GS_clicked()//循环圈数 减
//{
//    QString strOut = ui->theCycleNumber_lineEdit_GS->text();
//    int length = strOut.toInt();

//    length--;
//    if(length < 0)
//    {
//        length = 0;
//    }

//    QString strIn = QString::number(length);
//    ui->theCycleNumber_lineEdit_GS->setText(strIn);

//    //数据处理
//}

//void point_Edit_Widget::on_PS_theCycleNumber_pushButton_GS_clicked()//循环圈数 加
//{
//    QString strOut = ui->theCycleNumber_lineEdit_GS->text();
//    int length = strOut.toInt();

//    length++;

//    QString strIn = QString::number(length);
//    ui->theCycleNumber_lineEdit_GS->setText(strIn);


//    //数据处理
//}

//void point_Edit_Widget::on_endOfCycle_turnBack_pushButton_GS_clicked()//循环结束后动作 返航
//{
//    ui->endOfCycle_hover_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                  "border-width:0; border-style:outset;");
//    ui->endOfCycle_hover_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->endOfCycle_turnBack_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_bluebackground.png); outline:0px;"
//                                                     "border-width:0; border-style:outset;");
//    ui->endOfCycle_turnBack_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(254,254,254);");
//}

//void point_Edit_Widget::on_endOfCycle_hover_pushButton_GS_clicked()//循环结束后动作 悬停
//{
//    ui->endOfCycle_turnBack_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_blueFrame.png); outline:0px;"
//                                                     "border-width:0; border-style:outset;");
//    ui->endOfCycle_turnBack_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(3,168,247);");

//    ui->endOfCycle_hover_widget_GS->setStyleSheet("background-image: url(:/images/turningMode_bluebackground.png); outline:0px;"
//                                                  "border-width:0; border-style:outset;");
//    ui->endOfCycle_hover_pushButton_GS->setStyleSheet("font: 14pt \"思源黑体\";color: rgb(254,254,254);");
//}

//void point_Edit_Widget::on_angleExplain_pushButton_GS_clicked()//角度说明
//{
//    if(angleExplain_pushButton_GS_clicked == false)
//    {
//        ui->angleExplain_widget_GS->show();
//        angleExplain_pushButton_GS_clicked = true;
//    }
//    else
//    {
//        ui->angleExplain_widget_GS->hide();
//        angleExplain_pushButton_GS_clicked = false;
//    }
//}

//void point_Edit_Widget::on_angleSelection_pushButton_GS_clicked()//角度选择
//{

//}

//void point_Edit_Widget::on_spread_pushButton_GS_clicked()//水平速度、垂直速度 估算 展开按钮
//{

//}
