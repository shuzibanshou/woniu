#include "receivemsg.h"
#include "ui_receivemsg.h"

receiveMsg::receiveMsg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::receiveMsg)
{
    ui->setupUi(this);
    ui->receiveMsgLog->setStyleSheet("QListView{background-color:transparent;}");
    ui->receiveMsgLog->setFrameShape(QListView::NoFrame);

//    QStandardItemModel *model = new QStandardItemModel(ui->receiveMsgLog);
//    ui->receiveMsgLog->setModel(model);
//    static int itemHeight = 0;
//    for(int i = 0; i < 5; i++) {
//        QStandardItem *item = new QStandardItem(QString::number(i));
//        model->appendRow(item);
//        QModelIndex index = model->indexFromItem(item);
//        QPushButton *box = new QPushButton(this);
//        box->setGeometry(0,0,100,100);
//        //box->setStyleSheet("background:red");
//        box->setText("ssss");
//        ui->receiveMsgLog->setIndexWidget(index, box);
//        itemHeight =  ui->receiveMsgLog->sizeHintForRow(index.row());
//    }
//    ui->receiveMsgLog->setStyleSheet("QListView::item{height:"+QString::number(itemHeight)+"px}");
}

receiveMsg::~receiveMsg()
{
    delete ui;
}

/**
 * 自定义函数
 * @brief receiveMsg::setModel
 * @param receiveMsgLogModel
 */
void receiveMsg::setModel(QStringListModel* receiveMsgLogModel)
{
    ui->receiveMsgLog->setModel(receiveMsgLogModel);
}

/**
 * 自定义函数
 * @brief receiveMsg::setModel
 * @param receiveMsgLogModel
 */
void receiveMsg::setIndexWidget(const QModelIndex &index,QString ip,QString time)
{
    //构造item ui
//    QPushButton* itemWidget = new QPushButton(ui->receiveMsgLog);
//    itemWidget->setObjectName(QString::fromUtf8("itemWidget"));
//    itemWidget->setGeometry(QRect(10, 10, 441, 241));
//    itemWidget->setStyleSheet("background:blue");
//    itemWidget->setText("按钮");


    QWidget* itemWidget = new QWidget(ui->receiveMsgLog);
    //itemWidget->setContentsMargins(50,40,50,50);

    QWidget* widget = new QWidget(itemWidget);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(10, 10, 361, 22));
    QHBoxLayout* horizontalLayout = new QHBoxLayout(widget);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    QLabel* label = new QLabel(widget);
    label->setObjectName(QString::fromUtf8("label"));
    horizontalLayout->addWidget(label);
    QLabel* fromIP = new QLabel(widget);
    fromIP->setObjectName(QString::fromUtf8("fromIP"));
    horizontalLayout->addWidget(fromIP);
    QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    QLabel* timestamp = new QLabel(widget);
    timestamp->setObjectName(QString::fromUtf8("timestamp"));
    QFont font;
    font.setPointSize(10);
    timestamp->setFont(font);
    horizontalLayout->addWidget(timestamp);

    QWidget* widget1 = new QWidget(itemWidget);
    widget1->setObjectName(QString::fromUtf8("widget1"));
    widget1->setGeometry(QRect(10, 30, 401, 194));
    //widget1->setStyleSheet("background:red");

    QHBoxLayout* horizontalLayout_2 = new QHBoxLayout(widget1);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
    QTextBrowser* msgContent = new QTextBrowser(widget1);
    msgContent->setObjectName(QString::fromUtf8("msgContent"));
    msgContent->setGeometry(QRect(0, 0, 361, 194));
    //horizontalLayout_2->addWidget(msgContent);

    QVBoxLayout* verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    //QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //verticalLayout->addItem(verticalSpacer);
    QPushButton* copyMsg = new QPushButton(widget1);
    copyMsg->setObjectName(QString::fromUtf8("copyMsg"));
    copyMsg->setGeometry(QRect(365, 80, 41, 23));
    copyMsg->setFixedWidth(35);
    copyMsg->setLayoutDirection(Qt::LeftToRight);

    verticalLayout->addWidget(copyMsg);
    QSpacerItem* verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacer_2);
    //horizontalLayout_2->addLayout(verticalLayout);


//    QFrame * line = new QFrame(itemWidget);
//    line->setObjectName(QString::fromUtf8("line"));
//    line->setGeometry(QRect(10, 230, 441, 20));
//    line->setFrameShape(QFrame::HLine);
//    line->setFrameShadow(QFrame::Sunken);


    label->setText(QCoreApplication::translate("receiveMsg", "\346\235\245\350\207\252", nullptr));
    fromIP->setText(QCoreApplication::translate("receiveMsg", ip.toUtf8(), nullptr));
    timestamp->setText(QCoreApplication::translate("receiveMsg", time.toUtf8(), nullptr));
    msgContent->setHtml(QCoreApplication::translate("receiveMsg", index.data().toString().toUtf8(), nullptr));
    copyMsg->setText(QCoreApplication::translate("receiveMsg", "\345\244\215\345\210\266", nullptr));
    //界面设置要放在文本设置之后
    ui->receiveMsgLog->setIndexWidget(index,itemWidget);
    qint32 itemHeight =  ui->receiveMsgLog->sizeHintForRow(index.row());
    ui->receiveMsgLog->setCurrentIndex(index);
    ui->receiveMsgLog->setStyleSheet("QListView::item{height:"+QString::number(itemHeight)+"px;background-color:transparent;}");    //透明设置失效?
}
