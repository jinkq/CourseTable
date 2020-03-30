#ifndef NOTE_H
#define NOTE_H

#include <QWidget>
#include <QSqlDatabase>
#include<QFile>//文件
#include<QFileDialog>//文件对话框
#include<QFileInfo>//获取文件信息
#include<QDebug>
#include<QDateTime>//时间
#include<QList>

namespace Ui {
class Note;
}

class Note : public QWidget
{
    Q_OBJECT

public:
    explicit Note(QWidget *parent = nullptr);
    ~Note();

    void goback();

    //link信息
    int course_id;
    QString courseName;
    QList<QString> linkNameList;
    QList<QString> linkAddressList;

    //数据库
    QSqlDatabase note_db;

    //加载笔记
    void loadNote();

    //显示对应course的note
    void run(int courseId);

    //保存新笔记
    void saveNote();

    //删除笔记
    void delNote();

signals:
    void back2Course();

private:
    Ui::Note *ui;
};

#endif // NOTE_H
