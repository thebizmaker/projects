#include "check.h"
#include <QDate>
#include <QMessageBox>

Check::Check(shared_ptr<TravelAgency> travelAgency) : travelAgency(travelAgency)
{

}
bool findId(vector<long> v, long i);
void Check::operator()()
{
    meldungen.clear();
    for(const auto& check : travelAgency->getAktivChecks())
    {
        if(check == 1)
            checkTravelDisjunct();
        if(check == 2)
            checkNoOverlappingHotels();
        if(check == 3)
            checkNoMissingHotels();
        if(check == 4)
            checkNoOverlappingRentalCars();
    }

    travelAgency->setMeldungen(meldungen);
    if(!travelAgency->getMeldungen().empty())
        QMessageBox::information(nullptr, "Fehlerhafte Konsistenzprüfungen", "Bitte kontrolieren Sie das Ergebnis der Prüfungen");
}

void Check::checkTravelDisjunct()
{
    map<long, pair<int,int>> travelIds;
    for(const auto& customer : travelAgency->getAllCustomers())
    {
        for(size_t i=0; i < customer->getTravelList().size(); i++)
        {
            auto travel1 = customer->getTravelList()[i];
            size_t x = i+1;
            while(x < customer->getTravelList().size())
            {
                auto travel2 = customer->getTravelList()[x];
                if(travel1->getId() != travel2->getId())
                //toDate1 < fromDate2 || toDate2 < fromDate1
                if(!(travel1->calculateDates().second <= travel2->calculateDates().first || travel2->calculateDates().second <= travel1->calculateDates().first))
                {
                    //wir speichern beide Reisen im map, die sich überlappen
                    travelIds[customer->getId()] = make_pair(travel1->getId(),travel2->getId());
                }
                x++;
            }
        }
    }
    if(!travelIds.empty())
    {
        for(const auto& ids : travelIds)
        {
            auto travel1 = travelAgency->findTravel(ids.second.first);
            auto travel2 = travelAgency->findTravel(ids.second.second);
            QString message;
            message = "Die Reisen "+QString::number(travel1->getId())+ " und "+QString::number(travel2->getId())+" von Kunde "+QString::number(ids.first)+" überlappen zeitlich.";
            meldungen.push_back(message);
        }        
    }
}

void Check::checkNoOverlappingRentalCars()
{
    //vector<long> cIds;
    for(const auto& customer : travelAgency->getAllCustomers())
    {
        //vector<long> iDs; cIds.push_back(customer->getId());
        for(const auto& travel : customer->getTravelList())
        {
            // if(findId(iDs, travel->getId()) && findId(cIds, customer->getId()))
            //     continue;
            // else
            //     iDs.push_back(travel->getId());
            for(size_t i = 0; i < travel->getBookingDataSorted().size(); i++)
            {
                auto currentBooking = travel->getBookingDataSorted()[i];
                size_t x = i+1;
                if(currentBooking->booking->showDetails()[0] == 'H')
                    continue;
                if(x < travel->getBookingDataSorted().size())
                {
                    auto nexBooking = travel->getBookingDataSorted()[x];
                    if(nexBooking->booking->showDetails()[0] == 'H')
                        continue;
                    //toDate1 <= fromDate2 || toDate2 <= fromDate1
                    if(!(currentBooking->booking->getToDate() <= nexBooking->booking->getFromDate()))
                    {
                        QDate* datum1 = new QDate(QDate::fromString(QString::fromStdString(currentBooking->booking->getToDate()), "yyyyMMdd"));
                        QDate* datum2 = new QDate(QDate::fromString(QString::fromStdString(nexBooking->booking->getFromDate()), "yyyyMMdd"));
                        QString message = "Mietwagenreservierung der Reise "+QString::number(travel->getId())+" von Kunde "
                                          +QString::number(customer->getId())+" überlappt mit benachbarter Buchung vom "
                                          +datum1->toString("dd.MM.yyyy")+" bis zum "+datum2->toString("dd.MM.yyyy");
                        meldungen.push_back(message);
                    }
                }
            }
        }
    }
}

void Check::checkNoOverlappingHotels()
{
    for(const auto& customer : travelAgency->getAllCustomers())
    {
        //vector<long> iDs; cIds.push_back(customer->getId());
        for(const auto& travel : customer->getTravelList())
        {
            bool test = true;
            shared_ptr<Booking> prevBooking = nullptr;bool stop = true;
            for(const auto& bookingData : travel->getBookingDataSorted())
            {
                size_t size = travel->getBookingDataSorted().size();
                if(!stop)
                    break;
                shared_ptr<Booking> currentBooking = bookingData->booking;

                if(test && currentBooking->showDetails()[0] != 'M')
                {
                    prevBooking = currentBooking;
                    test = false;
                    continue;
                }
                if(currentBooking->showDetails()[0] == 'M')
                    continue;
                if(currentBooking == travel->getBookingDataSorted()[size-2]->booking && prevBooking == travel->getBookingDataSorted()[0]->booking)
                    if(travel->getBookingDataSorted()[size-1]->booking->showDetails()[0] == 'H')
                    {prevBooking = travel->getBookingDataSorted()[size-1]->booking; stop = false;}
                //toDate1 < fromDate2 || toDate2 < fromDate1
                if(!(prevBooking->getToDate() <= currentBooking->getFromDate()))
                {
                    QDate* datum1 = new QDate(QDate::fromString(QString::fromStdString(prevBooking->getToDate()), "yyyyMMdd"));
                    QDate* datum2 = new QDate(QDate::fromString(QString::fromStdString(currentBooking->getFromDate()), "yyyyMMdd"));
                    QString message = "Hotelbuchung der Reise "+QString::number(travel->getId())+" von Kunde "
                                      +QString::number(customer->getId())+" überlappt mit benachbarter Buchung vom "
                                      +datum1->toString("dd.MM.yyyy")+" bis zum "+datum2->toString("dd.MM.yyyy");
                    meldungen.push_back(message);
                    delete datum1; delete datum2;
                }
                prevBooking = currentBooking;
            }
        }
    }
}

void Check::checkNoMissingHotels()
{   
    for(const auto& customer : travelAgency->getAllCustomers())
    {
        //vector<long> iDs; cIds.push_back(customer->getId());
        for(auto& travel : customer->getTravelList())
        {            
            string lastToDate;
            for(size_t i=0; i < travel->getBookingDataSorted().size(); i++)
            {
                if(travel->getBookingDataSorted()[i]->booking->showDetails()[0] != 'H')
                    continue;
                if(!lastToDate.empty() && travel->getBookingDataSorted()[i]->booking->getFromDate() < lastToDate)
                {
                    QDate* datum1 = new QDate(QDate::fromString(QString::fromStdString(lastToDate), "yyyyMMdd"));
                    QDate* datum2 = new QDate(QDate::fromString(QString::fromStdString(travel->getBookingDataSorted()[i]->booking->getFromDate()), "yyyyMMdd"));
                    QString message = "Für Reise "+QString::number(travel->getId())+" des Kunden "
                                      +QString::number(travel->getCustomerId())+" fehlt eine Hotelbuchung zwischen dem "
                                      +datum1->toString("dd.MM.yyyy")+" und dem "+datum2->toString("dd.MM.yyyy");
                    meldungen.push_back(message);
                    delete datum1; delete datum2;
                }
                lastToDate = travel->getBookingDataSorted()[i]->booking->getToDate();
            }
        }
    }
}

bool findId(vector<long> v, long i)
{
    for(long j : v)
        if(i == j)
            return true;
    return false;
}
