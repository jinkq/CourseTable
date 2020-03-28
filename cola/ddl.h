#ifndef DDL_H
#define DDL_H

#include <QWidget>

namespace Ui {
class DDL;
}

class DDL : public QWidget
{
    Q_OBJECT

public:
    explicit DDL(QWidget *parent = nullptr);
    ~DDL();

    void goback();

signals:
    void back2Course();

private:
    Ui::DDL *ui;
};

#endif // DDL_H
