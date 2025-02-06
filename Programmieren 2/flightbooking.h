#ifndef FLIGHTBOOKING_H
#define FLIGHTBOOKING_H

#include "booking.h"

class FlightBooking : public Booking
{
public:
    FlightBooking(string id_, double price_, long travelId_, string fromDate_, string toDate_, string fDest, string tDest
                  , string airl, string bClass, double fDlat, double fDlon, double tDlat, double tDLon);
    ~FlightBooking();
    string showDetails();

    string getFromDest() const;

    string getToDest() const;

    string getAirline() const;

    string getBookingClass() const;

    void setFromDest(const string &newFromDest);

    void setToDest(const string &newToDest);

    void setAirline(const string &newAirline);

    void setBookingClass(const string &newBookingClass);

    double getFDestLatitude() const;

    double getFDestLongitude() const;

    double getTDestLatitude() const;

    double getTDestLongitude() const;

private:
    string fromDest;
    string toDest;
    string airline;
    string bookingClass;
    double fDestLatitude;
    double fDestLongitude;
    double tDestLatitude;
    double tDestLongitude;
};

#endif // FLIGHTBOOKING_H
