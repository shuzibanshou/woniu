#include "remoteitem.h"
#include "ui_remoteitem.h"

remoteItem::remoteItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::remoteItem)
{
    ui->setupUi(this);
}

remoteItem::~remoteItem()
{
    delete ui;
}

void remoteItem::setData()
{

}

void remoteItem::on_pushButton_clicked()
{

}


void remoteItem::on_pushButton_2_clicked()
{

}

