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
    //程序健壮性测试
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
        lastStr = (QString)m_expression[length-1];   //表达式最后一个元素
    if(length>1)
        nextLastStr =(QString)m_expression[length-2];    //表达式倒数第二个元素
    if(isNum(str))
    {
        if(length == 1 && m_expression[0] == '0'){}  //当表达式只有1个元素且为0时，不能再追加

        else if(length > 1 && lastStr == '0')
        {
            if(isOp(nextLastStr)){}   //当表达式元素>1且最后一个元素为0，倒数第二个元素为运算符时，不再追加

            else
                m_expression.append(str);
        }
        else if(length > 1 && lastStr == ')'){} //当表达式最后一个元素为）时，不能再追加
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
        else if(isOp((lastStr)))  //如果最后一个元素是运算符，追加“（”
        {
            m_expression.append("(");
        }
        else
        {
            if(isNum(lastStr)||lastStr == ")") //如果最后一个元素是数字，追加“）”
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
            for(;i >= 0;i--)    //寻找小数点，看最后一个数字是不是已经是小数
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
        if(isNum(lastStr) || lastStr == ')') //如果最后一个元素是数字或‘）’，追加运算符
        {
            m_expression.append(str);
        }
    }

}

bool MainWindow::isOp(QString str)
{
    size_t i = 12;
    for(; i < 18; i++) //btnStr12~17的其中一个运算符
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
    std::stack<double> num;  //数字栈
    std::stack<char> op;      //符号栈

    //定义符号位的优先级
    std::map<char,int> priority;
    priority['+'] = 1;
    priority['-'] = 1;
    priority['*'] = 2;
    priority['/'] = 2;
    priority['%'] = 2;
    priority['('] = 0;

    //定义表达式长度
    int length = m_expression.length();

    for(int i = 0; i < length; )
    {
        if(isOp((QString)m_expression[i]) || m_expression[i] == ')')    //如果是运算符
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
                //栈中的优先级大于当前符号
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
    while (op.size())   //将优先级相等的进行运算
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
        if(b != 0)          //防止除数为0
            return a / b;
        else
            return 0;
        break;
    case '%':
        if(b > 1)       //防止模数为小数或0
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
        //随机输入字符
        int temp = rand()%btn.size();
        input(btnStr[temp]);
        i++;
    }
}


