#ifndef REMOTEITEM_H
#define REMOTEITEM_H

#include <QWidget>
#include <QListWidgetItem>

#include "common.h"

namespace Ui {
class remoteItem;
}

//定义设备信息结构体
typedef struct
{
    QString deviceOS;
    QString deviceName;
    QString deviceIPv4;
    QListWidgetItem* item;  //关联的widgetItem指针
    quint64 timestamp;      //最新广播UDP时间戳
} deviceItem;

class remoteItem : public QWidget
{
    Q_OBJECT

public:
    explicit remoteItem(QWidget *parent = nullptr);
    ~remoteItem();
    void setData(deviceItem);

private slots:

private:
    Ui::remoteItem *ui;
};

#endif // REMOTEITEM_H
