#include "rentalcarreservation.h"
#include <sstream>
#include <iomanip>

RentalCarReservation::RentalCarReservation(string id_, double price_, long travelId_, string fromDate_, string toDate_, string pLocation, string rLocation, string company_, string vClass, double pLat, double pLon, double rLat, double rLon)
    : Booking(id_, price_, travelId_, fromDate_, toDate_)
    , pickupLocation(pLocation), returnLocation(rLocation), company(company_), vehicleClass(vClass)
    , pickupLatitude(pLat), pickupLongitude(pLon), returnLatitude(rLat), returnLongitude(rLon)
{

}

RentalCarReservation::~RentalCarReservation()
{

}

string RentalCarReservation::showDetails()
{
    stringstream ss;
    ss<<fixed<<setprecision(2)<<price;
    return "Mietwagenreservierung "+id+": mit "+company+". Abholung am "+
           fromDate+" in "+pickupLocation+". Rueckgabe am "+
           toDate+" in "+returnLocation+". Preis: "+ss.str();
}

string RentalCarReservation::getPickupLocation() const
{
    return pickupLocation;
}

string RentalCarReservation::getReturnLocation() const
{
    return returnLocation;
}

string RentalCarReservation::getCompany() const
{
    return company;
}

string RentalCarReservation::getVehicleClass() const
{
    return vehicleClass;
}

void RentalCarReservation::setPickupLocation(const string &newPickupLocation)
{
    pickupLocation = newPickupLocation;
}

void RentalCarReservation::setReturnLocation(const string &newReturnLocation)
{
    returnLocation = newReturnLocation;
}

void RentalCarReservation::setCompany(const string &newCompany)
{
    company = newCompany;
}

void RentalCarReservation::setVehicleClass(const string &newVehicleClass)
{
    vehicleClass = newVehicleClass;
}

double RentalCarReservation::getPickupLatitude() const
{
    return pickupLatitude;
}

double RentalCarReservation::getPickupLongitude() const
{
    return pickupLongitude;
}

double RentalCarReservation::getReturnLatitude() const
{
    return returnLatitude;
}

double RentalCarReservation::getReturnLongitude() const
{
    return returnLongitude;
}
