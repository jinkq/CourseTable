#include "ddl.h"
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

    //处理添加ddl的信号
    connect(ui->addDdlButton,&QPushButton::clicked,this,&DDL::addDdl);

    //处理删除ddl的信号
    connect(ui->delDdlButton,&QPushButton::clicked,this,&DDL::delDdl);

    //处理保存ddl的信号
    connect(ui->saveDdlButton, &QPushButton::clicked,this,&DDL::saveDdl);

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
    //最后一列填满表格
    //ui->ddlTable->horizontalHeader()->setStretchLastSection(true);

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
    header<<"ddl（必填）"<<"要求（必填，没有则填\"无\"）"<<"截止时间（必填）\n（格式：2020-01-01 00:00）"<<"状态（不用填写，自动生成）"<<"";
    ui->ddlTable->setHorizontalHeaderLabels(header);

    //表格列宽
    ui->ddlTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->ddlTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->ddlTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->ddlTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

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
        QLineEdit * contentLabel = new QLineEdit();
        contentLabel->setText(newDdl.ddlContent);
        ui->ddlTable->setCellWidget(rowNum,0,contentLabel);

        //插入ddl要求
        QLineEdit * requirementLabel = new QLineEdit();
        requirementLabel->setText(newDdl.ddlRequirement);
        ui->ddlTable->setCellWidget(rowNum,1,requirementLabel);

        //插入ddl截止时间
        QLineEdit * timeLabel = new QLineEdit();
        timeLabel->setText(newDdl.ddlTime);
        ui->ddlTable->setCellWidget(rowNum,2,timeLabel);

        //插入ddl状态
        QString currentTime= QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
        if(newDdl.ddlStatus==0&&compareTime(currentTime,newDdl.ddlTime))
        {
            //逾期
            newDdl.ddlStatus=2;
        }

        QLabel * statusLabel=new QLabel();
        QString status;
        switch(newDdl.ddlStatus)
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
                statusLabel->setStyleSheet("font:bold;color:rgb(73,183,110)");

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
    //QTableWidgetItem *ddlStatusItem=ui->ddlTable->item(row-1,3);

    //获得添加的内容（新一行）
    if(ddlContentItem==nullptr||ddlRequirementItem==nullptr||ddlTimeItem==nullptr)
    {
        QMessageBox::warning(this,"error","添加失败，ddl内容、要求、时间均不能为空");
        return;
    }
    QString ddlContent=ddlContentItem->text();
    QString ddlRequirement = ddlRequirementItem->text();

    //判断输入时间的合法性
    QString ddlTime=ddlTimeItem->text();
    if(!isValidTime(ddlTime))
    {
        ddlTimeItem->setText("");
        QMessageBox::warning(this,"error","添加失败，非法的ddl截止时间，格式：2020-01-01 12:00");
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
    QLineEdit* ddlContentLabel=new QLineEdit(this);
    QLineEdit* ddlRequirementLabel=new QLineEdit(this);
    QLineEdit* ddlTimeLabel=new QLineEdit(this);
    QLabel* ddlStatusLabel=new QLabel(this);

    ddlContentLabel->setText(ddlContent);
    ui->ddlTable->setCellWidget(row-1,0,ddlContentLabel);

    ddlRequirementLabel->setText(ddlRequirement);
    ui->ddlTable->setCellWidget(row-1,1,ddlRequirementLabel);

    ddlTimeLabel->setText(ddlTime);
    ui->ddlTable->setCellWidget(row-1,2,ddlTimeLabel);

    ddlStatusLabel->setText("待完成");
    ddlStatusLabel->setStyleSheet("font:bold;color:rgb(250,157,6)");
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
        ddlStatusLabel->setStyleSheet("font:bold;color:rgb(73,183,110)");

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
    QRegExp rx("\\d{4}[-][0-1]\\d[-][0-3]\\d[ ]\\d{2}\:[0-5]\\d");

    if(rx.exactMatch(time))
    {
        return true;
    }
    return false;
}

void DDL::delDdl()
{
    int sure=QMessageBox::question(this,"sure","确定要删除该ddl吗？");
    if(sure==QMessageBox::Yes)
    {
        //定位到行
        int currentRow=ui->ddlTable->currentRow();

        //加一个判断行是否删除空的（未选中返回-1）
        if(currentRow<0||currentRow==ui->ddlTable->rowCount()-1)
        {
            QMessageBox::warning(this,"error","删除失败，删除不能为空");
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
    }
}

//将中文的状态转化为数字
int statusTransorm(QString status)
{
    if (QString::compare(status, QString("已完成")) == 0)
    {
        return 1;
    }
    else if (QString::compare(status, "待完成") == 0)
    {
        return 0;
    }
    else if (QString::compare(status, "逾期") == 0)
    {
        return 2;
    }
    return -1;
}

void DDL::saveDdl()
{
    bool success=true;//成功

    QSqlQuery query;
    //依次去读出每一行的状态，更新到ddlList中，然后更新数据库
    for (int i = 0;i < ui->ddlTable->rowCount()-1;i++)
    {
        qDebug() << i;
        //更新list中的信息（强制类型转换）
        this->ddlList[i].ddlContent = dynamic_cast<QLineEdit*>(ui->ddlTable->cellWidget(i,0))->text();
        this->ddlList[i].ddlRequirement = dynamic_cast<QLineEdit*>(ui->ddlTable->cellWidget(i,1))->text();
        this->ddlList[i].ddlTime = dynamic_cast<QLineEdit*>(ui->ddlTable->cellWidget(i,2))->text();
        this->ddlList[i].ddlStatus = statusTransorm(dynamic_cast<QLabel*>(ui->ddlTable->cellWidget(i,3))->text());
        //检查时间的合法性
        if(!isValidTime(ddlList[i].ddlTime))
        {
            QMessageBox::warning(this,"error",QString("第%1行:非法的ddl截止时间，格式：2020-01-01 12:00").arg(i+1));
            success=false;
            continue;
        }
        //写到数据库中
        //将编辑区内容写入sql
        query.exec(QString("update courseDdl set ddlContent = '%1', ddlRequirement = '%2',ddlTime = '%3', ddlStatus = '%4' where ddl_id = '%5';")
                   .arg(ddlList[i].ddlContent).arg(ddlList[i].ddlRequirement)
                   .arg(ddlList[i].ddlTime).arg(ddlList[i].ddlStatus).arg(ddlList[i].ddl_id));
    }
    if(success)
    {
        //保存成功
        QMessageBox::information(this,"success","保存成功");
    }
}

bool DDL::compareTime(QString time1, QString time2)
{
   if(time1>=time2)
        return true;
   return false;
}
