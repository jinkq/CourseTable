#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "course.h"
#include "addcourse.h"
#include <QList>
#include <QLabel>
#include <QString>
#include<QPushButton>
#include<QSqlQuery>
#include<QTableWidget>
#include<QSqlDatabase>
#include<QDebug>
#include<QMessageBox>
#include<QSqlError>
#include <QMap>

#include "ddl.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //进入课程页面查看某一门课的详细信息
    Course course;

    //记录每一个course_id对应的course_Name
    QMap<int,QString> courseId_Name;

    //所有课程的ddl列表
    QList<ddl> ddlList;
    void insert(ddl, QList<ddl>&);

    //添加新课程的页面
    AddCourse addCourse;

    //连接数据库
    void connectSql();

    //初始化课程表
    void initCourseTable();

    //初始化ddl表
    void initDdlTable();

    //初始化课程表和ddl表
    void initMain();

    Ui::MainWindow *ui;

    //database
    QSqlDatabase db;

    //添加课程按钮
    void addCourseButton(QString courseName,int courseDay, int courseTimeBegin, int courseTimeEnd, QString courseLocation,QString courseTeacher);

    //比较两个ddl的时间
    bool compareTime(ddl&, ddl&);
private:
    QPushButton *courseButton;
private slots:
    void on_addCourseButton_clicked();
};
#endif // MAINWINDOW_H
