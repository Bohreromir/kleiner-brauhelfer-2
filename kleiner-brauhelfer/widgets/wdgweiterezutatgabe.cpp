#include "wdgweiterezutatgabe.h"
#include "ui_wdgweiterezutatgabe.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include "brauhelfer.h"
#include "settings.h"
#include "model/rohstoffauswahlproxymodel.h"

extern Brauhelfer* bh;
extern Settings* gSettings;

WdgWeitereZutatGabe::WdgWeitereZutatGabe(int index, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WdgWeitereZutatGabe),
    mIndex(index),
    mEnabled(true)
{
    ui->setupUi(this);

    ui->tbMenge->setErrorOnLimit(true);
    ui->tbMengeTotal->setErrorOnLimit(true);

    checkEnabled(true);
    updateValues();
    connect(bh, SIGNAL(discarded()), this, SLOT(updateValues()));
    connect(bh->sud()->modelWeitereZutatenGaben(), SIGNAL(modified()), this, SLOT(updateValues()));
    connect(bh->sud(), SIGNAL(modified()), this, SLOT(updateValues()));
}

WdgWeitereZutatGabe::~WdgWeitereZutatGabe()
{
    delete ui;
}

bool WdgWeitereZutatGabe::isEnabled() const
{
    return mEnabled;
}

QVariant WdgWeitereZutatGabe::data(const QString &fieldName) const
{
    return bh->sud()->modelWeitereZutatenGaben()->data(mIndex, fieldName);
}

bool WdgWeitereZutatGabe::setData(const QString &fieldName, const QVariant &value)
{
    return bh->sud()->modelWeitereZutatenGaben()->setData(mIndex, fieldName, value);
}

void WdgWeitereZutatGabe::checkEnabled(bool force)
{
    bool enabled = bh->sud()->getStatus() == Sud_Status_Rezept;
    if (data("Zeitpunkt").toInt() == EWZ_Zeitpunkt_Gaerung)
        enabled = bh->sud()->getStatus() < Sud_Status_Abgefuellt;
    if (gSettings->ForceEnabled)
        enabled = true;
    if (enabled == mEnabled && !force)
        return;

    int typ = data("Typ").toInt();

    if (typ == EWZ_Typ_Hopfen)
    {
        ui->frameColor->setToolTip(tr("<strong>Hopfentyp<br>"
                                      "<font color=\"%1\">Aroma</font><br>"
                                      "<font color=\"%2\">Bitter</font><br>"
                                      "<font color=\"%3\">Universal</font></strong>")
                .arg(gSettings->HopfenTypBackgrounds[1].name())
                .arg(gSettings->HopfenTypBackgrounds[2].name())
                .arg(gSettings->HopfenTypBackgrounds[3].name()));
    }
    else
    {
        ui->frameColor->setToolTip(tr("<strong>Zutattyp<br>"
                                  "<font color=\"%1\">Honig</font><br>"
                                  "<font color=\"%2\">Zucker</font><br>"
                                  "<font color=\"%3\">Gewürz</font><br>"
                                  "<font color=\"%4\">Frucht</font><br>"
                                  "<font color=\"%5\">Sonstiges</font></strong>")
            .arg(gSettings->WZTypBackgrounds[0].name())
            .arg(gSettings->WZTypBackgrounds[1].name())
            .arg(gSettings->WZTypBackgrounds[2].name())
            .arg(gSettings->WZTypBackgrounds[3].name())
            .arg(gSettings->WZTypBackgrounds[4].name()));
    }

    mEnabled = enabled;
    if (mEnabled)
    {
        RohstoffAuswahlProxyModel* model = new RohstoffAuswahlProxyModel(ui->cbZutat);
        if (typ == EWZ_Typ_Hopfen)
        {
            model->setSourceModel(bh->modelHopfen());
            model->setColumnMenge(bh->modelHopfen()->fieldIndex("Menge"));
            model->setIndexMengeBenoetigt(bh->sud()->modelHopfengaben()->index(mIndex, bh->sud()->modelHopfengaben()->fieldIndex("erg_Menge")));
            model->sort(bh->modelHopfen()->fieldIndex("Beschreibung"), Qt::AscendingOrder);
            ui->cbZutat->setModel(model);
            ui->cbZutat->setModelColumn(bh->modelHopfen()->fieldIndex("Beschreibung"));
        }
        else
        {
            model->setSourceModel(bh->modelWeitereZutaten());
            model->setColumnMenge(bh->modelWeitereZutaten()->fieldIndex("MengeGramm"));
            model->setIndexMengeBenoetigt(bh->sud()->modelWeitereZutatenGaben()->index(mIndex, bh->sud()->modelWeitereZutatenGaben()->fieldIndex("erg_Menge")));
            model->sort(bh->modelWeitereZutaten()->fieldIndex("Beschreibung"), Qt::AscendingOrder);
            ui->cbZutat->setModel(model);
            ui->cbZutat->setModelColumn(bh->modelWeitereZutaten()->fieldIndex("Beschreibung"));
        }
        ui->cbZutat->setEnabled(true);
        ui->cbZutat->setCurrentIndex(-1);
        ui->btnLoeschen->setVisible(true);
        ui->tbVorhanden->setVisible(true);
        ui->lblVorhanden->setVisible(true);
        ui->lblEinheit2->setVisible(true);
        ui->tbMenge->setReadOnly(false);
        ui->tbDauerMin->setReadOnly(false);
        ui->tbZugabeNach->setReadOnly(false);
        ui->tbDatumVon->setReadOnly(false);
        ui->tbDauerTage->setReadOnly(false);
        ui->tbDatumBis->setReadOnly(false);
    }
    else
    {
        QStandardItemModel *model = new QStandardItemModel(ui->cbZutat);
        model->setItem(0, 0, new QStandardItem(data("Name").toString()));
        ui->cbZutat->setModel(model);
        ui->cbZutat->setModelColumn(0);
        ui->cbZutat->setEnabled(false);
        ui->cbZutat->setCurrentIndex(-1);
        ui->btnLoeschen->setVisible(false);
        ui->tbVorhanden->setVisible(false);
        ui->lblVorhanden->setVisible(false);
        ui->lblEinheit2->setVisible(false);
        ui->cbZugabezeitpunkt->setEnabled(false);
        ui->cbEntnahme->setEnabled(false);
        ui->wdgKochdauer->setVisible(false);
        ui->tbMenge->setReadOnly(true);
        ui->tbDauerMin->setReadOnly(true);
        ui->tbZugabeNach->setReadOnly(true);
        ui->tbDatumVon->setReadOnly(true);
        ui->tbDauerTage->setReadOnly(true);
        ui->tbDatumBis->setReadOnly(true);
    }
    ui->cbEntnahme->setVisible(typ != EWZ_Typ_Hopfen);
}

void WdgWeitereZutatGabe::updateValues(bool full)
{
    QString name = data("Name").toString();
    int typ = data("Typ").toInt();
    int zeitpunkt = data("Zeitpunkt").toInt();
    int entnahme = data("Entnahmeindex").toInt();
    int einheit = data("Einheit").toInt();
    int status = data("Zugabestatus").toInt();
    int dauer = data("Zugabedauer").toInt();

    checkEnabled(full);

    if (!ui->cbZutat->hasFocus())
    {
        ui->cbZutat->setCurrentIndex(-1);
        ui->cbZutat->setCurrentText(name);
    }
    if (!ui->tbMenge->hasFocus())
    {
        if (einheit == EWZ_Einheit_mg)
            ui->tbMenge->setValue(data("Menge").toDouble() * 1000);
        else
            ui->tbMenge->setValue(data("Menge").toDouble());
    }
    if (!ui->tbMengeTotal->hasFocus())
    {
        switch (einheit)
        {
        case EWZ_Einheit_Kg:
            ui->lblEinheit->setText(tr("kg"));
            ui->lblEinheit2->setText(tr("kg"));
            ui->lblEinheitProLiter->setText(tr("g/l"));
            ui->tbMengeTotal->setDecimals(2);
            ui->tbVorhanden->setDecimals(2);
            ui->tbMengeTotal->setValue(data("erg_Menge").toDouble() / 1000);
            break;
        case EWZ_Einheit_g:
            ui->lblEinheit->setText(tr("g"));
            ui->lblEinheit2->setText(tr("g"));
            ui->lblEinheitProLiter->setText(tr("g/l"));
            ui->tbMengeTotal->setDecimals(0);
            ui->tbVorhanden->setDecimals(0);
            ui->tbMengeTotal->setValue(data("erg_Menge").toDouble());
            break;
        case EWZ_Einheit_mg:
            ui->lblEinheit->setText(tr("mg"));
            ui->lblEinheit2->setText(tr("mg"));
            ui->lblEinheitProLiter->setText(tr("mg/l"));
            ui->tbMengeTotal->setDecimals(0);
            ui->tbVorhanden->setDecimals(0);
            ui->tbMengeTotal->setValue(data("erg_Menge").toDouble() * 1000);
            break;
        case EWZ_Einheit_Stk:
            ui->lblEinheit->setText(tr("Stk."));
            ui->lblEinheit2->setText(tr("Stk."));
            ui->lblEinheitProLiter->setText(tr("Stk./l"));
            ui->tbMengeTotal->setDecimals(1);
            ui->tbVorhanden->setDecimals(0);
            ui->tbMengeTotal->setValue(data("erg_Menge").toDouble());
            break;
        }
    }
    if (!ui->cbZugabezeitpunkt->hasFocus())
        ui->cbZugabezeitpunkt->setCurrentIndex(zeitpunkt);
    if (!ui->tbDauerMin->hasFocus())
    {
        ui->tbDauerMin->setMinimum(-bh->sud()->getNachisomerisierungszeit());
        ui->tbDauerMin->setMaximum(bh->sud()->getKochdauerNachBitterhopfung());
        ui->tbDauerMin->setValue(dauer);
    }
    ui->cbEntnahme->setChecked(entnahme == EWZ_Entnahmeindex_KeineEntnahme);
    if (!ui->tbZugabeNach->hasFocus())
        ui->tbZugabeNach->setValue(data("ZugabeNach").toInt());
    if (!ui->tbDauerTage->hasFocus())
        ui->tbDauerTage->setValue(dauer / 1440);

    QDateTime braudatum = bh->sud()->getBraudatum();
    if (braudatum.isValid())
    {
        ui->tbDatumVon->setMinimumDateTime(braudatum);
        if (!ui->tbDatumVon->hasFocus())
            ui->tbDatumVon->setDate(data("ZugabeDatum").toDate());
        ui->tbDatumBis->setMinimumDateTime(ui->tbDatumVon->dateTime());
        if (!ui->tbDatumBis->hasFocus())
            ui->tbDatumBis->setDate(data("EntnahmeDatum").toDate());
    }
    ui->tbDatumVon->setVisible(braudatum.isValid());

    if (!ui->tbKomentar->hasFocus())
        ui->tbKomentar->setText(data("Bemerkung").toString());

    if (typ == EWZ_Typ_Hopfen)
    {
        int idx = bh->modelHopfen()->getValueFromSameRow("Beschreibung", name, "Typ").toInt();
        if (idx >= 0 && idx < gSettings->HopfenTypBackgrounds.count())
        {
            QPalette pal = ui->frameColor->palette();
            pal.setColor(QPalette::Background, gSettings->HopfenTypBackgrounds[idx]);
            ui->frameColor->setPalette(pal);
        }
        else
        {
            ui->frameColor->setPalette(gSettings->palette);
        }
    }
    else
    {
        if (typ >= 0 && typ < gSettings->WZTypBackgrounds.count())
        {
            QPalette pal = ui->frameColor->palette();
            pal.setColor(QPalette::Background, gSettings->WZTypBackgrounds[typ]);
            ui->frameColor->setPalette(pal);
        }
        else
        {
            ui->frameColor->setPalette(gSettings->palette);
        }
    }

    if (mEnabled)
    {
        if (typ == EWZ_Typ_Hopfen)
            ui->tbVorhanden->setValue(bh->modelHopfen()->getValueFromSameRow("Beschreibung", name, "Menge").toInt());
        else
            ui->tbVorhanden->setValue(bh->modelWeitereZutaten()->getValueFromSameRow("Beschreibung", name, "Menge").toDouble());
        double benoetigt = 0.0;
        ProxyModel* model = bh->sud()->modelWeitereZutatenGaben();
        for (int i = 0; i < model->rowCount(); ++i)
        {
            if (model->data(i, "Name").toString() == name)
                benoetigt += model->data(i, "erg_Menge").toDouble();
        }
        if (einheit == EWZ_Einheit_Kg)
            benoetigt /= 1000;
        else if (einheit == EWZ_Einheit_mg)
            benoetigt *= 1000;
        ui->tbVorhanden->setError(benoetigt > ui->tbVorhanden->value());

        ui->btnEntnehmen->setPalette(gSettings->palette);
        switch (status)
        {
        case EWZ_Zugabestatus_nichtZugegeben:
            ui->tbVorhanden->setVisible(true);
            ui->lblVorhanden->setVisible(true);
            ui->lblEinheit2->setVisible(true);
            ui->btnLoeschen->setVisible(true);
            ui->tbZugabeNach->setReadOnly(false);
            ui->tbDatumVon->setReadOnly(false);
            ui->tbDatumBis->setReadOnly(false);
            ui->tbDauerTage->setReadOnly(false);
            ui->cbEntnahme->setEnabled(true);
            ui->tbMenge->setReadOnly(false);
            ui->cbZutat->setEnabled(true);
            break;
        case EWZ_Zugabestatus_Zugegeben:
            ui->tbVorhanden->setVisible(false);
            ui->lblVorhanden->setVisible(false);
            ui->lblEinheit2->setVisible(false);
            ui->btnLoeschen->setVisible(false);
            ui->tbZugabeNach->setReadOnly(true);
            ui->tbDatumVon->setReadOnly(true);
            ui->tbDatumBis->setReadOnly(false);
            ui->tbDauerTage->setReadOnly(false);
            ui->cbEntnahme->setEnabled(true);
            ui->tbMenge->setReadOnly(true);
            ui->cbZutat->setEnabled(false);
            if (zeitpunkt == EWZ_Zeitpunkt_Gaerung)
            {
                QDate currentDate = QDate::currentDate();
                QDate dateBisSoll = data("ZugabeDatum").toDate().addDays(dauer / 1440);
                if (currentDate >= dateBisSoll)
                {
                    ui->btnEntnehmen->setPalette(gSettings->paletteErrorButton);
                }
            }
            break;
        case EWZ_Zugabestatus_Entnommen:
            ui->tbVorhanden->setVisible(false);
            ui->lblVorhanden->setVisible(false);
            ui->lblEinheit2->setVisible(false);
            ui->btnLoeschen->setVisible(false);
            ui->tbZugabeNach->setReadOnly(true);
            ui->tbDatumVon->setReadOnly(true);
            ui->tbDatumBis->setReadOnly(true);
            ui->tbDauerTage->setReadOnly(true);
            ui->cbEntnahme->setEnabled(false);
            ui->tbMenge->setReadOnly(true);
            ui->cbZutat->setEnabled(false);
            break;
        }
    }

    if (gSettings->ForceEnabled)
    {
        ui->tbVorhanden->setVisible(true);
        ui->lblVorhanden->setVisible(true);
        ui->lblEinheit2->setVisible(true);
        ui->btnLoeschen->setVisible(true);
        ui->tbZugabeNach->setReadOnly(false);
        ui->tbDatumVon->setReadOnly(false);
        ui->tbDatumBis->setReadOnly(false);
        ui->tbDauerTage->setReadOnly(false);
        ui->cbEntnahme->setEnabled(true);
        ui->tbMenge->setReadOnly(false);
        ui->cbZutat->setEnabled(true);
    }

    switch (zeitpunkt)
    {
    case EWZ_Zeitpunkt_Gaerung:
        ui->wdgKochdauer->setVisible(false);
        break;
    case EWZ_Zeitpunkt_Kochen:
        ui->wdgKochdauer->setVisible(true);
        break;
    case EWZ_Zeitpunkt_Maischen:
        ui->wdgKochdauer->setVisible(false);
        break;
    }
    ui->wdgZugabezeitpunkt->setVisible(typ != EWZ_Typ_Hopfen);
    ui->wdgZugabe->setVisible(zeitpunkt == EWZ_Zeitpunkt_Gaerung);
    ui->lblEntnahme->setVisible(entnahme == EWZ_Entnahmeindex_MitEntnahme);
    ui->tbDauerTage->setVisible(entnahme == EWZ_Entnahmeindex_MitEntnahme);
    ui->lblDauerTage->setVisible(entnahme == EWZ_Entnahmeindex_MitEntnahme);
    ui->tbDatumBis->setVisible(braudatum.isValid() && entnahme == EWZ_Entnahmeindex_MitEntnahme);
    ui->btnZugeben->setVisible(bh->sud()->getStatus() == Sud_Status_Gebraut && status == EWZ_Zugabestatus_nichtZugegeben);
    ui->btnEntnehmen->setVisible(bh->sud()->getStatus() == Sud_Status_Gebraut && status == EWZ_Zugabestatus_Zugegeben && entnahme == EWZ_Entnahmeindex_MitEntnahme);
    ui->cbZugabezeitpunkt->setEnabled(bh->sud()->getStatus() == Sud_Status_Rezept || gSettings->ForceEnabled);
}

void WdgWeitereZutatGabe::remove()
{
    bh->sud()->modelWeitereZutatenGaben()->removeRow(mIndex);
}

void WdgWeitereZutatGabe::on_cbZutat_currentIndexChanged(const QString &text)
{
    if (ui->cbZutat->hasFocus())
        setData("Name", text);
}

void WdgWeitereZutatGabe::on_tbMenge_valueChanged(double value)
{
    if (ui->tbMenge->hasFocus())
    {
        if (data("Einheit").toInt() == EWZ_Einheit_mg)
            setData("Menge", value / 1000);
        else
            setData("Menge", value);
    }
}

void WdgWeitereZutatGabe::on_cbZugabezeitpunkt_currentIndexChanged(int index)
{
    if (ui->cbZugabezeitpunkt->hasFocus())
        setData("Zeitpunkt", index);
}

void WdgWeitereZutatGabe::on_tbDauerMin_valueChanged(int value)
{
    if (ui->tbDauerMin->hasFocus())
        setData("Zugabedauer", value);
}

void WdgWeitereZutatGabe::on_btnZugeben_clicked()
{
    QDate currentDate = QDate::currentDate();
    QDate date = ui->tbDatumVon->date();
    setData("ZugabeDatum", currentDate < date ? currentDate : date);
    setData("Zugabestatus", EWZ_Zugabestatus_Zugegeben);
    if (QMessageBox::question(this, tr("Zutat vom Bestand abziehen"),
                              tr("Soll die Zutat vom Bestand abgezogen werden?")
       ) == QMessageBox::Yes)
        bh->sud()->zutatAbziehen(data("Name").toString(), data("Typ").toInt() == EWZ_Typ_Hopfen ? 0 : 2, data("erg_Menge").toDouble());
}

void WdgWeitereZutatGabe::on_cbEntnahme_clicked(bool checked)
{
    setData("Entnahmeindex", checked);
}

void WdgWeitereZutatGabe::on_tbZugabeNach_valueChanged(int value)
{
    if (ui->tbZugabeNach->hasFocus())
        setData("ZugabeNach", value);
}

void WdgWeitereZutatGabe::on_tbDauerTage_valueChanged(int value)
{
    if (ui->tbDauerTage->hasFocus())
        setData("Zugabedauer", value * 1440);
}

void WdgWeitereZutatGabe::on_tbDatumVon_dateChanged(const QDate &date)
{
    if (ui->tbDatumVon->hasFocus())
        setData("ZugabeDatum", date);
}

void WdgWeitereZutatGabe::on_tbDatumBis_dateChanged(const QDate &date)
{
    if (ui->tbDatumBis->hasFocus())
        setData("EntnahmeDatum", date);
}

void WdgWeitereZutatGabe::on_btnEntnehmen_clicked()
{
    QDate currentDate = QDate::currentDate();
    QDate date = ui->tbDatumBis->date();
    setData("EntnahmeDatum", currentDate < date ? currentDate : date);
    setData("Zugabestatus", EWZ_Zugabestatus_Entnommen);
}

void WdgWeitereZutatGabe::on_tbKomentar_textChanged()
{
    if (ui->tbKomentar->hasFocus())
        setData("Bemerkung", ui->tbKomentar->toPlainText());
}

void WdgWeitereZutatGabe::on_btnLoeschen_clicked()
{
    remove();
}
