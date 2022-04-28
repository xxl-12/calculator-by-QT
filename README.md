## 表达式输入限制

- 避免001、00.123等无效数字输入

```
if(m_expression.length() == 1 && m_expression[0] == '0'){}  //当表达式只有1个元素且为0时，不能再追加

        else if(m_expression.length() > 1 && m_expression[m_expression.length()-1] == '0')
        {
            if(isOp((QString)m_expression[m_expression.length()-2])){}   //当表达式元素>1且最后一个元素为0，倒数第二个元素为运算符时，不再追加

            else
                m_expression.append(str);
        }
```

-	避免 ）123、）001等无效表达式的输入

```
 else if(m_expression.length() > 1 &&m_expression[m_expression.length()-1] == ')'){} //当表达式最后一个元素为）时，不能再追加
```

- 避免如（1+2这样的括号不匹配和2+=这样的无效表达式的输入

```
 else if(bracketEqual()&&!isOp((QString)m_expression[m_expression.length()-1]))
        {
            qDebug()<<m_expression<<"=";
            expressHandle();
            qDebug()<<m_expression;
            ui->inputEdit->setText(m_expression);
            m_expression = "";
        }
```

- 避免（1+2）2这样的无效表达式的输入

```
else if(length > 1 && lastStr == ')'){} //当表达式最后一个元素为）时，不能再追加
```

- 避免1+2（这样的无效表达式的输入

```
 else if(isOp((lastStr)))  //如果最后一个元素是运算符，追加“（”
```

- 避免+)这样的无效表达式的输入

```
if(isNum(lastStr)||lastStr == ")") //如果最后一个元素是数字，追加“）”
            {
                if(!bracketEqual())
                    m_expression.append(")");
            }
```

- 避免2..0或0..4这样的非法小数的输入

```
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
```

## 防止非法运算

- 防止除数为0

```
		if(b != 0)          //防止除数为0
            return a / b;
        else
            return 0;
```

- 防止模操作为小数或模数为0

```
		if(b > 1)       //防止模数为小数或0
            return (int)a % (int)b;
        else
            return 0;
```

## 健壮性测试函数test()

随机生成字符，当随机到“=”时，在控制台输出表达式的结果

在第25行调用test()

```
void MainWindow::test()
{
    int i = 0;
    while (i < 1000000)
    {
        //随机输入字符
        int temp = rand()%btn.size();
        input(btnStr[temp]);
        i++;
    }
}
```

测试时可以将第52行弹出提示框代码注释掉

```
QMessageBox::question(this,"warning","expression error!",QMessageBox::Yes);
```

## 部分按钮说明

- CE：清空输入框
- Del：删除最后一个元素
- %：求模运算
- ()：加"("或")"
