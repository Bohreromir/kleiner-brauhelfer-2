#ifndef DLGROHSTOFFEABZIEHEN_H
#define DLGROHSTOFFEABZIEHEN_H

#include <QDialog>
#include "brauhelfer.h"

namespace Ui {
class DlgRohstoffeAbziehen;
}

class DlgRohstoffeAbziehen : public QDialog
{
    Q_OBJECT

public:
    explicit DlgRohstoffeAbziehen(QWidget *parent = nullptr);
    DlgRohstoffeAbziehen(Brauhelfer::RohstoffTyp typ, const QString& name, double menge, QWidget *parent = nullptr);
    virtual ~DlgRohstoffeAbziehen() Q_DECL_OVERRIDE;

private slots:
    void reject() Q_DECL_OVERRIDE;
    void on_btnAbziehen_clicked();

private:
    void setModels(bool alleBrauzutaten, Brauhelfer::RohstoffTyp typ = Brauhelfer::RohstoffTyp::Malz, const QString& name = QString(), double menge = 0.0);

private:
    Ui::DlgRohstoffeAbziehen *ui;
    bool mAbgezogen;
};

#endif // DLGROHSTOFFEABZIEHEN_H
