#ifndef TOAST_H
#define TOAST_H

#include <QWidget>
#include "ui_toast.h"

namespace Ui {
class Toast;
}

#pragma once

class Toast : public QWidget
{
    Q_OBJECT

public:
    Toast(QWidget *parent = Q_NULLPTR);
    ~Toast();

    void setText(const QString& text);

    void showAnimation(int timeout = 2000);// 动画方式show出，默认2秒后消失

public:
    // 静态调用
    static void showTip(const QString& text, QWidget* parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::ToastClass* ui;
};

#endif // TOAST_H
