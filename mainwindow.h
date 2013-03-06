#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "diccionario.h"
#include <QTableWidgetItem>

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
    void on_pbAddWord_clicked();

    void on_pbSave_clicked();

    void on_pbExit_clicked();

    void on_pbReload_clicked();

private:
    Ui::MainWindow *ui;
    Diccionario *dictionary;
    QTableWidgetItem* items;

    void setUI();
    void loadIntoTable();
};

#endif // MAINWINDOW_H
