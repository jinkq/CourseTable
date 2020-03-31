#include "ddl.h"
#include "ui_ddl.h"
#include<QSqlQuery>
#include<QDateTime>

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


}

void DDL::initDdlTable()
{
    //适应长度
    ui->ddlTable->horizontalHeader()->setStretchLastSection(true);

    //清空三个list
    ddlList.clear();
//    ddlContentList.clear();
//    ddlRequirementList.clear();
//    ddlTimeList.clear();
//    ddlStatusList.clear();

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
    //ui->ddlTable->resizeColumnsToContents();

    this->show();

    //设置行数、列数
    int row=1,col=5;
    ui->ddlTable->setRowCount(row);
    ui->ddlTable->setColumnCount(col);

    //设置表头
    QStringList header;
    //将表头写入表格
    header<<"ddl"<<"要求"<<"截止时间"<<"状态"<<"";
    ui->ddlTable->setHorizontalHeaderLabels(header);

    //查询课程编号为course_id的所有ddl信息
    //QSqlQuery query;
    query.exec(QString("select * from courseDdl where course_id = %1").arg(course_id));

    int rowNum = 0;
    while(query.next())
    {
        ddlContentList << query.value("ddlContent").toString();//ddl名
        ddlRequirementList<< query.value("ddlRequirement").toString();//ddl要求
        ddlTimeList<< query.value("ddlTime").toDateTime();//ddl截止时间
        ddlStatusList<<query.value("ddlStatus").toInt();//ddl状态

        //底部插入新行
        ui->ddlTable->insertRow(rowNum);

        //插入ddl内容
        QLabel * contentLabel = new QLabel();
        contentLabel->setText(ddlContentList[rowNum]);
        ui->ddlTable->setCellWidget(rowNum,0,contentLabel);

        //插入ddl要求
        QLabel * requirementLabel = new QLabel();
        requirementLabel->setText(ddlRequirementList[rowNum]);
        ui->ddlTable->setCellWidget(rowNum,1,requirementLabel);

        //插入ddl截止时间
        QLabel * timeLabel = new QLabel();
        timeLabel->setText(ddlTimeList[rowNum].toString("yyyy-MM-dd hh:mm"));
        ui->ddlTable->setCellWidget(rowNum,2,timeLabel);

        //插入ddl状态
        QLabel * statusLabel=new QLabel();
        QString status;
        switch(ddlStatusList[rowNum])
        {
        case 0:status="待完成";
            break;
        case 1:status="已完成";
            break;
        case 2:status="逾期";
            break;
        default:break;
        }

        statusLabel->setText(status);
        ui->ddlTable->setCellWidget(rowNum,3,statusLabel);

        //待完成或逾期
        if(ddlStatusList[rowNum]==0||ddlStatusList[rowNum]==2)
        {
            //插入已完成按钮
            QPushButton * finishButton=new QPushButton(this);
            finishButton->setText("已完成");
            ui->ddlTable->setCellWidget(rowNum,4,finishButton);

            //处理完成信号
            connect(finishButton,&QPushButton::clicked,
                  [=]()
            {
                ddlStatusList[rowNum]=1;
                QSqlQuery query1;
                query1.exec(QString("update courseDdl set ddlStatus = 1 where ddlContent = '%1', "
                            "ddlRequirement = '%2', ddlTime = %3;").arg(ddlContentList[rowNum])
                            .arg(ddlRequirementList[rowNum]).arg(ddlTimeList[rowNum]));

                //更改表格中的status
                statusLabel->setText("已完成");

                //删除按钮
                ui->ddlTable->removeCellWidget(rowNum,4);
                delete finishButton;

            }
                );
        }
        rowNum++;
    }
}
