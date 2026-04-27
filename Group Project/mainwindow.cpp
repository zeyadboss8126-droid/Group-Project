#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QVBoxLayout>
#include <vtkCamera.h>

#include "optiondialog.h"

#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkNew.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Setup VTK widget inside the placeholder */
    vtkWidget = new QVTKOpenGLNativeWidget(ui->vtkPlaceholder);

    QVBoxLayout* layout = new QVBoxLayout(ui->vtkPlaceholder);
    layout->addWidget(vtkWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    ui->vtkPlaceholder->setLayout(layout);

    /* Initialize VTK rendering pipeline */
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    vtkWidget->setRenderWindow(renderWindow);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);
    renderer->SetBackground(0.1, 0.2, 0.4);

    /* Model / tree setup */
    partList = new ModelPartList("Parts List");
    ui->treeView->setModel(partList);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeView->addAction(ui->actionItem_Options);

    /* Connect signals */
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;
    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());
    emit statusUpdateMessage(QString("Selected: %1").arg(item->getName()), 0);
}

void MainWindow::on_Button1_clicked()
{
    renderer->RemoveAllViewProps();

    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    // Change colour here
    actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    // Match worksheet style
    actor->RotateX(30.0);
    actor->RotateY(-45.0);

    renderer->AddActor(actor);

    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    vtkWidget->renderWindow()->Render();
}

void MainWindow::on_Button2_clicked()
{
    QMessageBox::information(this, "Info", "Button 2 Clicked");
}

void MainWindow::on_actionOpen_triggered()
{
    /* Get filename from user */
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open STL File"),
        "",
        tr("STL Files (*.stl);;All Files (*)")
    );

    if (fileName.isEmpty())
        return;

    /* Get currently selected index, or use root if nothing selected */
    QModelIndex index = ui->treeView->currentIndex();

    /* Add new item to tree as child of selected item (or root) */
    QFileInfo info(fileName);
    QList<QVariant> data = { info.fileName(), "true", "255,0,89" };
    QModelIndex newIndex = partList->appendChild(index, data);

    /* Get the newly created ModelPart and load the STL */
    ModelPart* newPart = static_cast<ModelPart*>(newIndex.internalPointer());
    if (newPart) {
        newPart->loadSTL(fileName);
    }

    /* Expand tree and refresh render */
    ui->treeView->expandAll();
    updateRender();
}

void MainWindow::on_actionSave_triggered() { /* Not implemented */ }

void MainWindow::on_actionExit_triggered() { close(); }

void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

    OptionDialog dlg(this);
    dlg.setModelPart(part);
    if (dlg.exec() == QDialog::Accepted) {
        partList->dataChanged(index, index);
        updateRender();
    }
}

void MainWindow::updateRender()
{
    renderer->RemoveAllViewProps();
    updateRenderFromTree(partList->index(0, 0, QModelIndex()));
    renderer->ResetCamera();
    renderer->Render();
    vtkWidget->renderWindow()->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index)
{
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        /* Add actor to renderer if part is visible */
        if (selectedPart->visible()) {
            vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
            if (actor != nullptr) {
                renderer->AddActor(actor);
            }
        }
    }

    /* Check for children and recurse */
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }

    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}
