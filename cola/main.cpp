#include "mainwindow.h"

#include "course.h"

#include <QApplication>
#include <string>
#include <iostream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //MainWindow进去就是课程表的主界面
    MainWindow w;
    w.show();

    //w.addACourse(1,1,"se");
    //w.addACourse(1,2,"financial");

    return a.exec();
}
