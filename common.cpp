#include "common.h"

QByteArray intToByte(int i)
{
    QByteArray abyte0;
    abyte0.resize(4);
    abyte0[0] = (uchar) (0x000000ff & i);
    abyte0[1] = (uchar) ((0x0000ff00 & i) >> 8);
    abyte0[2] = (uchar) ((0x00ff0000 & i) >> 16);
    abyte0[3] = (uchar) ((0xff000000 & i) >> 24);
    return abyte0;
}

int bytesToInt(QByteArray bytes)
{
    int addr = bytes[0] & 0x000000FF;
    addr |= ((bytes[1] << 8) & 0x0000FF00);
    addr |= ((bytes[2] << 16) & 0x00FF0000);
    addr |= ((bytes[3] << 24) & 0xFF000000);
    return addr;
}

QByteArray paddingQByteArray(quint64 data,quint8 paddingLength){
    QByteArray qbytes(QString::number(data).toUtf8());
    if(qbytes.length() < paddingLength){
         do {
            qbytes.insert(0,'0');
         } while (qbytes.length() < paddingLength);
    }
    return qbytes;
}

/**
 * 在界面上设置系统图标
 * @brief getSysIcon
 */
void getSysIcon(QLabel* sysIconParamter,QString osTypeParamter = "",QString osVersionParamter = "")
{
    QString osType = osTypeParamter.isEmpty() ? QSysInfo::productType().toLower() : osTypeParamter;
    QString osVersion = osVersionParamter.isEmpty() ? QSysInfo::productVersion() : osVersionParamter;
    if(osType == "macos"){
        sysIconParamter->setPixmap(QPixmap(":/sysIcon/icons/macos.png"));
    } else if(osType == "ios"){
        sysIconParamter->setPixmap(QPixmap(":/sysIcon/icons/iphone.png"));
    } else if(osType == "ubuntu" || osType == "centos" || osType == "redhat" || osType == "debian" || osType == "suse" || osType == "android"){
        sysIconParamter->setPixmap(QPixmap(":/sysIcon/icons/"+osType+".png"));
    } else if(osType == "windows"){
        QList<QString> ver = osVersion.split(".");
        QString bigVer = ver.at(0);
        sysIconParamter->setPixmap(QPixmap(":/sysIcon/icons/windows-"+bigVer+".png"));
    } else {
         sysIconParamter->setPixmap(QPixmap(":/sysIcon/icons/linux.png"));
    }
    //qDebug() << osVersion;
}
