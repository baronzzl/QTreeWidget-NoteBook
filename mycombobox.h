//测试控件是否会自动释放
#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H
#include <QComboBox>
#include <QDebug>

class myComboBox : public QComboBox
{
public:
    myComboBox(QWidget *parent = 0) : QComboBox(parent){
        //qDebug() << "combox construct"<<endl;
    }
    ~myComboBox() {
            //qDebug() << "combox disconstruct"<<endl;
        }
};

#endif // MYCOMBOBOX_H
