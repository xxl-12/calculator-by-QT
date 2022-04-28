#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stack"
#include "map"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    btnStr = {"0","1","2","3","4","5","6","7","8","9","CE","Del","(","%","/","*","-","+","=","."};
    btn = {ui->btnZero,ui->btnOne,ui->btnTwo,ui->btnThree,ui->btnFour,ui->btnFive,ui->btnSix,ui->btnSeven,
          ui->btnEight,ui->btnNine,ui->btnClear,ui->btnDelete,ui->btnBrackets,ui->btnMod,ui->btnDivide,
          ui->btnMultiply,ui->btnSubtract,ui->btnPlus,ui->btnEqual,ui->btnPoint
          };
    for(size_t i = 0;i<btn.size();i++)
    {
        connect(btn[i],&QPushButton::clicked,this,[=](){
            this->input(btnStr[i]);
        });
    }
    //����׳�Բ���
    //test();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::input(QString str)
{
    if(str == "=")
    {
        if(m_expression == "")
        {
            ui->inputEdit->setText(m_expression);
        }
        else if(bracketEqual()&&!isOp((QString)m_expression[m_expression.length()-1]))
        {
            qDebug()<<m_expression<<"=";
            expressHandle();
            qDebug()<<m_expression;
            ui->inputEdit->setText(m_expression);
            m_expression = "";
        }
        else
        {
            qDebug()<<m_expression<<"= error!";
            QMessageBox::question(this,"warning","expression error!",QMessageBox::Yes);
        }
    }
    else if(str == "Del")
    {
        m_expression.remove(m_expression.length()-1,1);
        ui->inputEdit->setText(m_expression);
    }
    else if (str == "CE")
    {
        m_expression.clear();
        ui->inputEdit->setText(m_expression);
    }
    else
    {
        addStr(str);
        ui->inputEdit->setText(m_expression);
    }
}

void MainWindow::addStr(QString str)
{
    size_t length = m_expression.length();
    QString lastStr = "";
    QString nextLastStr = "";
    if(length>0)
        lastStr = (QString)m_expression[length-1];   //���ʽ���һ��Ԫ��
    if(length>1)
        nextLastStr =(QString)m_expression[length-2];    //���ʽ�����ڶ���Ԫ��
    if(isNum(str))
    {
        if(length == 1 && m_expression[0] == '0'){}  //�����ʽֻ��1��Ԫ����Ϊ0ʱ��������׷��

        else if(length > 1 && lastStr == '0')
        {
            if(isOp(nextLastStr)){}   //�����ʽԪ��>1�����һ��Ԫ��Ϊ0�������ڶ���Ԫ��Ϊ�����ʱ������׷��

            else
                m_expression.append(str);
        }
        else if(length > 1 && lastStr == ')'){} //�����ʽ���һ��Ԫ��Ϊ��ʱ��������׷��
        else
        {
            m_expression.append(str);
        }
    }
    else if(str == "(")
    {
        if(length == 0)
        {
            m_expression.append("(");
        }
        else if(isOp((lastStr)))  //������һ��Ԫ�����������׷�ӡ�����
        {
            m_expression.append("(");
        }
        else
        {
            if(isNum(lastStr)||lastStr == ")") //������һ��Ԫ�������֣�׷�ӡ�����
            {
                if(!bracketEqual())
                    m_expression.append(")");
            }
        }
    }
    else if(str == ".")
    {
        if(length > 0 && isNum(lastStr))
        {
            int i = length - 2;
            for(;i >= 0;i--)    //Ѱ��С���㣬�����һ�������ǲ����Ѿ���С��
            {
                if(isOp((QString)m_expression[i]))
                    break;
                if(m_expression[i] == '.')
                    break;
            }
            if(m_expression[i<0?0:i] != '.')
                m_expression.append(str);
        }
    }
    else
    {
        if(isNum(lastStr) || lastStr == ')') //������һ��Ԫ�������ֻ򡮣�����׷�������
        {
            m_expression.append(str);
        }
    }

}

bool MainWindow::isOp(QString str)
{
    size_t i = 12;
    for(; i < 18; i++) //btnStr12~17������һ�������
    {
        if(str == btnStr[i])
            return true;
    }
    return false;
}

bool MainWindow::isNum(QString str)
{
    if(str >= "0" && str <= "9")
        return true;
    else
        return false;
}

bool MainWindow::bracketEqual()
{
    if(m_expression.count("(") == m_expression.count(")"))
        return true;
    else
        return false;
}

void MainWindow::expressHandle()
{
    std::stack<double> num;  //����ջ
    std::stack<char> op;      //����ջ

    //�������λ�����ȼ�
    std::map<char,int> priority;
    priority['+'] = 1;
    priority['-'] = 1;
    priority['*'] = 2;
    priority['/'] = 2;
    priority['%'] = 2;
    priority['('] = 0;

    //������ʽ����
    int length = m_expression.length();

    for(int i = 0; i < length; )
    {
        if(isOp((QString)m_expression[i]) || m_expression[i] == ')')    //����������
        {
            if(m_expression[i] == '(')
            {
                op.push('(');
            }
            else if(m_expression[i] == ')')
            {
                while (op.top() != '(')
                {
                    double a = num.top();
                    num.pop();
                    double b = num.top();
                    num.pop();
                    char c = op.top();
                    op.pop();
                    num.push(calculate(b,a,c));
                }
                op.pop();
            }
            else
            {
                //ջ�е����ȼ����ڵ�ǰ����
                while (op.size()>0 && priority[op.top()] >= priority[m_expression[i].toLatin1()])
                {
                    double a = num.top();
                    num.pop();
                    double b = num.top();
                    num.pop();
                    char c = op.top();
                    op.pop();
                    num.push(calculate(b,a,c));
                }
                op.push(m_expression[i].toLatin1());
            }

            i++;
        }
        if(isNum((QString)m_expression[i]))
        {
            QString temp = "";
            while (isNum((QString)m_expression[i]) || m_expression[i] == '.')
            {
                temp.append(m_expression[i]);
                i++;
            }
            num.push(temp.toDouble());

        }
    }
    while (op.size())   //�����ȼ���ȵĽ�������
    {
        double a = num.top();
        num.pop();
        double b = num.top();
        num.pop();
        char c = op.top();
        op.pop();
        num.push(calculate(b,a,c));
    }
    m_expression = m_expression.number(num.top());

}

double MainWindow::calculate(double a,double b,char op)
{
    switch (op)
    {
    case '+':
        return a + b;
        break;
    case '-':
        return a - b;
        break;
    case '*':
        return a * b;
        break;
    case '/':
        if(b != 0)          //��ֹ����Ϊ0
            return a / b;
        else
            return 0;
        break;
    case '%':
        if(b > 1)       //��ֹģ��ΪС����0
            return (int)a % (int)b;
        else
            return 0;
        break;
    default:
        return 0.0;
        break;
    }
}

void MainWindow::test()
{
    int i = 0;
    while (i < 100000)
    {
        //��������ַ�
        int temp = rand()%btn.size();
        input(btnStr[temp]);
        i++;
    }
}


