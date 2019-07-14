#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QAbstractItemView>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    curNote(NULL)           //tip:养成好习惯，将指针初始化成NULL，有诸多好处
{
    ui->setupUi(this);
    ui->mainToolBar->addAction(ui->newNote);
    ui->mainToolBar->addAction(ui->save);
    //ui->treeWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newNote_triggered()
{
    QString name = QInputDialog::getText(this,"", QString::fromLocal8Bit("note tile:")).toLocal8Bit();
    auto itm = new QTreeWidgetItem(ui->treeWidget, QStringList(name));
}

void MainWindow::on_save_triggered()
{
    if(!curNote) return;
    curNote->content = ui->textEdit->toPlainText();
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem *previous)
{
    if(!(item && item->parent())) {
        //ui->treeWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        ui->textEdit->setText("");
        return;
    }
    //ui->treeWidget->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    curNote = (Note *)(item->data(0,Qt::UserRole).toLongLong());  //获得该选中item所有管理的笔记数据的指针
    ui->textEdit->setText(curNote->content);
}
