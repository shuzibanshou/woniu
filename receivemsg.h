#ifndef RECEIVEMSG_H
#define RECEIVEMSG_H

#include <QDialog>
#include <QStringListModel>
#include <QLineEdit>

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QCheckBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QClipboard>

#include <QDebug>
#include <toast.h>

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
    void setIndexWidget(const QModelIndex &,QString,QString);
private:
    Ui::receiveMsg *ui;

private slots:
    void itemClicked(QModelIndex);
};

#endif // RECEIVEMSG_H
