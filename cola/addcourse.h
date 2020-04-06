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

    //数据库
    QSqlDatabase addcourse_db;

    //清空编辑区
    void clearEdit();

    //判断课程时间冲突
    bool conflict(int courseDay,int courseTimeBegin,int courseTimeEnd);

signals:
    //创建课程按钮->MainWindow
    void courseButtonSignal(QString courseName,int courseDay, int courseTimeBegin, int courseTimeEnd, QString courseLocation,QString courseTeacher);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddCourse *ui;
};

#endif // ADDCOURSE_H
