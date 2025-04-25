#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_XPlay2.h"

class XPlay2 : public QMainWindow
{
    Q_OBJECT

public:
    XPlay2(QWidget *parent = nullptr);
    ~XPlay2();

//private:
    Ui::XPlay2Class ui;
};
