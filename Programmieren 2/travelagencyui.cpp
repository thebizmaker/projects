#include "travelagencyui.h"
#include "ui_travelagencyui.h"
#include <QFont>
#include <QDesktopServices>
#include <QUrl>
#include <QSize>
#include <map>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QVBoxLayout>



using json = nlohmann::json;

struct Properties { string ort_name;};
struct Geometry{
    vector<double> coordinates;
};
struct Features{
    string type = "Feature";
    Geometry geometry;
    Properties properties;
};

json makeGeoJsonPoint (const Features features, const string booking="")
{
    json properties, feature, smallFeature;
    feature["type"] = features.type;
    if(booking.empty())
        feature["properties"] = json::object();
    else
    {
        properties[booking] = features.properties.ort_name;
        feature["properties"] = properties;
    }
    smallFeature["coordinates"] = features.geometry.coordinates;
    smallFeature["type"] = "Point";
    feature["geometry"] = smallFeature;

    return feature;
}
json makeGeoLineString(const vector<vector<double>> xy, const string bookingsTyp, string fDest, string tDest)
{
    json booking, properties, smallBooking;
    booking["type"] = "Feature";
    properties[bookingsTyp] = "von "+fDest+" nach "+tDest;
    booking["properties"] = properties;
    smallBooking["coordinates"] = xy;
    smallBooking["type"] = "LineString";
    booking["geometry"] = smallBooking;

    return booking;
}

bool findID(int id, vector<int> vec)
{
    for(auto a : vec)
    {
        if(id == a)
            return true;
    }
    return false;
}
string changeDateFormat(string date)
{    
    date.erase(0, date.find(", ")+2);
    string day1=date.substr(0, date.find('.'));
    date.erase(0, date.find('.')+2);
    string month1=date.substr(0, date.find(' '));
    date.erase(0, date.find(' ')+1);
    string year=date;
    string month="";
    string day;
    if(day1.size()==1)
        day="0"+day1;
    else
        day=day1;
    map<string, int> monthsMap{{"Januar",1}, {"Februar",2}, {"März",3}, {"April",4}, {"Mai",5}, {"Juni",6}
    ,{"Juli",7}, {"August",8}, {"September",9}, {"Oktober",10}, {"November",11}, {"Dezember",12}};
    for(const auto &m : monthsMap)
    {
        if(m.second<10)
        {
            if(month1==m.first)
                month="0"+to_string(m.second);
        }
        else
        {
            if(month1==m.first)
                month=to_string(m.second);
        }
    }
    return year+month+day;
}
TravelAgencyUI::TravelAgencyUI(shared_ptr<TravelAgency> travelAgency, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TravelAgencyUI)
    , travelAgency(travelAgency)
{

    check = make_shared<Check>(travelAgency);
    ui->setupUi(this);    
    ui->groupBox_Kunde->setHidden(true);
    ui->groupBox_Reisen->setHidden(true);
    buchungsDetails = new BuchungsDetails(travelAgency);
    connect(buchungsDetails, BuchungsDetails::checkSignal, this, slotCheck);
    meldungen = new Meldungen(travelAgency);
    settings = new Settings(travelAgency);
    connect(settings, Settings::checkSignal, this, slotCheck);
}

TravelAgencyUI::~TravelAgencyUI()
{
    delete ui;
}

void TravelAgencyUI::read_iatacodes()
{
    QString text = "Es wurden "+QString::number(travelAgency->read_Iata())+" gültige Iata-Codes gefunden";
    QFont boldFont = ui->statusbar->font();
    boldFont.setBold(true);
    boldFont.setPointSize(13);
    ui->statusbar->setFont(boldFont);
    ui->statusbar->showMessage(text, 10000);
}

void TravelAgencyUI::on_actionEinlesen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Buchung einlesen"), tr("Beispiel"),"JSON-Files (*.json)");
    try
    {
        QString text = QString::fromStdString(travelAgency->readFile(fileName));

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Datei erfolgreich eingelsen"));
        msgBox.setText(text);
        msgBox.addButton("OK", QMessageBox::AcceptRole);
        msgBox.exec();
        read_iatacodes();
    }
    catch(runtime_error &e)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Fehler beim Einlesen der Buchungen"));
        msgBox.setText(QString::fromStdString(e.what()));
        QString infoText=tr("Das Einlesen der Buchuingen wurde in der betroffenen Zeile gestoppt. Die vorangehenden Buchungen wurden erfolgreich angelegt.");
        msgBox.setInformativeText(infoText);
        QString details = tr("Wenn Sie die Datei bereits korrigiert haben, wählen Sie 'Retry'. Wählen Sie 'Discard', um alle Buchungen zu löschen und 'Cancel', um die vorhandenen Buchungen stehenzulassen");
        msgBox.setDetailedText(details);
        QPushButton* retryButton = msgBox.addButton(tr("Retry"), QMessageBox::AcceptRole);
        msgBox.addButton(tr("Discard"), QMessageBox::DestructiveRole);
        QPushButton* cancelButton = msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
        msgBox.exec();

        if(msgBox.clickedButton()==retryButton)
            on_actionEinlesen_triggered();  
        else if(msgBox.clickedButton()==cancelButton)        
            msgBox.close();
    }
    emit buchungsDetails->emitCheckSignal();
}


void TravelAgencyUI::on_actionSuchen_triggered()
{

    long customerId = QInputDialog::getInt(this,"Kund*innensuche","ID");
    if(!QString::number(customerId).toLong())
        return;
    shared_ptr<Customer> customer = travelAgency->findCustomer(customerId);
    if(!customer){
        ui->statusbar->showMessage(tr("Kund*in nicht gefunden!"),5000);
    }
    else
    {
        ui->groupBox_Kunde->setHidden(false);

        int count = ui->tableWidget->rowCount();
        while(ui->tableWidget->rowCount()>1){
            ui->tableWidget->removeRow(count);
            count--;
        }
        ui->lineEdit_Cust_Id->setText(QString::fromStdString(to_string(customerId)));
        ui->lineEdit_Cust_Id->setReadOnly(true);
        ui->lineEdit_Vname->setText(QString::fromStdString(customer->getFirstName()));
        ui->lineEdit_Vname->setReadOnly(true);
        ui->lineEdit_Nname->setText(QString::fromStdString(customer->getLastName()));
        ui->lineEdit_Nname->setReadOnly(true);
        int row=1, it=-1; vector<int> countId;
        for (auto& travel : customer->getTravelList())
        {
            if(findID(travel->getId(), countId))
               continue;
            {
                it = travel->getId();
                countId.push_back(it);

                QTableWidgetItem* itemId = new QTableWidgetItem();
                itemId->setText(QString::fromStdString(to_string(travel->getId())));
                QTableWidgetItem* itemfDate = new QTableWidgetItem();
                QTableWidgetItem* itemtDate = new QTableWidgetItem();

                QDate* fDate = new QDate(QDate::fromString(QString::fromStdString(travel->calculateDates().first), "yyyyMMdd"));
                itemfDate->setText(QLocale::system().toString(*fDate, "dddd, d. MMMM yyyy"));
                QDate* tDate = new QDate(QDate::fromString(QString::fromStdString(travel->calculateDates().second), "yyyyMMdd"));
                itemtDate->setText(QLocale::system().toString(*tDate, "dddd, d. MMMM yyyy"));


                delete fDate;
                delete tDate;

                ui->tableWidget->insertRow(row);
                ui->tableWidget->setItem(row,0,itemId);
                ui->tableWidget->setItem(row,1,itemfDate);
                ui->tableWidget->setItem(row,2,itemtDate);
                ui->tableWidget->setColumnWidth(0, 80);
                ui->tableWidget->setColumnWidth(1, 160);
                ui->tableWidget->setColumnWidth(2, 160);
                ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);

                row++;
            }
        }
        ui->groupBox_Reisen->setHidden(true);
    }
}


void TravelAgencyUI::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    int count = ui->tableWidget_2->rowCount();
    while(ui->tableWidget_2->rowCount()>1){
        ui->tableWidget_2->removeRow(count);
        count--;
    }
    ui->groupBox_Reisen->setHidden(false);

    int itemRow = item->row();
    long travelId = ui->tableWidget->item(itemRow, 0)->text().toLong();
    ui->lineEdit_Reise_Id->setText(ui->tableWidget->item(itemRow, 0)->text());
    ui->lineEdit_Reise_Id->setReadOnly(true);
    shared_ptr<Travel> travel = travelAgency->findTravel(travelId);
    int row = 1; json featuresJson;
    for(const auto& booking : travel->getTravelBookings())
    {
        if(booking->showDetails()[0]=='H')
        {
            ui->tableWidget_2->insertRow(row);
            QTableWidgetItem* itemIcon = new QTableWidgetItem();
            itemIcon->setIcon(QIcon(":/icons/hotel.png"));
            itemIcon->setSizeHint(QSize(26, 26));
            QTableWidgetItem* itemStart = new QTableWidgetItem();
            QTableWidgetItem* itemEnde = new QTableWidgetItem();
            QTableWidgetItem* itemPreis = new QTableWidgetItem();
            QDate* fromDate = new QDate(QDate::fromString(QString::fromStdString(booking->getFromDate()), "yyyyMMdd"));
            QDate* toDate = new QDate(QDate::fromString(QString::fromStdString(booking->getToDate()), "yyyyMMdd"));
            itemStart->setText(QLocale::system().toString(*fromDate, "dddd, d. MMMM yyyy"));
            itemEnde->setText(QLocale::system().toString(*toDate, "dddd, d. MMMM yyyy"));
            itemPreis->setText(QString::number(booking->getPrice()));
            ui->tableWidget_2->setItem(row,0,itemIcon);
            ui->tableWidget_2->setItem(row,1,itemStart);
            ui->tableWidget_2->setItem(row,2,itemEnde);
            ui->tableWidget_2->setItem(row,3,itemPreis);

            delete fromDate;
            delete toDate;

            shared_ptr<HotelBooking> hotelBuchung = dynamic_pointer_cast<HotelBooking>(booking);
            Features feature;
            feature.properties.ort_name = hotelBuchung->getHotel();
            feature.geometry.coordinates.push_back(hotelBuchung->getHotelLongitude());
            feature.geometry.coordinates.push_back(hotelBuchung->getHotelLatitude());

            featuresJson.push_back(makeGeoJsonPoint(feature,"Hotel"));
        }
        else if(booking->showDetails()[0]=='F')
        {
            ui->tableWidget_2->insertRow(row);
            QTableWidgetItem* itemIcon = new QTableWidgetItem();
            itemIcon->setIcon(QIcon(":/icons/airplane.png"));
            itemIcon->setSizeHint(QSize(26, 26));
            QTableWidgetItem* itemStart = new QTableWidgetItem();
            QTableWidgetItem* itemEnde = new QTableWidgetItem();
            QTableWidgetItem* itemPreis = new QTableWidgetItem();
            QDate* fromDate = new QDate(QDate::fromString(QString::fromStdString(booking->getFromDate()), "yyyyMMdd"));
            QDate* toDate = new QDate(QDate::fromString(QString::fromStdString(booking->getToDate()), "yyyyMMdd"));
            itemStart->setText(QLocale::system().toString(*fromDate, "dddd, d. MMMM yyyy"));
            itemEnde->setText(QLocale::system().toString(*toDate, "dddd, d. MMMM yyyy"));
            itemPreis->setText(QString::number(booking->getPrice()));
            ui->tableWidget_2->setItem(row,0,itemIcon);
            ui->tableWidget_2->setItem(row,1,itemStart);
            ui->tableWidget_2->setItem(row,2,itemEnde);
            ui->tableWidget_2->setItem(row,3,itemPreis);

            delete fromDate;
            delete toDate;


            shared_ptr<FlightBooking> flugBuchung = dynamic_pointer_cast<FlightBooking>(booking);
            shared_ptr<Airport> airport1 = travelAgency->findAirport(flugBuchung->getFromDest());
            shared_ptr<Airport> airport2 = travelAgency->findAirport(flugBuchung->getToDest());
            Features feature1, feature2;
            feature1.properties.ort_name = airport1->getName();
            feature1.geometry.coordinates.push_back(flugBuchung->getFDestLongitude());
            feature1.geometry.coordinates.push_back(flugBuchung->getFDestLatitude());

            feature2.properties.ort_name = airport2->getName();
            feature2.geometry.coordinates.push_back(flugBuchung->getTDestLongitude());
            feature2.geometry.coordinates.push_back(flugBuchung->getTDestLatitude());

            vector<vector<double>> lineFeatPoints;
            lineFeatPoints.push_back(feature1.geometry.coordinates);
            lineFeatPoints.push_back(feature2.geometry.coordinates);

            featuresJson.push_back(makeGeoJsonPoint(feature1));
            featuresJson.push_back(makeGeoJsonPoint(feature2));
            featuresJson.push_back(makeGeoLineString(lineFeatPoints,"Flug",airport1->getName(),airport2->getName()));
        }
        else if(booking->showDetails()[0]=='M')
        {
            ui->tableWidget_2->insertRow(row);
            QTableWidgetItem* itemIcon = new QTableWidgetItem();
            itemIcon->setIcon(QIcon(":/icons/rentalcar.png"));
            itemIcon->setSizeHint(QSize(26, 26));
            QTableWidgetItem* itemStart = new QTableWidgetItem();
            QTableWidgetItem* itemEnde = new QTableWidgetItem();
            QTableWidgetItem* itemPreis = new QTableWidgetItem();
            QDate* fromDate = new QDate(QDate::fromString(QString::fromStdString(booking->getFromDate()), "yyyyMMdd"));
            QDate* toDate = new QDate(QDate::fromString(QString::fromStdString(booking->getToDate()), "yyyyMMdd"));
            itemStart->setText(QLocale::system().toString(*fromDate, "dddd, d. MMMM yyyy"));
            itemEnde->setText(QLocale::system().toString(*toDate, "dddd, d. MMMM yyyy"));
            itemPreis->setText(QString::number(booking->getPrice()));
            ui->tableWidget_2->setItem(row,0,itemIcon);
            ui->tableWidget_2->setItem(row,1,itemStart);
            ui->tableWidget_2->setItem(row,2,itemEnde);
            ui->tableWidget_2->setItem(row,3,itemPreis);

            delete fromDate;
            delete toDate;

            shared_ptr<RentalCarReservation> mietwagenReservierung = dynamic_pointer_cast<RentalCarReservation>(booking);
            Features feature1, feature2;
            feature1.properties.ort_name = mietwagenReservierung->getPickupLocation();
            feature1.geometry.coordinates.push_back(mietwagenReservierung->getPickupLongitude());
            feature1.geometry.coordinates.push_back(mietwagenReservierung->getPickupLatitude());

            feature2.properties.ort_name = mietwagenReservierung->getReturnLocation();
            feature2.geometry.coordinates.push_back(mietwagenReservierung->getReturnLongitude());
            feature2.geometry.coordinates.push_back(mietwagenReservierung->getReturnLatitude());

            vector<vector<double>> lineFeatPoints;
            lineFeatPoints.push_back(feature1.geometry.coordinates);
            lineFeatPoints.push_back(feature2.geometry.coordinates);

            featuresJson.push_back(makeGeoJsonPoint(feature1));
            featuresJson.push_back(makeGeoJsonPoint(feature2));
            featuresJson.push_back(makeGeoLineString(lineFeatPoints,"Mietwagenreservierung",mietwagenReservierung->getPickupLocation(),mietwagenReservierung->getReturnLocation()));
        }
        else if(booking->showDetails()[0]=='Z')
        {
            ui->tableWidget_2->insertRow(row);
            QTableWidgetItem* itemIcon = new QTableWidgetItem();
            itemIcon->setIcon(QIcon(":/icons/train.png"));
            itemIcon->setSizeHint(QSize(26, 26));
            QTableWidgetItem* itemStart = new QTableWidgetItem();
            QTableWidgetItem* itemEnde = new QTableWidgetItem();
            QTableWidgetItem* itemPreis = new QTableWidgetItem();
            QDate* fromDate = new QDate(QDate::fromString(QString::fromStdString(booking->getFromDate()), "yyyyMMdd"));
            QDate* toDate = new QDate(QDate::fromString(QString::fromStdString(booking->getToDate()), "yyyyMMdd"));
            itemStart->setText(QLocale::system().toString(*fromDate, "dddd, d. MMMM yyyy"));
            itemEnde->setText(QLocale::system().toString(*toDate, "dddd, d. MMMM yyyy"));
            itemPreis->setText(QString::number(booking->getPrice()));
            ui->tableWidget_2->setItem(row,0,itemIcon);
            ui->tableWidget_2->setItem(row,1,itemStart);
            ui->tableWidget_2->setItem(row,2,itemEnde);
            ui->tableWidget_2->setItem(row,3,itemPreis);

            delete fromDate;
            delete toDate;

            shared_ptr<TrainTicket> zugBuchung = dynamic_pointer_cast<TrainTicket>(booking);
            Features feature1, feature2;
            feature1.properties.ort_name = zugBuchung->getFromDest();
            feature1.geometry.coordinates.push_back(zugBuchung->getFStatLongitude());
            feature1.geometry.coordinates.push_back(zugBuchung->getFStatLatitude());
            vector<vector<double>> lineFeatPoints;
            lineFeatPoints.push_back(feature1.geometry.coordinates);
            featuresJson.push_back(makeGeoJsonPoint(feature1));

            feature2.properties.ort_name = zugBuchung->getToDest();
            feature2.geometry.coordinates.push_back(zugBuchung->getTStatLongitude());
            feature2.geometry.coordinates.push_back(zugBuchung->getTStatLatitude());
            lineFeatPoints.push_back(feature2.geometry.coordinates);
            featuresJson.push_back(makeGeoJsonPoint(feature2));

            if(!(zugBuchung->getConnectingStations().empty()))
            {
                lineFeatPoints.pop_back();
                vector<Features> featuresArray;
                for(const auto& station : zugBuchung->getConnectingStations())
                {
                    Features feature;
                    feature.properties.ort_name = station.first;
                    feature.geometry.coordinates.push_back(station.second.second);
                    feature.geometry.coordinates.push_back(station.second.first);
                    lineFeatPoints.push_back(feature.geometry.coordinates);
                    featuresArray.push_back(feature);
                }
                lineFeatPoints.push_back(feature2.geometry.coordinates);
                featuresJson.push_back(makeGeoLineString(lineFeatPoints,"Zugbuchung",zugBuchung->getFromDest(),zugBuchung->getToDest()));
                for(const auto& feature : featuresArray)
                    featuresJson.push_back(makeGeoJsonPoint(feature,"Bahnhof"));
            }
            else
                featuresJson.push_back(makeGeoLineString(lineFeatPoints,"Zugbuchung",zugBuchung->getFromDest(),zugBuchung->getToDest()));
        }
        row++;
    }

    json featureCollection;
    featureCollection["type"] = "FeatureCollection";
    featureCollection["features"] = featuresJson;

    ui->tableWidget_2->setColumnWidth(0,80);
    ui->tableWidget_2->setColumnWidth(1,160);
    ui->tableWidget_2->setColumnWidth(2,160);
    ui->tableWidget_2->setColumnWidth(3,70);
    ui->tableWidget_2->setEditTriggers(QTableWidget::NoEditTriggers);

    QUrl url("https://jenningsanderson.com/geo/?geojson="+QString::fromStdString(featureCollection.dump()));
    QDesktopServices::openUrl(url);
    // ofstream file("beispiel.json");
    // if(file)
    //     file << featureCollection.dump(4);
    // file.close();
}


void TravelAgencyUI::on_tableWidget_2_itemDoubleClicked(QTableWidgetItem *item)
{
    //BuchungsDetails* buchungsDetails = new BuchungsDetails(travelAgency);

    int row = item->row();
    QString itemCol1 = ui->tableWidget_2->item(row,1)->text();
    QString itemCol2 = ui->tableWidget_2->item(row,2)->text();
    double preis = ui->tableWidget_2->item(row,3)->text().toDouble();

    QString fromDate = QString::fromStdString(changeDateFormat(itemCol1.toStdString()));
    QString toDate = QString::fromStdString(changeDateFormat(itemCol2.toStdString()));

    buchungsDetails->setDetails(fromDate, toDate, preis);
    //connect(buchungsDetails, BuchungsDetails::checkSignal, this, slotCheck);

    buchungsDetails->show();
}


void TravelAgencyUI::on_actionSpeichern_triggered()
{
    if(travelAgency->getAllCustomers().empty())
    {
        ui->statusbar->showMessage("Es wurde noch keine Buchung angelegt!");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,tr("Buchungen speichern"), tr("beispiel"),"JSON-Files (*.json)");

    nlohmann::json gesamtJson;
    for(auto& booking : travelAgency->getAllBookings())
    {
        nlohmann::json bookingJson;
        bookingJson["id"] = booking->getId();
        bookingJson["price"] = booking->getPrice();
        bookingJson["fromDate"] = booking->getFromDate();
        bookingJson["toDate"] = booking->getToDate();
        bookingJson["travelId"] = booking->getTravelId();
        shared_ptr<Travel> travel = travelAgency->findTravel(booking->getTravelId());
        bookingJson["customerId"] = travel->getCustomerId();
        shared_ptr<Customer> customer = travelAgency->findCustomer(travel->getCustomerId());
        bookingJson["customerLastname"] = customer->getLastName();
        bookingJson["customerFirstname"] = customer->getFirstName();
        if(booking->showDetails()[0]=='H')
        {
            bookingJson["type"] = "Hotel";
            shared_ptr<HotelBooking> hotelBuchung = dynamic_pointer_cast<HotelBooking>(booking);
            bookingJson["hotel"] = hotelBuchung->getHotel();
            bookingJson["town"] = hotelBuchung->getTown();
            bookingJson["roomType"] = hotelBuchung->getRoomType();
        }
        else if(booking->showDetails()[0]=='F')
        {
            bookingJson["type"] = "Flight";
            shared_ptr<FlightBooking> flugBuchung = dynamic_pointer_cast<FlightBooking>(booking);
            bookingJson["fromDest"] = flugBuchung->getFromDest();
            bookingJson["toDest"] = flugBuchung->getToDest();
            bookingJson["bookingClass"] = flugBuchung->getBookingClass();
            bookingJson["airline"] = flugBuchung->getAirline();
        }
        else if(booking->showDetails()[0]=='M')
        {
            bookingJson["type"] = "RentalCar";
            shared_ptr<RentalCarReservation> mietWagenReservierung = dynamic_pointer_cast<RentalCarReservation>(booking);
            bookingJson["pickupLocation"] = mietWagenReservierung->getPickupLocation();
            bookingJson["returnLocation"] = mietWagenReservierung->getReturnLocation();
            bookingJson["company"] = mietWagenReservierung->getCompany();
            bookingJson["vehicleClass"] = mietWagenReservierung->getVehicleClass();
        }
        else if(booking->showDetails()[0]=='Z')
        {
            bookingJson["type"] = "Train";
            shared_ptr<TrainTicket> zugBuchung = dynamic_pointer_cast<TrainTicket>(booking);
            bookingJson["fromStation"] = zugBuchung->getFromDest();
            bookingJson["toStation"] = zugBuchung->getToDest();
            bookingJson["departureTime"] = zugBuchung->getDepartureTime();
            bookingJson["arrivalTime"] = zugBuchung->getArrivalTime();
            bookingJson["ticketType"] = zugBuchung->getBookingClass();
            nlohmann::json stationsJson;
            map<string, pair<double,double>> connects = zugBuchung->getConnectingStations();
            if(connects.empty())
                bookingJson["connectingStations"] = connects;
            else
            {
                for(const auto& station : connects)
                {
                    nlohmann::json objekt;
                    objekt["latitude"] = station.second.first;
                    objekt["longitude"] = station.second.second;
                    objekt["stationName"] = station.first;
                    stationsJson.push_back(objekt);
                }
                bookingJson["connectingStations"] = stationsJson;
            }
        }
        gesamtJson.push_back(bookingJson);
    }

    if(!fileName.isEmpty())
    {
        ofstream newJasonFile(fileName.toStdString());
        if(!newJasonFile)
            throw invalid_argument("Datei konne nicht geoeffnet werden!");
        newJasonFile<<gesamtJson.dump(4);
        newJasonFile.close();
        ui->statusbar->showMessage("Datei erfolgreich gespeichert!",5000);
    }
}

void TravelAgencyUI::slotCheck()
{
    (*check)();
}


void TravelAgencyUI::on_actionFehlerhafte_Buchungen_triggered()
{
    meldungen->placeMeldung();
    meldungen->show();
}


void TravelAgencyUI::on_actionKonsistenzchecks_triggered()
{
    settings->exec();
}

