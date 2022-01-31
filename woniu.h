#ifndef WONIU_H
#define WONIU_H

#include <QMainWindow>
#include <QBitArray>
#include <QPushButton>
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QDebug>
#include <QHostInfo>
#include <QMap>
#include <QString>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QListWidget>
//#include <QElapsedTimer>
#include <QtMath>
#include <QException>

#include "progress.h"

namespace Ui {
class woniu;
}

//定义消息类型枚举格式
enum MessageType{
    fileInfo = 0x1,                 //发送文件信息
    fileContent = 0x2,              //发送文件内容
    acceptFile = 0x3,               //同意接收该文件
    rejectFile = 0x4,               //拒绝接收该文件
    sentFile = 0x5,                 //文件发送完毕
    recUdpPackSucc = 0x06,          //UDP包接收成功反馈通知
    recUdpPackFail = 0x07           //UDP包接收失败反馈通知
};

//定义设备信息结构体
typedef struct
{
    QString deviceOS;
    QString deviceName;
    QString deviceIPv4;
    QListWidgetItem* item;  //关联的widgetItem指针
    quint64 timestamp;      //最新广播UDP时间戳
} deviceItem;

class woniu : public QMainWindow
{
    Q_OBJECT

public:
    explicit woniu(QWidget *parent = nullptr);
    ~woniu();


private:
    Ui::woniu *ui;
    progress *sendProgress;                                           //发送进度条对话框
    progress *recvProgress;                                           //接收进度条对话框
    quint16 initPort = 10000;   //初始化UDP绑定端口 若端口占用绑定失败则在此基础上+1再次进行绑定直到绑定成功为止
    quint16 actualPort = 10000; //最后实际上绑定的UDP端口
    QUdpSocket* udpSocket;                                          //UDP广播的socket
    quint32 fileStartTransTime;                                     //文件传输开始时间
    quint32 fileEndTransTime;                                       //文件传输结束时间

    //文件相关
    QTcpServer* tcpSocketFileServer;                                //TCP收发文件的server socket
    QTcpSocket* tcpSocketFileClient;                                //TCP收发文件客户端
    QTcpSocket* tcpSocketFileClientList;                           //TCP server新连接的socket
    //QMap<QString,QTcpSocket*> tcpSocketFileClientList;              //TCP收发文件的client socket

    quint16 filePort = 20001;                                       //TCP收发文件的端口

    quint16 remotePort;                                             //当前与其进行文件收发通信的远端UDP端口 默认等于filePort
    QString remoteIPv4Addr;                                         //当前与其进行文件收发通信的远端IPv4地址

    QFile file;                                                     //发送文件对象
    QString fileName = "";                                          //当前发送文件名
    quint64 fileSize = 0;                                           //当前发送文件总大小 bytes
    quint64 fileSentSize = 0;                                       //当前已发送的总字节数 bytes
    quint8 preparedSend = 0;                                        //是否准备发送文件内容 0 未准备 1 已收到接收端反馈 准备发送

    //bool sendLock = true;                                           //当前发送UDP包的锁 当锁为true可发送 false则不可发送

    QFile receiveFileHandle;                                        //接收文件对象
    QString saveFileName;                                           //当前接收文件名
    QString saveDirPath;                                            //当前接收文件存储目录
    QString saveFilePath;                                           //当前接收文件存储路径
    quint64 saveFileSize;                                           //当前接收文件的总大小 bytes 等同于fileSize
    quint64 curSaveFileSize = 0;                                    //当前接收文件已接收总字节数
    quint8 receivedFileInfo = 0;                                    //是否接收到文件基本信息 0 未接收 1 已接收准备接收文件内容


    QString protocolName(QAbstractSocket::NetworkLayerProtocol);    //协议族名称转换
    QMap<QString,QString> getHostIP();                              //获取本地所有IPv4地址
    QMap<QString,QString> localIPv4;                                //保存本地所有IPv4地址的变量
    QString getDeviceInfo();                                        //获取设备信息
    QString localDevice;                                            //保存拼接的设备信息

    void checkEnv();  //检查环境
    void checkBroadcast(QString,QString);                           //检查广播迂回地址

    quint16 broadcastInterval = 5000;                               //局域网UDP循环广播时间间隔 默认5000毫秒
    quint16 scanDevicesInterval = 5000;                             //扫描活跃设备的时间间隔
    quint16 unactiveTimeout = 8;                                    //非活跃设备超时时间 默认 8秒
    QTimer*  broadcastTimer;                                        //局域网UDP循环广播定时器
    QTimer*  scanDevicesTimer;                                      //扫描活跃设备定时器

    QMap<QString,deviceItem> lanDevices;                            //局域网内设备IPv4地址合集-定时扫描踢出下线设备
    QMap<QString,deviceItem> newLanDevices;                         //下一次扫描新的局域网内设备IPv4地址合集 比对旧的数据 分别新增或更新widgetItem

    void addWidgetItem(QString,deviceItem);                         //动态添加item
    void updateWidgetItem(QString,deviceItem);                      //动态更新item
    void delWidgetItem(QString);                                    //动态删除item
    void itemClicked(QListWidgetItem *);
    void itemDoubleClicked(QListWidgetItem *);
    void itemPressed(QListWidgetItem *);
    void parseServerMessage(QByteArray);
    void parseClientMessage(QByteArray);

private slots:
    void onSocketStateChanged(QAbstractSocket::SocketState);
    void onSocketReadyRead();
    void lanBroadcast();                                            //程序启动时进行局域网广播
    void scanDevices();                                             //扫描活跃设备
    void openFile();                                                //打开文件管理器
    void openMsgDialog();                                           //打开发送消息框
    void onNewConnection();
    void onServerReadyRead();                                       //读取socket数据的槽函数
    void onClientReadyRead();
    //void on_remoteDevice_clicked(const QModelIndex &index);
    void acceptFile();                                              //确认接收文件
    void rejectFile();                                              //拒收文件
    //void retransMissionPacket();                                  //重发UDP数据包
};


#endif // WONIU_H
