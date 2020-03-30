#ifndef DDL_H
#define DDL_H

#include <QWidget>
#include <QSqlDatabase>
#include<QList>

namespace Ui {
class DDL;
}

class DDL : public QWidget
{
    Q_OBJECT

public:
    explicit DDL(QWidget *parent = nullptr);
    ~DDL();

    void goback();

    //ddl信息
    int course_id;
    QString courseName;
    QList<QString> ddlNameList;
    //QList<QString> ddlAddressList;

    //数据库
    QSqlDatabase ddl_db;

    //显示对应course的ddl
    void run(int courseId);

    //初始化ddlTable（加载信息）
    void initDdlTable();

    //添加ddl
    void addDdl();

    //删除ddl
    void delDdl();

signals:
    void back2Course();

private:
    Ui::DDL *ui;
};

#endif // DDL_H
