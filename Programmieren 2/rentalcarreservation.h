#ifndef RENTALCARRESERVATION_H
#define RENTALCARRESERVATION_H

#include "booking.h"

class RentalCarReservation : public Booking
{
public:
    RentalCarReservation(string id_, double price_, long travelId_, string fromDate_, string toDate_, string pLocation
        , string rLocation, string company_, string vClass, double pLat, double pLon, double rLat, double rLon);
    ~RentalCarReservation();
    string showDetails();

    string getPickupLocation() const;

    string getReturnLocation() const;

    string getCompany() const;

    string getVehicleClass() const;

    void setPickupLocation(const string &newPickupLocation);

    void setReturnLocation(const string &newReturnLocation);

    void setCompany(const string &newCompany);

    void setVehicleClass(const string &newVehicleClass);

    double getPickupLatitude() const;

    double getPickupLongitude() const;

    double getReturnLatitude() const;

    double getReturnLongitude() const;

private:
    string pickupLocation;
    string returnLocation;
    string company;
    string vehicleClass;
    double pickupLatitude;
    double pickupLongitude;
    double returnLatitude;
    double returnLongitude;
};

#endif // RENTALCARRESERVATION_H
