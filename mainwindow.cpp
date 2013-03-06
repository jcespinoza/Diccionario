#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dictionary = new Diccionario("diccionario.xml");
    dictionary->loadDictionary();
    loadIntoTable();

    setUI();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete dictionary;
}

void MainWindow::setUI(){
    QList<QString> headers;
    headers.push_back("Palabra");
    headers.push_back("Significado");
    headers.push_back("Tags");
    ui->tbWords->setHorizontalHeaderLabels(headers);
}

void MainWindow::loadIntoTable(){
    list<string> words = dictionary->getAllWords();
    list<string>::iterator iter;
    for(iter = words.begin(); iter != words.end(); iter++){
        qDebug() << QString::fromStdString(dictionary->getMeaning(*iter));
    }
}

void MainWindow::on_pbAddWord_clicked()
{
    dictionary->addword(ui->leWord->text().toStdString(), ui->teMeaning->toPlainText().toStdString());
    loadIntoTable();
}
