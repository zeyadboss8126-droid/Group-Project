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

private:
    Ui::OptionDialog *ui;
};
