#include "course.h"
#include "ui_course.h"

Course::Course(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::course)
{
    ui->setupUi(this);

    //转到ddl界面
    connect(ui->ddlButton, &QPushButton::clicked,
            [=]
    {
        this->hide();
        this->courseDdl.show();
    });

    //处理从ddl回来的
    connect(&courseDdl, &DDL::back2Course,
            [=]
            {
                this->courseDdl.hide();
                this->show();
            }
    );

    //转到link界面
    connect(ui->LinkButton, &QPushButton::clicked,
            [=]
    {
        this->hide();
        this->courseLink.show();
    });

    //处理从link回来的
    connect(&courseLink, &Link::back2Course,
            [=]
            {
                this->courseLink.hide();
                this->show();
            }
    );

    //转到note界面
    connect(ui->NoteButton, &QPushButton::clicked,
            [=]
    {
        this->hide();
        this->courseNote.show();
    });

    //处理从note回来的
    connect(&courseNote, &Note::back2Course,
            [=]
            {
                this->courseNote.hide();
                this->show();
            }
    );

    //回到课程表页面
    connect(ui->returnButton, &QPushButton::clicked,
            [=]
    {
        sendSlot();
    });

}

Course::~Course()
{
    delete ui;
}

//发送回去主页面的信号
void Course::sendSlot()
{
    emit back2Main();
}


