#ifndef ANALIZATOR_H
#define ANALIZATOR_H


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

#include <iostream>
#include <QTextStream>
#include <qdebug.h>
#include "lexem.h"
#include "QList"
#include "scanner.h"
#include "locale.h"
#include "QString"
#include "tree.h"


class Analizator
{
public:

    Analizator();
    int cur;
    int c;                          //порядковый номер текущей лексемы
    Scanner *scaner;
    QList <Lexem> *lex;
    bool right;
    std::string ErrorText;

    Tree* T;                        //семантическое дерево

    //анализ
    void toAnalize ();

    //схемы
    void Programm ();               //программа
    bool VariableDescription ();    //описание переменных
    bool Function ();               //функция
    bool Type (int& Typ);          //тип
    bool Block ();                  //блок
    bool Operator ();               //оператор
    bool CallFunction (int& Typ);           //вызов функции
    bool Assignment ();             //присваивание
    bool Phrasing (int& Typ);               //выражение
    bool BasicPhrasing (int& Typ);          //эл. выражение


};

#endif // ANALIZATOR_H
