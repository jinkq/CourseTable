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

    //添加课程信息
    void addCourseInfo();

    //数据库
    QSqlDatabase addcourse_db;

    //清空编辑区
    void clearEdit();

signals:
    //创建课程按钮->MainWindow
    void courseButtonSignal(QString courseName,int courseDay, int courseTimeBegin, int courseTimeEnd, QString courseLocation,QString courseTeacher);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddCourse *ui;
};

#endif // ADDCOURSE_H
