#ifndef SHGCFVMAPPLICATIONTIPSWIDGET_H
#define SHGCFVMAPPLICATIONTIPSWIDGET_H

#include <QWidget>

#include "MainWindow.h"

namespace Ui {
class SHGCFvmApplicationTipsWidget;
}

class SHGCFvmApplicationTipsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SHGCFvmApplicationTipsWidget(QWidget *parent = nullptr);
    ~SHGCFvmApplicationTipsWidget();

    static SHGCFvmApplicationTipsWidget* instance(void);

    /// @brief Deletes the ApplicationTips singleton
    void deleteInstance(void);

    /// @brief Creates the ApplicationTips singleton. Should only be called once by SHGCApplication.
    static SHGCFvmApplicationTipsWidget* _create(QWidget *parent);

private slots:
    void changeLabelText_isUnknown();
    void changeLabelText_isOk();
    void changeLabelText_isError();
    void changeLabelText_isWait(bool);

signals:
    void textStates(bool);

private:
    Ui::SHGCFvmApplicationTipsWidget *ui;
    static SHGCFvmApplicationTipsWidget *self;

    AutoResize *m_autoResizeHandler;

    bool states = false;

    MainWindow *_MianWindow = MainWindow::instance();
};

#endif //SHGCFVMAPPLICATIONTIPSWIDGET_H
