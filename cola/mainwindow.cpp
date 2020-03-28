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

    //你调用这个函数之前要先传db

    /*//进入课程详细信息页面
    connect(ui->enterLessonButton,&QPushButton::clicked,
            [=]
    {
        this->hide();
        this->course.show();
    });*/

    //处理创建课程按钮的信号
    connect(&addCourse,&AddCourse::courseButtonSignal,this,&MainWindow::courseButtonSlot);

    //要接收从课程基本信息页面的返回信号
    connect(&course, &Course::back2Main,
            [=]
    {
        course.hide();
        this->show();
    });


    /*QList<QFrame*> frameList = ui->widget->findChildren<QFrame*>();
    for(int i = 0; i < frameList.size(); i++)
    {
        QFrame *frame = frameList.at(i);

        QList<QLabel*> labelList = frame->findChildren<QLabel*>();
        if(labelList.size()>0)
        {
            labelList[0]->setText(QString::number(i));
        }
    }*/

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSql()
{
    //添加MySql数据库
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

    //将数据库内容写入table
    QSqlQuery query;
    query.exec("select * from schedule;");

    for(int i = 0; query.next(); i++)
    {
        for(int j = 0; j < 8; j++)
        {
            ui->courseTable->setItem(i,j, new QTableWidgetItem(query.value(j).toString()));
        }
    }
}

/*//day代表星期几，n代表是星期几的第几节课，courseName代表课程名字
void MainWindow::addACourse(int day, int n, QString courseName)
{
    //通过day,n在framelist中找到对应的frame
    QFrame* t = framelist[(day-1)+n-1];

    //找到frame中的label
    QList<QLabel*> labelList = t->findChildren<QLabel*>();

    //修改label中的内容
    labelList.at(0)->setText(courseName);
}*/


void MainWindow::on_addCourseButton_clicked()//添加新课程
{
   //模态对话框，添加新课程时不能做别的事情
   addCourse.exec();
}

void MainWindow::courseButtonSlot(QString courseName, int courseDay, int courseTimeBegin, int courseTimeEnd, QString courseLocation)
{
    if(courseTimeEnd>courseTimeBegin)
    {
        //合并单元格
        ui->courseTable->setSpan(courseTimeBegin-1,courseDay,(courseTimeEnd-courseTimeBegin+1),1);
    }

    //往表格添加按钮
    QPushButton *courseButton=new QPushButton(this);
    courseButton->setText(QString("%1\n(%2)").arg(courseName).arg(courseLocation));
    ui->courseTable->setCellWidget(courseTimeBegin-1,courseDay,courseButton);




}
