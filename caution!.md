# 使用Qt的注意事项

1. sql语句

运用query的时候要检查Qstring中的%有没有被适当地用括号引起来

2. tablewidget中获取某行某列的控件

不能直接用item(int row, int column)，这样拿不到里面的控件，要用

```c++
QWidget * QTableWidget::cellWidget ( int row, int column ) const
```

然后再进行动态类型转换

```c++
dynamic_cast<QLineEdit*>(ui->ddlTable->cellWidget(row,col))->text();
```

强制类型转换之前要注意好转换成什么类型