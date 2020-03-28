#ifndef ADDCOURSE_H
#define ADDCOURSE_H

#include <QDialog>

#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class AddCourse;
}

class AddCourse : public QDialog
{
    Q_OBJECT

public:
    explicit AddCourse(QWidget *parent = nullptr);
    ~AddCourse();

    //添加课程名
    void addCourseName();

    //添加课程时间
    void addCourseTime();

    //添加课程地点
    void addCourseLocation();

    //添加课程教师
    void addCourseTeacher();

    //添加课程相关链接
    void addCourselink();

    //database
    QSqlDatabase addcourse_db;

private:
    Ui::AddCourse *ui;
};

#endif // ADDCOURSE_H
