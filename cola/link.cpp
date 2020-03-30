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

    //处理添加链接的信号
    connect(ui->addLinkButton,&QPushButton::clicked,this,&Link::addLink);

    //处理删除链接的信号
    connect(ui->delLinkButton,&QPushButton::clicked,this,&Link::delLink);

}

void Link::initLinkTable()
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

    //一行显示完整链接
    ui->linkTable->resizeColumnsToContents();

    this->show();
    //ui->title->setText(QString(course_id));
    //设置行数、列数
    int row=1,col=2;
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
        linkNameList << query.value(2).toString();//链接名
        linkAddressList<< query.value(3).toString();//链接地址
        ui->linkTable->insertRow(rowNum);

        //插入链接名
        QLabel * nameLabel = new QLabel();
        nameLabel->setText(linkNameList[rowNum]);
        ui->linkTable->setCellWidget(rowNum,0,nameLabel);

        //插入链接地址
        QLabel * addressLabel = new QLabel();
        addressLabel->setText(QString("<a href=%1>%2</a>")
                              .arg(linkAddressList[rowNum]).arg(linkAddressList[rowNum]));
        addressLabel->setOpenExternalLinks(true);
        ui->linkTable->setCellWidget(rowNum,1,addressLabel);

        rowNum++;

    }
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

    //下面再加空行
    ui->linkTable->insertRow(row);

    QSqlQuery query;
    query.exec(QString("insert into courseLink(course_id, linkName, linkAddress) values (%1, '%2', '%3');")
               .arg(course_id).arg(linkName).arg(linkAddress));

    QMessageBox::information(this,"success","添加成功");

    //添加到list
    linkNameList << linkName;
    linkAddressList<< linkAddress;
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

    //从list中读取到相应的信息，方便从数据库删除
    QString linkName = linkNameList[currentRow];
    QString linkAddress = linkAddressList[currentRow];

    //读完后从list中删除掉
    linkName.remove(currentRow);
    linkAddress.remove(currentRow);

    //从数据库中删除
    QSqlQuery query;
    query.exec(QString("Delete from courseLink where linkName = '%1' and linkAddress = '%2';")
               .arg(linkName).arg(linkAddress));

    //删除当前行
    ui->linkTable->removeRow(currentRow);
}
