#include "booking.h"
#include <QDate>

Booking::Booking(string id_, double preis, long travelid, string fDate, string tDate)
    : id(id_), price(preis), travelId(travelid), fromDate(fDate), toDate(tDate)
{

}

Booking::~Booking()
{

}

bool Booking::operator<(const Booking *booking)
{
    QDate fromDate1 = QDate(QDate::fromString(QString::fromStdString(fromDate), "yyyyMMdd"));
    QDate fromDate2 = QDate(QDate::fromString(QString::fromStdString(booking->getFromDate()), "yyyyMMdd"));

    return fromDate1<fromDate2;
}

string Booking::getId() const
{
    return id;
}

string Booking::getFromDate() const
{
    return fromDate;
}

string Booking::getToDate() const
{
    return toDate;
}

double Booking::getPrice() const
{
    return price;
}

void Booking::setPrice(double newPrice)
{
    price = newPrice;
}

void Booking::setFromDate(const string &newFromDate)
{
    fromDate = newFromDate;
}

void Booking::setToDate(const string &newToDate)
{
    toDate = newToDate;
}

long Booking::getTravelId() const
{
    return travelId;
}

vector<string> Booking::getPredecessors() const
{
    return predecessors;
}

void Booking::setPredecessors(const vector<string> &newPredecessors)
{
    predecessors = newPredecessors;
}
