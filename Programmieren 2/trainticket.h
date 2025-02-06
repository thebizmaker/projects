#ifndef TRAINTICKET_H
#define TRAINTICKET_H
#include "booking.h"
#include <map>

class TrainTicket : public Booking
{
public:
    TrainTicket(string id_, double price_, long travelId_, string fromDate_, string toDate_, string fDest, string tDest
        , map<string, pair<double,double>> cStations, string dTime, string aTime, string bClass, double fSLat, double fSLon, double tSLat, double tSLon);
    ~TrainTicket();
    string showDetails();

    string getFromDest() const;

    string getToDest() const;

    map<string, pair<double,double>> getConnectingStations() const;

    string getDepartureTime() const;

    string getArrivalTime() const;

    string getBookingClass() const;

    void setFromDest(const string &newFromDest);

    void setToDest(const string &newToDest);

    void setDepartureTime(const string &newDepartureTime);

    void setArrivalTime(const string &newArrivalTime);

    void setBookingClass(const string &newBookingClass);

    void setConnectingStations(const map<string, pair<double,double>> &newConnectingStations);

    double getFStatLatitude() const;

    double getFStatLongitude() const;

    double getTStatLatitude() const;

    double getTStatLongitude() const;

private:
    string fromDest;
    string toDest;
    map<string, pair<double,double>> connectingStations;
    string departureTime;
    string arrivalTime;
    string bookingClass;
    double fStatLatitude;
    double fStatLongitude;
    double tStatLatitude;
    double tStatLongitude;
};

#endif // TRAINTICKET_H
