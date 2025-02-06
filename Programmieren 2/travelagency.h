#ifndef TRAVELAGENCY_H
#define TRAVELAGENCY_H
#include "customer.h"
#include <sstream>
#include <QString>
#include "json.hpp"
#include <fstream>
#include "trainticket.h"
#include "hotelbooking.h"
#include "rentalcarreservation.h"
#include "flightbooking.h"
#include "travel.h"
#include "customer.h"
#include <memory>
#include <map>

class Airport
{
public:
    Airport(string nom, string iso, string ville, string iata):name(nom), iso_country(iso), municipality(ville), iata_code(iata){}
    string getName() const{return name;}
    string getIata_Code() const{return iata_code;}
private:
    string name;
    string iso_country;
    string municipality;
    string iata_code;
};

class TravelAgency
{
public:
    TravelAgency();
    ~TravelAgency();

    string readFile(QString fileName);
    int read_Iata();
    shared_ptr<Booking> findBooking(string id);
    shared_ptr<Travel> findTravel(long id);
    shared_ptr<Customer> findCustomer(long id);
    shared_ptr<Airport> findAirport(const string iata_code);

    vector<shared_ptr<Booking>> getAllBookings() const;

    vector<shared_ptr<Customer>> getAllCustomers() const;

    vector<shared_ptr<Travel>> getAllTravels() const;

    map<string, shared_ptr<Airport>> getIataCodes() const;

    vector<QString> getMeldungen() const;

    void setMeldungen(const vector<QString> &newMeldungen);

    void setAktivChecks(const vector<int> &newAktivChecks);

    vector<int> getAktivChecks() const;

private:
    vector<shared_ptr<Booking>> allBookings;
    vector<shared_ptr<Customer>> allCustomers;
    vector<shared_ptr<Travel>> allTravels;
    map<string, shared_ptr<Airport>> iataCodes_map;
    vector<QString> meldungen;
    vector<int> aktivChecks={1,2,3,4};
};

#endif // TRAVELAGENCY_H
