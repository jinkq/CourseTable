#include "ddl.h"
#include "ui_ddl.h"
#include<QSqlQuery>

DDL::DDL(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DDL)
{
    ui->setupUi(this);

    setWindowTitle("ddl");

    //去除焦点
    ui->ddlTable->setFocusPolicy(Qt::NoFocus);

    //发送返回课程页面信号
    connect(ui->returnButton, &QPushButton::clicked, this, &DDL::goback);
}

DDL::~DDL()
{
    delete ui;
}

void DDL::goback()
{
    emit this->back2Course();
}

void DDL::run(int courseId)
{
    //传递course_id为成员
    this->course_id=courseId;

    //初始化ddlTable（加载信息）
    initDdlTable();
    //TODO: 设置第一列不可编辑


}

void DDL::initDdlTable()
{
    //获取对应course_id的courseName
    QSqlQuery query;
    query.exec(QString("select * from courseInfo where course_id = %1;").arg(course_id));
    while(query.next())
    {
        courseName=query.value("courseName").toString();
    }

    //设置标题
    ui->titleLabel->setText(courseName);

    //一行显示完整内容
    ui->ddlTable->resizeColumnsToContents();

    this->show();

    //设置行数、列数
    int row=1,col=2;
    ui->ddlTable->setRowCount(row);
    ui->ddlTable->setColumnCount(col);

    //设置表头
    QStringList header;
    //将表头写入表格
    header<<"ddl"<<"截止时间";
    ui->ddlTable->setHorizontalHeaderLabels(header);

    //查询课程编号为course_id的所有link信息
    //QSqlQuery query;
    query.exec(QString("select * from courseDdl where course_id = %1").arg(course_id));

    int rowNum = 0;
    while(query.next())
    {
        ddlNameList << query.value(2).toString();//ddl名
        //ddlAddressList<< query.value(3).toString();//链接地址
        ui->ddlTable->insertRow(rowNum);

//        //插入课程名
//        QLabel * courseLabel = new QLabel();
//        courseLabel->setText(courseName);
//        ui->ddlTable->setCellWidget(rowNum,0,courseLabel);

        //插入ddl名
        QLabel * nameLabel = new QLabel();
        nameLabel->setText(ddlNameList[rowNum]);
        ui->ddlTable->setCellWidget(rowNum,0,nameLabel);

        rowNum++;
    }
}
