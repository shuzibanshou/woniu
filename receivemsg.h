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
    void setIndexWidget(const QModelIndex &);
private:
    Ui::receiveMsg *ui;
};

#endif // RECEIVEMSG_H
