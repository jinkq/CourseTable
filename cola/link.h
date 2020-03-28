#ifndef LINK_H
#define LINK_H

#include <QWidget>

namespace Ui {
class Link;
}

class Link : public QWidget
{
    Q_OBJECT

public:
    explicit Link(QWidget *parent = nullptr);
    ~Link();

    void goback();

signals:
    void back2Course();

private:
    Ui::Link *ui;
};

#endif // LINK_H
