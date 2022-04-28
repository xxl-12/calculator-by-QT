#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

signals:


private:
    void input(QString str);            //接收输入的字符
    void addStr(QString str);           //向表达式添加字符
    bool isOp(QString str);             //判断是否为"(","%","/","*","-","+"运算符
    bool isNum(QString str);             //判断是否为数字
    bool bracketEqual();                 //左右括号数是否匹配
    void expressHandle();                //表达式处理
    double calculate(double a,double b,char op);     //计算
    void test();                         //测试函数
    std::vector<QString> btnStr;
    std::vector<QPushButton*> btn;
    QString m_expression;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
