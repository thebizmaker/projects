#ifndef BUCHUNGSDETAILS_H
#define BUCHUNGSDETAILS_H

#include <QWidget>
#include "travelagency.h"


namespace Ui {
class BuchungsDetails;
}

class BuchungsDetails : public QWidget
{
    Q_OBJECT

public:
    explicit BuchungsDetails(shared_ptr<TravelAgency> travelAgency, QWidget *parent = nullptr);
    ~BuchungsDetails();

    void setDetails(QString fDate, QString tDate, double preis);
    void emitCheckSignal(){emit checkSignal();}
signals:
    void checkSignal();

private slots:

    void on_buttonBox_accepted();

    void on_lineEdit_fDest_Zug_textEdited();

    void on_buttonBox_rejected();

    void on_doubleSpinBox_valueChanged();

    void on_dateEdit_userDateChanged();

    void on_dateEdit_2_userDateChanged();

    void on_timeEdit_abfahrt_userTimeChanged();

    void on_timeEdit_ankunft_userTimeChanged();

    void on_lineEdit_tDest_Zug_textEdited();

    void on_lineEdit_tickettyp_textEdited();

    void on_lineEdit_hotel_textEdited();

    void on_lineEdit_town_textEdited();

    void on_lineEdit_roomtype_textEdited();

    void on_lineEdit_fromStation_textEdited();

    void on_lineEdit_toStation_textEdited();

    void on_lineEdit_company_textEdited();

    void on_lineEdit_vehicleclass_textEdited();

    void on_lineEdit_fDest_Flug_textEdited();

    void on_lineEdit_tDest_Flug_textEdited();

    void on_lineEdit_airline_textEdited();

    void on_lineEdit_bookinglass_textEdited();

    void on_lineEdit_fDest_Flug_textChanged(const QString &arg1);

    void on_lineEdit_tDest_Flug_textChanged(const QString &arg1);

private:
    Ui::BuchungsDetails *ui;
    shared_ptr<TravelAgency> travelAgency;
};

#endif // BUCHUNGSDETAILS_H
