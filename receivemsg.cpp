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

/**
 * 自定义函数
 * @brief receiveMsg::setModel
 * @param receiveMsgLogModel
 */
void receiveMsg::setModel(QStringListModel* receiveMsgLogModel)
{
    ui->receiveMsgLog->setModel(receiveMsgLogModel);
}

/**
 * 自定义函数
 * @brief receiveMsg::setModel
 * @param receiveMsgLogModel
 */
void receiveMsg::setIndexWidget(const QModelIndex &index, QWidget *widget)
{
    //ui->receiveMsgLog->setModel(receiveMsgLogModel);
    ui->receiveMsgLog->setIndexWidget(index,new QLineEdit(ui->receiveMsgLog));
}
