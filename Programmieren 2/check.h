#ifndef CHECK_H
#define CHECK_H

#include <travelagency.h>

class Check
{
public:
    Check(shared_ptr<TravelAgency> travelAgency);
    void operator()();

private:
    shared_ptr<TravelAgency> travelAgency;
    void checkTravelDisjunct();
    void checkNoMissingHotels();
    void checkNoOverlappingHotels();
    void checkNoOverlappingRentalCars();
    vector<QString> meldungen;
};

#endif // CHECK_H
