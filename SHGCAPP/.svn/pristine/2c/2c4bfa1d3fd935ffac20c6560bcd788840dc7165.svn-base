#ifndef SHGCUPLOADWIDGET_H
#define SHGCUPLOADWIDGET_H

#include <QWidget>
#include "qmlobjecttablemodel.h"

namespace Ui {
class SHGCUpLoadWidget;
}

class SHGCUpLoadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SHGCUpLoadWidget(QWidget *parent = nullptr);
    ~SHGCUpLoadWidget();

    static SHGCUpLoadWidget* instance(void);

    /// @brief Creates the MainWindow singleton. Should only be called once by QGCApplication.
    static SHGCUpLoadWidget* _create(QWidget *parent);

    /// @brief Deletes the MainWindow singleton
    void deleteInstance(void);

    QmlObjectTableModel* uploadtablemodel;

private slots:
    void on_upload_clicked();

    void on_cancel_clicked();

    void updateCount(int count);

private:
    Ui::SHGCUpLoadWidget *ui;

signals:
    void uploadToUAV();
};

#endif // SHGCUPLOADWIDGET_H
