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

    //设置dialog button文字
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
}

AddCourse::~AddCourse()
{
    delete ui;
}

void AddCourse::on_buttonBox_accepted()
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
        QMessageBox::warning(this,"error","添加失败，课程名不能为空");
        //isValid=false;
        //continue;
        return;
    }

    //判断课程节数是否合法
    if(courseTimeBegin>courseTimeEnd)
    {
        QMessageBox::warning(this,"error","添加失败，输入的课程节数不合法");
        return;
    }

    //判断课程时间冲突
    if(conflict(courseDay,courseTimeBegin,courseTimeEnd))
    {
        QMessageBox::warning(this,"error","添加失败，新添加的课程与已有课程时间冲突");
        return;
    }

    QSqlQuery query;
    //将编辑区内容写入sql
    query.exec(QString("insert into courseInfo(courseName, courseDay,courseTimeBegin, courseTimeEnd, courseLocation, courseTeacher) values ('%1', %2, %3, %4, '%5', '%6');")
               .arg(courseName).arg(courseDay).arg(courseTimeBegin).arg(courseTimeEnd).arg(courseLocation).arg(courseTeacher));

    //向MainWindow发信号，创建课程按钮
    emit courseButtonSignal(courseName, courseDay,courseTimeBegin, courseTimeEnd, courseLocation,courseTeacher);
}

void AddCourse::clearEdit()
{
    ui->courseNameEdit->clear();
    ui->courseDayEdit->setCurrentIndex(0);
    ui->courseTimeEditBegin->setCurrentIndex(0);
    ui->courseTimeEditEnd->setCurrentIndex(0);
    ui->courseLocationEdit->clear();
    ui->courseTeacherEdit->clear();
}

bool AddCourse::conflict(int courseDay, int courseTimeBegin, int courseTimeEnd)
{
    QSqlQuery query;
    query.exec(QString("select * from courseInfo where courseDay = %1 and courseTimeBegin <= %2 and courseTimeEnd >= %3;")
               .arg(courseDay).arg(courseTimeBegin).arg(courseTimeEnd));

    int num=0;//用于判断有无冲突
    while(query.next())
    {
        num++;
        qDebug()<<query.value(1).toString();
    }

    query.exec(QString("select * from courseInfo where courseDay = %1 and courseTimeBegin >= %2 and courseTimeBegin <= %3;")
               .arg(courseDay).arg(courseTimeBegin).arg(courseTimeEnd));
    while(query.next())
    {
        num++;
        qDebug()<<2;
    }

    query.exec(QString("select * from courseInfo where courseDay = %1 and courseTimeEnd >= %2 and courseTimeEnd <= %3;")
               .arg(courseDay).arg(courseTimeBegin).arg(courseTimeEnd));
    while(query.next())
    {
        num++;
        qDebug()<<3;
    }

    if(num>0)
    {
        return true;
    }
    return false;
}
