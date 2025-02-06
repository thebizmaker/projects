#include "buchungsdetails.h"
#include "ui_buchungsdetails.h"



BuchungsDetails::BuchungsDetails(shared_ptr<TravelAgency> travelAgency, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BuchungsDetails)
    , travelAgency(travelAgency)
{
    ui->setupUi(this);
}


BuchungsDetails::~BuchungsDetails()
{
    delete ui;
}


void BuchungsDetails::setDetails(QString fromDate, QString toDate, double preis)
{
    QDate* fDate = new QDate(QDate::fromString(fromDate, "yyyyMMdd"));
    QDate* tDate = new QDate(QDate::fromString(toDate, "yyyyMMdd"));

    ui->lineEdit_id->setReadOnly(true);
    ui->doubleSpinBox->setMaximum(15000);

    for(auto& booking : travelAgency->getAllBookings())
    {
        if(fromDate.toStdString()==booking->getFromDate() && toDate.toStdString()==booking->getToDate() && preis==booking->getPrice())
        {
            if(booking->showDetails()[0]=='H')
            {
                ui->tabWidget->setCurrentIndex(2);
                shared_ptr<HotelBooking> hotelBuchung = dynamic_pointer_cast<HotelBooking>(booking);
                ui->lineEdit_id->setText(QString::fromStdString(hotelBuchung->getId()));
                ui->dateEdit->setDate(*fDate);
                ui->dateEdit_2->setDate(*tDate);
                ui->doubleSpinBox->setValue(preis);
                ui->lineEdit_hotel->setText(QString::fromStdString(hotelBuchung->getHotel()));
                ui->lineEdit_town->setText(QString::fromStdString(hotelBuchung->getTown()));
                ui->lineEdit_roomtype->setText(QString::fromStdString(hotelBuchung->getRoomType()));
                if(hotelBuchung->getRoomType()=="EZ")
                    ui->lineEdit_roomtype->setText("Einzelzimmer");
                else if(hotelBuchung->getRoomType()=="DZ")
                    ui->lineEdit_roomtype->setText("Doppelzimmer");
                else if(hotelBuchung->getRoomType()=="SU")
                    ui->lineEdit_roomtype->setText("Suite");
                else if(hotelBuchung->getRoomType()=="AP")
                    ui->lineEdit_roomtype->setText("Appartment");
                else
                    ui->lineEdit_roomtype->setText("Unbekannter Zimmertyp");
            }
            else if(booking->showDetails()[0]=='F')
            {
                ui->tabWidget->setCurrentIndex(0);
                ui->lineEdit_Flug_Von->setReadOnly(true);
                ui->lineEdit_Flug_Nach->setReadOnly(true);
                shared_ptr<FlightBooking> flugBuchung = dynamic_pointer_cast<FlightBooking>(booking);
                ui->lineEdit_id->setText(QString::fromStdString(flugBuchung->getId()));
                ui->dateEdit->setDate(*fDate);
                ui->dateEdit_2->setDate(*tDate);
                ui->doubleSpinBox->setValue(preis);
                ui->lineEdit_fDest_Flug->setText(QString::fromStdString(flugBuchung->getFromDest()));
                ui->lineEdit_tDest_Flug->setText(QString::fromStdString(flugBuchung->getToDest()));
                shared_ptr<Airport> airport1 = travelAgency->findAirport(flugBuchung->getFromDest());
                shared_ptr<Airport> airport2 = travelAgency->findAirport(flugBuchung->getToDest());
                if(airport1)
                    ui->lineEdit_Flug_Von->setText(QString::fromStdString(airport1->getName()));
                if(airport2)
                    ui->lineEdit_Flug_Nach->setText(QString::fromStdString(airport2->getName()));
                ui->lineEdit_airline->setText(QString::fromStdString(flugBuchung->getAirline()));

                if(flugBuchung->getBookingClass()=="Y")
                    ui->lineEdit_bookinglass->setText("Economy");
                else if(flugBuchung->getBookingClass()=="W")
                    ui->lineEdit_bookinglass->setText("Premium Economy");
                else if(flugBuchung->getBookingClass()=="J")
                    ui->lineEdit_bookinglass->setText("Business");
                else if(flugBuchung->getBookingClass()=="F")
                    ui->lineEdit_bookinglass->setText("First");
                else
                    ui->lineEdit_bookinglass->setText("Unbekannte Klasse");
            }
            else if(booking->showDetails()[0]=='M')
            {
                ui->tabWidget->setCurrentIndex(1);
                shared_ptr<RentalCarReservation> mietWagenReservierung = dynamic_pointer_cast<RentalCarReservation>(booking);
                ui->lineEdit_id->setText(QString::fromStdString(mietWagenReservierung->getId()));
                ui->dateEdit->setDate(*fDate);
                ui->dateEdit_2->setDate(*tDate);
                ui->doubleSpinBox->setValue(preis);
                ui->lineEdit_fromStation->setText(QString::fromStdString(mietWagenReservierung->getPickupLocation()));
                ui->lineEdit_toStation->setText(QString::fromStdString(mietWagenReservierung->getReturnLocation()));
                ui->lineEdit_company->setText(QString::fromStdString(mietWagenReservierung->getCompany()));
                ui->lineEdit_vehicleclass->setText(QString::fromStdString(mietWagenReservierung->getVehicleClass()));
            }
            else if(booking->showDetails()[0]=='Z')
            {
                ui->tabWidget->setCurrentIndex(3);
                shared_ptr<TrainTicket> zugBuchung = dynamic_pointer_cast<TrainTicket>(booking);
                ui->lineEdit_id->setText(QString::fromStdString(zugBuchung->getId()));
                ui->dateEdit->setDate(*fDate);
                ui->dateEdit_2->setDate(*tDate);
                ui->doubleSpinBox->setValue(preis);
                ui->lineEdit_fDest_Zug->setText(QString::fromStdString(zugBuchung->getFromDest()));
                ui->lineEdit_tDest_Zug->setText(QString::fromStdString(zugBuchung->getToDest()));
                QTime* abfahrt = new QTime(QTime::fromString(QString::fromStdString(zugBuchung->getDepartureTime()),"hh:mm"));
                QTime* ankunft = new QTime(QTime::fromString(QString::fromStdString(zugBuchung->getArrivalTime()),"hh:mm"));
                ui->timeEdit_abfahrt->setTime(*abfahrt);
                ui->timeEdit_ankunft->setTime(*ankunft);
                for(const auto& station : zugBuchung->getConnectingStations()){
                    ui->listWidget_ueber->addItem(QString::fromStdString(station.first));
                }
                if(zugBuchung->getBookingClass()=="SSP1")
                    ui->lineEdit_tickettyp->setText("Supersparpreis 1. Klasse");
                else if(zugBuchung->getBookingClass()=="SSP2")
                    ui->lineEdit_tickettyp->setText("Supersparpreis 2. Klasse");
                else if(zugBuchung->getBookingClass()=="SP1")
                    ui->lineEdit_tickettyp->setText("Sparpreis 1. Klasse");
                else if(zugBuchung->getBookingClass()=="SP2")
                    ui->lineEdit_tickettyp->setText("Sparpreis 2. Klasse");
                else if(zugBuchung->getBookingClass()=="FP1")
                    ui->lineEdit_tickettyp->setText("Flexpreis 1. Klasse");
                else if(zugBuchung->getBookingClass()=="FP2")
                    ui->lineEdit_tickettyp->setText("Flexpreis 2. Klasse");
                else
                    ui->lineEdit_tickettyp->setText("Unbekannte Klasse");

                delete abfahrt;
                delete ankunft;
            }
        }
    }

    delete fDate;
    delete tDate;
    ui->buttonBox->setEnabled(false);
}

void BuchungsDetails::on_buttonBox_accepted()
{
    shared_ptr<Booking> booking = travelAgency->findBooking(ui->lineEdit_id->text().toStdString());

    if(booking->showDetails()[0]=='H')
    {
        shared_ptr<HotelBooking> hotelbuchung = dynamic_pointer_cast<HotelBooking>(booking);
        hotelbuchung->setHotel(ui->lineEdit_hotel->text().toStdString());
        hotelbuchung->setTown(ui->lineEdit_town->text().toStdString());
        if(ui->lineEdit_roomtype->text()=="Einzelzimmer")
            hotelbuchung->setRoomType("EZ");
        else if(ui->lineEdit_roomtype->text()=="Doppelzimmer")
            hotelbuchung->setRoomType("DZ");
        else if(ui->lineEdit_roomtype->text()=="Suite")
            hotelbuchung->setRoomType("SU");
        else if(ui->lineEdit_roomtype->text()=="Appartment")
            hotelbuchung->setRoomType("AP");
    }
    else if(booking->showDetails()[0]=='F')
    {
        shared_ptr<FlightBooking> flugBuchung = dynamic_pointer_cast<FlightBooking>(booking);
        if(ui->lineEdit_Flug_Von->text()=="Ungültiger Iata-Code" || ui->lineEdit_Flug_Nach->text()=="Ungültiger Iata-Code")
            return;
        else
        {
            flugBuchung->setFromDest(ui->lineEdit_fDest_Flug->text().toStdString());
            flugBuchung->setToDest(ui->lineEdit_tDest_Flug->text().toStdString());
        }
        flugBuchung->setToDest(ui->lineEdit_tDest_Flug->text().toStdString());
        flugBuchung->setAirline(ui->lineEdit_airline->text().toStdString());
        if(ui->lineEdit_bookinglass->text()=="Economy")
            flugBuchung->setBookingClass("Y");
        else if(ui->lineEdit_bookinglass->text()=="Premium Economy")
            flugBuchung->setBookingClass("W");
        else if(ui->lineEdit_bookinglass->text()=="Business")
            flugBuchung->setBookingClass("J");
        else if(ui->lineEdit_bookinglass->text()=="First")
            flugBuchung->setBookingClass("F");
    }
    else if(booking->showDetails()[0]=='M')
    {
        shared_ptr<RentalCarReservation> mietwagenReservierung = dynamic_pointer_cast<RentalCarReservation>(booking);
        mietwagenReservierung->setPickupLocation(ui->lineEdit_fromStation->text().toStdString());
        mietwagenReservierung->setReturnLocation(ui->lineEdit_toStation->text().toStdString());
        mietwagenReservierung->setCompany(ui->lineEdit_company->text().toStdString());
        mietwagenReservierung->setVehicleClass(ui->lineEdit_vehicleclass->text().toStdString());
    }
    else if(booking->showDetails()[0]=='Z')
    {
        shared_ptr<TrainTicket> zugBuchung = dynamic_pointer_cast<TrainTicket>(booking);
        zugBuchung->setFromDest(ui->lineEdit_fDest_Zug->text().toStdString());
        zugBuchung->setToDest(ui->lineEdit_tDest_Zug->text().toStdString());
        zugBuchung->setDepartureTime(ui->timeEdit_abfahrt->time().toString().toStdString());
        zugBuchung->setArrivalTime(ui->timeEdit_ankunft->time().toString().toStdString());
        if(ui->lineEdit_tickettyp->text()=="Supersparpreis 1. Klasse")
            zugBuchung->setBookingClass("SSP1");
        else if(ui->lineEdit_tickettyp->text()=="Supersparpreis 2. Klasse")
            zugBuchung->setBookingClass("SSP2");
        else if(ui->lineEdit_tickettyp->text()=="Sparpreis 1. Klasse")
            zugBuchung->setBookingClass("SP1");
        else if(ui->lineEdit_tickettyp->text()=="Sparpreis 2. Klasse")
            zugBuchung->setBookingClass("SP2");
        else if(ui->lineEdit_tickettyp->text()=="Flexpreis 1. Klasse")
            zugBuchung->setBookingClass("FP1");
        else if(ui->lineEdit_tickettyp->text()=="Flexpreis 2. Klasse")
            zugBuchung->setBookingClass("FP1");
        // map<string, pair<double,double>> stations;
        // for(size_t i=0; i<zugBuchung->getConnectingStations().size(); i++)
        // {
        //     stations.push_back(ui->listWidget_ueber->item(i)->text().toStdString());
        // }
        // zugBuchung->setConnectingStations(stations);
    }
    booking->setPrice(ui->doubleSpinBox->value());
    booking->setFromDate(ui->dateEdit->date().toString("yyyyMMdd").toStdString());
    booking->setToDate(ui->dateEdit_2->date().toString("yyyyMMdd").toStdString());

    emit this->checkSignal();
    ui->buttonBox->setEnabled(false);
}

void BuchungsDetails::on_lineEdit_fDest_Zug_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_buttonBox_rejected()
{
    ui->lineEdit_Flug_Von->setStyleSheet("color: black;");
    ui->lineEdit_Flug_Nach->setStyleSheet("color: black;");
    shared_ptr<Booking> booking = travelAgency->findBooking(ui->lineEdit_id->text().toStdString());

    double preis = booking->getPrice();
    QDate* fDate = new QDate(QDate::fromString(QString::fromStdString(booking->getFromDate()), "yyyyMMdd"));
    QDate* tDate = new QDate(QDate::fromString(QString::fromStdString(booking->getToDate()), "yyyyMMdd"));
    ui->dateEdit->setDate(*fDate);
    ui->dateEdit_2->setDate(*tDate);
    ui->doubleSpinBox->setValue(preis);

    if(booking->showDetails()[0]=='H')
    {
        ui->tabWidget->setCurrentIndex(2);
        shared_ptr<HotelBooking> hotelBuchung = dynamic_pointer_cast<HotelBooking>(booking);

        ui->lineEdit_hotel->setText(QString::fromStdString(hotelBuchung->getHotel()));
        ui->lineEdit_town->setText(QString::fromStdString(hotelBuchung->getTown()));
        ui->lineEdit_roomtype->setText(QString::fromStdString(hotelBuchung->getRoomType()));
        if(hotelBuchung->getRoomType()=="EZ")
            ui->lineEdit_roomtype->setText("Einzelzimmer");
        else if(hotelBuchung->getRoomType()=="DZ")
            ui->lineEdit_roomtype->setText("Doppelzimmer");
        else if(hotelBuchung->getRoomType()=="SU")
            ui->lineEdit_roomtype->setText("Suite");
        else if(hotelBuchung->getRoomType()=="AP")
            ui->lineEdit_roomtype->setText("Appartment");
        else
            ui->lineEdit_roomtype->setText("Unbekannter Zimmertyp");
    }
    else if(booking->showDetails()[0]=='F')
    {
        ui->tabWidget->setCurrentIndex(0);
        shared_ptr<FlightBooking> flugBuchung = dynamic_pointer_cast<FlightBooking>(booking);

        ui->lineEdit_fDest_Flug->setText(QString::fromStdString(flugBuchung->getFromDest()));
        ui->lineEdit_tDest_Flug->setText(QString::fromStdString(flugBuchung->getToDest()));
        shared_ptr<Airport> airport1 = travelAgency->findAirport(flugBuchung->getFromDest());
        shared_ptr<Airport> airport2 = travelAgency->findAirport(flugBuchung->getToDest());
        if(airport1)
            ui->lineEdit_Flug_Von->setText(QString::fromStdString(airport1->getName()));
        if(airport2)
            ui->lineEdit_Flug_Nach->setText(QString::fromStdString(airport2->getName()));
        ui->lineEdit_airline->setText(QString::fromStdString(flugBuchung->getAirline()));

        if(flugBuchung->getBookingClass()=="Y")
            ui->lineEdit_bookinglass->setText("Economy");
        else if(flugBuchung->getBookingClass()=="W")
            ui->lineEdit_bookinglass->setText("Premium Economy");
        else if(flugBuchung->getBookingClass()=="J")
            ui->lineEdit_bookinglass->setText("Business");
        else if(flugBuchung->getBookingClass()=="F")
            ui->lineEdit_bookinglass->setText("First");
        else
            ui->lineEdit_bookinglass->setText("Unbekannte Klasse");
    }
    else if(booking->showDetails()[0]=='M')
    {
        ui->tabWidget->setCurrentIndex(1);
        shared_ptr<RentalCarReservation> mietWagenReservierung = dynamic_pointer_cast<RentalCarReservation>(booking);

        ui->lineEdit_fromStation->setText(QString::fromStdString(mietWagenReservierung->getPickupLocation()));
        ui->lineEdit_toStation->setText(QString::fromStdString(mietWagenReservierung->getReturnLocation()));
        ui->lineEdit_company->setText(QString::fromStdString(mietWagenReservierung->getCompany()));
        ui->lineEdit_vehicleclass->setText(QString::fromStdString(mietWagenReservierung->getVehicleClass()));
    }
    else if(booking->showDetails()[0]=='Z')
    {
        ui->tabWidget->setCurrentIndex(3);
        shared_ptr<TrainTicket> zugBuchung = dynamic_pointer_cast<TrainTicket>(booking);

        ui->lineEdit_fDest_Zug->setText(QString::fromStdString(zugBuchung->getFromDest()));
        ui->lineEdit_tDest_Zug->setText(QString::fromStdString(zugBuchung->getToDest()));
        QTime* abfahrt = new QTime(QTime::fromString(QString::fromStdString(zugBuchung->getDepartureTime()),"hh:mm"));
        QTime* ankunft = new QTime(QTime::fromString(QString::fromStdString(zugBuchung->getArrivalTime()),"hh:mm"));
        ui->timeEdit_abfahrt->setTime(*abfahrt);
        ui->timeEdit_ankunft->setTime(*ankunft);
        for(const auto& station : zugBuchung->getConnectingStations()){
            ui->listWidget_ueber->addItem(QString::fromStdString(station.first));
        }
        if(zugBuchung->getBookingClass()=="SSP1")
            ui->lineEdit_tickettyp->setText("Supersparpreis 1. Klasse");
        else if(zugBuchung->getBookingClass()=="SSP2")
            ui->lineEdit_tickettyp->setText("Supersparpreis 2. Klasse");
        else if(zugBuchung->getBookingClass()=="SP1")
            ui->lineEdit_tickettyp->setText("Sparpreis 1. Klasse");
        else if(zugBuchung->getBookingClass()=="SP2")
            ui->lineEdit_tickettyp->setText("Sparpreis 2. Klasse");
        else if(zugBuchung->getBookingClass()=="FP1")
            ui->lineEdit_tickettyp->setText("Flexpreis 1. Klasse");
        else if(zugBuchung->getBookingClass()=="FP2")
            ui->lineEdit_tickettyp->setText("Flexpreis 2. Klasse");
        else
            ui->lineEdit_tickettyp->setText("Unbekannte Klasse");

        delete abfahrt;
        delete ankunft;
    }
    delete fDate;
    delete tDate;
    ui->buttonBox->setEnabled(false);
}


void BuchungsDetails::on_doubleSpinBox_valueChanged()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_dateEdit_userDateChanged()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_dateEdit_2_userDateChanged()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_timeEdit_abfahrt_userTimeChanged()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_timeEdit_ankunft_userTimeChanged()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_tDest_Zug_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_tickettyp_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_hotel_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_town_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_roomtype_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_fromStation_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_toStation_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_company_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_vehicleclass_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_fDest_Flug_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_tDest_Flug_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_airline_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_bookinglass_textEdited()
{
    ui->buttonBox->setEnabled(true);
}


void BuchungsDetails::on_lineEdit_fDest_Flug_textChanged(const QString &arg1)
{
    shared_ptr<Airport> airport = travelAgency->findAirport(arg1.toStdString());

    if(airport)
    {
        ui->lineEdit_Flug_Von->setStyleSheet("color: black;");
        ui->lineEdit_Flug_Von->setText(QString::fromStdString(airport->getName()));
    }
    else
    {
        ui->lineEdit_Flug_Von->setStyleSheet("color: red;");
        ui->lineEdit_Flug_Von->setText(tr("Ungültiger Iata-Code"));
    }
}


void BuchungsDetails::on_lineEdit_tDest_Flug_textChanged(const QString &arg1)
{
    shared_ptr<Airport> airport = travelAgency->findAirport(arg1.toStdString());

    if(airport)
    {
        ui->lineEdit_Flug_Nach->setStyleSheet("color: black;");
        ui->lineEdit_Flug_Nach->setText(QString::fromStdString(airport->getName()));
    }
    else
    {
        ui->lineEdit_Flug_Nach->setStyleSheet("color: red;");
        ui->lineEdit_Flug_Nach->setText(tr("Ungültiger Iata-Code"));
    }
}

