#ifndef RECEIVEFILE_H
#define RECEIVEFILE_H

#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

namespace Ui {
class Dialog;
}



class receiveFile : public QDialog
{
    Q_OBJECT
    public:
        explicit receiveFile(QWidget *parent = nullptr);
        void setIPv4(QString);
        void setFileName(QString);
        void setFileSize(QString);
        QString formateFileSize(QString);
        void setSaveFilePath(QString);

    signals:
        void acceptFile();
        void rejectFile();
        void modifySaveFilePath(QString);
    private:
        Ui::Dialog *ui;
        QString saveFilePath;
        quint8 accept = 0;      //是否点击了接收按钮 0 未点击 1 已点击

    private slots:
        void on_modifySaveFilePath_clicked();
        void on_rejectFile_clicked();
        void on_acceptFile_clicked();

protected:
        void closeEvent(QCloseEvent*);
        //bool eventFilter(QObject *, QEvent *);
};

#endif // RECEIVEFILE_H
