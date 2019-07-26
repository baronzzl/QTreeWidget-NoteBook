#include "notetree.h"
#include "time.h"
#include <QMenu>
#include <QActionGroup>
#include <QComboBox>
#include "mycombobox.h"
#include <QInputDialog>
#include <QDebug>
#include <myaction.h>

#define C(X) QString::fromLocal8Bit(X)
NoteTree::NoteTree(QWidget *parent):QTreeWidget(parent)
{
    setHeaderLabels({C("Tile"), C("Create time"),C("Type")});
    setSelectionMode(SelectionMode::ExtendedSelection);
    setUniformRowHeights(true);  //在数据量大时，设置这个可以提高性能，默认是关闭。

//    setDragEnabled(true);
//    setDragDropMode(DragDropMode::DragDrop);
//    setDefaultDropAction(Qt::MoveAction);
//    setDragDropOverwriteMode(false);

    clearAct = new QAction(C("delete all"));
    deleteAct = new QAction(C("delete"));
    addAct = new QAction(C("add note"));
    test = new myAction(QString("test"),this);

    QActionGroup *grp = new QActionGroup(this);
    grp->addAction(clearAct);
    grp->addAction(addAct);
    grp->addAction(deleteAct);
    connect(grp, SIGNAL(triggered(QAction *)), this, SLOT(onActTriggered(QAction *)));
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(onCurrentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));


    //for test
//    auto itm = new QTreeWidgetItem(this, QStringList("test"));
//    for(int i = 0; i < 10; i++){
//        Note *note = new Note("tile");
//        auto nitm = new QTreeWidgetItem(itm, QStringList({note->tile, note->recordTime}));
//        nitm->setData(0, Qt::UserRole, QVariant((long long)note));
//    }

}

NoteTree::~NoteTree()
{
    //重要：对tree中管理的笔记数据进行手动清理
    for(int i = 0; i < topLevelItemCount(); i++){
        auto citms = topLevelItem(i)->takeChildren();
        for(QTreeWidgetItem *itm : citms){
            deleteItem(itm);
        }
    }

    delete test;
    qDebug() << "action had clear";
}

void NoteTree::contextMenuEvent(QContextMenuEvent *event)
{
    QTreeWidget::contextMenuEvent(event);
    //根据当前选中项目的类型选择创建不同菜单，并在鼠标点击位置弹出
    QMenu menu;  //因为这里菜单是模态显示，所以这里直接在栈上申请内存，使用完毕后由栈自动清理内存。
    auto itm = currentItem();
    if(!itm) return;
    if(itm->parent() == NULL){
        menu.addAction(clearAct);
        menu.addAction(addAct);
        menu.addAction(deleteAct);
    }
    else{
        menu.addAction(deleteAct);
    }

    menu.exec(QCursor::pos());
}

void NoteTree::onActTriggered(QAction *act)
{
    auto itm = currentItem();
    if(act == clearAct){
        //clear();  //该方法表示清理树的全部内容
        auto children = itm->takeChildren();  //移除所有子项目，并没有清理项目的内存
        clock_t t1 = clock();

        //方法一
#if 1
        for(auto citm : children){
            deleteItem(citm);
        }
#endif
        //方法二
#if 0
        for(int i = 0; i < children.count(); i++){
            deleteItem(children.at(i));
        }
#endif
        //方法三
#if 0
        QTreeWidgetItemIterator it(currentItem()->child(0));
        while(*it){
            deleteItem((*it));
            it++;
        }
#endif

        /*相关结论：
         * 1）方法一和方法二速度差不多，QList其实不是真正意义上的链表。详见qt的说明
        */
        clock_t t2 = clock();
        qDebug() << t2 -  t1 <<endl;

    }
    else if (act == deleteAct){
        if(itm->parent() == NULL){
            takeTopLevelItem( currentIndex().row());
            for(auto citm : itm->takeChildren()){
                deleteItem(citm);
            }
            delete itm;
        }
        else{
            auto pItm = itm->parent();
            auto itms = selectedItems();
            for(auto itm : itms){
                pItm->removeChild(itm); //移除指定的子项，并没有清理子项的内存
                deleteItem(itm);
            }
        }
    }
    else if(act == addAct){
        QString tile = QInputDialog::getText(this, QString::fromLocal8Bit(""),QString::fromLocal8Bit("Please input Note tile: ")).toLocal8Bit();
        Note *note = new Note(tile);
        auto nitm = new QTreeWidgetItem(itm, QStringList({note->tile, note->recordTime}));
        //将笔记数据的指针设置成nitm的用户数据，这样就可以通过树的item获得实际的笔记数据，从而实现对笔记的管理
        //因64位系统中指针为8字节刚好和long long一致，所以将指针用long long的用户类型保存在对应的item的用户数据中，方法如下：
        nitm->setData(0, Qt::UserRole, QVariant((long long)note));

        myComboBox *cbx = new myComboBox();
        cbx->addItems({C("original"), C("cite")});
        setItemWidget(nitm, 2, cbx);  //tip:设置后，移除该item时qt自动会释放该widget
        connect(cbx, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, [&](int id){ currentNote()->type = id; } );
    }
}

void NoteTree::onCurrentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem *previous)
{
    //设置选择的模式
    if(!(item && item->parent())) {
        setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        return;
    }
    setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);  //该模式ctrl和shift都有效
}

void NoteTree::deleteItem(QTreeWidgetItem *itm)
{
    if(!itm) return;
    Note *note = (Note*)(itm->data(0,Qt::UserRole).toLongLong());
    delete note;
    delete itm;
    qDebug() << "item delete"<<endl;
}

Note *NoteTree::currentNote()
{
    Note *note = NULL;
    auto itm = currentItem();
    if(!(itm && itm->parent())) return note;
    note = (Note *)(itm->data(0,Qt::UserRole).toLongLong());
    qDebug() << "note type" <<note->type<<endl;
    return note;
}
