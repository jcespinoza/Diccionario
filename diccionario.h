/******************************************************************
Por: Rogger Vásquez , Enero 2011
Clase Diccionario utilizando clases STL (list, map, multimap)
Y guardandose en archivos XML (usando los XMLSTREAM de QT)

- Todavia se tienen que revisar y hacer algunas validaciones (comentadas)
- El proposito es la aplicación de Containers de STL y los mas usados
  Generalmente las listas, maps, multimaps.
******************************************************************/

#ifndef DICCIONARIO_H
#define DICCIONARIO_H
#include <map>
#include <list>
#include <string>
#include <algorithm> // Para el find en la lista
#include <QIODevice>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
using namespace std;
class WordInfo
{
 public:
    string sMeaning;
    list<string> tags;
};

class Diccionario
{
private:
     map<string,WordInfo> lPalabras;
     multimap<string,string> lTagsDiccionario;
     map<char, map<string, WordInfo> > otro;
     string sFileName;
     QXmlStreamWriter xmlW;
     QXmlStreamReader xmlR;
     bool writeFile(QIODevice *device);
     bool readFile(QIODevice *device);
     void readDiccionario();


public:
    Diccionario();
    Diccionario(string filename);
    bool addword(string palabra, string significado);
    bool tagword(string palabra, string tag);
    bool deleteWord(string palabra);
    bool existsWord(string palabra);
    string getMeaning(string palabra);
    WordInfo getWordInfo(string palabra);
    bool setMeaning (string palabra,string signif) ;

    int getWordCount() const;
    list<string> getWordsbyTag(string tag);
    list<string> getAllWords();
    list<string> getAllTags();
    void deleteFromTag(string);


    bool loadDictionary();
    bool saveDictionary();
    bool saveDictionary(string filename);

    ~Diccionario();

};

#endif // DICCIONARIO_H
