#include "remoteitem.h"
#include "ui_remoteitem.h"

remoteItem::remoteItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::remoteItem)
{
    ui->setupUi(this);
    connect(ui->sendFile,SIGNAL(clicked()),parent,SLOT(openFile()));
    connect(ui->sendMsg,SIGNAL(clicked()),parent,SLOT(openMsgDialog()));
}

remoteItem::~remoteItem()
{
    delete ui;
}

/**
 * 向item写入数据
 * @brief remoteItem::setData
 */
void remoteItem::setData(deviceItem di)
{
    //设置远程主机系统图标
    QStringList osInfoList = di.deviceOS.split("-");
    getSysIcon(ui->sysIcon,osInfoList[0],osInfoList[1]);
    //设置远程主机名 文本在label溢出则显示 "..."
    QString text(di.deviceName);
    QFontMetrics metrics(ui->deviceName->font());
    QString elidedText = metrics.elidedText(text, Qt::ElideRight, ui->deviceName->width());
    ui->deviceName->setText(elidedText);
    //设置远程主机ip
    ui->deviceIPv4->setText(di.deviceIPv4);
    //动态设置属性
    ui->sendFile->setProperty("ip",di.deviceIPv4);
    ui->sendMsg->setProperty("ip",di.deviceIPv4);
}


