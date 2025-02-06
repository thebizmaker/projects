#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "travel.h"

class Customer
{
public:
    Customer(long id_, string lastName_, string firstName_);
    ~Customer();
    void addTravel(shared_ptr<Travel> travel);

    long getId() const;

    string getLastName() const;

    string getFirstName() const;

    vector<shared_ptr<Travel>> getTravelList() const;

private:
    long id;
    string lastName;
    string firstName;
    vector<shared_ptr<Travel>> travelList;
};

#endif // CUSTOMER_H
