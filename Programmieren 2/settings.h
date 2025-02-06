#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "travelagency.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(shared_ptr<TravelAgency> travelAgency, QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_buttonBox_accepted();

signals:
    void checkSignal();

private:
    Ui::Settings *ui;
    shared_ptr<TravelAgency> travelAgency;
};

#endif // SETTINGS_H
