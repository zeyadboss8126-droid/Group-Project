#include "ModelPart.h"

#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent)
    : m_itemData(data), m_parentItem(parent), m_colour(255, 255, 255)
{
    /* Default colour is white */
}

ModelPart::~ModelPart()
{
    qDeleteAll(m_childItems);
}

void ModelPart::appendChild(ModelPart* item)
{
    item->m_parentItem = this;
    m_childItems.append(item);
}

ModelPart* ModelPart::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const
{
    return m_childItems.count();
}

int ModelPart::columnCount() const
{
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return {};
    return m_itemData.at(column);
}

void ModelPart::set(int column, const QVariant& value)
{
    if (column < 0 || column >= m_itemData.size())
        return;
    m_itemData.replace(column, value);
}

ModelPart* ModelPart::parentItem()
{
    return m_parentItem;
}

int ModelPart::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B)
{
    m_colour = QColor(R, G, B);
    if (actor)
        actor->GetProperty()->SetColor(R / 255.0, G / 255.0, B / 255.0);
}

unsigned char ModelPart::getColourR() { return (unsigned char)(m_colour.red()); }
unsigned char ModelPart::getColourG() { return (unsigned char)(m_colour.green()); }
unsigned char ModelPart::getColourB() { return (unsigned char)(m_colour.blue()); }

void ModelPart::setVisible(bool isVisible)
{
    m_visible = isVisible;
    if (m_itemData.size() < 2) m_itemData.resize(2);
    set(1, isVisible ? "true" : "false");
    if (actor)
        actor->SetVisibility(isVisible ? 1 : 0);
}

bool ModelPart::visible()
{
    QString s = data(1).toString().trimmed().toLower();
    return (s == "true" || s == "1" || s == "yes");
}

void ModelPart::loadSTL(QString fileName)
{
    /* 1. Create STL reader and load file */
    file = vtkSmartPointer<vtkSTLReader>::New();
    file->SetFileName(fileName.toStdString().c_str());
    file->Update();

    /* 2. Create mapper and link to reader */
    vtkSmartPointer<vtkPolyDataMapper> polyMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    polyMapper->SetInputConnection(file->GetOutputPort());
    mapper = polyMapper;

    /* 3. Create actor and link to mapper */
    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    /* Set colour from stored colour */
    actor->GetProperty()->SetColor(
        m_colour.redF(),
        m_colour.greenF(),
        m_colour.blueF()
    );
}

vtkSmartPointer<vtkActor> ModelPart::getActor()
{
    return actor;
}

QString ModelPart::getName() const { return m_name; }
QColor ModelPart::getColour() const { return m_colour; }
bool ModelPart::getVisible() const { return m_visible; }

void ModelPart::setName(const QString& name)
{
    m_name = name;
    if (m_itemData.size() < 1) m_itemData.resize(1);
    set(0, name);
}

void ModelPart::setColour(int r, int g, int b)
{
    m_colour = QColor(r, g, b);
    if (m_itemData.size() < 3) m_itemData.resize(3);
    set(2, QString("%1,%2,%3").arg(r).arg(g).arg(b));
    if (actor)
        actor->GetProperty()->SetColor(r / 255.0, g / 255.0, b / 255.0);
}
