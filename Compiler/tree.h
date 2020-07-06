#ifndef TREE_H
#define TREE_H
#include <node.h>
#include <iostream>
#include <QTextStream>
#include <QTextCodec>
#include <lexem.h>

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


class Tree
{
public:
    Node *N;                                         //вершина
    Tree *Parent, *Left, *Right;                     //родитель, левый и правый потомки
    static Tree * Cur;                               //текущая вершина
    static Tree * F;                                 //текущая функция

    ///функции дерева
    Tree();                                          //конструктор без параметров
    Tree(Node *n);                                   //конструктор с вершиной
    Tree(Tree *p, Tree *l, Tree *r, Node *n);        //конструктор с параметрами

    void addLeft (Node *n);                          //добавить левого потомка
    void addRight (Node *n);                         //добавить правого потомка

    Tree *Find (Tree *From, Node* n);                //поиск в таблице. Может вернуть null если нету
    Tree *Find (QString name);                       //поиск в таблице. Может вернуть null если нету
    Tree *Find (Node* n);                            //поиск в таблице. Может вернуть null если нету

    Tree *FindOneLevel (Tree *From, Node* n);        //поиск в таблице на одном уровне. Может вернуть null если нету
    Tree *FindOneLevel (Node* n);                    //поиск в таблице на одном уровне. Может вернуть null если нету

    Tree *FindRightLeft (Tree *From, Node* n);       //поиск среди прямых потомков. Может вернуть null если нету
    Tree *FindRightLeft (Node* n);                   //поиск среди прямых потомков. Может вернуть null если нету

    ///семантические подпрограммы
    int semType (Lexem *lex);                        //определение семантического типа
    bool semToTable (Node *n);                       //проверка на дублирование, занесение идентификатора вместе с семантическим типом в таблицу
    bool semFToTable (QString name);                 //занесение имени функции вместе с типом возвращаемого значения в таблицу, создание пустой правой вершины. Возвращает указатель на созданную
    void semSetParAmount (Tree* f, int n);           //установить число параметров
    void semRep ();                                  //восстановление указателя на вершину. Поднимаемся по левым связям и подн. на след уровень
    void semInc (Tree *func);                        //увеличить число параметров функции
    void semInc ();                                  //увеличить число параметров функции
    bool semSearch (Node* n);                        //поиск функции в таблице
    bool semSearch (QString name);                   //поиск идентификатора
    bool semParAccord ();                            //проверка соотв. параметров
    int semTypeRes (int o1, int o2, Lexem* l);       //проверка результата операции
    void semMas(int size);                           //увеличить измерение и занести размерность массива
    bool semMasEl(QString name, int num);            //проверка на существование элемента массива

    void semToRight ();                              //уйти вправо
    void semError (std::string err, Lexem* L);

};

#endif // TREE_H
