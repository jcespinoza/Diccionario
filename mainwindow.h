#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "diccionario.h"
#include <QTableWidgetItem>
#include <QList>
#include <QPushButton>

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
    void stdTagClicked();

private:
    Ui::MainWindow *ui;
    Diccionario *dictionary;
    void tagWord();
    bool layoutSet;

    void setUI();
    void loadIntoTable();
    void loadTags();
    void loadWithFilter(QString);
    QPushButton *createTagElement(QString, QString, int, bool append=true);
};

#endif // MAINWINDOW_H
