﻿#ifndef LINK_H
#define LINK_H

#include <QWidget>
#include <QSqlDatabase>
#include<QList>

namespace Ui {
class Link;
}

class Link : public QWidget
{
    Q_OBJECT

public:
    explicit Link(QWidget *parent = nullptr);
    ~Link();

    //link信息
    int course_id;
    QList<QString> linkNameList;
    QList<QString> linkAddressList;

    void goback();

    //数据库
    QSqlDatabase link_db;

    //显示对应course的link
    void run(int courseId);

    //添加链接
    void addLink();

    //删除链接
    void delLink();
signals:
    void back2Course();

private:
    Ui::Link *ui;
};

#endif // LINK_H
