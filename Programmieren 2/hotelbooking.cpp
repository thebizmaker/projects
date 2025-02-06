#include "hotelbooking.h"
#include <sstream>
#include <iomanip>

HotelBooking::HotelBooking(string id_, double price_, long travelId_, string fromDate_, string toDate_, string hotel_, string town_, string roomType_, double hLat, double hLon)
    : Booking(id_, price_, travelId_, fromDate_, toDate_)
    , hotel(hotel_), town(town_), roomType(roomType_), hotelLatitude(hLat), hotelLongitude(hLon)
{

}

HotelBooking::~HotelBooking()
{

}

string HotelBooking::showDetails()
{
    stringstream ss;
    ss<<fixed<<setprecision(2)<<price;
    return "Hotelreservierung "+id+": im "+hotel+" in "+town+" vom "+
           fromDate+" bis zum "+toDate+". Preis: "+ss.str();
}

string HotelBooking::getHotel() const
{
    return hotel;
}

string HotelBooking::getTown() const
{
    return town;
}

string HotelBooking::getRoomType() const
{
    return roomType;
}

void HotelBooking::setHotel(const string &newHotel)
{
    hotel = newHotel;
}

void HotelBooking::setTown(const string &newTown)
{
    town = newTown;
}

void HotelBooking::setRoomType(const string &newRoomType)
{
    roomType = newRoomType;
}

double HotelBooking::getHotelLatitude() const
{
    return hotelLatitude;
}

double HotelBooking::getHotelLongitude() const
{
    return hotelLongitude;
}
