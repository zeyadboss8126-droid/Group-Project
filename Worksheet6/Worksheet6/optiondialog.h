#include <QDialog>
#include <QColor>

class ModelPart;   // forward declare

namespace Ui { class OptionDialog; }

class OptionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();

    void loadFromPart(ModelPart* part);
    void saveToPart(ModelPart* part);

    void setModelPart(ModelPart* part);

private:
    Ui::OptionDialog *ui;
    void accept() override;
     ModelPart *m_part = nullptr;
};
