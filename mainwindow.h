#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "notetree.h"
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
    void on_newNote_triggered();
    void on_save_triggered();
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem *previous);

private:
    Ui::MainWindow *ui;
    Note *curNote;
};

#endif // MAINWINDOW_H
