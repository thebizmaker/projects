#include "customer.h"
#include "algorithm"

Customer::Customer(long id_, string lastName_, string firstName_)
    : id(id_), lastName(lastName_), firstName(firstName_)
{}

Customer::~Customer()
{

}

void Customer::addTravel(shared_ptr<Travel> travel)
{
    travelList.push_back(travel);
}

long Customer::getId() const
{
    return id;
}

string Customer::getLastName() const
{
    return lastName;
}

string Customer::getFirstName() const
{
    return firstName;
}

vector<shared_ptr<Travel>> Customer::getTravelList() const
{
    return travelList;
}
