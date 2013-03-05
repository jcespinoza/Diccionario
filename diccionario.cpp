#include "diccionario.h"

//METODOS PRIVATE **************************

bool Diccionario::writeFile(QIODevice *device)
{
    xmlW.setDevice(device);
    xmlW.writeStartDocument();
    xmlW.writeDTD("<!DOCTYPE Diccionario>");
    xmlW.writeStartElement("Diccionario");
    xmlW.writeAttribute("version", "1.0");

    map<string,WordInfo>::iterator it;
    string pal ;
    WordInfo tempWinfo;
    for(it=this->lPalabras.begin();it!=lPalabras.end();it++)
    {
      pal = (*it).first;
      tempWinfo = (*it).second;
      xmlW.writeStartElement("palabra");  // Escribe <palabra>
      xmlW.writeAttribute("valor",pal.c_str());
      xmlW.writeTextElement("significado",
                            tempWinfo.sMeaning.c_str());
      xmlW.writeStartElement("tags"); //<tags>
      QString temp;
      temp.setNum(tempWinfo.tags.size());
      xmlW.writeAttribute("cuantos",temp );
      list<string>::iterator i;
      for (i=tempWinfo.tags.begin(); i!=tempWinfo.tags.end();i++)
      {
          xmlW.writeTextElement("tag",(*i).c_str());
      }

      xmlW.writeEndElement();  // </tags>
      xmlW.writeEndElement(); // </palabra>
    }

    xmlW.writeEndDocument();
    return true;

}
bool Diccionario::readFile(QIODevice *device)
{
   xmlR.setDevice(device);

   if (xmlR.readNextStartElement())
   {
      if (xmlR.name() == "Diccionario" && xmlR.attributes().value("version") == "1.0")
       {
           readDiccionario();
           return true;
       }

   }

   return false;


}
void Diccionario::readDiccionario()
{
    string palabra,significado;


    while (xmlR.readNextStartElement())
    {
        if (xmlR.name() == "palabra")
        {
            palabra = xmlR.attributes().value("valor").toString().toStdString();
            while (xmlR.readNextStartElement())
            {
                if (xmlR.name() == "significado")
                {
                     significado = xmlR.readElementText().toStdString();
                     this->addword(palabra,significado);

                }
                else if (xmlR.name() == "tags")
                {
                     string tag;
                     while(xmlR.readNextStartElement())
                     {
                         if(xmlR.name()=="tag")
                         {
                             tag = xmlR.readElementText().toStdString();
                             this->tagword(palabra,tag);

                         }
                         else
                             xmlR.skipCurrentElement();
                     }

                }
                else
                    xmlR.skipCurrentElement();

            } // Fin del While
        }
        else
            xmlR.skipCurrentElement();
    }

  }

// FIN METODOS PRIVATE **********************

// METODOS PUBLIC ******************************
Diccionario::Diccionario()
{
    this->sFileName="diccionario.xml";
     xmlW.setAutoFormatting(true);
}
Diccionario::Diccionario(string filename)
{
    this->sFileName = filename;
    xmlW.setAutoFormatting(true);

}
bool Diccionario::addword(string palabra, string significado)
{
   if(!this->existsWord(palabra))
    {
       WordInfo temp;
       temp.sMeaning = significado;
       this->lPalabras[palabra]=temp;
       return true;
    }
   else
       return false;

}
bool Diccionario::tagword(string palabra, string tag)
{
    list<string>::iterator it;
    WordInfo wInfo;
   if(this->existsWord(palabra))
    {
      it = find(this->lPalabras[palabra].tags.begin(),
                this->lPalabras[palabra].tags.end(),
                tag);

      if(it==this->lPalabras[palabra].tags.end())
      // Sino esta tageada con este tag
      {
          this->lPalabras[palabra].tags.push_back(tag);
          this->lTagsDiccionario.insert(pair<string,string>(tag,palabra));

      }
      return true;

    }
   else
      return false;
}
bool Diccionario::deleteWord(string palabra)
{

   // Se puede usar el Find o se puede indexar directamente
     map<string,WordInfo>::iterator it;

     it = this->lPalabras.find(palabra);
     if (it==this->lPalabras.end())
         return false;
     else
     {
         this->lPalabras.erase(it);
         return true;
     }
}
bool Diccionario::existsWord(string palabra)
{
   // Se puede usar el Find o se puede indexar directamente
    map<string,WordInfo>::iterator it;

    it = this->lPalabras.find(palabra);
    if (it==this->lPalabras.end())
        return false;
    else
        return true;


}
string Diccionario::getMeaning(string palabra)
{
   if(this->existsWord(palabra))
    {
         return this->lPalabras[palabra].sMeaning;
    }

    else
        return "";

}
WordInfo Diccionario::getWordInfo(string palabra)
{
    WordInfo temp;
    temp.sMeaning="";
    if(this->existsWord(palabra))
     {
          temp= this->lPalabras[palabra];
     }

    return temp;
}
bool Diccionario::setMeaning(string palabra,string signif)
{
    if(this->existsWord(palabra))
     {
          this->lPalabras[palabra].sMeaning = signif ;
          return true;
     }

     else
         return false;
}

int Diccionario::getWordCount() const
{
    return this->lPalabras.size();
}
list<string> Diccionario::getWordsbyTag(string tag)
{
    list<string> temp;
    pair<multimap<string,string>::iterator,multimap<string,string>::iterator > itrange;
    itrange = this->lTagsDiccionario.equal_range(tag);
    for (multimap<string, string>::iterator it1 = itrange.first;
         it1 != itrange.second
         ;++it1)
    {
        // Revisar si esa palabra en realidad sigue existiendo
        if (this->existsWord((*it1).second))
             temp.push_back((*it1).second);
        else
            continue; // Aca deberia haber codigo para borrar esa entrada del multimap

    }

    return temp;

}
list<string> Diccionario::getAllWords()
{
    list<string> lista_temporal;
    map<string,WordInfo>::iterator it;
    for(it=this->lPalabras.begin();it!=lPalabras.end(); it++)
    {
        lista_temporal.push_back((*it).first );

    }
    return lista_temporal;
}
list<string> Diccionario::getAllTags()
{
    list<string> lista_temporal;
    //TODO - Logica para recorrer el multimap y solo insertar un TAG
    // a la vez en la lista sin repetirlos y sin estar usando FIND

    return lista_temporal;
}

bool Diccionario::saveDictionary()
{
   return this->saveDictionary(this->sFileName);
}
bool Diccionario::saveDictionary(string filename)
{
  QFile file(filename.c_str());
   if (!file.open(QFile::WriteOnly | QFile::Text))
       return false;
   else
       return this->writeFile(&file);
 }
bool Diccionario::loadDictionary()
{

    QFile file(this->sFileName.c_str());
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return false;
    else
        return this->readFile(&file);

}

Diccionario::~Diccionario()
{
    this->lPalabras.clear();
    this->lTagsDiccionario.clear();
}



