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
    void input(QString str);            //����������ַ�
    void addStr(QString str);           //����ʽ����ַ�
    bool isOp(QString str);             //�ж��Ƿ�Ϊ"(","%","/","*","-","+"�����
    bool isNum(QString str);             //�ж��Ƿ�Ϊ����
    bool bracketEqual();                 //�����������Ƿ�ƥ��
    void expressHandle();                //���ʽ����
    double calculate(double a,double b,char op);     //����
    void test();                         //���Ժ���
    std::vector<QString> btnStr;
    std::vector<QPushButton*> btn;
    QString m_expression;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
