#ifndef RECEIVEMSG_H
#define RECEIVEMSG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class receiveMsg;
}

class receiveMsg : public QDialog
{
    Q_OBJECT

public:
    explicit receiveMsg(QWidget *parent = nullptr);
    ~receiveMsg();
    void setModel(QStringListModel*);
private:
    Ui::receiveMsg *ui;
};

#endif // RECEIVEMSG_H
