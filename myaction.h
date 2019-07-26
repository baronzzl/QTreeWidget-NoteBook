#ifndef MYACTION_H
#define MYACTION_H

#include <QAction>
#include <QDebug>
class myAction : public QAction
{
public:
    myAction(const QString &text, QObject *parent = nullptr): QAction(text,parent){
        qDebug() << text << " contructed!";
    }
    ~myAction(){qDebug() << text() << " action delete";}
};

#endif // MYACTION_H
