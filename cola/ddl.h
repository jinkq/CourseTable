#ifndef DDL_H
#define DDL_H

#include <QWidget>
#include <QSqlDatabase>
#include<QList>
#include<QDateTime>

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
    //结构体
    struct ddl
    {
        QString ddlContent;
        QString ddlRequirement;
        QDateTime ddlTime;
        int ddlStatus;
        int ddl_id;
    };

    //ddl信息
    int course_id;
    QString courseName;
    QList<QString> ddlContentList;
    QList<QString> ddlRequirementList;
    QList<QDateTime>ddlTimeList;
    QList<int>ddlStatusList;

    QList<ddl> ddlList;


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
