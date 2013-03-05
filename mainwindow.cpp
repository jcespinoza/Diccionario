#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dictionary = new Diccionario();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete dictionary;
}
