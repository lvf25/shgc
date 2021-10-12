#ifndef LEFTMAINWINDOW_H
#define LEFTMAINWINDOW_H

#include <QDialog>

namespace Ui {
class leftMainWindow;
}

class leftMainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit leftMainWindow(QWidget *parent = nullptr);
    ~leftMainWindow();

    static leftMainWindow* instance(void);

    /// @brief Deletes the MainWindow singleton
    void deleteInstance(void);

    /// @brief Creates the MainWindow singleton. Should only be called once by QGCApplication.
    static leftMainWindow* _create(QWidget *parent);

private slots:
    void on_btnFlightMonit_clicked();

    void on_btnRoutePlan_clicked();

    void on_btnActionAdjust_clicked();

    void on_btnActionSetUp_clicked();

private:
    Ui::leftMainWindow *ui;
    bool event(QEvent *event);

signals:
    void sendBtnData(QString);

};

#endif // LEFTMAINWINDOW_H
