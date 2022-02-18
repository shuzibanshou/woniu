#ifndef REMOTEITEM_H
#define REMOTEITEM_H

#include <QWidget>

namespace Ui {
class remoteItem;
}

class remoteItem : public QWidget
{
    Q_OBJECT

public:
    explicit remoteItem(QWidget *parent = nullptr);
    ~remoteItem();
    void setData();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::remoteItem *ui;
};

#endif // REMOTEITEM_H
