#ifndef DDL_H
#define DDL_H

#include <QWidget>
#include <QSqlDatabase>
#include<QList>
#include<QDateTime>
#include <QLineEdit>

namespace Ui {
class DDL;
}

//ddl结构体
struct ddl
{
    QString ddlContent;
    QString ddlRequirement;
    QString ddlTime;
    int ddlStatus;
    int ddl_id;
    int course_id;
};

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

    //保存ddl修改
    void saveDdl();

    //按时间排序ddl
    void sortDdl();

    //判断时间合法性
    bool isValidTime(const QString);

    //比较时间
    bool compareTime(QString, QString);


signals:
    void back2Course();

private:
    Ui::DDL *ui;
};

#endif // DDL_H
