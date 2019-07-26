#ifndef NOTETREE_H
#define NOTETREE_H
#include <QTreeWidget>
#include <QUuid>
#include <QDateTime>
//笔记数据的结构体
struct Note{
    Note(QString tile){
        id = QUuid::createUuid().toString();
        this->tile = tile;
        QDateTime time =QDateTime::currentDateTime();
        recordTime =time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    }
    ~Note(){}
    int type;           //笔记的类型
    QString id;         //笔记的唯一id
    QString tile;       //笔记的内容
    QString recordTime; //记录时间

    QString content;    //笔记的具体内容
};

class NoteTree : public QTreeWidget
{
    Q_OBJECT
public:
    NoteTree(QWidget *parent = 0);
    ~NoteTree();
    QAction *test;

protected:
    //鼠标右键点击时会调用此函数，调用过程已经在基类中完成，子类中只要实现子定义的操作进行，这里实现弹出菜单的操作
    void contextMenuEvent(QContextMenuEvent*event);
protected slots:
    void onActTriggered(QAction *); 
    void onCurrentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem *previous);

private:
    QAction *addAct;    //添加项目
    QAction *deleteAct; //删除项目
    QAction *clearAct;  //清空项目

    void deleteItem(QTreeWidgetItem *);  //清理item及其管理的笔记本的内存
    Note * currentNote();   //当前选中笔记
};

#endif // NOTETREE_H
