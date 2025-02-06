#include "meldungen.h"
#include "ui_meldungen.h"

Meldungen::Meldungen(shared_ptr<TravelAgency> travelAgency, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Meldungen)
    , travelAgency(travelAgency)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 726);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->item(0,1)->setTextAlignment(Qt::AlignCenter);

}

Meldungen::~Meldungen()
{
    delete ui;
}

void Meldungen::placeMeldung()
{
    int count = ui->tableWidget->rowCount();
    while(ui->tableWidget->rowCount()>1){
        ui->tableWidget->removeRow(count);
        count--;
    }
    int row=0;
    for(const auto& meldung : travelAgency->getMeldungen())
    {
        if(meldung[0]=='D')
        {
            ui->tableWidget->insertRow(++row);
            QTableWidgetItem* itemIcon = new QTableWidgetItem();
            itemIcon->setIcon(QIcon(":/icons/double-arrows.png"));
            ui->tableWidget->setItem(row, 0, itemIcon);
            QTableWidgetItem* itemBeschreibung = new QTableWidgetItem();
            itemBeschreibung->setText(meldung);
            ui->tableWidget->setItem(row, 1, itemBeschreibung);
        }
        else if(meldung[0]=='F')
        {
            ui->tableWidget->insertRow(++row);
            QTableWidgetItem* itemIcon = new QTableWidgetItem();
            itemIcon->setIcon(QIcon(":/icons/missing_hotel.png"));
            ui->tableWidget->setItem(row, 0, itemIcon);
            QTableWidgetItem* itemBeschreibung = new QTableWidgetItem();
            itemBeschreibung->setText(meldung);
            ui->tableWidget->setItem(row, 1, itemBeschreibung);
        }
        else if(meldung[0]=='H')
        {
            ui->tableWidget->insertRow(++row);
            QTableWidgetItem* itemIcon = new QTableWidgetItem();
            itemIcon->setIcon(QIcon(":/icons/hotel.png"));
            ui->tableWidget->setItem(row, 0, itemIcon);
            QTableWidgetItem* itemBeschreibung = new QTableWidgetItem();
            itemBeschreibung->setText(meldung);
            ui->tableWidget->setItem(row, 1, itemBeschreibung);
        }
        else if(meldung[0]=='M')
        {
            ui->tableWidget->insertRow(++row);
            QTableWidgetItem* itemIcon = new QTableWidgetItem();
            itemIcon->setIcon(QIcon(":/icons/rentalcar.png"));
            ui->tableWidget->setItem(row, 0, itemIcon);
            QTableWidgetItem* itemBeschreibung = new QTableWidgetItem();
            itemBeschreibung->setText(meldung);
            ui->tableWidget->setItem(row, 1, itemBeschreibung);
        }
    }
}
