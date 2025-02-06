#include "flightbooking.h"
#include <sstream>
#include <iomanip>

FlightBooking::FlightBooking(string id_, double price_, long travelId_, string fromDate_, string toDate_, string fDest, string tDest, string airl, string bClass, double fDlat, double fDlon, double tDlat, double tDLon)
    : Booking(id_, price_, travelId_, fromDate_, toDate_)
    ,fromDest(fDest), toDest(tDest), airline(airl), bookingClass(bClass), fDestLatitude(fDlat), fDestLongitude(fDlon), tDestLatitude(tDlat), tDestLongitude(tDLon)
{}

FlightBooking::~FlightBooking()
{

}

string FlightBooking::showDetails()
{
    stringstream ss;
    ss <<fixed <<setprecision(2)<<price;
    return "Flugbuchung "+id+": Von "+fromDest+" nach "+
           toDest+" mit "+airline+" vom "+fromDate+" bis zum "+toDate+
           ". Preis: "+ss.str();
}

string FlightBooking::getFromDest() const
{
    return fromDest;
}

string FlightBooking::getToDest() const
{
    return toDest;
}

string FlightBooking::getAirline() const
{
    return airline;
}

string FlightBooking::getBookingClass() const
{
    return bookingClass;
}

void FlightBooking::setFromDest(const string &newFromDest)
{
    fromDest = newFromDest;
}

void FlightBooking::setToDest(const string &newToDest)
{
    toDest = newToDest;
}

void FlightBooking::setAirline(const string &newAirline)
{
    airline = newAirline;
}

void FlightBooking::setBookingClass(const string &newBookingClass)
{
    bookingClass = newBookingClass;
}

double FlightBooking::getFDestLatitude() const
{
    return fDestLatitude;
}

double FlightBooking::getFDestLongitude() const
{
    return fDestLongitude;
}

double FlightBooking::getTDestLatitude() const
{
    return tDestLatitude;
}

double FlightBooking::getTDestLongitude() const
{
    return tDestLongitude;
}
