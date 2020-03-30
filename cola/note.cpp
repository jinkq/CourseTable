#include "note.h"
#include "ui_note.h"
#include<QSqlQuery>
#include<QMessageBox>

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

void Note::goback()
{
    emit this->back2Course();
}

void Note::run(int courseId)
{
    //传递course_id为成员
    this->course_id=courseId;

    //获取对应course_id的courseName
    QSqlQuery query;
    query.exec(QString("select * from courseInfo where course_id = %1;").arg(course_id));
    while(query.next())
    {
        courseName=query.value("courseName").toString();
    }

    //设置标题
    ui->titleLabel->setText(courseName);

    this->show();

    //处理保存笔记的信号
    connect(ui->saveNoteButton,&QPushButton::clicked,this,&Note::saveNote);

    //处理打开笔记的信号
    connect(ui->loadNoteButton,&QPushButton::clicked,this,&Note::loadNote);
}

void Note::saveNote()
{
    //获取当前时间
    QDateTime dateTime(QDateTime::currentDateTime());
    //QString currentTime = dateTime.toString("yyyy-MM-dd hh:mm::ss");
    QString currentTime = dateTime.toString("yyyy-MM-dd");

    //获取笔记标题
    QString noteTitle=ui->noteTitleEdit->text();

    //创建文件夹
    QDir dir;
    if(!dir.exists(QString("../note/%1").arg(courseName)))
    {
        dir.mkdir(QString("../note/%1").arg(courseName));
    }
    dir=QString("../note/%1").arg(courseName);

    //保存文件
    //QString path=QFileDialog::getSaveFileName(this,"保存",
                              //QString("../note/%1.txt").arg(currentTime),"TXT(*.txt)");

    QString path=dir.filePath(QString("%1 %2.txt").arg(currentTime).arg(noteTitle));

    if(path.isEmpty()==false)
    {
        QFile file;//创建文件对象

        //关联文件名字
        file.setFileName(path);

        //打开文件，只写方式
        bool isOk=file.open(QIODevice::WriteOnly);
        if(isOk==true)
        {
            //获取编辑区内容
            QString str=ui->noteEdit->toPlainText();

            //写文件
            //QString->QByteArray
            file.write(str.toUtf8());
        }
        //关闭文件
        file.close();
    }

    //保存成功
    QMessageBox::information(this,"success","保存成功，路径为note/课程名/笔记创建日期 笔记标题.txt");
}

void Note::loadNote()
{
    //获取笔记标题
    QString noteTitle=ui->noteTitleEdit->text();

    //创建文件夹
    QDir dir;
    if(!dir.exists(QString("../note/%1").arg(courseName)))
    {
        dir.mkdir(QString("../note/%1").arg(courseName));
    }
    dir=QString("../note/%1").arg(courseName);

    //打开文件
    QString path=QFileDialog::getOpenFileName(this,"打开",
                              QString("../note/%1").arg(courseName),"TXT(*.txt)");

    //QString path=dir.filePath(QString("%1 %2.txt").arg(currentTime).arg(noteTitle));

    if(path.isEmpty()==false)
    {
        QFile file;//创建文件对象

        //关联文件名字
        file.setFileName(path);

        //打开文件，只写方式
        bool isOk=file.open(QIODevice::ReadWrite);
        if(isOk==true)
        {
            //读文件（一行一行读）
            QByteArray array;
            while(file.atEnd()==false)
            {
                //读一行
                array+=file.readLine();
            }
            ui->noteEdit->setText(array);
        }
        //关闭文件
        file.close();
    }
}
