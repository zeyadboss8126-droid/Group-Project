#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* Create / allocate the ModelList */
    this->partList = new ModelPartList("Parts List");

    /* Link it to the treeview in the GUI */
    ui->treeView->setModel(this->partList);

    /* Manually create a model tree (quick example) */
    ModelPart* rootItem = this->partList->getRootItem();

    /* Add 3 top level items */
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({name, visible});
        rootItem->appendChild(childItem);

        /* Add 5 sub-items */
        for (int j = 0; j < 5; j++) {
            QString subname = QString("Item %1,%2").arg(i).arg(j);
            QString subvisible("true");

            ModelPart* childChildItem = new ModelPart({subname, subvisible});
            childItem->appendChild(childChildItem);
        }
    }
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);

    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Button1_clicked()
{
QMessageBox::information(this, "Button 1", "You clicked Button 1");
    emit statusUpdateMessage("Button 1 clicked", 2000);
}


void MainWindow::on_Button2_clicked()
{
QMessageBox::information(this, "Button 2", "You clicked Button 2");
emit statusUpdateMessage("Button 2 clicked", 2000);

}

void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}


