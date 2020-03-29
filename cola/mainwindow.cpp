#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << QSqlDatabase::drivers();

    //连接数据库
    connectSql();

    //初始化课程表
    initCourseTable();

    //传递db
    this->addCourse.addcourse_db = this->db;
    this->course.course_db=this->db;

    //调用这个函数之前要先传db


    //处理创建课程按钮的信号
    connect(&addCourse,&AddCourse::courseButtonSignal,this,&MainWindow::addCourseButton);

    //处理修改课程按钮的信号
    connect(&course,&Course::changeCourseButtonSignal,this,&MainWindow::changeCourseButton);

    //处理删除课程的信号
    connect(&course,&Course::delCourseButtonSignal,this,&MainWindow::delCourseButton);

    //要接收从课程基本信息页面的返回信号
    connect(&course, &Course::back2Main,
            [=]
    {
        course.hide();
        this->show();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSql()
{
    //添加Sql数据库
    this->db=QSqlDatabase::addDatabase("QSQLITE");

    //连接数据库
//    db.setHostName("127.0.0.1");//数据库服务器IP
//    db.setUserName("root");//数据库用户名
//    db.setPassword("pzjinkq");//密码
    db.setDatabaseName("../database/cms.db");//使用哪个数据库

    //打开数据库
    if(!db.open())
    {
        QMessageBox::warning(this,QStringLiteral("错误"),"error");
        qDebug()<<"open sql error";
        exit(0);//退出程序
    }
}

void MainWindow::initCourseTable()
{
    //设置行数、列数
    int row=11,col=8;
    ui->courseTable->setRowCount(row);
    ui->courseTable->setColumnCount(col);

    //设置表头
    QStringList header;
    //将表头写入表格
    header<<"时间"<<"星期一"<<"星期二"<<"星期三"<<"星期四"<<"星期五"<<"星期六"<<"星期日";
    ui->courseTable->setHorizontalHeaderLabels(header);

    //将schedule第一列内容写入table
    QSqlQuery query;
    query.exec("select * from schedule;");

    for(int i = 0; query.next(); i++)
    {
       ui->courseTable->setItem(i,0, new QTableWidgetItem(query.value(0).toString()));
    }

    //创建课程按钮
    QString courseName;
    int courseDay;
    int courseTimeBegin;
    int courseTimeEnd;
    QString courseLocation;
    QString courseTeacher;

    query.exec("select * from courseInfo;");
    while(query.next())
    {
        //获取信息
        courseName=query.value("courseName").toString();
        courseDay=query.value("courseDay").toInt();
        courseTimeBegin=query.value("courseTimeBegin").toInt();
        courseTimeEnd=query.value("courseTimeEnd").toInt();
        courseLocation=query.value("courseLocation").toString();
        courseTeacher=query.value("courseTeacher").toString();

        //添加课程按钮
        addCourseButton(courseName,courseDay,courseTimeBegin,courseTimeEnd,courseLocation,courseTeacher);
    }
}

void MainWindow::on_addCourseButton_clicked()//添加新课程
{
    //清空编辑区
    addCourse.clearEdit();

    //模态对话框，添加新课程时不能做别的事情
    addCourse.exec();
}

void MainWindow::addCourseButton(QString courseName, int courseDay, int courseTimeBegin, int courseTimeEnd, QString courseLocation,QString courseTeacher)
{
    if(courseTimeEnd>courseTimeBegin)
    {
        //合并单元格
        ui->courseTable->setSpan(courseTimeBegin-1,courseDay,(courseTimeEnd-courseTimeBegin+1),1);
    }

    //往表格添加按钮
    //QPushButton *courseButton=new QPushButton(this);
    courseButton=new QPushButton(this);
    courseButton->setText(QString("%1\n(%2)").arg(courseName).arg(courseLocation));
    ui->courseTable->setCellWidget(courseTimeBegin-1,courseDay,courseButton);

    //获取courseButton对应的course_id
    int course_id;
    QSqlQuery query;
    query.exec(QString("select * from courseInfo where courseName = '%1';").arg(courseName));
    while(query.next())
    {
        course_id=query.value(0).toInt();
    }

    //将课程按钮转到课程界面
    connect(courseButton,&QPushButton::clicked,
            [=]()
    {
        this->hide();
        this->course.course_id=course_id;//传递course_id
        this->course.courseName=courseName;
        this->course.courseDay=courseDay;
        this->course.courseTimeBegin=courseTimeBegin;
        this->course.courseTimeEnd=courseTimeEnd;
        this->course.courseLocation=courseLocation;
        this->course.courseTeacher=courseTeacher;

        this->course.run();
    }
    );
}

void MainWindow::changeCourseButton(QString courseName, int courseDay, int courseTimeBegin, int courseTimeEnd, QString courseLocation,QString courseTeacher)
{
    if(courseTimeEnd>courseTimeBegin)
    {
        //合并单元格
        ui->courseTable->setSpan(courseTimeBegin-1,courseDay,(courseTimeEnd-courseTimeBegin+1),1);
    }

    //修改按钮
    courseButton->setText(QString("%1\n(%2)").arg(courseName).arg(courseLocation));
    ui->courseTable->setCellWidget(courseTimeBegin-1,courseDay,courseButton);

    connect(courseButton,&QPushButton::clicked,
            [=]()
    {
        this->hide();
        this->course.courseName=courseName;
        this->course.courseDay=courseDay;
        this->course.courseTimeBegin=courseTimeBegin;
        this->course.courseTimeEnd=courseTimeEnd;
        this->course.courseLocation=courseLocation;
        this->course.courseTeacher=courseTeacher;

        this->course.run();
    }
    );
}

void MainWindow::delCourseButton(int courseTimeBegin,int courseDay)
{
    ui->courseTable->removeCellWidget(courseTimeBegin-1,courseDay);
    delete courseButton;
}
