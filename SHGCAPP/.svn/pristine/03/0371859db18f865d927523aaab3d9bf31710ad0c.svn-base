#include "AutoResize.h"
#include <QLabel>
#include <QMapIterator>
#include <QObject>
#include <QDebug>
#include <QComboBox>
#include <QLineEdit>
#include <QLayout>
#include <QScreen>
#include <QApplication>

#include "flightdashboard.h"
#include "SHGCMainSetup.h"
#include <QStackedWidget>

#include "clickedWidget.h"
#include "switchbutton.h"
#include "autoSaveSlider.h"
#include "sliderPointer.h"
#include "elecPointer.h"

#include "SHGCVelocimeterInteractiveWidget.h"
#include "SHGCVelocimeterSetupWidget.h"
#include "SHGCFvmApplicationTipsWidget.h"
#include <QScrollArea>

#include "MainWindow.h"

AutoResize::AutoResize(QWidget* obj, float base_width, float base_height):m_autoResizeObj(obj)
{
    m_autoResize = false;
    m_baseWidth = base_width;
    m_baseHeight = base_height;
    m_horRatio = 1.0;
    m_verRatio = 1.0;
    m_fontRatio = 1.0;
    dealLayoutItems();
}


AutoResize::~AutoResize(void)
{
}

void AutoResize::pushAllResizeItem()
{
    QWidget *item=NULL;
    AutoResizeOriginalData resizeData;
    QRect tmp;
    //add other items
    for(auto it = m_otherItem.begin(); it != m_otherItem.end(); it++)
    {
        //qDebug() << *it;
        item = *it;
        tmp = item->geometry();
        tmp.setX(item->x());
        tmp.setY(item->y());
        tmp.setWidth(abs(tmp.width()));
        tmp.setHeight(abs(tmp.height()));
        resizeData.data_rect = tmp;
        resizeData.data_font = item->font();
        m_resizeMap[item] = resizeData;
    }
    if(m_autoResizeFlag&INCLUDE_DASHBOARD)
    {
        QList<FlightDashboard*> _flightDashboardList = m_autoResizeObj->findChildren<FlightDashboard*>();
        for(auto it = _flightDashboardList.begin();it != _flightDashboardList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_MAINSETUP)
    {
        QList<SHGCMainSetup*> _mainSetupList = m_autoResizeObj->findChildren<SHGCMainSetup*>();
        for(auto it = _mainSetupList.begin();it != _mainSetupList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_STACKED)
    {
        QList<QStackedWidget*> _stackedList = m_autoResizeObj->findChildren<QStackedWidget*>();
        for(auto it = _stackedList.begin();it != _stackedList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_WIDGET)
    {
        QList<QWidget*> _widgetList = m_autoResizeObj->findChildren<QWidget*>();
        for(auto it = _widgetList.begin();it != _widgetList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_PUSHBUTTON)
    {
        QList<QAbstractButton*> _buttonList = m_autoResizeObj->findChildren<QAbstractButton*>();
        for(auto it = _buttonList.begin();it != _buttonList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_LABEL)
    {
        QList<QLabel*> _labelList = m_autoResizeObj->findChildren<QLabel*>();
        for(auto it = _labelList.begin();it != _labelList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_CLICKEDWIDGET)
    {
        QList<clickedWidget*> _clickedWidgetList = m_autoResizeObj->findChildren<clickedWidget*>();
        for(auto it = _clickedWidgetList.begin();it != _clickedWidgetList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_SWITCHBUTTON)
    {
        QList<SwitchButton*> _switchButtonList = m_autoResizeObj->findChildren<SwitchButton*>();
        for(auto it = _switchButtonList.begin();it != _switchButtonList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_AUTOSAVESLIDER)
    {
        QList<autoSaveSlider*> _autoSaveSliderList = m_autoResizeObj->findChildren<autoSaveSlider*>();
        for(auto it = _autoSaveSliderList.begin();it != _autoSaveSliderList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_LINEEDIT)
    {
        QList<QLineEdit*> _lineEditList = m_autoResizeObj->findChildren<QLineEdit*>();
        for(auto it = _lineEditList.begin();it != _lineEditList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_SLIDERPOINTER)
    {
        QList<sliderPointer*> _sliderPointerList = m_autoResizeObj->findChildren<sliderPointer*>();
        for(auto it = _sliderPointerList.begin();it != _sliderPointerList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_ELECPOINTER)
    {
        QList<elecPointer*> _elecPointerList = m_autoResizeObj->findChildren<elecPointer*>();
        for(auto it = _elecPointerList.begin();it != _elecPointerList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_COMBOBOX)
    {
        QList<QComboBox*> _comboboxList = m_autoResizeObj->findChildren<QComboBox*>();
        for(auto it = _comboboxList.begin();it != _comboboxList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_VELOCIMETERINTERACTIVE)
    {
        QList<SHGCVelocimeterInteractiveWidget*> _velocimeterInteractiveWidgetList = m_autoResizeObj->findChildren<SHGCVelocimeterInteractiveWidget*>();
        for(auto it = _velocimeterInteractiveWidgetList.begin();it != _velocimeterInteractiveWidgetList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_VELOCIMETERSETUP)
    {
        QList<SHGCVelocimeterSetUpWidget*> _velocimeterSetUpWidgetList = m_autoResizeObj->findChildren<SHGCVelocimeterSetUpWidget*>();
        for(auto it = _velocimeterSetUpWidgetList.begin();it != _velocimeterSetUpWidgetList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_FVMAPPLICATIONTIPS)
    {
        QList<SHGCFvmApplicationTipsWidget*> _FvmApplicationTipsWidgetList = m_autoResizeObj->findChildren<SHGCFvmApplicationTipsWidget*>();
        for(auto it = _FvmApplicationTipsWidgetList.begin();it != _FvmApplicationTipsWidgetList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_CHECKBOX)
    {
        QList<QAbstractButton*> _checkBoxList = m_autoResizeObj->findChildren<QAbstractButton*>();
        for(auto it = _checkBoxList.begin();it != _checkBoxList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_TABLEWIDGET)
    {
        QList<QTableWidget*> _tableWidgetList = m_autoResizeObj->findChildren<QTableWidget*>();
        for(auto it = _tableWidgetList.begin();it != _tableWidgetList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    if(m_autoResizeFlag&INCLUDE_SCROLLAREA)
    {
        QList<QScrollArea*> _scrollAreaList = m_autoResizeObj->findChildren<QScrollArea*>();
        for(auto it = _scrollAreaList.begin();it != _scrollAreaList.end();it++)
        {
            item = *it;
            tmp = item->geometry();
            tmp.setX(item->x());
            tmp.setY(item->y());
            tmp.setWidth(abs(tmp.width()));
            tmp.setHeight(abs(tmp.height()));
            resizeData.data_rect = tmp;
            resizeData.data_font = item->font();
            m_resizeMap[item] = resizeData;
        }
    }
    for(auto it = m_ignoreItem.begin(); it != m_ignoreItem.end(); it++)
    {
        if(m_resizeMap.contains(*it))
            m_resizeMap.remove(*it);
    }
    m_autoResize = true;
}

float AutoResize::getHorResizeRatio()
{
    return m_horRatio;
}

float AutoResize::getVerResizeRatio()
{
    return m_verRatio;
}

float AutoResize::getFontResizeRatio()
{
    return m_fontRatio;
}

void AutoResize::calculateResizeRatio()
{
    m_horRatio = m_autoResizeObj->width() / m_baseWidth;
    m_verRatio = m_autoResizeObj->height() / m_baseHeight;\
    m_fontRatio = m_horRatio<m_verRatio?m_horRatio:m_verRatio;
}

void AutoResize::fontAutoResize(QWidget *obj,int fontSize)
{
    if(fontSize <= 0)
        return;
    bool hasTextStyle = false;
    fontSize*=m_fontRatio;
    QString fontTextReg = "font:\\s+[0-9]+pt";
    QString fontFormat = "font: %1pt";
    QString fontSizeReg = "[0-9]+";
    QRegExp reg = QRegExp(fontTextReg);
    QRegExp size = QRegExp(fontSizeReg);
    QString styleText = obj->styleSheet();
    QString fontText = "";
    QString fontSizeText = "";
    QFont changedFont;
    if(reg.indexIn(obj->styleSheet()) != -1)
    {
        fontText = reg.capturedTexts().at(0);
        if(size.indexIn(fontText) != -1)
        {
            hasTextStyle = true;
        }
    }
    //set stylesheet to change font size
    if(hasTextStyle)
    {
        styleText.replace(reg,fontFormat.arg(fontSize));
        obj->setStyleSheet(styleText);
    }
    //set font pointsize to change font size
    else
    {
        changedFont = obj->font();
        changedFont.setPointSize(fontSize);
        obj->setFont(changedFont);
    }
}

void AutoResize::dealLayoutItems()
{
    QString desName = "layoutWidget";
    QList<QLayout*> layoutList = m_autoResizeObj->findChildren<QLayout*>();
    for(auto it = layoutList.begin(); it != layoutList.end(); it++)
    {
        QString objName = (*it)->parent()->objectName();
        if(objName.contains(desName))
        {
            QWidget* layoutWidget = qobject_cast<QWidget*>((*it)->parent());
            m_otherItem.push_back(layoutWidget);
            ignoreAllChiledren(layoutWidget);
        }
    }
}

void AutoResize::ignoreAllChiledren(QObject* obj)
{
    QList<QObject*> children = obj->children();
    for(auto it = children.begin(); it != children.end(); it++)
    {
        QWidget *item = qobject_cast<QWidget*>(*it);
        m_ignoreItem.push_back(item);
        AutoResizeOriginalData resizeData;
        if(!item)
            continue;
        resizeData.data_font = item->font();
        m_fontMap[item] = resizeData;
    }
}

void AutoResize::addOtherItem(QWidget *other)
{
    m_otherItem.push_back(other);
}

void AutoResize::addIgnoreItem(QWidget *ignore)
{
    m_ignoreItem.push_back(ignore);
}

void AutoResize::setAutoResizeFlag(int flag)
{
    m_autoResizeFlag = flag;
}

void AutoResize::doAutoResize()
{
    calculateResizeRatio();
    if(m_autoResize)
    {
        QMapIterator<QWidget*, AutoResizeOriginalData> _itarator(m_resizeMap);
        QFont changedFont;
        while(_itarator.hasNext())
        {
            _itarator.next();
            QWidget* _item = _itarator.key();

            QRect tmp = _itarator.value().data_rect;
            tmp.setWidth(tmp.width()*m_horRatio);
            tmp.setHeight(tmp.height()*m_verRatio);
            QRect after = QRect(tmp.x()*m_horRatio, tmp.y()*m_verRatio, tmp.width(), tmp.height());
            //changed font
            changedFont = _itarator.value().data_font;
            fontAutoResize(_item,changedFont.pointSize());
            _item->setGeometry(after);
        }
        QMapIterator<QWidget*,AutoResizeOriginalData> _fontIt(m_fontMap);
        while(_fontIt.hasNext())
        {
            _fontIt.next();
            QWidget* _item = _fontIt.key();
            changedFont = _fontIt.value().data_font;
            fontAutoResize(_item,changedFont.pointSize());
        }
    }
}

void AutoResize::ResizeParentWidget(QWidget *mainwindow,QWidget *parent,int widgetValue)
{
//    MainWindow *_MainWindow = MainWindow::instance();
//    float horRatio = _MainWindow->resize_Coefficient_width;
//    float verRatio = _MainWindow->resize_Coefficient_height;

    QScreen* scr = QApplication::primaryScreen();
    QSize scrSize = scr->availableSize();

    if(widgetValue&INCLUDE_DASHBOARD)
    {
        QWidget * resizeWidget = parent;
        QRect resizeRect = resizeWidget->rect();
        static float baseWidth = scrSize.width();
        static float baseHeight = scrSize.height();
        static float widgetWidth = resizeRect.width();
        static float widgetHeight = resizeRect.height();
        static float widgetX=parent->geometry().x();
        static float widgetY=parent->geometry().y();
        float horRatio = mainwindow->rect().width()/baseWidth;
        float verRatio = mainwindow->rect().height()/baseHeight;

        //dajust the position of parent
        resizeRect.setX(widgetX*horRatio);
        resizeRect.setY(widgetY*verRatio);

        //resize the parent
        resizeRect.setWidth(widgetWidth*horRatio);
        resizeRect.setHeight(widgetHeight*verRatio);
        //set Geometry
        resizeWidget->setGeometry(resizeRect);
    }
    if(widgetValue&INCLUDE_MAINSETUP)
    {
        QWidget * resizeWidget = parent;
        QRect resizeRect = resizeWidget->rect();
        static float baseWidth = scrSize.width();
        static float baseHeight = scrSize.height();
        static float widgetWidth = resizeRect.width();
        static float widgetHeight = resizeRect.height();
        static float widgetX=parent->geometry().x();
        static float widgetY=parent->geometry().y();
        float horRatio = mainwindow->rect().width()/baseWidth;
        float verRatio = mainwindow->rect().height()/baseHeight;

        //dajust the position of parent
        resizeRect.setX(widgetX*horRatio);
        resizeRect.setY(widgetY*verRatio);

        //resize the parent
        resizeRect.setWidth(widgetWidth*horRatio);
        resizeRect.setHeight(widgetHeight*verRatio);
        //set Geometry
        resizeWidget->setGeometry(resizeRect);
    }
    if(widgetValue&INCLUDE_VELOCIMETERINTERACTIVE)
    {
        QWidget * resizeWidget = parent;
        QRect resizeRect = resizeWidget->rect();
        static float baseWidth = scrSize.width();
        static float baseHeight = scrSize.height();
        static float widgetWidth = resizeRect.width();
        static float widgetHeight = resizeRect.height();
        static float widgetX=parent->geometry().x();
        static float widgetY=parent->geometry().y();
        float horRatio = mainwindow->rect().width()/baseWidth;
        float verRatio = mainwindow->rect().height()/baseHeight;

        //dajust the position of parent
        resizeRect.setX(widgetX*horRatio);
        resizeRect.setY(widgetY*verRatio);

        //resize the parent
        resizeRect.setWidth(widgetWidth*horRatio);
        resizeRect.setHeight(widgetHeight*verRatio);
        //set Geometry
        resizeWidget->setGeometry(resizeRect);
    }
    if(widgetValue&INCLUDE_VELOCIMETERSETUP)
    {
        QWidget * resizeWidget = parent;
        QRect resizeRect = resizeWidget->rect();
        static float baseWidth = scrSize.width();
        static float baseHeight = scrSize.height();
        static float widgetWidth = resizeRect.width();
        static float widgetHeight = resizeRect.height();
        static float widgetX=parent->geometry().x();
        static float widgetY=parent->geometry().y();
        float horRatio = mainwindow->rect().width()/baseWidth;
        float verRatio = mainwindow->rect().height()/baseHeight;

        //dajust the position of parent
        resizeRect.setX(widgetX*horRatio);
        resizeRect.setY(widgetY*verRatio);

        //resize the parent
        resizeRect.setWidth(widgetWidth*horRatio);
        resizeRect.setHeight(widgetHeight*verRatio);
        //set Geometry
        resizeWidget->setGeometry(resizeRect);
    }
    if(widgetValue&INCLUDE_FVMAPPLICATIONTIPS)
    {
        QWidget * resizeWidget = parent;
        QRect resizeRect = resizeWidget->rect();
        static float baseWidth = scrSize.width();
        static float baseHeight = scrSize.height();
        static float widgetWidth = resizeRect.width();
        static float widgetHeight = resizeRect.height();
        static float widgetX=parent->geometry().x();
        static float widgetY=parent->geometry().y();
        float horRatio = mainwindow->rect().width()/baseWidth;
        float verRatio = mainwindow->rect().height()/baseHeight;

        //dajust the position of parent
        resizeRect.setX(widgetX*horRatio);
        resizeRect.setY(widgetY*verRatio);

        //resize the parent
        resizeRect.setWidth(widgetWidth*horRatio);
        resizeRect.setHeight(widgetHeight*verRatio);
        //set Geometry
        resizeWidget->setGeometry(resizeRect);
    }
}
