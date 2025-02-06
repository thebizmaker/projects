#include "travelagency.h"
//#include <algorithm>


using namespace nlohmann;

TravelAgency::TravelAgency() {}

TravelAgency::~TravelAgency()
{

}

string TravelAgency::readFile(QString fileName)
{
    double hotelPrice=0, zugPrice =0, flugPrice =0, rentalPrice=0;
    int totalFlight=0, totalRentalCar=0, totalHotelBooking=0, totalTrainTicket=0;

    allCustomers.clear();
    allTravels.clear();
    allBookings.clear();

    ifstream jsonFile(fileName.toStdString());
    if(!jsonFile.is_open()){
        throw runtime_error("Datei konnte nicht geöffnet werden!");
    }
    json jsonDatei;
    jsonFile>>jsonDatei;

    jsonFile.close();

    if(!jsonDatei.is_array())
        throw runtime_error("Die Datei enthaelt kein Array!");

    int zaehler=0;    
    for(auto& booking : jsonDatei)
    {
        string type=booking["type"];
        string id=booking["id"];

        if(booking["type"]=="Hotel")
        {
            zaehler+=16;
            double price;
            if(!booking["price"].is_number())
                throw runtime_error("Falscher Preis in Zeile "+to_string(zaehler-5));
            else
                price=booking["price"];

            long travelId;
            if(!booking["travelId"].is_number_unsigned())
                throw runtime_error("Falsche ID-Nummer in Zeile "+to_string(zaehler-1));
            else
                travelId=booking["travelId"];

            string fromDate;
            if(!booking["fromDate"].is_string())
                throw runtime_error("Falsches Datum in id: "+to_string(zaehler-10));
            else
            {
                fromDate=booking["fromDate"];
                if(fromDate.empty())
                    throw runtime_error("Falsches Datum in id: "+to_string(zaehler-10));
            }

            string toDate;
            if(!booking["toDate"].is_string())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-3));
            else
                toDate=booking["toDate"];
            if(toDate.empty())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-3));

            string hotel;
            if(!booking["hotel"].is_string())
                throw runtime_error("Falscher Hotelname in Zeile "+to_string(zaehler-9));
            else
                hotel=booking["hotel"];
            if(hotel.empty())
                throw runtime_error("Falscher Hotelname in Zeile "+to_string(zaehler-9));

            string town;
            if(!booking["town"].is_string())
                throw runtime_error("Falsche Stadt in Zeile "+to_string(zaehler-2));
            else
                town=booking["town"];
            if(town.empty())
                throw runtime_error("Falsche Stadt in Zeile "+to_string(zaehler-2));

            string roomType;
            if(!booking["roomType"].is_string())
                throw runtime_error("Falscher Zimmertyp in Zeile "+to_string(zaehler-4));
            else
                roomType=booking["roomType"];
            if(roomType.empty())
                throw runtime_error("Falscher Zimmertyp in Zeile "+to_string(zaehler-4));

            long customerId;
            if(!booking["customerId"].is_number_unsigned())
                throw runtime_error("Falsche Kunde-ID in Zeile "+to_string(zaehler-12));
            else
                customerId=booking["customerId"];

            string customerFirstname;
            if(!booking["customerFirstName"].is_string())
                throw runtime_error("Falscher Vorname in Zeile "+to_string(zaehler-13));
            else
                customerFirstname = booking["customerFirstName"];
            if(customerFirstname.empty())
                throw runtime_error("Falscher Vorname in Zeile "+to_string(zaehler-13));

            string customerLastname;
            if(!booking["customerLastName"].is_string())
                throw runtime_error("Falscher Nachname in Zeile "+to_string(zaehler-11));
            else
                customerLastname = booking["customerLastName"];
            if(customerLastname.empty())
                throw runtime_error("Falscher Nachname in Zeile "+to_string(zaehler-11));
            double hLat;
            if(!booking["hotelLatitude"].is_string())
                throw runtime_error("Falscher Breitengrad im Zeile "+to_string(zaehler-8));
            else
            {
                string strHLat = booking["hotelLatitude"];
                hLat = stod(strHLat);
            }
            double hLon;
            if(!booking["hotelLongitude"].is_string())
                throw runtime_error("Falscher Längengrad in Zeile "+to_string(zaehler-7));
            else
            {
                string strHLon = booking["hotelLongitude"];
                hLon = stod(strHLon);
            }

            shared_ptr<HotelBooking> hotelBuchung =  make_shared<HotelBooking>(id,price,travelId,fromDate,toDate,hotel,town,roomType,hLat,hLon);
            vector<string> predecessors;
            if(booking.find("predecessor1") != booking.end())
            {
                string predecessor1 = booking["predecessor1"];
                predecessors.push_back(predecessor1);
            }
            if(booking.find("predecessor2") != booking.end())
            {
                string predecessor2 = booking["predecessor2"];
                predecessors.push_back(predecessor2);
            }
            hotelBuchung->setPredecessors(predecessors);

            shared_ptr<Customer> customer = findCustomer(customerId);
            shared_ptr<Travel> travel = findTravel(travelId);
            if(!customer)
            {
                customer =  make_shared<Customer>(customerId, customerLastname, customerFirstname);
                travel = make_shared<Travel>(travelId, customerId);
                travel->addBookings(hotelBuchung);
                customer->addTravel(travel);
                allTravels.push_back(travel);
                allCustomers.push_back(customer);
            }
            else
            {
                if(!travel)
                {
                    travel = make_shared<Travel>(travelId, customerId);
                    allTravels.push_back(travel);
                    customer->addTravel(travel);
                }
                travel->addBookings(hotelBuchung);
            }
            allBookings.push_back(hotelBuchung);
            totalHotelBooking++;
            hotelPrice += price;
        }
        else if(booking["type"]=="Flight")
        {
            zaehler+=19;

            double price;
            if(!booking["price"].is_number())
                throw runtime_error("Falscher Preis in Zeile "+to_string(zaehler-6));
            else
                price=booking["price"];

            long travelId;
            if(!booking["travelId"].is_number_unsigned())
                throw runtime_error("Falsche ID-Nummer in Zeile "+to_string(zaehler-1));
            else
                travelId=booking["travelId"];

            string fromDate;
            if(!booking["fromDate"].is_string())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-11));
            else
                fromDate=booking["fromDate"];
            if(fromDate.empty())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-11));

            string toDate;
            if(!booking["toDate"].is_string())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-5));
            else
                toDate=booking["toDate"];
            if(toDate.empty())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-5));

            string fromDest;
            if(!booking["fromDest"].is_string())
                throw runtime_error("Falscher Abreiseort in Zeile "+to_string(zaehler-10));
            else
            {
                fromDest=booking["fromDest"];
                if(fromDest.size() != 3)
                    throw runtime_error("Falscher Abreiseort in Zeile "+to_string(zaehler-10));
            }

            string toDest;
            if(!booking["toDest"].is_string())
                throw runtime_error("Falscher Ankunftsort in Zeile "+to_string(zaehler-4));
            else
            {
                toDest=booking["toDest"];
                if(toDest.size()!=3)
                    throw runtime_error("Falscher Ankunftsort in Zeile "+to_string(zaehler-4));
            }

            string airline;
            if(!booking["airline"].is_string())
                throw runtime_error("Falsche Fluglinie in Zeile "+to_string(zaehler-16));
            else
                airline=booking["airline"];
            if(airline.empty())
                throw runtime_error("Falsche Fluglinie in Zeile "+to_string(zaehler-16));

            string bookingClass;
            if(!booking["bookingClass"].is_string())
                throw runtime_error("Falsche Bookingclass in Zeile "+to_string(zaehler-15));
            else
                bookingClass = booking["bookingClass"];
            if(bookingClass.empty())
                throw runtime_error("Falsche Bookingclass in Zeile "+to_string(zaehler-15));

            long customerId;
            if(!booking["customerId"].is_number_unsigned())
                throw runtime_error("Falsche Kunde-ID in Zeile "+to_string(zaehler-13));
            else
                customerId=booking["customerId"];

            string customerFirstname;
            if(!booking["customerFirstName"].is_string())
                throw runtime_error("Falscher Vorname in Zeile "+to_string(zaehler-14));
            else
                customerFirstname = booking["customerFirstName"];
            if(customerFirstname.empty())
                throw runtime_error("Falscher Vorname in Zeile "+to_string(zaehler-14));

            string customerLastname;
            if(!booking["customerLastName"].is_string())
                throw runtime_error("Falscher Nachname in Zeile "+to_string(zaehler-12));
            else
                customerLastname = booking["customerLastName"];
            if(customerLastname.empty())
                throw runtime_error("Falscher Nachname in Zeile "+to_string(zaehler-12));
            double fDestLat;
            if(!booking["fromDestLatitude"].is_string())
                throw runtime_error("Falscher Breitengrad in Zeile "+to_string(zaehler-9));
            else
            {
                string strfDestLat = booking["fromDestLatitude"];
                fDestLat = stod(strfDestLat);
            }
            double tDestLat;
            if(!booking["toDestLatitude"].is_string())
                throw runtime_error("Falscher Beitengrad in Zeile "+to_string(zaehler-3));
            else
            {
                string strtDestLat = booking["toDestLatitude"];
                tDestLat = stod(strtDestLat);
            }
            double fDestLon;
            if(!booking["fromDestLongitude"].is_string())
                throw runtime_error("Falscher Längengrad in Zeile "+to_string(zaehler-8));
            else
            {
                string strfDestLon = booking["fromDestLongitude"];
                fDestLon = stod(strfDestLon);
            }
            double tDestLon;
            if(!booking["toDestLongitude"].is_string())
                throw runtime_error("Falscher Längengrad in Zeile "+to_string(zaehler-2));
            else
            {
                string strtDestLon = booking["toDestLongitude"];
                tDestLon = stod(strtDestLon);
            }

            shared_ptr<FlightBooking> flugBuchung =  make_shared<FlightBooking>(id,price,travelId,fromDate,toDate,fromDest,toDest,airline,bookingClass,fDestLat,fDestLon,tDestLat,tDestLon);
            vector<string> predecessors;
            if(booking.find("predecessor1") != booking.end())
            {
                string predecessor1 = booking["predecessor1"];
                predecessors.push_back(predecessor1);
            }
            if(booking.find("predecessor2") != booking.end())
            {
                string predecessor2 = booking["predecessor2"];
                predecessors.push_back(predecessor2);
            }
            flugBuchung->setPredecessors(predecessors);
            shared_ptr<Customer> customer = findCustomer(customerId);
            shared_ptr<Travel> travel = findTravel(travelId);
            if(!customer)
            {
                customer = make_shared<Customer>(customerId, customerLastname, customerFirstname);
                travel = make_shared<Travel>(travelId, customerId);
                travel->addBookings(flugBuchung);
                customer->addTravel(travel);
                allTravels.push_back(travel);
                allCustomers.push_back(customer);//il n'a pas encore de travel, le customer
            }
            else
            {
                if(!travel)
                {
                    travel = make_shared<Travel>(travelId, customerId);
                    allTravels.push_back(travel);
                    customer->addTravel(travel);
                }
                travel->addBookings(flugBuchung);
            }
            totalFlight++;
            allBookings.push_back(flugBuchung);            
            flugPrice += price;
        }
        else if(booking["type"]=="RentalCar")
        {
            zaehler+=19;

            double price;
            if(!booking["price"].is_number())
                throw runtime_error("Falscher Preis in Zeile "+to_string(zaehler-5));
            else
                price=booking["price"];

            long travelId;
            if(!booking["travelId"].is_number_unsigned())
                throw runtime_error("Falsche ID-Nummer in Zeile "+to_string(zaehler-2));
            else
                travelId=booking["travelId"];

            string fromDate;
            if(!booking["fromDate"].is_string())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-8));
            else
                fromDate=booking["fromDate"];
            if(fromDate.empty())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-8));

            string toDate;
            if(!booking["toDate"].is_string())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-3));
            else
                toDate=booking["toDate"];
            if(toDate.empty())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-3));

            string pickupLocation;
            if(!booking["pickupLocation"].is_string())
                throw runtime_error("Falsche Abholstation in Zeile "+to_string(zaehler-6));
            else
                pickupLocation=booking["pickupLocation"];
            if(pickupLocation.empty())
                throw runtime_error("Falsche Abholstation in Zeile "+to_string(zaehler-6));

            string returnLocation;
            if(!booking["returnLocation"].is_string())
                throw runtime_error("Falsche Rueckgabestation in Zeile "+to_string(zaehler-4));
            else
                returnLocation=booking["returnLocation"];
            if(returnLocation.empty())
                throw runtime_error("Falsche Rueckgabestation in Zeile "+to_string(zaehler-4));

            string company;
            if(!booking["company"].is_string())
                throw runtime_error("Falscher Firmenname in Zeile "+to_string(zaehler-12));
            else
                company=booking["company"];
            if(company.empty())
                throw runtime_error("Falscher Firmenname in Zeile "+to_string(zaehler-12));

            string vehicleClass;
            if(!booking["vehicleClass"].is_string())
                throw runtime_error("Falsche Vehicleclass in Zeile "+to_string(zaehler));
            else
                vehicleClass = booking["vehicleClass"];
            if(vehicleClass.empty())
                throw runtime_error("Falsche Vehicleclass in Zeile "+to_string(zaehler));

            long customerId;
            if(!booking["customerId"].is_number_unsigned())
                throw runtime_error("Falsche Kunde-ID in Zeile "+to_string(zaehler-10));
            else
                customerId=booking["customerId"];

            string customerFirstname;
            if(!booking["customerFirstName"].is_string())
                throw runtime_error("Falscher Vorname in Zeile "+to_string(zaehler-11));
            else
                customerFirstname = booking["customerFirstName"];
            if(customerFirstname.empty())
                throw runtime_error("Falscher Vorname in Zeile "+to_string(zaehler-11));

            string customerLastname;
            if(!booking["customerLastName"].is_string())
                throw runtime_error("Falscher Nachname in Zeile "+to_string(zaehler-9));
            else
                customerLastname = booking["customerLastName"];
            if(customerLastname.empty())
                throw runtime_error("Falscher Nachname in Zeile "+to_string(zaehler-9));
            double pickupLat;
            if(!booking["pickupLatitude"].is_string())
                throw runtime_error("Falscher Breitengrad in Zeile "+to_string(zaehler-9));
            else
            {
                string strpickupLat = booking["pickupLatitude"];
                pickupLat = stod(strpickupLat);
            }
            double returnLat;
            if(!booking["returnLatitude"].is_string())
                throw runtime_error("Falscher Breitengrad in Zeile "+to_string(zaehler-9));
            else
            {
                string strreturnLat = booking["returnLatitude"];
                returnLat = stod(strreturnLat);
            }
            double pickupLon;
            if(!booking["pickupLongitude"].is_string())
                throw runtime_error("Falscher Längengrad in Zeile "+to_string(zaehler-9));
            else
            {
                string strpickupLon = booking["pickupLongitude"];
                pickupLon = stod(strpickupLon);
            }
            double returnLon;
            if(!booking["returnLongitude"].is_string())
                throw runtime_error("Falscher Längengrad in Zeile "+to_string(zaehler-9));
            else
            {
                string strreturnLon = booking["returnLongitude"];
                returnLon = stod(strreturnLon);
            }

            shared_ptr<RentalCarReservation> mietwagenReservierung = make_shared<RentalCarReservation>(id,price,travelId,fromDate,toDate,pickupLocation,returnLocation,company,vehicleClass,pickupLat,pickupLon,returnLat,returnLon);
            vector<string> predecessors;
            if(booking.find("predecessor1") != booking.end())
            {
                string predecessor1 = booking["predecessor1"];
                predecessors.push_back(predecessor1);
            }
            if(booking.find("predecessor2") != booking.end())
            {
                string predecessor2 = booking["predecessor2"];
                predecessors.push_back(predecessor2);
            }
            mietwagenReservierung->setPredecessors(predecessors);
            shared_ptr<Customer> customer = findCustomer(customerId);
            shared_ptr<Travel> travel = findTravel(travelId);
            if(!customer)
            {
                customer = make_shared<Customer>(customerId, customerLastname, customerFirstname);
                travel = make_shared<Travel>(travelId, customerId);
                travel->addBookings(mietwagenReservierung);
                customer->addTravel(travel);
                allTravels.push_back(travel);
                allCustomers.push_back(customer);
            }
            else
            {
                if(!travel)
                {
                    travel = make_shared<Travel>(travelId, customerId);
                    allTravels.push_back(travel);
                    customer->addTravel(travel);
                }
                travel->addBookings(mietwagenReservierung);
            }
            allBookings.push_back(mietwagenReservierung);
            totalRentalCar++;            
            rentalPrice += price;
        }
        else if(booking["type"]=="Train")
        {
            map<string, pair<double,double>>connectingStations;
            if(booking["connectingStations"].is_array())
            {
                for(auto& objekt : booking["connectingStations"])
                {
                    double latitude = objekt["latitude"];
                    double longitude = objekt["longitude"];
                    string stationName = objekt["stationName"];
                    connectingStations[stationName] = make_pair(latitude, longitude);
                }
            }
            else
                throw runtime_error("Fehler(connecting stations) in id: "+id);

            zaehler+=22+(int)connectingStations.size()*5;

            double price;
            if(!booking["price"].is_number())
                throw runtime_error("Falscher Preis in Zeile "+to_string(zaehler-5));
            else
                price=booking["price"];

            long travelId;
            if(!booking["travelId"].is_number_unsigned())
                throw runtime_error("Falsche ID-Nummer in Zeile "+to_string(zaehler-1));
            else
                travelId=booking["travelId"];

            string fromDate;
            if(!booking["fromDate"].is_string())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-8));
            else
                fromDate=booking["fromDate"];
            if(fromDate.empty())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-8));

            string toDate;
            if(!booking["toDate"].is_string())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-3));
            else
                toDate=booking["toDate"];
            if(toDate.empty())
                throw runtime_error("Falsches Datum in Zeile "+to_string(zaehler-3));

            string fromStation;
            if(!booking["fromStation"].is_string())
                throw runtime_error("Falsche Abfahrtstation in Zeile "+to_string(zaehler-7));
            else
                fromStation=booking["fromStation"];
            if(fromStation.empty())
                throw runtime_error("Falsche Abfahrtstation in Zeile "+to_string(zaehler-7));

            string toStation;
            if(!booking["toStation"].is_string())
                throw runtime_error("Falsche Ankunftstation in Zeile "+to_string(zaehler-2));
            else
                toStation=booking["toStation"];
            if(toStation.empty())
                throw runtime_error("Falsche Ankunftstation in Zeile "+to_string(zaehler-2));

            string departureTime;
            if(!booking["departureTime"].is_string())
                throw runtime_error("Falsche Abfahrtzeit in Zeile "+to_string(zaehler-9));
            else
                departureTime=booking["departureTime"];
            if(departureTime.empty())
                throw runtime_error("Falsche Abfahrtzeit in Zeile "+to_string(zaehler-9));

            string arrivalTime;
            if(!booking["arrivalTime"].is_string())
            {
                throw runtime_error("Falsche Ankunftzeit in Zeile "+to_string(zaehler-15-(int)connectingStations.size()));
            }
            else
                arrivalTime=booking["arrivalTime"];
            if(arrivalTime.empty())
                throw runtime_error("Falsche Ankunftzeit in Zeile "+to_string(zaehler-15-(int)connectingStations.size()));

            string ticketType;
            if(!booking["ticketType"].is_string())
                throw runtime_error("Falscher Ticketype in Zeile "+to_string(zaehler-4));
            else
                ticketType = booking["ticketType"];
            if(ticketType.empty())
                throw runtime_error("Falscher Ticketype in Zeile "+to_string(zaehler-4));

            long customerId;
            if(!booking["customerId"].is_number_unsigned())
                throw runtime_error("Falsche Kunde-ID in Zeile "+to_string(zaehler-11));
            else
                customerId=booking["customerId"];

            string customerFirstname;
            if(!booking["customerFirstName"].is_string())
                throw runtime_error("Falscher Vorname in Zeile "+to_string(zaehler-12));
            else
                customerFirstname = booking["customerFirstName"];
            if(customerFirstname.empty())
                throw runtime_error("Falscher Vorname in Zeile "+to_string(zaehler-12));

            string customerLastname;
            if(!booking["customerLastName"].is_string())
                throw runtime_error("Falscher Nachname in Zeile "+to_string(zaehler-10));
            else
                customerLastname = booking["customerLastName"];
            if(customerLastname.empty())
                throw runtime_error("Falscher Nachname in Zeile "+to_string(zaehler-10));
            double fromStatLat;
            if(!booking["fromStationLatitude"].is_number())
                throw runtime_error("Falscher Breitengrad in Zeile "+to_string(zaehler-9));
            else
                fromStatLat = booking["fromStationLatitude"];
            double toStatLat;
            if(!booking["toStationLatitude"].is_number())
                throw runtime_error("Falscher Breitengrad in Zeile "+to_string(zaehler-9));
            else
                toStatLat = booking["toStationLatitude"];
            double fromStatLon;
            if(!booking["fromStationLongitude"].is_number())
                throw runtime_error("Falscher Längengrad in Zeile "+to_string(zaehler-9));
            else
                fromStatLon = booking["fromStationLongitude"];
            double toStatLon;
            if(!booking["toStationLongitude"].is_number())
                throw runtime_error("Falscher Längengrad in Zeile "+to_string(zaehler-9));
            else
                toStatLon = booking["toStationLongitude"];

            shared_ptr<TrainTicket> zugBuchung = make_shared<TrainTicket>(id,price,travelId,fromDate,toDate,fromStation,toStation,connectingStations,departureTime,arrivalTime,ticketType,fromStatLat,fromStatLon,toStatLat,toStatLon);
            vector<string> predecessors;
            if(booking.find("predecessor1") != booking.end())
            {
                string predecessor1 = booking["predecessor1"];
                predecessors.push_back(predecessor1);
            }
            if(booking.find("predecessor2") != booking.end())
            {
                string predecessor2 = booking["predecessor2"];
                predecessors.push_back(predecessor2);
            }
            zugBuchung->setPredecessors(predecessors);
            shared_ptr<Customer> customer = findCustomer(customerId);
            shared_ptr<Travel> travel = findTravel(travelId);

            if(!customer)
            {
                customer = make_shared<Customer>(customerId, customerLastname, customerFirstname);
                travel = make_shared<Travel>(travelId, customerId);
                travel->addBookings(zugBuchung);
                customer->addTravel(travel);
                allTravels.push_back(travel);
                allCustomers.push_back(customer);//il n'a pas encore de travel, le customer
            }
            else
            {
                if(!travel)
                {
                    travel = make_shared<Travel>(travelId, customerId);
                    allTravels.push_back(travel);
                    customer->addTravel(travel);
                }
                travel->addBookings(zugBuchung);
            }
            allBookings.push_back(zugBuchung);
            totalTrainTicket++;            
            zugPrice += price;
        }
    } // Ende des Einlesens der JSON-Datei

    // ich weise jedem Booking ein Index zu
    for(auto& customer : allCustomers)
    for(auto& travel : customer->getTravelList())
    {
        map<shared_ptr<Booking>, int> bpi;
        int index=0;
        for(auto& booking : travel->getTravelBookings())
            bpi[booking] = ++index;
        travel->setBookingPlusId(bpi);
    }
    // Graphen einlegen
    for(auto& customer : allCustomers)
    for(auto& travel : customer->getTravelList())
    {
        //cerr << "Travel: " << travel->getId()<<endl;
        Graph<shared_ptr<Booking>, 20> graph ;
        // insert Vertex
        for(const auto& booking : travel->getBookingPlusId())
            graph.insertVertex(booking.second, booking.first);
        // insert Edge
        for(const auto& booking : travel->getBookingPlusId())
            for(auto& predecessor : booking.first->getPredecessors())
                graph.insertEdge(travel->getBookingPlusId().find(findBooking(predecessor))->second, booking.second);
        travel->setTravelGraph(graph);
    }
    // Tiefensuche und topologische Sortierung
    for(auto& customer : allCustomers)
    for(auto& travel : customer->getTravelList())
    {
        //cerr << "Travel: " << travel->getId()<<endl;
        vector<shared_ptr<BookingData>> allBookingData;
        // Tiefensuche
        travel->depthFirstSuche();
        for(const auto& booking : travel->getBookingPlusId())
        {
            allBookingData.push_back(make_shared<BookingData>(booking.second, travel->getTravelGraph().getVertexValue(booking.second), travel->getTravelGraph().getEnd(booking.second)));
        }
        travel->setBookingDataSorted(allBookingData);
        // topologische Sortierung
        travel->topologishSortieren();
        //travel->getTravelGraph().printAdjMatrix();
    }    
    stringstream ss;
    ss<<"Es wurden "<<totalFlight<<" Flugreservierungen im Wert von "<<fixed<<setprecision(2)<<flugPrice<<" Euro, "<<totalHotelBooking<<" Hotelbuchungen im Wert von "<<fixed<<setprecision(2)<<hotelPrice<<" Euro, "<<totalRentalCar<<" Mietwagenreservierungen in Wert von "<<
        fixed<<setprecision(2)<<rentalPrice<<" Euro und "<<totalTrainTicket<<" Zugtickets im Wert von "<<fixed<<setprecision(2)<<zugPrice<<" Euro eingelesen.";

    return ss.str();
}

int TravelAgency::read_Iata()
{
    ifstream jsonFile ("iatacodes.json");
    if(!jsonFile.is_open())
        throw runtime_error("iatacodes.json konnte nicht geoeffnet werden!");
    json jsonData;
    jsonFile >> jsonData;
    jsonFile.close();

    for(const auto& iata : jsonData)
    {
        string name = iata["name"];
        string iso_country = iata["iso_country"];
        string municipality = iata["municipality"];
        string iata_code = iata["iata_code"];

        if(!iata_code.empty())
        {
            shared_ptr<Airport> airport = make_shared<Airport>(name, iso_country, municipality, iata_code);
            iataCodes_map.emplace(iata_code, airport);
        }
    }
    return iataCodes_map.size();
}

shared_ptr<Booking> TravelAgency::findBooking(string id)
{
    for(auto& booking : allBookings)
    {
        if(booking->getId()==id)
            return booking;
    }
    return nullptr;
}

shared_ptr<Travel> TravelAgency::findTravel(long id)
{
    for(auto& travel : allTravels)
    {
        if(travel->getId()==id)
            return travel;
    }
    return nullptr;
}

shared_ptr<Customer> TravelAgency::findCustomer(long id)
{
    for(auto& customer : allCustomers)
    {
        if(customer->getId()==id)
            return customer;
    }
    return nullptr;
}

shared_ptr<Airport> TravelAgency::findAirport(const string iata_code)
{
    auto it = iataCodes_map.begin();
    while(it != iataCodes_map.end())
    {
        if(iata_code == it->first)
            return it->second;
        ++it;
    }
    return nullptr;
}

vector<shared_ptr<Booking>> TravelAgency::getAllBookings() const
{
    return allBookings;
}

vector<shared_ptr<Customer>> TravelAgency::getAllCustomers() const
{
    return allCustomers;
}

vector<shared_ptr<Travel>> TravelAgency::getAllTravels() const
{
    return allTravels;
}

map<string, shared_ptr<Airport> > TravelAgency::getIataCodes() const
{
    return iataCodes_map;
}

vector<QString> TravelAgency::getMeldungen() const
{
    return meldungen;
}

void TravelAgency::setMeldungen(const vector<QString> &newMeldungen)
{
    meldungen = newMeldungen;
}

void TravelAgency::setAktivChecks(const vector<int> &newAktivChecks)
{
    aktivChecks = newAktivChecks;
}

vector<int> TravelAgency::getAktivChecks() const
{
    return aktivChecks;
}
