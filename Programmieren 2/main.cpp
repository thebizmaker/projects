#include "travelagencyui.h"
#include "travelagency.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    shared_ptr<TravelAgency> travelAgency = make_shared<TravelAgency>();
    TravelAgencyUI w(travelAgency);

    w.show();

    return a.exec();
}
