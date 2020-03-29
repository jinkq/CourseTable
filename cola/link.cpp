#include "link.h"
#include "ui_link.h"
#include <QSqlQuery>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>

Link::Link(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Link)
{
    ui->setupUi(this);

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

void Link::run(int course_id)
{
    this->show();
    ui->title->setText(QString(course_id));
    //设置行数、列数
    int row=1,col=3;
    ui->linkTable->setRowCount(row);
    ui->linkTable->setColumnCount(col);

    //设置表头
    QStringList header;
    //将表头写入表格
    header<<"名称"<<"地址"<<"";
    ui->linkTable->setHorizontalHeaderLabels(header);

    //查询课程编号为course_id的所有link信息
    QSqlQuery query;
    query.exec(QString("select * from courseLink where course_id = %1").arg(course_id));

    int rowNum = 0;
    while(query.next())
    {
        QString name = query.value(2).toString();
        QString address = query.value(3).toString();
        ui->linkTable->insertRow(rowNum);

        QLabel * label = new QLabel();
        label->setText(QString("<a href=%1>%2</a>").arg(address).arg(name));
        label->setOpenExternalLinks(true);
        ui->linkTable->setCellWidget(rowNum,0,label);

        ui->linkTable->setItem(rowNum,1,new QTableWidgetItem(address));
        QPushButton *test = new QPushButton();
        test->setText("进入链接");
        ui->linkTable->setCellWidget(rowNum,2,test);
    }
}

void Link::on_linkTable_cellClicked(int row, int column)
{
    qDebug() << "haha";
    if(column == 2)
    {
        QString address = ui->linkTable->item(row,1)->text();
        qDebug() << address;
        QUrl url(QString("www.baidu.com"));
        bool is_open = QDesktopServices::openUrl(url);
    }
}

void Link::on_linkTable_cellPressed(int row, int column)
{
    qDebug() << row << column;
}
