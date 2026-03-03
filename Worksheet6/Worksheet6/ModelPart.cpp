/**     @file ModelPart.cpp
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#include "ModelPart.h"

/* Commented out for now, will be uncommented later when you have
 * installed the VTK library
 */
//#include <vtkSmartPointer.h>
//#include <vtkDataSetMapper.h>

ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent)
    : m_itemData(data), m_parentItem(parent)
{
    /* You probably want to give the item a default colour */
}

ModelPart::~ModelPart()
{
    qDeleteAll(m_childItems);
}

void ModelPart::appendChild(ModelPart* item)
{
    /* Add another model part as a child of this part
     * (it will appear as a sub-branch in the treeview)
     */
    item->m_parentItem = this;
    m_childItems.append(item);
}

ModelPart* ModelPart::child(int row)
{
    /* Return pointer to child item in row below this item.
     */
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const
{
    /* Count number of child items
     */
    return m_childItems.count();
}

int ModelPart::columnCount() const
{
    /* Count number of columns (properties) that this item has.
     */
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const
{
    // ✅ IMPORTANT: TreeView uses this function, so return m_itemData
    if (column < 0 || column >= m_itemData.size())
        return {};
    return m_itemData.at(column);
}

void ModelPart::set(int column, const QVariant& value)
{
    /* Set the data associated with a column of this item
     */
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
    /* Return the row index of this item, relative to it's parent.
     */
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B)
{
    /* This is a placeholder function that you will need to modify if you want to use it */
    Q_UNUSED(R);
    Q_UNUSED(G);
    Q_UNUSED(B);
}

unsigned char ModelPart::getColourR()
{
    /* This is a placeholder function that you will need to modify if you want to use it */
    return 0;   // needs updating
}

unsigned char ModelPart::getColourG()
{
    /* This is a placeholder function that you will need to modify if you want to use it */
    return 0;   // needs updating
}

unsigned char ModelPart::getColourB()
{
    /* This is a placeholder function that you will need to modify if you want to use it */
    return 0;   // needs updating
}

void ModelPart::setVisible(bool isVisible)
{
    /* ✅ Implemented: store visibility in column 1 ("Visible?") */
    m_visible = isVisible; // keep member in sync if you use it elsewhere

    // Ensure we have at least 2 columns
    if (m_itemData.size() < 2)
        m_itemData.resize(2);

    set(1, isVisible ? "true" : "false");
}

bool ModelPart::visible()
{
    /* ✅ Implemented: read visibility from column 1 */
    QString s = data(1).toString().trimmed().toLower();
    return (s == "true" || s == "1" || s == "yes");
}

void ModelPart::loadSTL(QString fileName)
{
    /* This is a placeholder function that you will need to modify if you want to use it */
    Q_UNUSED(fileName);

    /* 1. Use the vtkSTLReader class to load the STL file
     *     https://vtk.org/doc/nightly/html/classvtkSTLReader.html
     */

    /* 2. Initialise the part's vtkMapper */

    /* 3. Initialise the part's vtkActor and link to the mapper */
}

QString ModelPart::getName() const
{
    return m_name;
}

QColor ModelPart::getColour() const
{
    return m_colour;
}

bool ModelPart::getVisible() const
{
    return m_visible;
}

void ModelPart::setName(const QString& name)
{
    // ✅ Keep member and tree column 0 in sync
    m_name = name;

    if (m_itemData.size() < 1)
        m_itemData.resize(1);

    set(0, name);
}

void ModelPart::setColour(int r, int g, int b)
{
    m_colour = QColor(r, g, b);

    if (m_itemData.size() < 3) m_itemData.resize(3);
    set(2, QString("%1,%2,%3").arg(r).arg(g).arg(b));  // <-- this is what the tree shows
}




