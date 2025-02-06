#include "travel.h"
#include <algorithm>


Travel::Travel(long id_, long customerId_)
    : id(id_), customerId(customerId_)
{
    //travelGraph = Graph<shared_ptr<Booking>, 20>();
}

Travel::~Travel()
{
}

void Travel::addBookings(shared_ptr<Booking> booking)
{
    travelBookings.push_back(booking);
}

long Travel::getId() const
{
    return id;
}

vector<shared_ptr<Booking>> Travel::getTravelBookings() const
{
    return travelBookings;
}

bool Travel::operator<(const shared_ptr<Travel> travel)
{
    return id < travel->getId();
}

long Travel::getCustomerId() const
{
    return customerId;
}

pair<string, string> Travel::calculateDates() const
{
    string firstDate;
    string lastDate;
    for(auto& booking : travelBookings)
    {
        string date1= booking->getFromDate();
        string date2=booking->getToDate();
        if(firstDate.empty() || firstDate>date1)
            firstDate = date1;
        if(lastDate.empty() || lastDate<date2)
            lastDate=date2;
    }
    return make_pair(firstDate, lastDate);
}

Graph<shared_ptr<Booking>, 20> Travel::getTravelGraph() const
{
    return travelGraph;
}

void Travel::setTravelGraph(const Graph<shared_ptr<Booking>, 20> &newTravelGraph)
{
    travelGraph = newTravelGraph;
}

map<shared_ptr<Booking>, int> Travel::getBookingPlusId() const
{
    return bookingPlusId;
}

vector<shared_ptr<BookingData>> Travel::getBookingDataSorted() const
{
    return bookingDataSorted;
}

void Travel::topologishSortieren()
{
    std::sort(bookingDataSorted.begin(), bookingDataSorted.end(), [](shared_ptr<BookingData> b1, shared_ptr<BookingData> b2){ return b1->endTime > b2->endTime;});
}

void Travel::depthFirstSuche()
{
    depthFirstSearch(travelGraph);
}

void Travel::setBookingDataSorted(const vector<shared_ptr<BookingData>> &newBookingDataSorted)
{
    bookingDataSorted = newBookingDataSorted;
}

void Travel::setBookingPlusId(const map<shared_ptr<Booking>, int> &newBookingPlusId)
{
    bookingPlusId = newBookingPlusId;
}
