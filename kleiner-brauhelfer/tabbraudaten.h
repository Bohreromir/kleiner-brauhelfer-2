#ifndef TABBRAUDATEN_H
#define TABBRAUDATEN_H

#include "tababstract.h"
#include <QTimer>

namespace Ui {
class TabBraudaten;
}

class TabBraudaten : public TabAbstract
{
    Q_OBJECT

public:
    explicit TabBraudaten(QWidget *parent = nullptr);
    virtual ~TabBraudaten() Q_DECL_OVERRIDE;
    void saveSettings() Q_DECL_OVERRIDE;
    void restoreView() Q_DECL_OVERRIDE;
    void checkEnabled();

private slots:
    void focusChanged(QWidget *old, QWidget *now);
    void sudLoaded();
    void updateValues();
    void updateWebView();
    void sudDataChanged(const QModelIndex& index);

    void on_tbBraudatum_dateChanged(const QDate &date);
    void on_tbBraudatumZeit_timeChanged(const QTime &time);
    void on_btnBraudatumHeute_clicked();
    void on_btnWuerzemengeKochbeginn_clicked();
    void on_tbTempKochbeginn_valueChanged(double);
    void on_btnWuerzemengeKochende_clicked();
    void on_tbTempKochende_valueChanged(double);
    void on_btnSWKochende_clicked();
    void on_btnSWAnstellen_clicked();
    void on_btnWasserVerschneidung_clicked();
    void on_btnWuerzemengeAnstellenTotal_clicked();
    void on_tbSpeiseSRE_valueChanged(double);
    void on_tbSpeiseT_valueChanged(double);
    void on_btnSpeisemengeNoetig_clicked();

    void on_cbDurchschnittIgnorieren_clicked(bool checked);
    void on_btnSudGebraut_clicked();
    void on_btnSudTeilen_clicked(); 

private:
    void onTabActivated() Q_DECL_OVERRIDE;

private:
    Ui::TabBraudaten *ui;
    QByteArray mDefaultSplitterState;
    QByteArray mDefaultSplitterHelpState;
    QTimer mTimerWebViewUpdate;
};

#endif // TABBRAUDATEN_H
