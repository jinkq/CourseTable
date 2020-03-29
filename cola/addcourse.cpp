#include "addcourse.h"
#include "ui_addcourse.h"
#include<QSqlQuery>
#include<QDebug>
#include<QMessageBox>
#include<QPushButton>
#include<QVariantList>

AddCourse::AddCourse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCourse)
{
    ui->setupUi(this);

    //设置窗口标题
    setWindowTitle("添加课程");

}

AddCourse::~AddCourse()
{
    delete ui;
}

void AddCourse::addCourseInfo()
{
    if(!addcourse_db.open())
       {
            QMessageBox::warning(this,QStringLiteral("错误"),"error");
            qDebug()<<"open sql error (in addcourse)";
            exit(0);//退出程序
        }

    //获取编辑区内容
    QString courseName=ui->courseNameEdit->text();
    int courseDay=ui->courseDayEdit->currentIndex()+1;
    int courseTimeBegin=ui->courseTimeEditBegin->currentIndex()+1;
    int courseTimeEnd=ui->courseTimeEditEnd->currentIndex()+1;
    QString courseLocation=ui->courseLocationEdit->text();
    QString courseTeacher=ui->courseTeacherEdit->text();

    //判断课程名是否非空
    if(courseName=="")
    {
        QMessageBox::warning(this,"error","课程名不能为空");
        return;
    }

    //判断课程节数是否合法
    if(courseTimeBegin>courseTimeEnd)
    {
        QMessageBox::warning(this,"error","输入的课程节数不合法");
        return;
    }

    QSqlQuery query;
    //将编辑区内容写入sql
    query.exec(QString("insert into courseInfo(courseName, courseDay,courseTimeBegin, courseTimeEnd, courseLocation, courseTeacher) values ('%1', %2, %3, %4, '%5', '%6');")
               .arg(courseName).arg(courseDay).arg(courseTimeBegin).arg(courseTimeEnd).arg(courseLocation).arg(courseTeacher));

    //向MainWindow发信号，创建课程按钮
    emit courseButtonSignal(courseName, courseDay,courseTimeBegin, courseTimeEnd, courseLocation,courseTeacher);
}

void AddCourse::on_buttonBox_accepted()
{
    addCourseInfo();
}

void AddCourse::clearEdit()
{
    ui->courseNameEdit->clear();
    ui->courseDayEdit->setCurrentIndex(0);
    ui->courseTimeEditBegin->setCurrentIndex(0);
    ui->courseTimeEditEnd->setCurrentIndex(0);
    ui->courseLocationEdit->clear();
    ui->courseTeacherEdit->clear();
    ui->courseLinkEdit->clear();
}

