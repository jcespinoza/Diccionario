#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    items = 0;
    dictionary = new Diccionario("diccionario.xml");
    dictionary->loadDictionary();
    loadIntoTable();

    setUI();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete dictionary;
    delete [] items;
}

void MainWindow::setUI(){
    QList<QString> headers;
    headers.push_back("Palabra");
    headers.push_back("Significado");
    headers.push_back("Tags");
    ui->tbWords->setHorizontalHeaderLabels(headers);
}

void MainWindow::loadIntoTable(){
    ui->tbWords->clear();
    delete [] items;
    if(dictionary->getWordCount() <= 0)
        return;
    items = new QTableWidgetItem[dictionary->getWordCount()];
    list<string> words = dictionary->getAllWords();
    list<string>::iterator iter;
    int i;
    for(iter = words.begin(), i = 0; iter != words.end(), i < dictionary->getWordCount(); iter++, i++){
        WordInfo temp = dictionary->getWordInfo(*iter);
        QString pal = QString::fromStdString(*iter);
        QString sig = QString::fromStdString(temp.sMeaning);
        list<string> tags = temp.tags;
        list<string>::iterator itag;
        QString qTags;
        for(itag = tags.begin(); itag != tags.end(); itag++){
            qTags.append(QString::fromStdString(*itag) + ",");
        }
        ui->tbWords->setItem(i, 1, new QTableWidgetItem(pal));
        qDebug() << "i was: " << i;
    }
    QTableWidgetItem* t = new QTableWidgetItem("Hola");
    ui->tbWords->setItem(1, 1, t);
}

void MainWindow::on_pbAddWord_clicked()
{
    if(dictionary->addword(ui->leWord->text().toStdString(), ui->teMeaning->toPlainText().toStdString())){
        ui->leWord->clear();
        ui->teMeaning->clear();
    }
}

void MainWindow::on_pbSave_clicked()
{
    if(!dictionary->saveDictionary()){
        QMessageBox::warning(this, "Error", "Un error ocurrio al intentar guardar el diccionario.",
                             QMessageBox::Ok);
    }
}

void MainWindow::on_pbExit_clicked()
{
    this->close();
}

void MainWindow::on_pbReload_clicked()
{
    loadIntoTable();
}
