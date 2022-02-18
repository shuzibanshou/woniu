#include "woniu.h"
#include "ui_woniu.h"

#include "receivefile.h"
#include "sendmsg.h"
#include "receivemsg.h"
#include "progress.h"

#include "common.h"

woniu::woniu(QWidget *parent) :QMainWindow(parent),ui(new Ui::woniu)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(380,530));
    //qsrand(QTime::currentTime().msec());
    getSysIcon();
    localIPv4 = getHostIP();
    localDevice = getDeviceInfo();
    checkEnv();
    //启动UDP协议
    udpSocket = new QUdpSocket(this);
    //启动tcp协议
    tcpSocketFileServer = new QTcpServer(this);
    tcpSocketFileClient = new QTcpSocket(this);

    broadcastTimer = new QTimer(this);
    scanDevicesTimer = new QTimer(this);
    checkFireWallTimer = new QTimer(this);
    //retransMissionTimer = new QTimer(this);
//    for(quint16 port = initPort;; port++){
//        if(udpSocket->bind(port)){
//            actualPort = port;
//            break;
//        }
//    }
    if(!udpSocket->bind(initPort)){
       qDebug("绑定UDP广播端口失败,%s",qPrintable(udpSocket->errorString()));
       udpSocket->bind(initPort);
    } else {
       qDebug("绑定UDP广播端口成功");
    }
    if(!tcpSocketFileServer->listen(QHostAddress::Any,filePort)){
       qDebug("文件传输-TCP监听失败,%s",qPrintable(tcpSocketFileServer->errorString()));
       tcpSocketFileServer->listen(QHostAddress::Any,filePort);
    } else {
       qDebug("文件传输-TCP监听成功");
    }
    //再次启动tcp协议用于文本消息的传输
    tcpSocketMsgServer = new QTcpServer(this);
    if(!tcpSocketMsgServer->listen(QHostAddress::Any,msgPort)){
       qDebug("文本消息-TCP监听失败,%s",qPrintable(tcpSocketMsgServer->errorString()));
       tcpSocketMsgServer->listen(QHostAddress::Any,msgPort);
    } else {
       qDebug("文本消息-TCP监听成功");
    }
    //初始化文本模型
    receiveMsgLogModel = new QStringListModel();

    connect(udpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
    connect(broadcastTimer,SIGNAL(timeout()),this,SLOT(lanBroadcast()));
    connect(scanDevicesTimer,SIGNAL(timeout()),this,SLOT(scanDevices()));
    connect(checkFireWallTimer,SIGNAL(timeout()),this,SLOT(checkFireWall()));

    //有新连接到达服务端
    connect(tcpSocketFileServer,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
    connect(tcpSocketMsgServer,SIGNAL(newConnection()),this,SLOT(onNewMsgConnection()));
    //文件发送方准备读取数据（文件接收方发送的消息格式数据，非文件内容）
    connect(tcpSocketFileClient,SIGNAL(readyRead()),this,SLOT(onClientReadyRead()));

    //收发文件信号槽
//    QMap<QString, QTcpSocket*>::iterator tcpClientIter = tcpSocketFileClientList.begin();
//    while(tcpClientIter != tcpSocketFileClientList.end()){
//        connect(tcpClientIter.value(),SIGNAL(readyRead()),this,SLOT(onReadyRead()));
//    }

    lanBroadcast();
    broadcastTimer->start(broadcastInterval);
    scanDevicesTimer->start(scanDevicesInterval);
    checkFireWallTimer->start(checkFireWallInterval);
}

woniu::~woniu()
{
    delete ui;
}

QString woniu::protocolName(QAbstractSocket::NetworkLayerProtocol protocol)
{
    switch (protocol) {
        case QAbstractSocket::IPv4Protocol:
            return "IPv4";
        case QAbstractSocket::IPv6Protocol:
            return "IPv6";
        case QAbstractSocket::AnyIPProtocol:
            return "AnyIP";
        default:
            return "UnknownNetwork";
    }
}

/**
 * 读取所有本地地址信息（物理网卡和虚拟机）
 * @brief woniu::getHostIP
 * @return
 */
QMap<QString,QString> woniu::getHostIP()
{
    QMap<QString,QString> map;
    QHostAddress strIpAddress;
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface interfaceItem, interfaceList)
    {
        //qDebug() << interfaceItem.humanReadableName();
        if(interfaceItem.flags().testFlag(QNetworkInterface::IsUp)
                &&interfaceItem.flags().testFlag(QNetworkInterface::IsRunning)
                &&interfaceItem.flags().testFlag(QNetworkInterface::CanBroadcast)
                &&interfaceItem.flags().testFlag(QNetworkInterface::CanMulticast)
                &&!interfaceItem.flags().testFlag(QNetworkInterface::IsLoopBack)
                /*&&interfaceItem.hardwareAddress()!="00:50:56:C0:00:01"          //虚拟机保留Mac地址
                &&interfaceItem.hardwareAddress()!="00:50:56:C0:00:08"
                &&
                (interfaceItem.humanReadableName()=="WLAN"
                ||
                interfaceItem.humanReadableName()=="以太网")*/)
        {
            QList<QNetworkAddressEntry> addressEntryList=interfaceItem.addressEntries();
            foreach(QNetworkAddressEntry addressEntryItem, addressEntryList)
            {
                if(addressEntryItem.ip().protocol()==QAbstractSocket::IPv4Protocol)
                {
//                    qDebug()<<"------------------------------------------------------------";
//                    qDebug()<<"Adapter Name:"<<interfaceItem.name();
//                    qDebug()<<"Adapter Name:"<<interfaceItem.index();
//                    qDebug()<<"Adapter Name:"<<interfaceItem.humanReadableName();
//                    qDebug()<<"Adapter Address:"<<interfaceItem.hardwareAddress();
//                    qDebug()<<"IP Address:"<<addressEntryItem.ip().toString();
//                    qDebug()<<"IP Mask:"<<addressEntryItem.netmask().toString();

                    strIpAddress = addressEntryItem.ip();
                    //qDebug() << strIpAddress;
                    map.insert(strIpAddress.toString(),interfaceItem.hardwareAddress());
                }
            }
        }
    }

    return map;
}

/**
 * 封装所有本地IPv4地址和设备信息
 * 主机名获取
 * @brief woniu::getDeviceInfo
 * @return
 */
QString woniu::getDeviceInfo()
{
    QString localHost = QHostInfo::localHostName();
    QString os = QSysInfo::prettyProductName();
    //deviceInfo dev = {.deviceOS = os,.deviceName = localHost};
    return os+"##"+localHost;
}

/**
 * 检查环境
 */
void woniu::checkEnv()
{
    QMap<QString, QString>::iterator iter = localIPv4.begin();
    while (iter != localIPv4.end())
    {
        if(iter.value() == "00:50:56:C0:00:08" || iter.value() == "00:50:56:C0:00:01"){
            QMessageBox::warning(this, tr("提示"),tr("检查到您的主机安装有虚拟网卡.\n请确保您的虚拟机与宿主机为桥接模式,否则可能无法通信"),QMessageBox::Ok,QMessageBox::Ok);
            return;
        }
        iter++;
    }
}

/**
 * 检查广播迂回地址
 * 如果地址包含在本地所有地址中 则需要检查是否为虚拟网卡地址 如果是虚拟地址则本机可能在一个孤立的子网中 需要用户重启虚拟网卡 否则可能无法通信
 * 如果地址未包含在本地所有地址中 则说明是局域网内其他设备的广播响应
 * @brief woniu::checkBroadcast
 * @param remoteIPv4Addr
 */
void woniu::checkBroadcast(QString data,QString remoteIPv4Addr)
{
    //qDebug() << remoteIPv4Addr;
    //qDebug() << data;

    if(!data.isEmpty()){
        QStringList list = data.split("##");
        QString deviceOS = list[0];
        QString deviceName = list[1];
        quint32 timestamp = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();   //获取当前时间

        deviceItem di = {.deviceOS = deviceOS, .deviceName = deviceName, .deviceIPv4 = remoteIPv4Addr, .item = NULL, .timestamp = timestamp};
        if(localIPv4.contains(remoteIPv4Addr)){
            if(localIPv4.find(remoteIPv4Addr).value() == "00:50:56:C0:00:01" || localIPv4.find(remoteIPv4Addr).value() == "00:50:56:C0:00:08"){
                QMessageBox::critical(this, tr("错误"),tr("广播迂回地址为虚拟网卡地址,初始化失败,请重启虚拟网卡尝试解决,详情请见教程"),QMessageBox::Ok,QMessageBox::Ok);
                return;
            }
            //根据广播迂回数据填充本机信息
            ui->localName->setText(deviceName);
            ui->localIPv4->setText(remoteIPv4Addr);
        } else {
            //将局域网其他主机写入最新列表 存在则更新 不存在则添加
            if(newLanDevices.contains(remoteIPv4Addr)){
                di.item = newLanDevices[remoteIPv4Addr].item;
            }
            newLanDevices.insert(remoteIPv4Addr,di);
        }
    }
}


/**
 * 广播携带主机名和设备信息
 * 程序初始化后进行局域网UDP广播
 * @brief woniu::lanBroadcast
 */
void woniu::lanBroadcast()
{
    udpSocket->writeDatagram(localDevice.toUtf8(),QHostAddress::Broadcast,initPort);
}

/**
 * 遍历局域网活跃设备列表 比对时间戳
 * 将超时设备踢出设备列表
 * @brief woniu::scanDevices
 */
void woniu::scanDevices()
{
    //ui->remoteDevice->clear();
    quint32 now = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
    QMap<QString, deviceItem>::iterator iter = newLanDevices.begin();

    //addWidgetIte
    while (iter != newLanDevices.end())
    {

        //第一种写法
        if(now - iter.value().timestamp >= unactiveTimeout){
            //先删除UI界面再清除数据
            delWidgetItem(iter.key());
            newLanDevices.erase(iter++);
        } else {
            if(lanDevices.contains(iter.value().deviceIPv4)){
                QString ipv4 = iter.value().deviceIPv4;
                if((iter.value().deviceOS != lanDevices[ipv4].deviceOS) || (iter.value().deviceName != lanDevices[ipv4].deviceName)){
                    updateWidgetItem(iter.key(),iter.value());
                }
            } else {
                //qDebug() << iter.key();
                //deviceItem test = {.deviceOS = "linux",.deviceName="localhost-12locanho",.deviceIPv4="192.168.1.122",.item = nullptr,.timestamp=121231};
                addWidgetItem(iter.key(),iter.value());
                //addWidgetItem(iter.key(),test);
            }
            iter++;
        }
    }
    lanDevices = newLanDevices;
}

void woniu::checkFireWall()
{
    QMessageBox::critical(this, tr("错误"),tr("请检查防火墙是否放行了10000,20001,20002端口，若没有请放行或者关闭防火墙再进行文件和消息传输"),QMessageBox::Ok,QMessageBox::Ok);
    return;
}

/**
 * @brief woniu::onSocketStateChanged
 * @param socketState
 */
void woniu::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    switch (socketState) {
        case QAbstractSocket::UnconnectedState:
            qDebug() << "UnconnectedState";
            break;
        case QAbstractSocket::HostLookupState:
            qDebug() << "HostLookupState";
            break;
        case QAbstractSocket::ConnectingState:
            qDebug() << "ConnectingState";
            break;
        case QAbstractSocket::ConnectedState:
            qDebug() << "ConnectedState";
            break;
        case QAbstractSocket::BoundState:
            qDebug() << "BoundState";
            break;
        case QAbstractSocket::ListeningState:
            qDebug() << "ListeningState";
            break;
        case QAbstractSocket::ClosingState:
            qDebug() << "ClosingState";
            break;
    }
}

/**
 * @brief woniu::onSocketReadyRead
 * 监听网络的UDP广播 忽略本机IP地址
 */
void woniu::onSocketReadyRead()
{
    //qDebug() << udpSocket->state();
    checkFireWallTimer->stop();
    checkFireWallTimer->start(checkFireWallInterval);
    while(udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(udpSocket->pendingDatagramSize()));
        QHostAddress remoteIPv6Addr;        //远程主机地址ipv6
        quint16 remotePort;             //远程主机UDP端口
        udpSocket->readDatagram(datagram.data(),datagram.size(),&remoteIPv6Addr,&remotePort);
        QString remoteIPv4Addr = QHostAddress(remoteIPv6Addr.toIPv4Address()).toString();
        checkBroadcast(datagram.data(),remoteIPv4Addr);
        /*if(remoteIPv6Addr.isEqual(localIPv4,QHostAddress::ConversionModeFlag::ConvertV4MappedToIPv4)){
            ui->textEdit->append("忽略本机广播");
        }*/
    }
}

/**
 * @brief woniu::addWidgetItem
 * @param dev
 */
void woniu:: addWidgetItem(QString key,deviceItem di){
    QListWidgetItem* remoteItem = new  QListWidgetItem(ui->remoteDevice);
    remoteItem->setSizeHint(QSize(10,100));
    //保存该item
    newLanDevices[key].item = remoteItem;
    //设置item布局
    QWidget *itemWidget = new QWidget;

    QLabel *deviceName = new QLabel(itemWidget);
    deviceName->setObjectName(QStringLiteral("deviceName"));
    QFont font1;
    font1.setFamily(QStringLiteral("Arial"));
    font1.setPointSize(12);
    deviceName->setFont(font1);
    deviceName->setAlignment(Qt::AlignCenter);
    deviceName->setText(di.deviceName);
    deviceName->setMinimumWidth(80);
    deviceName->setMaximumWidth(80);
    //文本在label溢出则显示...
    QString text(di.deviceName);
    QFontMetrics metrics(deviceName->font());
    QString elidedText = metrics.elidedText(text, Qt::ElideRight, deviceName->width());
    deviceName->setText(elidedText);

    QLabel *deviceIPv4 = new QLabel(itemWidget);
    deviceName->setObjectName(QStringLiteral("deviceIPv4"));
    QFont font2;
    font2.setFamily(QStringLiteral("Arial"));
    font2.setPointSize(10);
    deviceIPv4->setFont(font2);
    deviceName->setAlignment(Qt::AlignCenter);
    deviceIPv4->setText(di.deviceIPv4);
    deviceIPv4->setMinimumWidth(100);
    deviceIPv4->setMaximumWidth(100);

    QPushButton *sendFile = new QPushButton(itemWidget);
    sendFile->setObjectName(QStringLiteral("sendFile"));
    sendFile->setProperty("ip",di.deviceIPv4);
    sendFile->setText(QApplication::translate("woniu", "\345\217\221\351\200\201\346\226\207\344\273\266", Q_NULLPTR));

    QPushButton *sendMsg = new QPushButton(itemWidget);
    sendMsg->setObjectName(QStringLiteral("sendMsg"));
    sendMsg->setProperty("ip",di.deviceIPv4);
    sendMsg->setText(QApplication::translate("woniu", "\345\217\221\351\200\201\346\266\210\346\201\257", Q_NULLPTR));

    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    layout->addWidget(deviceName);
    layout->addWidget(deviceIPv4);
    layout->addWidget(sendFile);
    layout->addWidget(sendMsg);
    itemWidget->setLayout(layout);
    ui->remoteDevice->setItemWidget(remoteItem,itemWidget);

    //sendMsgBtnList.append(sendMsg);
    //是否会内存泄漏
    connect(sendFile,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(sendMsg,SIGNAL(clicked()),this,SLOT(openMsgDialog()));
    //itemWidget->installEventFilter(this);
}


//void woniu::itemClicked(QListWidgetItem * item){
//    qDebug() << item;
//}

//void woniu::itemDoubleClicked(QListWidgetItem * item){
//    qDebug() << item;
//}

//void woniu::itemPressed(QListWidgetItem * item){
//    qDebug() << item;
//}

/**
 * 查找widget的控件并更新信息
 * @brief woniu::updateWidgetItem
 * @param key 键名 IPv4地址
 * @param di
 */
void woniu:: updateWidgetItem(QString key,deviceItem di){
    QWidget* widget = ui->remoteDevice->itemWidget(newLanDevices[key].item);
    QLabel* q = widget->findChild<QLabel*>("deviceName");
    q->setText(di.deviceName);
    //qDebug() << q->text();
}

/**
 * 删除控件
 * 删除之前判断指针是否为空
 * @brief woniu::delWidgetItem
 * @param key
 */
void woniu:: delWidgetItem(QString key){
    if(newLanDevices[key].item != nullptr){
        quint64 row = ui->remoteDevice->row(newLanDevices[key].item);
        ui->remoteDevice->takeItem(row);
        ui->remoteDevice->removeItemWidget(newLanDevices[key].item);
        delete newLanDevices[key].item;
        newLanDevices[key].item = nullptr;
    }
}

/**
 * 打开系统文件管理器
 * @brief woniu::openFile
 */
void woniu:: openFile(){
    QObject* o = sender();
    QString ip = o->property("ip").toString();

//    QString filePath = QFileDialog::getOpenFileName(this,"open","../");  //选择文件
//    qDebug() << filePath;
//    QFile file;
//    if(!filePath.isEmpty()){
//        QFileInfo info(filePath);
//        fileName = info.fileName();
//        fileSize = info.size();

//        //只读方式打开 file文件对象在私有成员变量中定义
//        file.setFileName(filePath);
//        bool succ = file.open(QIODevice::ReadOnly);
//        if(succ){
//            //连接文件接收方的服务器并向其发送文件信息
//            tcpSocketFileClient->connectToHost(QHostAddress(ip),filePort);
//            QString fi = QString("%1##%2").arg(fileName).arg(fileSize); //整型消息格式不能以QString格式发送 会被转成对应字符的ASCII码
//            qint32 block = tcpSocketFileClient->write(fi.toUtf8().insert(0,MessageType::fileInfo));
//            qDebug() << "连接成功，发送字节数"+QString::number(block);
//        } else {
//            qDebug() << "打开文件失败";
//        }
//    } else {
//        qDebug() << "文件路径有误";
//    }

    QStringList filePaths = QFileDialog::getOpenFileNames(this,"open","../");  //选择多文件
    //qDebug() << filePaths;
    if(filePaths.length() > 0){
        QStringList temp;
        foreach(auto filePath,filePaths){
            if(!filePath.isEmpty()){
                //判断文件是否可读
                QFile* _file = new QFile(filePath);
                if(_file->open(QIODevice::ReadOnly)){
                    files.append(_file);

                    QFileInfo info(filePath);
                    QString singleFileName = info.fileName();
                    quint64 singleFileSize = info.size();
                    fileSize += singleFileSize;
                    QString fi = QString("%1##%2").arg(singleFileName).arg(singleFileSize);
                    temp.append(fi);
                } else {
                    QMessageBox::critical(this, tr("错误"),tr("文件无法打开"),QMessageBox::Ok,QMessageBox::Ok);
                    return;
                }
            }
        }
        QString res = temp.join("||");
        if(tcpSocketFileClient->state() == QAbstractSocket::SocketState::UnconnectedState){
            tcpSocketFileClient->connectToHost(QHostAddress(ip),filePort);
        }
        //tcpSocketFileClient->write(res.toUtf8().insert(0,MessageType::fileInfo));
        qDebug() << res;
        tcpSocketFileClient->write(res.toUtf8());
    }

    //TODO
    //QString srcDirPath = QFileDialog::getExistingDirectory(this, "choose src Directory","/"); //选择文件夹
}

/**
 * 打开消息发送窗口
 * @brief woniu::openMsgDialog
 */
void woniu:: openMsgDialog(){
    //qDebug() << "打开消息管理器";
    QObject* o = sender();
    QString ip = o->property("ip").toString();
    sendmsg* smsg = new sendmsg(this);
    smsg->setProperty("ip",ip);
    smsg->show();
    //qDebug() << ip;
}

void woniu::onNewConnection()
{
//    QTcpSocket* temp = tcpSocketFileServer->nextPendingConnection();
//    QString remoteIP = temp->peerAddress().toString();
//    tcpSocketFileClientList = QMap<QString, QTcpSocket*>();
//    tcpSocketFileClientList.insert(remoteIP,temp);

    tcpSocketFileClientList = tcpSocketFileServer->nextPendingConnection();
    connect(tcpSocketFileClientList,SIGNAL(readyRead()),this,SLOT(onServerReadyRead()));
}

void woniu::onNewMsgConnection()
{
    tcpSocketMsgClientList = tcpSocketMsgServer->nextPendingConnection();
    connect(tcpSocketMsgClientList,SIGNAL(readyRead()),this,SLOT(onServerReadyReadMsg()));
}


void woniu::onServerReadyRead(){
    QByteArray receiveBytes = tcpSocketFileClientList->readAll();
    //qDebug() << receiveBytes;
    parseServerMessage(receiveBytes);
}

void woniu::onClientReadyRead()
{
    QByteArray receiveBytes = tcpSocketFileClient->readAll();
    //qDebug() << receiveBytes;
    parseClientMessage(receiveBytes);
}

/**
 * 解析“文件发送方”发送的消息
 * @brief woniu::parseServerMessage
 * @param data
 */
void woniu::parseServerMessage(QByteArray data)
{

    if(receivedFileInfo == 0){
        //qDebug() << receivedFileInfo;
        QString receiveData = QString::fromUtf8(data);
        receiveFiles = receiveData.split("||");
        QString fileSize,fileName;

        if(receiveFiles.count() > 1){
            fileName = receiveFiles.at(0).split("##")[0]+"等"+QString::number(receiveFiles.count())+"个文件";
            foreach(auto v,receiveFiles){
                saveFileSize += v.split("##")[1].toUInt();
            }
            fileSize = QString::number(saveFileSize);
            qDebug() << fileSize;
        } else if(receiveFiles.count() == 1){
            fileName = receiveFiles.at(0).split("##")[0];
            fileSize = receiveData.split("##")[1];
            saveFileSize = fileSize.toUInt();
        }  else {
            qDebug() << "未收到任何文件消息";
        }

        //是否接受文件-实例化非模态对话框
        receiveFile* rFile = new receiveFile(this);
        rFile->setIPv4(remoteIPv4Addr);
        rFile->setFileName(fileName);
        rFile->setFileSize(fileSize);

        //qDebug() << fileName;
        //qDebug() << fileSize;

        saveFileName = receiveFiles.at(curReceiveFileIndex).split("##")[0];
        saveDirPath = QCoreApplication::applicationDirPath() + "/receiveFiles";
        rFile->setSaveFilePath(saveDirPath);
        rFile->show();
    } else {
        //已接收到文件基本信息
        //接收文件内容
        //qDebug() << data;
        if(data.length() > 0){
            qint64 len = 0;
            len = receiveFileHandle.write(data);
            //qDebug() << len;
            if(len > 0){
                //接收成功
                curSaveFileSize += len;
                curSaveFileTotalSize += len;
                //qDebug() << "接收成功" << curSaveFileSize;
                recvProgress->setValue(((float)curSaveFileTotalSize/saveFileSize)*100);
            }
            //单个文件传输完成
            if(curSaveFileSize == receiveFiles.at(curReceiveFileIndex).split("##")[1].toULongLong()){
                curSaveFileSize = 0;
                receiveFileHandle.close();
                //发送一个文件接收完毕消息给文件发送方
                tcpSocketFileClientList->write(QByteArray().append(MessageType::receiveSingleFile));
                if((curReceiveFileIndex + 1) < receiveFiles.length()){
                    ++curReceiveFileIndex;
                    receiveFileHandle.setFileName(saveDirPath + "/" + receiveFiles.at(curReceiveFileIndex).split("##")[0]);
                    receiveFileHandle.open(QIODevice::WriteOnly);
                } else {
                    //所有文件都传输完成
                    //接收文件标识符置为0 方便下一次文件传输
                    curSaveFileTotalSize = receivedFileInfo = 0;
                    curReceiveFileIndex = 0;
                    recvProgress->close();

                    fileEndTransTime = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
                    quint32 transNeedTime = fileEndTransTime - fileStartTransTime;
                    //提示框是阻塞的 要放在最后面
                    QMessageBox::information(this, "成功",QString("文件已接收完成,耗时%1秒").arg(transNeedTime),QMessageBox::Ok,QMessageBox::Ok);
                }

            }

        }
    }
}

/**
 * 解析“文件接收方”发送的消息
 * @brief woniu::parseMessage
 */
void woniu::parseClientMessage(QByteArray data)
{
    //if(preparedSend == 0){
        int first = data[0];            //第一个字节
        QByteArray content;
        content.append(data.data() + 1, data.size() - 1);   //去掉data字节流的第一个字节
        if(MessageType::acceptFile == first){
            preparedSend = 1;

            //打开传输进度窗口 读取文件并发送
            sendProgress = new progress(this);
            sendProgress->setRange(0,100);
            sendProgress->show();

            //qDebug() << "接收方已同意,开始分块并发送多个文件";
            sendFile(curFileIndex);
        } else if(MessageType::rejectFile == first){
            //拒绝接收文件
            files.clear();
        } else if(MessageType::receiveSingleFile == first){
            //某个文件已接收完毕 curFileIndex++ 传输下一个文件 如果还有未传输的文件
            if((curFileIndex + 1) < files.length() ){
                ++curFileIndex;
                sendFile(curFileIndex);
            } else {
                qDebug() << "文件发送完毕";
                //文件发送完毕 清空变量 进行清扫工作
                fileSentSize = preparedSend = fileSize = curFileIndex = 0;
                sendProgress->close();
                foreach(auto pFile,files){
                    if(pFile->isOpen()){
                        pFile->close();
                    }
                }
                files.clear();
            }

        }
//    } else {

//    }
}

/**
 * 单个文件发送函数
 * @brief woniu::sendFile
 * @param index
 */
void woniu::sendFile(quint64 index)
{
    qint64 sendUnit = 4096;
    quint64 unitBytes = 0;      //每次实际发送字节数
    do {
        QByteArray buff = files.at(index)->read(sendUnit);
        unitBytes = buff.length();
        if(unitBytes > 0){
            unitBytes = tcpSocketFileClient->write(buff);
            if(unitBytes > 0){
                fileSentSize += unitBytes;
                sendProgress->setValue(((float)fileSentSize/fileSize)*100);
            }
        }
    } while(unitBytes > 0);
}

/**
 * 在界面上设置系统图标
 * @brief woniu::getSysIcon
 */
void woniu::getSysIcon()
{
    QString osType = QSysInfo::productType().toLower();
    QString osVersion = QSysInfo::productVersion();
    if(osType == "macos"){
        ui->sysIcon->setPixmap(QPixmap(":/sysIcon/icons/macos.png"));
    } else if(osType == "ios"){
        ui->sysIcon->setPixmap(QPixmap(":/sysIcon/icons/iphone.png"));
    } else if(osType == "ubuntu" || osType == "centos" || osType == "redhat" || osType == "debian" || osType == "suse" || osType == "android"){
        ui->sysIcon->setPixmap(QPixmap(":/sysIcon/icons/"+osType+".png"));
    } else if(osType == "windows"){
        QList<QString> ver = osVersion.split(".");
        QString bigVer = ver.at(0);
        ui->sysIcon->setPixmap(QPixmap(":/sysIcon/icons/windows-"+bigVer+".png"));
    } else {
         ui->sysIcon->setPixmap(QPixmap(":/sysIcon/icons/linux.png"));
    }
    //qDebug() << osVersion;
}



/**
 * 同意接收文件并打开保存文件句柄
 * 如果打开成功则向文件发送方回馈一个通知消息
 * 如果打开失败则提示消息
 * @brief woniu::acceptFile
 */
void woniu::acceptFile()
{
    //打开接收文件句柄
    //qDebug() << saveFilePath;
    receivedFileInfo = 1;
    saveFilePath = saveDirPath + "/" + saveFileName;
    receiveFileHandle.setFileName(saveFilePath);
    bool succ = receiveFileHandle.open(QIODevice::WriteOnly);
    if(succ){
        QByteArray msg;
        msg.append(MessageType::acceptFile);
        tcpSocketFileClientList->write(msg);
        //时间点
        fileStartTransTime = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
        //显示接收文件进度条
        recvProgress = new progress(this);
        recvProgress->setRange(0,100);
        recvProgress->setValue(0);
        recvProgress->show();
    } else {
        QMessageBox::warning(this, tr("提示"),tr("打开文件句柄失败,无法保存文件"),QMessageBox::Ok,QMessageBox::Ok);
        exit(0);
    }

}

/**
 * 拒绝接收文件并关闭窗口
 * @brief woniu::rejectFile
 */
void woniu::rejectFile()
{
    //qDebug() << "拒绝接收文件";
    QByteArray msg;
    msg.append(MessageType::rejectFile);
    tcpSocketFileClientList->write(msg);
}

/**
 * 修改文件保存路径
 * @brief woniu::modifySaveFilePath
 */
void woniu::modifySaveFilePath(QString newSaveFilePath)
{
    saveDirPath = newSaveFilePath;
}

//////
//////消息传输
//////
void woniu::onServerReadyReadMsg(){
    QByteArray receiveBytes = tcpSocketMsgClientList->readAll();
    //qDebug() << receiveBytes;
    receiveMsgLogModel->insertRow(receiveMsgLogModel->rowCount());              //插入新行
    QModelIndex index = receiveMsgLogModel->index(receiveMsgLogModel->rowCount() - 1, 0);
    QString ip = tcpSocketMsgClientList->peerAddress().toString();
    QString time  = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //封装模型数据
//    QMap<QString,QVariant> modelItemData = QMap<QString,QVariant>();
//    modelItemData.insert("ip","192.168.1.1");
//    modelItemData.insert("time","2022-02-16 00:00:00");
//    modelItemData.insert("receiveBytes",receiveBytes);
//    qDebug()<< modelItemData;

//    QVariant temp(modelItemData); //QMap 转 QVariant
//    qDebug()<< temp;
    receiveMsgLogModel->setData(index,receiveBytes,Qt::DisplayRole);      //设置接收到的文本消息
    static receiveMsg* rMsgDialog = new receiveMsg();
    rMsgDialog->setModel(receiveMsgLogModel);
    //rMsgDialog->setIndexWidget(index,ip,time);
    rMsgDialog->show();
}
