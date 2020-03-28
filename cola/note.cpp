#include "note.h"
#include "ui_note.h"

Note::Note(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Note)
{
    ui->setupUi(this);

    //发送返回课程页面信号
    connect(ui->returnButton, &QPushButton::clicked, this, &Note::goback);
}

Note::~Note()
{
    delete ui;
}

void  Note::goback()
{
    emit this->back2Course();
}
