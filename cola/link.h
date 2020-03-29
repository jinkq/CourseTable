#ifndef LINK_H
#define LINK_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class Link;
}

class Link : public QWidget
{
    Q_OBJECT

public:
    explicit Link(QWidget *parent = nullptr);
    ~Link();

    void goback();

    //数据库
    QSqlDatabase link_db;

    //显示对应course的link
    void run(int course_id);
signals:
    void back2Course();

private:
    Ui::Link *ui;
};

#endif // LINK_H
