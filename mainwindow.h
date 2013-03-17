#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "diccionario.h"
#include <QTableWidgetItem>
#include <QList>

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
    void on_pbTagWord_clicked();

    void on_leTags_returnPressed();
    void on_leWord_returnPressed();

    void on_pbDelete_clicked();

private:
    Ui::MainWindow *ui;
    Diccionario *dictionary;
    QList<QTableWidgetItem*> items;
    void tagWord();

    void setUI();
    void loadIntoTable();
    void loadTags();
};

#endif // MAINWINDOW_H
