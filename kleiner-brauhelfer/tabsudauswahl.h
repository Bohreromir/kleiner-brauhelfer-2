#ifndef TABSUDAUSWAHL_H
#define TABSUDAUSWAHL_H

#include "tababstract.h"
#include <QAbstractItemModel>
#include <QAbstractItemDelegate>

namespace Ui {
class TabSudAuswahl;
}

class TabSudAuswahl : public TabAbstract
{
    Q_OBJECT

public:
    explicit TabSudAuswahl(QWidget *parent = nullptr);
    virtual ~TabSudAuswahl() Q_DECL_OVERRIDE;
    void saveSettings() Q_DECL_OVERRIDE;
    void restoreView() Q_DECL_OVERRIDE;
    QAbstractItemModel* model() const;

signals:
    void clicked(int sudId);

private slots:
    void databaseModified();
    void filterChanged();
    void selectionChanged();
    void spalteAnzeigen(bool checked);
    void updateWebView();
    void generateTemplateTags(QVariantMap& tags);
    void on_tableSudauswahl_doubleClicked(const QModelIndex &index);
    void on_tableSudauswahl_customContextMenuRequested(const QPoint &pos);
    void on_cbAlle_clicked(bool checked);
    void on_cbRezept_clicked();
    void on_cbGebraut_clicked();
    void on_cbAbgefuellt_clicked();
    void on_cbVerbraucht_clicked();
    void on_cbMerkliste_stateChanged(int state);
    void on_tbFilter_textChanged(const QString &pattern);
    void on_tbDatumVon_dateChanged(const QDate &date);
    void on_tbDatumBis_dateChanged(const QDate &date);
    void on_cbDatumAlle_stateChanged(int state);
    void on_btnMerken_clicked();
    void on_btnVergessen_clicked();
    void onVerbraucht_clicked();
    void onNichtVerbraucht_clicked();
    void on_btnAlleVergessen_clicked();
    void on_btnAnlegen_clicked();
    void on_btnKopieren_clicked();
    void on_btnLoeschen_clicked();
    void on_btnImportieren_clicked();
    void on_btnExportieren_clicked();
    void on_btnLaden_clicked();
    void on_btnToPdf_clicked();

private:
    void onTabActivated() Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void setFilterStatus();

private:
    struct AuswahlSpalten
    {
        QString fieldName;
        bool visible;
        bool canHide;
        int width;
        QAbstractItemDelegate* itemDelegate;
    };

private:
    Ui::TabSudAuswahl *ui;
    QByteArray mDefaultTableState;
    QByteArray mDefaultSplitterState;
    QList<AuswahlSpalten> mSpalten;
};

#endif // TABSUDAUSWAHL_H
