#include "settings.h"
#include "ui_settings.h"

Settings::Settings(shared_ptr<TravelAgency> travelAgency, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
    , travelAgency(travelAgency)
{
    ui->setupUi(this);
    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(true);
    ui->checkBox_3->setChecked(true);
    ui->checkBox_4->setChecked(true);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_buttonBox_accepted()
{
    vector<int> checks;
    travelAgency->setAktivChecks(checks);
    if(ui->checkBox->isChecked())
        checks.push_back(1);
    if(ui->checkBox_2->isChecked())
        checks.push_back(2);
    if(ui->checkBox_3->isChecked())
        checks.push_back(3);
    if(ui->checkBox_4->isChecked())
        checks.push_back(4);
    travelAgency->setAktivChecks(checks);
    //if(!checks.empty())
        emit checkSignal();
}
