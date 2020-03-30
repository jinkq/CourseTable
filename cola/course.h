#ifndef COURSE_H
#define COURSE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "ddl.h"
#include "link.h"
#include "note.h"


namespace Ui {
class course;
}

class Course : public QWidget
{
    Q_OBJECT

public:
    explicit Course(QWidget *parent = nullptr);
    ~Course();

    //基本信息
    int course_id;
    QString courseName;
    int courseDay;
    int courseTimeBegin;
    int courseTimeEnd;
    QString courseLocation;
    QString courseTeacher;

    DDL courseDdl;
    Link courseLink;
    Note courseNote;


    //数据库
    QSqlDatabase course_db;

    void sendSlot();

    //
    void run(int courseId);

    //初始化编辑区
    void initEdit();

    //保存修改
    void save();

    //删除课程
    void del();
signals:
    void back2Main();

    //修改课程按钮->MainWindow
    void changeCourseButtonSignal(QString courseName,int courseDay, int courseTimeBegin, int courseTimeEnd, QString courseLocation,QString courseTeacher);

    //删除课程按钮->MainWindow
    void delCourseButtonSignal(int courseTimeBegin,int courseDay);

private slots:

private:
    Ui::course *ui;

};

#endif // COURSE_H
