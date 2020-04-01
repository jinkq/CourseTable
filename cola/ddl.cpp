﻿#include "ddl.h"
#include "ui_ddl.h"
#include<QSqlQuery>
#include<QDateTime>
#include<QMessageBox>
#include<QDebug>

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

    //处理添加ddl的信号
    connect(ui->addDdlButton,&QPushButton::clicked,this,&DDL::addDdl);

    //处理删除ddl的信号
    connect(ui->delDdlButton,&QPushButton::clicked,this,&DDL::delDdl);

}

void DDL::initDdlTable()
{
    //适应长度
    ui->ddlTable->horizontalHeader()->setStretchLastSection(true);

    //清空ddl列表
    ddlList.clear();

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
    int row=0,col=5;
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
        //新增一个ddl struct并加入到总的ddlList中去
        ddl newDdl;

        newDdl.ddl_id = query.value("ddl_id").toInt();//ddl_id
        newDdl.course_id = query.value("course_id").toInt();//course_id
        newDdl.ddlContent = query.value("ddlContent").toString();//ddl名
        newDdl.ddlRequirement = query.value("ddlRequirement").toString();//ddl要求
        newDdl.ddlTime = query.value("ddlTime").toString();//ddl截止时间
        newDdl.ddlStatus = query.value("ddlStatus").toInt();//ddl状态

        //插入ddlList中
        this->ddlList << newDdl;

        //底部插入新行
        ui->ddlTable->insertRow(rowNum);

        //插入ddl内容
        QLabel * contentLabel = new QLabel();
        contentLabel->setText(newDdl.ddlContent);
        ui->ddlTable->setCellWidget(rowNum,0,contentLabel);

        //插入ddl要求
        QLabel * requirementLabel = new QLabel();
        requirementLabel->setText(newDdl.ddlRequirement);
        ui->ddlTable->setCellWidget(rowNum,1,requirementLabel);

        //插入ddl截止时间
        QLabel * timeLabel = new QLabel();
        timeLabel->setText(newDdl.ddlTime);
        ui->ddlTable->setCellWidget(rowNum,2,timeLabel);

        //插入ddl状态
        QLabel * statusLabel=new QLabel();
        QString status;
        switch(newDdl.ddlStatus)
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
        if(newDdl.ddlStatus==0||newDdl.ddlStatus==2)
        {
            //插入已完成按钮
            QPushButton * finishButton=new QPushButton(this);
            finishButton->setText("已完成");
            ui->ddlTable->setCellWidget(rowNum,4,finishButton);

            //处理完成信号
            connect(finishButton,&QPushButton::clicked,
                  [=]()
            {
                ddlList[rowNum].ddlStatus=1;
                QSqlQuery query1;
                query1.exec(QString("update courseDdl set ddlStatus = 1 where ddl_id = %1;")
                            .arg(newDdl.ddl_id));

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
    ui->ddlTable->insertRow(rowNum);
}

void DDL::addDdl()
{
    //当前行数（包含最后的空行）
    int row=ui->ddlTable->rowCount();

    QTableWidgetItem *ddlContentItem=ui->ddlTable->item(row-1,0);
    QTableWidgetItem *ddlRequirementItem=ui->ddlTable->item(row-1,1);
    QTableWidgetItem *ddlTimeItem=ui->ddlTable->item(row-1,2);
    QTableWidgetItem *ddlStatusItem=ui->ddlTable->item(row-1,3);

    //获得添加的内容（新一行）
    if(ddlContentItem==nullptr||ddlRequirementItem==nullptr)
    {
        QMessageBox::warning(this,"error","不能提交空添加");
        return;
    }
    QString ddlContent=ddlContentItem->text();
    QString ddlRequirement = ddlRequirementItem->text();

    //判断输入时间的合法性
    QString ddlTime=ddlTimeItem->text();
    if(!isValidTime(ddlTime))
    {
        ddlTimeItem->setText("");
        QMessageBox::warning(this,"error","非法的ddl截止时间，格式：2020-01-01 12:00");
        return;
    }

   int ddlStatus=0;

    //item设为空
    ddlContentItem->setText("");
    ddlRequirementItem->setText("");
    ddlTimeItem->setText("");
    //ddlStatusItem->setText("");

    //添加到list
    ddl newDdl;
    newDdl.course_id = course_id;
    newDdl.ddlContent = ddlContent;
    newDdl.ddlRequirement = ddlRequirement;
    newDdl.ddlTime = ddlTime;
    newDdl.ddlStatus = ddlStatus;

    //添加到数据库
    QSqlQuery query;
    query.exec(QString("insert into courseDdl(course_id, ddlContent, ddlRequirement, ddlTime, ddlStatus)"
                       " values (%1, '%2', '%3', '%4', %5);")
               .arg(course_id).arg(ddlContent).arg(ddlRequirement).arg(ddlTime)
               .arg(ddlStatus));

    //读ddl_id出来
    query.exec(QString("select ddl_id from courseDdl where course_id = '%1' and ddlContent = '%2' and ddlRequirement = '%3' and ddlTime = '%4' and ddlStatus = '%5';")
               .arg(newDdl.course_id).arg(newDdl.ddlContent).arg(newDdl.ddlRequirement)
               .arg(newDdl.ddlTime).arg(newDdl.ddlStatus));
    query.next();
    newDdl.ddl_id = query.value("ddl_id").toInt();

    qDebug()<<newDdl.ddl_id;


    ddlList << newDdl;

    //label插入表格
    QLabel* ddlContentLabel=new QLabel(this);
    QLabel* ddlRequirementLabel=new QLabel(this);
    QLabel* ddlTimeLabel=new QLabel(this);
    QLabel* ddlStatusLabel=new QLabel(this);

    ddlContentLabel->setText(ddlContent);
    ui->ddlTable->setCellWidget(row-1,0,ddlContentLabel);

    ddlRequirementLabel->setText(ddlRequirement);
    ui->ddlTable->setCellWidget(row-1,1,ddlRequirementLabel);

    ddlTimeLabel->setText(ddlTime);
    ui->ddlTable->setCellWidget(row-1,2,ddlTimeLabel);

    ddlStatusLabel->setText("待完成");
    ui->ddlTable->setCellWidget(row-1,3,ddlStatusLabel);

    //插入已完成按钮
    QPushButton * finishButton=new QPushButton(this);
    finishButton->setText("已完成");
    ui->ddlTable->setCellWidget(row-1,4,finishButton);

    //处理完成信号
    connect(finishButton,&QPushButton::clicked,
          [=]()mutable
    {
        newDdl.ddlStatus=1;
        QSqlQuery query1;
        query1.exec(QString("update courseDdl set ddlStatus = 1 where ddl_id = %1;")
                    .arg(newDdl.ddl_id));
        //qDebug()<<newDdl.ddl_id;
        //更改表格中的status
        ddlStatusLabel->setText("已完成");

        //删除按钮
        ui->ddlTable->removeCellWidget(row-1,4);
        delete finishButton;

    }
        );


    //下面再加空行
    ui->ddlTable->insertRow(row);


    QMessageBox::information(this,"success","添加成功");
}

bool DDL::isValidTime(const QString time)
{
    QRegExp rx("\\d{4}[-]\\d{2}[-]\\d{2}[ ]\\d{2}\:[0-5]\\d");

    if(rx.exactMatch(time))
    {
        return true;
    }
    return false;
}

void DDL::delDdl()
{
    //定位到行
    int currentRow=ui->ddlTable->currentRow();

    //加一个判断行是否删除空的（未选中返回-1）
    if(currentRow<0||currentRow==ui->ddlTable->rowCount()-1)
    {
        QMessageBox::warning(this,"error","删除不能为空");
        return;
    }

    //从list中读取ddl_id
    int delDdl_id = ddlList[currentRow].ddl_id;

    //读完后从list中删除掉
    ddlList.removeAt(currentRow);

    //从数据库中删除
    QSqlQuery query;
    query.exec(QString("Delete from courseDdl where ddl_id = %1;")
               .arg(delDdl_id));

    //删除当前行
    ui->ddlTable->removeRow(currentRow);

    QMessageBox::information(this,"success","删除成功");
    //initLinkTable();
}

void DDL::saveDdl()
{
    if(!ddl_db.open())
       {
            QMessageBox::warning(this,QStringLiteral("错误"),"error");
            qDebug()<<"open sql error (in ddl)";
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
