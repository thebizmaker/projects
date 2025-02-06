#include "trainticket.h"
#include <sstream>
#include <iomanip>
TrainTicket::TrainTicket(string id_, double price_, long travelId_, string fromDate_, string toDate_, string fDest, string tDest, map<string, pair<double,double>> cStations, string dTime, string aTime, string bClass, double fSLat, double fSLon, double tSLat, double tSLon)
    : Booking(id_, price_, travelId_, fromDate_, toDate_)
    , fromDest(fDest), toDest(tDest), connectingStations(cStations), departureTime(dTime), arrivalTime(aTime)
    , bookingClass(bClass), fStatLatitude(fSLat), fStatLongitude(fSLon), tStatLatitude(tSLat), tStatLongitude(tSLon)
{

}

TrainTicket::~TrainTicket()
{

}

string TrainTicket::showDetails()
{
    stringstream ss;
    ss <<fixed<<setprecision(2)<<price;
    string stations="";
    unsigned int i=0;

    for(const auto& connect : connectingStations)
    {
        if(i==connectingStations.size()-1){
            stations += connect.first+".";
        }
        else
            stations += connect.first+",";
        i++;
    }

    return "Zugbuchung "+id+": von "+fromDest+" nach "+toDest+
           " am "+fromDate+" von "+departureTime+" bis "+toDate+" um "+
           arrivalTime+" ueber "+stations+" Preis: "+ss.str();
}

string TrainTicket::getFromDest() const
{
    return fromDest;
}

string TrainTicket::getToDest() const
{
    return toDest;
}

map<string, pair<double,double>> TrainTicket::getConnectingStations() const
{
    return connectingStations;
}

string TrainTicket::getDepartureTime() const
{
    return departureTime;
}

string TrainTicket::getArrivalTime() const
{
    return arrivalTime;
}

string TrainTicket::getBookingClass() const
{
    return bookingClass;
}

void TrainTicket::setFromDest(const string &newFromDest)
{
    fromDest = newFromDest;
}

void TrainTicket::setToDest(const string &newToDest)
{
    toDest = newToDest;
}

void TrainTicket::setDepartureTime(const string &newDepartureTime)
{
    departureTime = newDepartureTime;
}

void TrainTicket::setArrivalTime(const string &newArrivalTime)
{
    arrivalTime = newArrivalTime;
}

void TrainTicket::setBookingClass(const string &newBookingClass)
{
    bookingClass = newBookingClass;
}

void TrainTicket::setConnectingStations(const map<string, pair<double,double>> &newConnectingStations)
{
    connectingStations = newConnectingStations;
}

double TrainTicket::getFStatLatitude() const
{
    return fStatLatitude;
}

double TrainTicket::getFStatLongitude() const
{
    return fStatLongitude;
}

double TrainTicket::getTStatLatitude() const
{
    return tStatLatitude;
}

double TrainTicket::getTStatLongitude() const
{
    return tStatLongitude;
}
