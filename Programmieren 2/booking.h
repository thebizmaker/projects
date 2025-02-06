#ifndef BOOKING_H
#define BOOKING_H
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Booking
{
public:
    Booking(string id, double price, long travelId, string fromDate, string toDate);
    virtual ~Booking();
    virtual string showDetails()=0;
    bool operator<(const Booking* booking);

    string getId() const;

    string getFromDate() const;

    string getToDate() const;

    double getPrice() const;

    void setPrice(double newPrice);

    void setFromDate(const string &newFromDate);

    void setToDate(const string &newToDate);

    long getTravelId() const;

    vector<string> getPredecessors() const;

    void setPredecessors(const vector<string> &newPredecessors);

protected:
    string id;
    double price;
    long travelId;
    string fromDate;
    string toDate;
    vector<string> predecessors;
};

#endif // BOOKING_H
