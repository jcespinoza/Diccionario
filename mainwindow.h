#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "diccionario.h"

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

private:
    Ui::MainWindow *ui;
    Diccionario *dictionary;

    void setUI();
    void loadIntoTable();
};

#endif // MAINWINDOW_H
