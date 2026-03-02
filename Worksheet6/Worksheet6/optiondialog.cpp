#include "optiondialog.h"
#include "ui_optiondialog.h"
#include "ModelPart.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::loadFromPart(ModelPart* part)
{
    ui->lineEditName->setText(part->getName());

    QColor c = part->getColour();
    ui->spinR->setValue(c.red());
    ui->spinG->setValue(c.green());
    ui->spinB->setValue(c.blue());

    ui->checkVisible->setChecked(part->getVisible());
}

void OptionDialog::saveToPart(ModelPart* part)
{
    part->setName(ui->lineEditName->text());
    part->setColour(ui->spinR->value(), ui->spinG->value(), ui->spinB->value());
    part->setVisible(ui->checkVisible->isChecked());
}
