#include "link.h"
#include "ui_link.h"

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
