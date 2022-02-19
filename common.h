#ifndef COMMON_H
#define COMMON_H
#include <QByteArray>
#include <QString>
#include <QLabel>

QByteArray intToByte(int);
int bytesToInt(QByteArray);
QByteArray paddingQByteArray(quint64,quint8);
void getSysIcon(QLabel*,QString,QString);

#endif // COMMON_H
