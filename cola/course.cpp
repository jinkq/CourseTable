#include "course.h"
#include "ui_course.h"
#include<QDebug>
#include<QMessageBox>

Course::Course(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::course)
{
    ui->setupUi(this);

    //设置窗口标题
    setWindowTitle("课程信息");

    //传递数据库
    this->courseLink.link_db = this->course_db;
    this->courseDdl.ddl_db=this->course_db;
    this->courseNote.note_db=this->course_db;

    //转到ddl界面
    connect(ui->ddlButton, &QPushButton::clicked,
            [=]
    {
        this->hide();
        this->courseDdl.run(course_id);
    });

    //处理从ddl回来的
    connect(&courseDdl, &DDL::back2Course,
            [=]
            {
                this->courseDdl.close();
                this->show();
            }
    );

    //转到link界面
    connect(ui->linkButton, &QPushButton::clicked,
            [=]
    {
        this->hide();
        this->courseLink.run(course_id);
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
        this->courseNote.run(course_id);
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

    //处理保存修改
    connect(ui->saveButton,&QPushButton::clicked,this,&Course::save);

    //处理删除课程
    connect(ui->delButton,&QPushButton::clicked,this,&Course::del);
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

void Course::run(int courseId)
{
    //传course_id
    course_id=courseId;

    //初始化编辑区信息
    initEdit();

    //展示界面
    this->show();
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
    qDebug() << "save!";
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
    emit changeCourseButtonSignal();

    qDebug() << "here";
    this->close();
}

void Course::del()
{
    QSqlQuery query;
    //从sql中删除课程
    query.exec(QString("delete from courseInfo where course_id = %1;")
               .arg(course_id));

    //删除成功
    QMessageBox::information(this,"success","删除成功");

    //向MainWindow发信号，修改课程按钮
    this->close();
    emit delCourseButtonSignal();
}
