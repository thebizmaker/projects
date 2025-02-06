#ifndef TRAVEL_H
#define TRAVEL_H
#include "booking.h"
#include <vector>
#include "algorithmen.cpp"
#include <map>

class BookingData
{
public:
    BookingData(int i, shared_ptr<Booking> book, int endZeit):index(i),booking(book),endTime(endZeit){}
    int index;
    shared_ptr<Booking> booking;
    int endTime;
};
class Travel
{
public:
    Travel(long id_, long customerId_);
    ~Travel();
    void addBookings(shared_ptr<Booking> booking);

    long getId() const;

    vector<shared_ptr<Booking>> getTravelBookings() const;
    bool operator<(const shared_ptr<Travel> travel);

    long getCustomerId() const;
    pair<string, string> calculateDates() const;

    Graph<shared_ptr<Booking>, 20> getTravelGraph() const;
    void setTravelGraph(const Graph<shared_ptr<Booking>, 20> &newTravelGraph);

    map<shared_ptr<Booking>, int> getBookingPlusId() const;

    vector<shared_ptr<BookingData>> getBookingDataSorted() const;

    void topologishSortieren();

    void depthFirstSuche();

    void setBookingDataSorted(const vector<shared_ptr<BookingData>> &newBookingDataSorted);

    void setBookingPlusId(const map<shared_ptr<Booking>, int> &newBookingPlusId);

private:
    long id;
    long customerId;
    vector<shared_ptr<Booking>> travelBookings;
    Graph<shared_ptr<Booking>, 20> travelGraph;
    map<shared_ptr<Booking>, int> bookingPlusId;
    vector<shared_ptr<BookingData>> bookingDataSorted;
};

#endif // TRAVEL_H
