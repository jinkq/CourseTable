#include "ddl.h"
#include "ui_ddl.h"

DDL::DDL(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DDL)
{
    ui->setupUi(this);

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
