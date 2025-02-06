#ifndef MELDUNGEN_H
#define MELDUNGEN_H

#include <QWidget>
#include "travelagency.h"

namespace Ui {
class Meldungen;
}

class Meldungen : public QWidget
{
    Q_OBJECT

public:
    explicit Meldungen(shared_ptr<TravelAgency> travelAgency, QWidget *parent = nullptr);
    ~Meldungen();
    void placeMeldung();

private:
    Ui::Meldungen *ui;
    shared_ptr<TravelAgency> travelAgency;
};

#endif // MELDUNGEN_H
