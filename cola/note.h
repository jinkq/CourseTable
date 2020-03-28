#ifndef NOTE_H
#define NOTE_H

#include <QWidget>

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

signals:
    void back2Course();

private:
    Ui::Note *ui;
};

#endif // NOTE_H
