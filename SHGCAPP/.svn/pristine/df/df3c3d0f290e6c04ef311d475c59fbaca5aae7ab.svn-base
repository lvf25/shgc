#ifndef AUTORESIZE_H
#define AUTORESIZE_H
#include <QWidget>
#include <QRadioButton>
#include <QMap>
struct AutoResizeOriginalData
{
	QRect data_rect;
	QFont data_font;
};
class AutoResize
{
public:
	typedef enum
	{
        INCLUDE_DASHBOARD=0x1,
        INCLUDE_MAINSETUP=0x2,
        INCLUDE_STACKED=0x4,
        INCLUDE_WIDGET=0x8,
        INCLUDE_PUSHBUTTON=0x10,
        INCLUDE_LABEL=0x12,
        INCLUDE_CLICKEDWIDGET=0x14,
        INCLUDE_SWITCHBUTTON=0x16,
        INCLUDE_AUTOSAVESLIDER=0x18,
        INCLUDE_LINEEDIT=0x20,
        INCLUDE_SLIDERPOINTER=0x22,
        INCLUDE_ELECPOINTER=0x24,
        INCLUDE_COMBOBOX=0x26,
        INCLUDE_VELOCIMETERINTERACTIVE=0x28,
        INCLUDE_VELOCIMETERSETUP=0x30,
        INCLUDE_FVMAPPLICATIONTIPS=0x32,
        INCLUDE_CHECKBOX=0x34,
        INCLUDE_TABLEWIDGET=0x36,
        INCLUDE_SCROLLAREA=0x38,


	}AUTORESIZEFLAG;
	AutoResize(QWidget* obj,float base_width,float base_height);
	~AutoResize(void);
	void doAutoResize();
	void addOtherItem(QWidget *other);
	void addIgnoreItem(QWidget *ignore);	
	void setAutoResizeFlag(int flag);
	void pushAllResizeItem();
    float getHorResizeRatio();
    float getVerResizeRatio();
    float getFontResizeRatio();
    static void ResizeParentWidget(QWidget *mainwindow,QWidget *parent,int widgetValue);
private:
	bool m_autoResize;
	QMap<QWidget*,AutoResizeOriginalData> m_resizeMap;
	QMap<QWidget*,AutoResizeOriginalData> m_fontMap;
	QList<QWidget*> m_otherItem;
	QList<QWidget*> m_ignoreItem;
	int m_autoResizeFlag;
	float m_horRatio;
	float m_verRatio;
	float m_fontRatio;
	float m_baseWidth;
	float m_baseHeight;
	QWidget *m_autoResizeObj;
	void calculateResizeRatio();
   void fontAutoResize(QWidget *obj,int fontSIze);
   void dealLayoutItems();
   void ignoreAllChiledren(QObject* obj);
};
#endif//AUTORESIZE_H

