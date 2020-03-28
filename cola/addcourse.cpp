#include "addcourse.h"
#include "ui_addcourse.h"
#include<QSqlQuery>
#include<QDebug>
#include<QMessageBox>

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

void AddCourse::addCourseName()
{
    if(!addcourse_db.open())
       {
            QMessageBox::warning(this,QStringLiteral("错误"),"error");
            qDebug()<<"open sql error (in addcourse)";
            exit(0);//退出程序
        }

    QString courseName=ui->courseNameEdit->text();
    QString courseLocation=ui->courseLocationEdit->text();
    QString courseTeacher=ui->courseTeacherEdit->text();

    //query.exec("insert into course(course_id) values (2);");

    //query.exec(QString("insert into course(课程名, 课程地点, 课程教师) values ('%1', '%2', '%3');")
      //         .arg(courseName).arg(courseLocation).arg(courseTeacher));

    QSqlQuery query;
    query.exec("select * from course;");

    while(query.next())//看有无内容
    {
        //一行一行遍历
        //取出当前行的内容
        qDebug()<<query.value(0).toInt()//类型转换
                <<query.value(1).toString();
    }
}
