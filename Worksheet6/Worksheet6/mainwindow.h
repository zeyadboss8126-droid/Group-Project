#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ModelPart.h"
#include "ModelPartList.h"
#include <QModelIndex>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    ModelPartList* partList;
signals:
    void statusUpdateMessage(const QString &message, int timeout);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Button1_clicked();

    void on_Button2_clicked();

    void handleTreeClicked(const QModelIndex &index);





private:
    Ui::MainWindow *ui;

    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionExit_triggered();

   void on_actionItem_Options_triggered();
};
#endif // MAINWINDOW_H


