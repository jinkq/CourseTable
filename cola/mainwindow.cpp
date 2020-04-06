#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //qDebug() << QSqlDatabase::drivers();

    //连接数据库
    connectSql();

    //设置tabWidget标题
    ui->tabWidget->setTabText(0,"课程表");
    ui->tabWidget->setTabText(1,"DDL表");

    //初始化main窗口的所有内容
    initMain();

    //传递db
    this->addCourse.addcourse_db = this->db;
    this->course.course_db=this->db;

    //处理创建课程按钮的信号
    connect(&addCourse,&AddCourse::courseButtonSignal,this,&MainWindow::addCourseButton);

    //处理修改课程按钮的信号
    connect(&course,&Course::changeCourseButtonSignal,this,&MainWindow::initMain);

    //处理删除课程的信号
    connect(&course,&Course::delCourseButtonSignal,this,&MainWindow::initMain);

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
    //适应长度
    //ui->courseTable->horizontalHeader()->setStretchLastSection(true);

    //初始化表格
    ui->courseTable = new QTableWidget(ui->tab);
    //ui->courseTable->verticalHeader()->setVisible(false); //设置垂直头不可见,即左边序号不见
    ui->courseTable->setGeometry(10,0,1281,675);

    //设置行数、列数
    int row=11,col=8;
    ui->courseTable->setRowCount(row);
    ui->courseTable->setColumnCount(col);

    ui->courseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->courseTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

        //记录course_id和courseName
        this->courseId_Name.insert(query.value("course_id").toInt(),courseName);

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
        //this->course.course_id=course_id;//传递course_id
        this->course.courseName=courseName;
        this->course.courseDay=courseDay;
        this->course.courseTimeBegin=courseTimeBegin;
        this->course.courseTimeEnd=courseTimeEnd;
        this->course.courseLocation=courseLocation;
        this->course.courseTeacher=courseTeacher;

        this->course.run(course_id);
    }
    );
}

void MainWindow::initDdlTable()
{
    //初始化表格
    ui->ddlTable = new QTableWidget(ui->tab_2);
    //ui->courseTable->verticalHeader()->setVisible(false); //设置垂直头不可见,即左边序号不见
    ui->ddlTable->setGeometry(10,0,1281,675);

    //设置行数、列数
    int row=0,col=6;
    ui->ddlTable->setRowCount(row);
    ui->ddlTable->setColumnCount(col);

    //设置自适应
    ui->ddlTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ddlTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //设置表头
    QStringList header;
    //将表头写入表格
    header<<"课程名"<<"ddl"<<"要求"<<"截止时间"<<"状态"<<"";
    ui->ddlTable->setHorizontalHeaderLabels(header);

    //将ddl信息全部读出，放到ddlList中
    QSqlQuery query;
    query.exec("select * from courseDdl;");
    while(query.next())
    {
        ddl newddl;
        newddl.ddl_id = query.value("ddl_id").toInt();
        newddl.course_id = query.value("course_id").toInt();
        newddl.ddlContent = query.value("ddlContent").toString();
        newddl.ddlRequirement = query.value("ddlRequirement").toString();
        newddl.ddlTime = query.value("ddlTime").toString();
        newddl.ddlStatus = query.value("ddlStatus").toInt();
        insert(newddl,ddlList);
    }

    //将所有ddl展示出来
    for(int i = 0;i < ddlList.size();i++)
    {
        ui->ddlTable->insertRow(i);

        //插入ddl课程名
        QLabel * courseNameLabel = new QLabel();
        courseNameLabel->setText(courseId_Name[ddlList[i].course_id]);
        ui->ddlTable->setCellWidget(i,0,courseNameLabel);

        //插入ddl内容
        QLineEdit * contentLabel = new QLineEdit();
        contentLabel->setText(ddlList[i].ddlContent);
        ui->ddlTable->setCellWidget(i,1,contentLabel);

        //插入ddl要求
        QLineEdit * requirementLabel = new QLineEdit();
        requirementLabel->setText(ddlList[i].ddlRequirement);
        ui->ddlTable->setCellWidget(i,2,requirementLabel);

        //插入ddl截止时间
        QLineEdit * timeLabel = new QLineEdit();
        timeLabel->setText(ddlList[i].ddlTime);
        ui->ddlTable->setCellWidget(i,3,timeLabel);

        //插入ddl状态
        QString currentTime= QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
        if(ddlList[i].ddlStatus==0&&currentTime>ddlList[i].ddlTime)
        {
            //逾期
            ddlList[i].ddlStatus=2;
        }

        QLineEdit * statusLabel=new QLineEdit();
        QString status;
        switch(ddlList[i].ddlStatus)
        {
        case 0:status="待完成";
            statusLabel->setStyleSheet("font:bold;color:rgb(250,157,6)");
            break;
        case 1:status="已完成";
            statusLabel->setStyleSheet("font:bold;color:rgb(73,183,110)");
            break;
        case 2:status="逾期";
            statusLabel->setStyleSheet("font:bold;color:red");
            break;
        default:break;
        }

        statusLabel->setText(status);
        ui->ddlTable->setCellWidget(i,4,statusLabel);

        //待完成或逾期
        if(ddlList[i].ddlStatus==0||ddlList[i].ddlStatus==2)
        {
            //插入已完成按钮
            QPushButton * finishButton=new QPushButton(this);
            finishButton->setText("已完成");
            ui->ddlTable->setCellWidget(i,5,finishButton);

            //处理完成信号
            connect(finishButton,&QPushButton::clicked,
                  [=]()
            {
                ddlList[i].ddlStatus=1;
                QSqlQuery query1;
                query1.exec(QString("update courseDdl set ddlStatus = 1 where ddl_id = %1;")
                            .arg(ddlList[i].ddl_id));

                //更改表格中的status
                statusLabel->setText("已完成");
                statusLabel->setStyleSheet("font:bold;color:rgb(73,183,110)");

                //删除按钮
                ui->ddlTable->removeCellWidget(i,5);
                delete finishButton;

            }
                );
        }
    }
}

void MainWindow::insert(ddl newddl, QList<ddl>& ddlList)
{
    int i = 0;
    while (i < ddlList.size())
    {
        if(!compareTime(newddl, ddlList[i]))
            break;
        i++;
    }
    ddlList.insert(i,newddl);
}

//返回true表示ddl1比ddl2更早截止
bool MainWindow::compareTime(ddl& ddl1, ddl& ddl2)
{
    if(ddl1.ddlTime>=ddl2.ddlTime)
        return true;
    return false;
}

void MainWindow::initMain()
{
    //默认先看到课程表
    ui->tabWidget->setCurrentIndex(0);

    initCourseTable();
    initDdlTable();

    this->show();
}
