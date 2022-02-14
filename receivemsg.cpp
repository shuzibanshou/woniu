#include "receivemsg.h"
#include "ui_receivemsg.h"

receiveMsg::receiveMsg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::receiveMsg)
{
    ui->setupUi(this);
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
void receiveMsg::setIndexWidget(const QModelIndex &index)
{
    //ui->receiveMsgLog->setModel(receiveMsgLogModel);
    //构造item ui
    QWidget* widget = new QWidget(ui->receiveMsgLog);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(10, 10, 371, 22));
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

    QWidget* widget1 = new QWidget(ui->receiveMsgLog);
    widget1->setObjectName(QString::fromUtf8("widget1"));
    widget1->setGeometry(QRect(10, 30, 441, 194));
    QHBoxLayout* horizontalLayout_2 = new QHBoxLayout(widget1);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
    QTextBrowser* msgContent = new QTextBrowser(widget1);
    msgContent->setObjectName(QString::fromUtf8("msgContent"));
    horizontalLayout_2->addWidget(msgContent);
    QVBoxLayout* verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacer);
    QPushButton* copyMsg = new QPushButton(widget1);
    copyMsg->setObjectName(QString::fromUtf8("copyMsg"));
    copyMsg->setLayoutDirection(Qt::LeftToRight);
    verticalLayout->addWidget(copyMsg);
    QSpacerItem* verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacer_2);
    horizontalLayout_2->addLayout(verticalLayout);


    QFrame * line = new QFrame(ui->receiveMsgLog);
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(10, 230, 441, 20));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);


    label->setText(QCoreApplication::translate("receiveMsg", "\346\235\245\350\207\252", nullptr));
    fromIP->setText(QCoreApplication::translate("receiveMsg", "192.168.165.122", nullptr));
    timestamp->setText(QCoreApplication::translate("receiveMsg", "2022-01-01 00:22:22", nullptr));
    msgContent->setHtml(QCoreApplication::translate("receiveMsg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'.AppleSystemUIFont'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Microsoft Yahei'; font-size:14px; color:#444444; background-color:#ffffff;\">QLable: \346\230\276\347\244\272\346\226\207\345\255\227\346\210\226\345\233\276\347\211\207\357\274\214\344\270\215\346\217\220\344\276\233\347\224\250\346\210\267\344\272\244\344\272\222 QSimpleRichText: \347\224\250\344\272\216\346\230\276\347\244\272\347\224\261stylesheet\345\256\232\345\210\266\347\232\204rich text\357\274\214\344\270\200\344\275\206\346\236\204\351\200\240\345\256\214\346\210\220</span><span style=\" "
                    "font-family:'Microsoft Yahei'; color:#444444; background-color:#ffffff;\">QLable: \346\230\276\347\244\272\346\226\207\345\255\227\346\210\226\345\233\276\347\211\207\357\274\214\344\270\215\346\217\220\344\276\233\347\224\250\346\210\267\344\272\244\344\272\222 QSimpleRichText: \347\224\250\344\272\216\346\230\276\347\244\272\347\224\261stylesheet\345\256\232\345\210\266\347\232\204rich text\357\274\214\344\270\200\344\275\206\346\236\204\351\200\240\345\256\214\346\210\220QLable: \346\230\276\347\244\272\346\226\207\345\255\227\346\210\226\345\233\276\347\211\207\357\274\214\344\270\215\346\217\220\344\276\233\347\224\250\346\210\267\344\272\244\344\272\222 QSimpleRichText: \347\224\250\344\272\216\346\230\276\347\244\272\347\224\261stylesheet\345\256\232\345\210\266\347\232\204rich text\357\274\214\344\270\200\344\275\206\346\236\204\351\200\240\345\256\214\346\210\220</span></p></body></html>", nullptr));
    copyMsg->setText(QCoreApplication::translate("receiveMsg", "\345\244\215\345\210\266", nullptr));
    //界面设置要放在文本设置之后
    //ui->receiveMsgLog->setIndexWidget(index,widget);
    //ui->receiveMsgLog->setIndexWidget(index,widget1);
    //ui->receiveMsgLog->setIndexWidget(index,line);
}
