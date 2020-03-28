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
        this->course_ddl.show();
    });

    //处理从ddl回来的
    connect(&course_ddl, &DDL::back2Course,
            [=]
            {
                this->course_ddl.hide();
                this->show();
            }
    );

    //转到link界面
    connect(ui->LinkButton, &QPushButton::clicked,
            [=]
    {
        this->hide();
        this->course_link.show();
    });

    //处理从link回来的
    connect(&course_link, &Link::back2Course,
            [=]
            {
                this->course_link.hide();
                this->show();
            }
    );

    //转到note界面
    connect(ui->NoteButton, &QPushButton::clicked,
            [=]
    {
        this->hide();
        this->course_note.show();
    });

    //处理从note回来的
    connect(&course_note, &Note::back2Course,
            [=]
            {
                this->course_note.hide();
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


