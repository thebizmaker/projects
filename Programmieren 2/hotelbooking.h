#ifndef HOTELBOOKING_H
#define HOTELBOOKING_H
#include "booking.h"

class HotelBooking : public Booking
{
public:
    HotelBooking(string id_, double price_, long travelId_, string fromDate_, string toDate_, string hotel_
                 , string town_, string roomType_, double hLat, double hLon);
    ~HotelBooking();
    string showDetails();

    string getHotel() const;

    string getTown() const;

    string getRoomType() const;

    void setHotel(const string &newHotel);

    void setTown(const string &newTown);

    void setRoomType(const string &newRoomType);

    double getHotelLatitude() const;

    double getHotelLongitude() const;

private:
    string hotel;
    string town;
    string roomType;
    double hotelLatitude;
    double hotelLongitude;
};

#endif // HOTELBOOKING_H
