#ifndef NODE_H
#define NODE_H
#include <QString>

#define TypeVoid 0
#define TypeMain 1
#define TypeFunc 2
#define TypeMassive 3

#define TypeDouble 4
#define TypeChar 5
#define TypeString 6
#define TypeInt 7

#define TypeUnKnown -1
#define TypeEmpty 10

class Node
{
public:
    QString Id;             //идентификатор объекта
    int TypeObj;            //тип значения

    //обязательные параметры
    int ParamCount;         //число параметров функции
    int DimensionCount;     //число измерений
    int LowerBoundMas;      //нижняя граница массива (размерность)

    Node(QString I, int T, int PC = 0, int DC = 0, int LB = 0);
};
#endif // NODE_H
