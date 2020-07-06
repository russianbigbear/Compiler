#include "tree.h"
using namespace std;

Tree* Tree::Cur;    //текущая вершина
Tree* Tree::F;      //текущая функция

/*по умолчанию - пустая вершина*/
Tree::Tree()
{
    N = new Node ("", TypeEmpty);
    Right = Left = Parent = nullptr;
}

/*создание веришины с параметрами*/
Tree::Tree(Tree *p, Tree *l, Tree *r, Node *n)
{
    N = n;
    Parent = p;
    Left = l;
    Right = r;
}

/*создание вершины*/
Tree::Tree(Node *n)
{
    N = n;
    Parent = nullptr;
    Left = nullptr;
    Right = nullptr;
}

/*добавление левого потомка вершины*/
void Tree::addLeft (Node *n)
{
    Tree *a = new Tree (this, nullptr, new Tree (), n);
    a->Right->Parent = a;    //родитель - текущая
    Left = a;
    //Cur = Left;
}

/*добавление правого потомка вершины*/
void Tree::addRight (Node *n)
{
    Tree *a = new Tree (this, nullptr, new Tree (), n);
    a->Right->Parent = a;        //родитель - текущая
    Right = a;
    //Cur = Right;
}

/*поиск по дереву вверх*/
Tree *Tree::Find (Tree *From, Node* n)
{
    Tree *cur = From;
    while (cur != nullptr)
        //объект таблицы не совпадает с заданным
        if (n->Id != cur->N->Id || n->TypeObj != cur->N->TypeObj)
            cur = cur->Parent;
        else break;
    return cur;
}

/*поиск по дереву вверх*/
Tree *Tree::Find (QString name)
{
    Tree *cur = this;
    while (cur != nullptr)
        //имя объекта таблицы не совпадает с заданным
        if (cur->N->Id != name)
            cur = cur->Parent;
        else break;
    return cur;
}

/*поиск по дереву вверх*/
Tree *Tree::Find (Node* n)
{
    //возращение рещультата после поиска по вершине
    return Find (this, n);
}

/*поиск на одном уровне*/
Tree *Tree::FindOneLevel (Tree *From, Node* n)
{
    Tree *cur = From;

    //пока не дошли до конца или до пустой вершины
    while (cur != nullptr)
    {
        if (cur->Parent != nullptr)
        {
            //правый потомок
            if (cur->Parent->Right == cur)
            {
                break;
            }
            else
            {
                //объект таблицы не совпадает с заданным
                if (n->Id != cur->N->Id || n->TypeObj != cur->N->TypeObj)
                {
                    cur = cur->Parent;
                }
                else break;
            }
        }
        else cur = cur->Parent;
    }

    if (cur != nullptr)
        if (n->Id != cur->N->Id || n->TypeObj != cur->N->TypeObj)
            cur = nullptr;

    return cur;
}

/*поиск на одном уровне*/
Tree *Tree::FindOneLevel (Node* n)
{
    //возращение рещультата после поиска на одном уровне
    return FindOneLevel (this, n);
}



Tree *Tree::FindRightLeft (Tree *From, Node* n)
{
    Tree *cur = From->Right;
    while (cur != nullptr)
        //объект таблицы не совпадает с заданным
        if (n->Id != cur->N->Id || n->TypeObj != cur->N->TypeObj)
            cur = cur->Left;
        else break;
    return cur;
}

/*поиск в правом ряду*/
Tree *Tree::FindRightLeft (Node* n)
{
    //возращение результата после поиска в правом ряду
    return FindRightLeft (this, n);
}

/*определение семантического типа лексемы*/
int Tree::semType(Lexem *lex)
{
    switch (lex->type)
    {
    case Tdouble:
    {
        return TypeDouble;
    }
    case Tint10:
    {
        return TypeInt;
    }
    case Tchar:
    {
        return TypeChar;
    }
    case Tcchar:
    {
        return TypeChar;
    }
    case Tstring:
    {
        return TypeString;
    }
    case Tmain:
    {
        return TypeMain;
    }
    case Tvoid:
    {
        return TypeVoid;
    }
    default:
        return TypeUnKnown;
    }
}

/*проверка на дублирование, занесение идентификатора вместе с семантическим типом в таблицу*/
bool Tree::semToTable (Node *n)
{
    //узла еще нет
    if (!Cur->FindOneLevel(n))
    {
        Cur->addLeft(n);
        Cur = Cur->Left;
        return true;
    }
    //дублирование
    else return false;

}

/*занесение имени функции  в таблицу, создание пустой правой вершины. Устанавливает указатель на правую созданную*/
bool Tree::semFToTable (QString name)
{
    //узла еще нет
    if (!Cur->Find(name))
    {
        Cur->addLeft(new Node (name, TypeFunc));
        Cur = Cur->Left;
        F = Cur;                //указатель на функцию
        Cur = Cur->Right;       //переходим на пустую правую
        return true;
    }
    //дублирование
    else return false;
}

/*восстановление указателя на вершину. Поднимаемся по левым связям и подн. на след уровень*/
void Tree::semRep ()
{
    Tree *c = Cur;
    //пока не дошли до конца цепочки
    while (Cur != nullptr)
        if (Cur->Parent != nullptr)
            //не правый потомок
            if (Cur->Parent->Right != Cur)
                Cur = Cur->Parent;
            else break;
        else Cur = Cur->Parent;

    //указатель на родителя
    if (Cur != nullptr)
        Cur = Cur->Parent;
}

/*добавить пустую вершину справа*/
void Tree::semToRight()
{
    Cur->addRight(new Node ("", TypeEmpty));     //пустая вершина
    Cur = Cur->Right;
}

/*увеличить число параметров функции*/
void Tree::semInc (Tree *func)
{
    func->N->ParamCount++;
}

/*увеличить число параметров функции*/
void Tree::semInc ()
{
    F->N->ParamCount++;
}

/*Увеличить измерение массива и занести размерность массива*/
void Tree::semMas(int size){
    Cur->N->DimensionCount++;
    Cur->N->LowerBoundMas = size;

}

/*установить число параметров*/
void Tree::semSetParAmount (Tree* f, int n)
{
    f->N->ParamCount = n;
}

/*поиск функции в таблице*/
bool Tree::semSearch (Node* n)
{
    //ищем объект
    Tree* ptr = Cur->Find(n);

    if (ptr == nullptr)
        return false;
    else
        return true;
}

/*поиск идентификатора в таблице*/
bool Tree::semSearch (QString name)
{
    //ищем объект
    Tree* ptr = Cur->Find(name);

    if (ptr == nullptr)
        return false;
    else
        return true;
}

/*проверка на наличие элемента*/
bool Tree::semMasEl(QString name, int num)
{
    //ищем объект
    Tree* ptr = Find(name);
    int LB = ptr->Cur->N->LowerBoundMas;

    cout << LB << endl;
    system("pause");

    if(num >= 0 && num < LB)
        return true;
    else
        return false;
}

/*вывод семантической ошибки*/
void Tree::semError(std::string err, Lexem* L)
{
    QTextStream cout (stdout);

    cout.setCodec(QTextCodec::codecForName("cp866"));

    std::cout<<"\n Семантическая ошибка в "<<L->str<<":"<<L->pos<<": "<<err;
}

/*проверка результата операции*/
int Tree::semTypeRes (int o1, int o2, Lexem* l)
{
    if (o1 == TypeInt && o2 == TypeInt)
        return TypeInt;
    if (o1 == TypeDouble && o2 == TypeDouble)
        return TypeDouble;
    if (o1 == TypeChar && o2 == TypeChar)
        return TypeChar;
    if (o1 == TypeString && o2 == TypeString)
        return TypeString;
    if (o1 == TypeDouble && o2 == TypeDouble)
        return TypeDouble;

    if (o1 == TypeInt && o2 == TypeDouble)
        return TypeDouble;
    if (o1 == TypeDouble && o2 == TypeInt)
        return TypeDouble;
    if (o1 == TypeChar && o2 == TypeDouble)
        return TypeChar;
    if (o1 == TypeDouble && o2 == TypeChar)
        return TypeDouble;

    if (o1 == TypeInt && o2 == TypeChar)
        return TypeInt;
    if (o1 == TypeChar && o2 == TypeInt)
        return TypeChar;

    if (o1 == TypeChar && o2 == TypeString)
        return TypeString;
    if (o1 == TypeString && o2 == TypeChar)
        return TypeString;

    if (o1 == TypeInt && o2 == TypeString)
        semError("Несоответствие типов", l);
    if (o1 == TypeString && o2 == TypeInt)
        semError("Несоответствие типов", l);
    if (o1 == TypeString && o2 == TypeDouble)
        semError("Несоответствие типов", l);
    if (o1 == TypeDouble && o2 == TypeString)
        semError("Несоответствие типов", l);
    if (o1 != TypeUnKnown && o2 != TypeUnKnown)
        semError("Несоответствие типов", l);
    if (o1 == TypeUnKnown && o2 == TypeDouble)
        semError("Несоответствие типов", l);
    if (o1 == TypeDouble && o2 == TypeUnKnown)
        semError("Несоответствие типов", l);
    if (o1 == TypeInt && o2 == TypeUnKnown)
        semError("Несоответствие типов", l);
    if (o1 == TypeUnKnown && o2 == TypeInt)
        semError("Несоответствие типов", l);
    if (o1 == TypeChar && o2 == TypeUnKnown)
        semError("Несоответствие типов", l);
    if (o1 == TypeUnKnown && o2 == TypeChar)
        semError("Несоответствие типов", l);
    if (o1 == TypeString && o2 == TypeUnKnown)
        semError("Несоответствие типов", l);
    if (o1 == TypeUnKnown && o2 == TypeString)
        semError("Несоответствие типов", l);

    cout << endl << o1 << "   " << o2;

    return TypeUnKnown;
}
