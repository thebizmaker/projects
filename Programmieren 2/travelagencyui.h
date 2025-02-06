#ifndef TRAVELAGENCYUI_H
#define TRAVELAGENCYUI_H
#include "travelagency.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QInputDialog>
#include <QDate>
#include <QLocale>
#include <QTableWidget>
#include <QIcon>
#include <QDialogButtonBox>

#include "buchungsdetails.h"
#include "meldungen.h"
#include "check.h"
#include "settings.h"
// #include "ui_buchungsdetails.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class TravelAgencyUI;
}
QT_END_NAMESPACE

class TravelAgencyUI : public QMainWindow
{
    Q_OBJECT

public:
    TravelAgencyUI(shared_ptr<TravelAgency> travelAgency, QWidget *parent = nullptr);
    ~TravelAgencyUI();
    void read_iatacodes();

private slots:
    void on_actionEinlesen_triggered();

    void on_actionSuchen_triggered();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_tableWidget_2_itemDoubleClicked(QTableWidgetItem *item);

    void on_actionSpeichern_triggered();

    void slotCheck();

    void on_actionFehlerhafte_Buchungen_triggered();

    void on_actionKonsistenzchecks_triggered();

private:
    Ui::TravelAgencyUI *ui;
    shared_ptr<TravelAgency> travelAgency;
    BuchungsDetails* buchungsDetails;
    Meldungen* meldungen;
    Settings* settings;
    shared_ptr<Check> check;
};
#endif // TRAVELAGENCYUI_H
