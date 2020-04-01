#include "link.h"
#include "ui_link.h"
#include <QSqlQuery>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include<QMessageBox>


Link::Link(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Link)
{
    ui->setupUi(this);
    setWindowTitle("链接");

    //去除焦点
    ui-> linkTable->setFocusPolicy(Qt::NoFocus);

    //发送返回课程页面信号
    connect(ui->returnButton, &QPushButton::clicked, this, &Link::goback);

    //处理添加链接的信号
    connect(ui->addLinkButton,&QPushButton::clicked,this,&Link::addLink);

    //处理删除链接的信号
    connect(ui->delLinkButton,&QPushButton::clicked,this,&Link::delLink);
}

Link::~Link()
{
    delete ui;
}

void Link::goback()
{
    emit this->back2Course();
}

void Link::run(int courseId)
{
    //传递course_id为成员
    this->course_id=courseId;

    //初始化linkTable（加载信息）
    initLinkTable();
}

void Link::initLinkTable()
{
    //适应长度
    ui->linkTable->horizontalHeader()->setStretchLastSection(true);

    //先清空link列表
    linkList.clear();

    //获取对应course_id的courseName
    QSqlQuery query;
    query.exec(QString("select * from courseInfo where course_id = %1;").arg(course_id));
    while(query.next())
    {
        courseName=query.value("courseName").toString();
    }

    //设置标题
    ui->titleLabel->setText(courseName);

    //一行显示完整链接
    //ui->linkTable->resizeColumnsToContents();

    this->show();

    //设置行数、列数
    int row=0,col=2;
    ui->linkTable->setRowCount(row);
    ui->linkTable->setColumnCount(col);

    //设置表头
    QStringList header;
    //将表头写入表格
    header<<"名称"<<"地址";
    ui->linkTable->setHorizontalHeaderLabels(header);

    //查询课程编号为course_id的所有link信息
    //QSqlQuery query;
    query.exec(QString("select * from courseLink where course_id = %1").arg(course_id));

    int rowNum = 0;
    while(query.next())
    {
        //创建一个新的link
        link newLink;
        //读取数据
        newLink.link_id = query.value("link_id").toInt();//link id
        newLink.course_id = query.value("course_id").toInt();//course id
        newLink.linkName = query.value(2).toString();//链接名
        newLink.linkAddress = query.value(3).toString();//链接地址

        //插入linkList中
        this->linkList << newLink;

        //底部插入新行
        ui->linkTable->insertRow(rowNum);

        //插入链接名
        QLabel * nameLabel = new QLabel();
        nameLabel->setText(newLink.linkName);
        ui->linkTable->setCellWidget(rowNum,0,nameLabel);

        //插入链接地址
        QLabel * addressLabel = new QLabel();
        addressLabel->setText(QString("<a href=%1>%1</a>")
                              .arg(newLink.linkAddress));
        addressLabel->setOpenExternalLinks(true);
        ui->linkTable->setCellWidget(rowNum,1,addressLabel);

        rowNum++;
    }
    ui->linkTable->insertRow(rowNum);
    //qDebug()<<linkNameList;
}

void Link::addLink()
{
    //当前行数（包含最后的空行）
    int row=ui->linkTable->rowCount();

    QTableWidgetItem *linkNameItem=ui->linkTable->item(row-1,0);
    QTableWidgetItem *linkAddressItem=ui->linkTable->item(row-1,1);

    //获得添加的内容（新一行）
    if(linkNameItem==nullptr||linkAddressItem==nullptr)
    {
        QMessageBox::warning(this,"error","不能提交空添加");
        return;
    }
    QString linkName=linkNameItem->text();
    QString linkAddress = linkAddressItem->text();

    //item设为空
    linkNameItem->setText("");
    linkAddressItem->setText("");

    //label插入表格
    QLabel* linkNameLabel=new QLabel(this);
    QLabel* linkAddressLabel=new QLabel(this);
    linkNameLabel->setText(linkName);
    ui->linkTable->setCellWidget(row-1,0,linkNameLabel);
    linkAddressLabel->setText(QString("<a href=%1>%2</a>")
                          .arg(linkAddress).arg(linkAddress));
    linkAddressLabel->setOpenExternalLinks(true);
    ui->linkTable->setCellWidget(row-1,1,linkAddressLabel);


    //下面再加空行
    ui->linkTable->insertRow(row);

    //插入数据库
    QSqlQuery query;
    query.exec(QString("insert into courseLink(course_id, linkName, linkAddress) values (%1, '%2', '%3');")
               .arg(course_id).arg(linkName).arg(linkAddress));
    QMessageBox::information(this,"success","添加成功");

    //添加到list
    link newlink;
    newlink.course_id = course_id;
    newlink.linkName = linkName;
    newlink.linkAddress = linkAddress;
    qDebug() << newlink.course_id << newlink.linkName << newlink.linkAddress;
    //读link_id出来
    query.exec(QString("select link_id from courseLink where course_id = '%1' and linkName = '%2' and linkAddress = '%3'")
               .arg(course_id).arg(newlink.linkName).arg(newlink.linkAddress));
    query.next();
    newlink.link_id = query.value("link_id").toInt();
    qDebug() << newlink.link_id;

    linkList << newlink;
    //initLinkTable();
}

void Link::delLink()
{
    //定位到行
    int currentRow=ui->linkTable->currentRow();

    //加一个判断行是否删除空的（未选中返回-1）
    if(currentRow<0||currentRow==ui->linkTable->rowCount()-1)
    {
        QMessageBox::warning(this,"error","删除不能为空");
        return;
    }

    //从list中读取link_id
    int delLink_id = linkList[currentRow].link_id;

    //读完后从list中删除掉
    linkList.removeAt(currentRow);

    //从数据库中删除
    QSqlQuery query;
    query.exec(QString("Delete from courseLink where link_id = %1;")
               .arg(delLink_id));

    //删除当前行
    ui->linkTable->removeRow(currentRow);

    QMessageBox::information(this,"success","删除成功");
    //initLinkTable();
}
