#ifndef SENDMSG_H
#define SENDMSG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>

#include <QStringListModel>

namespace Ui {
class sendmsg;
}

class sendmsg : public QDialog
{
    Q_OBJECT

public:
    explicit sendmsg(QWidget *parent = nullptr);
    ~sendmsg();

private slots:
    void on_clearMsgAction_clicked();

    void on_sendMsgAction_clicked();

private:
    Ui::sendmsg *ui;
    //文本消息相关
    QTcpSocket* tcpSocketMsgClient;                                 //TCP收发文本消息客户端
    quint16 msgPort = 20002;                                        //TCP收发文本消息的端口
    QStringListModel* sentMsgLogModel;
};

#endif // SENDMSG_H
