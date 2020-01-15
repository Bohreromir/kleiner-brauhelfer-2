#ifndef WDGWEITEERZUTATGABE_H
#define WDGWEITEERZUTATGABE_H

#include <QWidget>

namespace Ui {
class WdgWeitereZutatGabe;
}

class WdgWeitereZutatGabe : public QWidget
{
    Q_OBJECT

public:
    explicit WdgWeitereZutatGabe(int index, QWidget *parent = nullptr);
    ~WdgWeitereZutatGabe();
    bool isEnabled() const;
    QVariant data(int col) const;
    bool setData(int col, const QVariant &value);
    QString name() const;
    void remove();

public slots:
    void updateValues(bool full = false);

private slots:
    void on_btnZutat_clicked();
    void on_btnLoeschen_clicked();
    void on_tbMenge_valueChanged(double value);
    void on_tbMengeTotal_valueChanged(double value);
    void on_cbZugabezeitpunkt_currentIndexChanged(int index);
    void on_tbDauerMin_valueChanged(int value);
    void on_btnZugeben_clicked();
    void on_cbEntnahme_clicked(bool checked);
    void on_tbZugabeNach_valueChanged(int arg1);
    void on_tbDauerTage_valueChanged(int value);
    void on_btnEntnehmen_clicked();
    void on_tbKomentar_textChanged();
    void on_tbDatumVon_dateChanged(const QDate &date);
    void on_tbDatumBis_dateChanged(const QDate &date);
    void on_btnAufbrauchen_clicked();

private:
    void checkEnabled(bool force);

private:
    Ui::WdgWeitereZutatGabe *ui;
    int mIndex;
    bool mEnabled;
};

#endif // WDGWEITEERZUTATGABE_H
