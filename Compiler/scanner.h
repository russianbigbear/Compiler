#ifndef SCANNER_H
#define SCANNER_H

#include <qfile.h>
#include <QString>
#include <QList>
#include <lexem.h>
#include <QMultiMap>
#include <iostream>
#include <QTextStream>
using namespace std;

#define MaxLenth 10

//Ключевые слова
#define Tvoid 1
#define Tmain 2
#define Twhile 3
#define Tdouble 4
#define Tchar 5

#define Tid 10
#define Tstring 20
#define Tcchar 21
#define Tint10 22

//Специальные знаки
#define Tcomma 30
#define Tsemicolon 31
#define Tobracket 32
#define Tcbracket 33
#define Tobrace 34
#define Tcbrace 35
#define Tsqbbracket 36
#define Tsqcbracket 37

//Знаки операций
#define Tplus 40
#define Tminus 41
#define Tmult 42
#define Tdiv 43
#define Tmod 44
#define Tsave 45
#define Tless 46
#define Tmore 47
#define Teq_less 48
#define Teq_more 49
#define Teq 50
#define Tneq 51

#define Tend 100
#define Terr 200


class Scanner
{
public:
    int lexLenth;
    QFile *file;
    QString *text;
    QMultiMap <int, QString> Types;

    QMultiMap <int, QString> KeyWords;      //ключевые слова
    QMultiMap <int, QChar> Symbols;         //символы
    QMultiMap <int, QChar> IgnoreSymbols;   //игнорируемые символы
    QList <int*> Errors;

    Scanner();
    Scanner(QString fileName);
    void toScan (QList <Lexem> *lex);

};

#endif // SCANNER_H
