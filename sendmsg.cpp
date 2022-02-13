#include "sendmsg.h"
#include "ui_sendmsg.h"
#include <QAbstractSocket>
#include <QHostAddress>

sendmsg::sendmsg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sendmsg)
{
    ui->setupUi(this);
    sentMsgLogModel = new QStringListModel(this);
}

sendmsg::~sendmsg()
{
    delete ui;
}

/**
 * 清空消息内容
 * @brief sendmsg::on_clearMsgAction_clicked
 */
void sendmsg::on_clearMsgAction_clicked()
{
    ui->msgContent->clear();
}


void sendmsg::on_sendMsgAction_clicked()
{
    QString ip = property("ip").toString();
    QString msgContent = ui->msgContent->toPlainText();
    //连接消息接收方
    if(tcpSocketMsgClient->state() == QAbstractSocket::SocketState::UnconnectedState){
        tcpSocketMsgClient->connectToHost(QHostAddress(ip),msgPort);
    }
    tcpSocketMsgClient->write(msgContent.toUtf8());

    sentMsgLogModel->insertRow(sentMsgLogModel->rowCount());
    QModelIndex index = sentMsgLogModel->index(sentMsgLogModel->rowCount() - 1, 0);
    sentMsgLogModel->setData(index,msgContent,Qt::DisplayRole);
}

