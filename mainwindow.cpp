#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    items = QList<QTableWidgetItem*>();
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
    ui->tbWords->setEditTriggers(0);
}

void MainWindow::loadIntoTable(){
    ui->tbWords->clearContents();
    ui->tbWords->setRowCount(dictionary->getWordCount());
    ui->tbWords->horizontalHeader()->resizeSection(1,200);
    items.clear();
    if(dictionary->getWordCount() <= 0)
        return;
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
        for(itag = tags.begin(); itag != tags.end(); itag++)
            qTags.append(QString::fromStdString(*itag) + ",");
        ui->tbWords->setItem(i, 0, new QTableWidgetItem(pal));
        ui->tbWords->setItem(i, 1, new QTableWidgetItem(sig));
        ui->tbWords->setItem(i, 2, new QTableWidgetItem(qTags));
    }
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
    }else{
        QMessageBox::information(this, "Succes!", "El diccionario se guardo exitosamente.", QMessageBox::Ok);
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

void MainWindow::on_pbTagWord_clicked()
{
    tagWord();
    list<string>::iterator it;
    list<string> tags = dictionary->getAllTags();
    qDebug() << "TAGS:";
    for(it = tags.begin(); it != tags.end(); it++){
        qDebug() << "->" << QString::fromStdString(*it);
    }
}

void MainWindow::tagWord(){
    int index = ui->tbWords->currentRow();
    if(index != -1){
        QString tags = ui->leTags->text().trimmed();
        string pal = ui->tbWords->item(index, 0)->text().toStdString();
        dictionary->tagword(pal, tags.toStdString());
        ui->leTags->clear();
    }
}

void MainWindow::on_leTags_returnPressed()
{
    tagWord();
}

void MainWindow::on_leWord_returnPressed()
{
    on_pbAddWord_clicked();
}

void MainWindow::on_pbDelete_clicked()
{
    int index = ui->tbWords->currentRow();
    if(index != -1){
        string word = ui->tbWords->item(index, 0)->text().toStdString();
        dictionary->deleteWord(word);
        loadIntoTable();
    }
}
