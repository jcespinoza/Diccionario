#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dictionary = new Diccionario("diccionario.xml");
    dictionary->loadDictionary();
    loadIntoTable();
    layoutSet = false;
    loadTags();

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
    loadWithFilter("");
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
        loadTags();
        loadWithFilter("");
    }
}

void MainWindow::loadTags(){
    map<string,int> tags = dictionary->getTagsWithCount();
    map<string,int>::iterator mit;
    QGridLayout* gridTags = new QGridLayout();
    if(!layoutSet)
        ui->graphTags->setLayout(gridTags);
    for(mit = tags.begin(); mit != tags.end(); mit++){
        /*QLabel* lt = new QLabel(QString::fromStdString((*mit).first).append("(").append(QString::number((*mit).second)).append(")"), this);*/
        QPushButton* lt = createTagElement(QString::fromStdString((*mit).first),QString::fromStdString((*mit).first), (*mit).second);
        gridTags->addWidget(lt);
    }
    QPushButton* all = createTagElement("All", "", 1, false);
    gridTags->addWidget(all);
    layoutSet = true;
}

void MainWindow::loadWithFilter(QString filter){
    ui->tbWords->clearContents();
    ui->tbWords->horizontalHeader()->resizeSection(1,200);
    if(dictionary->getWordCount() <= 0)
        return;
    list<string> words;
    if(filter.isEmpty())
         words = dictionary->getAllWords();
    else
        words = dictionary->getWordsbyTag(filter.toStdString());
    ui->tbWords->setRowCount(words.size());
    list<string>::iterator iter;
    int i;
    for(iter = words.begin(), i = 0; iter != words.end(), i < words.size(); iter++, i++){
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

void MainWindow::stdTagClicked(){
    QString filter = QObject::sender()->property("tag").toString();
    loadWithFilter(filter);
}

QPushButton* MainWindow::createTagElement(QString text, QString tag, int count, bool appendCount){
    QPushButton* lt = new QPushButton(text, this);
    if(appendCount)
        lt->setText(text.append("(").append(QString::number(count)).append(")"));
    lt->setStyleSheet("background-color:transparent");
    lt->setStyleSheet(":hover{background-color: red; font-size: 200%}:pressed{border-radius:5px}:default{background-color:blue}");
    lt->setProperty("tag", tag);
    lt->setProperty("words", count);
    QFont f = lt->font();
    f.setPixelSize(f.pixelSize() + 14 + 2*count);
    lt->setFont(f);
    connect(lt, SIGNAL(clicked()), this, SLOT(stdTagClicked()));
    return lt;
}
