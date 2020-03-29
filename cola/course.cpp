#include "course.h"
#include "ui_course.h"
#include<QDebug>
#include<QMessageBox>

Course::Course(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::course)
{
    ui->setupUi(this);



    //转到ddl界面
    connect(ui->ddlButton, &QPushButton::clicked,
            [=]
    {
        this->hide();
        this->courseDdl.show();
    });

    //处理从ddl回来的
    connect(&courseDdl, &DDL::back2Course,
            [=]
            {
                this->courseDdl.hide();
                this->show();
            }
    );

    //转到link界面
    connect(ui->linkButton, &QPushButton::clicked,
            [=]
    {
        this->hide();
        this->courseLink.show();
    });

    //处理从link回来的
    connect(&courseLink, &Link::back2Course,
            [=]
            {
                this->courseLink.hide();
                this->show();
            }
    );

    //转到note界面
    connect(ui->noteButton, &QPushButton::clicked,
            [=]
    {
        this->hide();
        this->courseNote.show();
    });

    //处理从note回来的
    connect(&courseNote, &Note::back2Course,
            [=]
            {
                this->courseNote.hide();
                this->show();
            }
    );

    //回到课程表页面
    connect(ui->returnButton, &QPushButton::clicked,
            [=]
    {
        sendSlot();
    });

}

Course::~Course()
{
    delete ui;
}

//发送回去主页面的信号
void Course::sendSlot()
{
    emit back2Main();
}

void Course::run()
{
    //初始化编辑区信息
    initEdit();

    //展示界面
    this->show();

    //处理保存修改
    connect(ui->saveButton,&QPushButton::clicked,this,&Course::save);

    //处理删除课程
    connect(ui->delButton,&QPushButton::clicked,this,&Course::del);

}

void Course::initEdit()
{
    //初始化编辑区信息
    ui->courseNameEdit->setText(courseName);
    ui->courseDayEdit->setCurrentIndex(courseDay-1);
    ui->courseTimeBeginEdit->setCurrentIndex(courseTimeBegin-1);
    ui->courseTimeEndEdit->setCurrentIndex(courseTimeEnd-1);
    ui->courseLocationEdit->setText(courseLocation);
    ui->courseTeacherEdit->setText(courseTeacher);
}

void Course::save()
{
    if(!course_db.open())
       {
            QMessageBox::warning(this,QStringLiteral("错误"),"error");
            qDebug()<<"open sql error (in course)";
            exit(0);//退出程序
        }

    //获取编辑区内容
    courseName=ui->courseNameEdit->text();
    courseDay=ui->courseDayEdit->currentIndex()+1;
    courseTimeBegin=ui->courseTimeBeginEdit->currentIndex()+1;
    courseTimeEnd=ui->courseTimeEndEdit->currentIndex()+1;
    courseLocation=ui->courseLocationEdit->text();
    courseTeacher=ui->courseTeacherEdit->text();

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
    query.exec(QString("update courseInfo set courseName = '%1', courseDay = %2,courseTimeBegin = %3, courseTimeEnd = %4, courseLocation = '%5',courseTeacher = '%6' where course_id = %7;")
               .arg(courseName).arg(courseDay).arg(courseTimeBegin).arg(courseTimeEnd).arg(courseLocation).arg(courseTeacher).arg(course_id));

    //保存成功
    QMessageBox::information(this,"success","保存成功");

    //向MainWindow发信号，修改课程按钮
    emit changeCourseButtonSignal(courseName, courseDay,courseTimeBegin, courseTimeEnd, courseLocation,courseTeacher);
}

void Course::del()
{
    QSqlQuery query;
    //将编辑区内容写入sql
    query.exec(QString("delete from courseInfo where course_id = %1;")
               .arg(course_id));

    //删除成功
    QMessageBox::information(this,"success","删除成功");

    //向MainWindow发信号，修改课程按钮
    emit delCourseButtonSignal(courseTimeBegin,courseDay);
}
