#include "receivemsg.h"
#include "ui_receivemsg.h"

receiveMsg::receiveMsg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::receiveMsg)
{
    ui->setupUi(this);
}

receiveMsg::~receiveMsg()
{
    delete ui;
}

void receiveMsg::setModel(QStringListModel* receiveMsgLogModel)
{
    ui->receiveMsgLog->setModel(receiveMsgLogModel);
}
