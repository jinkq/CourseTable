#ifndef LINK_H
#define LINK_H

#include <QWidget>
#include <QSqlDatabase>
#include<QList>

namespace Ui {
class Link;
}

//link结构体
struct link
{
    QString linkName;
    QString linkAddress;
    int link_id;
    int course_id;
    QString linkPsw;
};


class Link : public QWidget
{
    Q_OBJECT

public:
    explicit Link(QWidget *parent = nullptr);
    ~Link();

    //link信息
    QList<link> linkList;
    int course_id;
    QString courseName;

    void goback();

    //数据库
    QSqlDatabase link_db;

    //初始化linkTable（加载信息）
    void initLinkTable();

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
