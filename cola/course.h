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
    QSqlDatabase db;

    void sendSlot();

    //
    void run(QString courseName,int courseDay, int courseTimeBegin, int courseTimeEnd, QString courseLocation, QString courseTeacher);

signals:
    void back2Main();



private slots:
    void on_ddlButton_clicked();

private:
    Ui::course *ui;

};

#endif // COURSE_H
