#include "tree.h"
using namespace std;

Tree* Tree::Cur;    //������� �������
Tree* Tree::F;      //������� �������

/*�� ��������� - ������ �������*/
Tree::Tree()
{
    N = new Node ("", TypeEmpty);
    Right = Left = Parent = nullptr;
}

/*�������� �������� � �����������*/
Tree::Tree(Tree *p, Tree *l, Tree *r, Node *n)
{
    N = n;
    Parent = p;
    Left = l;
    Right = r;
}

/*�������� �������*/
Tree::Tree(Node *n)
{
    N = n;
    Parent = nullptr;
    Left = nullptr;
    Right = nullptr;
}

/*���������� ������ ������� �������*/
void Tree::addLeft (Node *n)
{
    Tree *a = new Tree (this, nullptr, new Tree (), n);
    a->Right->Parent = a;    //�������� - �������
    Left = a;
    //Cur = Left;
}

/*���������� ������� ������� �������*/
void Tree::addRight (Node *n)
{
    Tree *a = new Tree (this, nullptr, new Tree (), n);
    a->Right->Parent = a;        //�������� - �������
    Right = a;
    //Cur = Right;
}

/*����� �� ������ �����*/
Tree *Tree::Find (Tree *From, Node* n)
{
    Tree *cur = From;
    while (cur != nullptr)
        //������ ������� �� ��������� � ��������
        if (n->Id != cur->N->Id || n->TypeObj != cur->N->TypeObj)
            cur = cur->Parent;
        else break;
    return cur;
}

/*����� �� ������ �����*/
Tree *Tree::Find (QString name)
{
    Tree *cur = this;
    while (cur != nullptr)
        //��� ������� ������� �� ��������� � ��������
        if (cur->N->Id != name)
            cur = cur->Parent;
        else break;
    return cur;
}

/*����� �� ������ �����*/
Tree *Tree::Find (Node* n)
{
    //���������� ���������� ����� ������ �� �������
    return Find (this, n);
}

/*����� �� ����� ������*/
Tree *Tree::FindOneLevel (Tree *From, Node* n)
{
    Tree *cur = From;

    //���� �� ����� �� ����� ��� �� ������ �������
    while (cur != nullptr)
    {
        if (cur->Parent != nullptr)
        {
            //������ �������
            if (cur->Parent->Right == cur)
            {
                break;
            }
            else
            {
                //������ ������� �� ��������� � ��������
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

/*����� �� ����� ������*/
Tree *Tree::FindOneLevel (Node* n)
{
    //���������� ���������� ����� ������ �� ����� ������
    return FindOneLevel (this, n);
}



Tree *Tree::FindRightLeft (Tree *From, Node* n)
{
    Tree *cur = From->Right;
    while (cur != nullptr)
        //������ ������� �� ��������� � ��������
        if (n->Id != cur->N->Id || n->TypeObj != cur->N->TypeObj)
            cur = cur->Left;
        else break;
    return cur;
}

/*����� � ������ ����*/
Tree *Tree::FindRightLeft (Node* n)
{
    //���������� ���������� ����� ������ � ������ ����
    return FindRightLeft (this, n);
}

/*����������� �������������� ���� �������*/
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

/*�������� �� ������������, ��������� �������������� ������ � ������������� ����� � �������*/
bool Tree::semToTable (Node *n)
{
    //���� ��� ���
    if (!Cur->FindOneLevel(n))
    {
        Cur->addLeft(n);
        Cur = Cur->Left;
        return true;
    }
    //������������
    else return false;

}

/*��������� ����� �������  � �������, �������� ������ ������ �������. ������������� ��������� �� ������ ���������*/
bool Tree::semFToTable (QString name)
{
    //���� ��� ���
    if (!Cur->Find(name))
    {
        Cur->addLeft(new Node (name, TypeFunc));
        Cur = Cur->Left;
        F = Cur;                //��������� �� �������
        Cur = Cur->Right;       //��������� �� ������ ������
        return true;
    }
    //������������
    else return false;
}

/*�������������� ��������� �� �������. ����������� �� ����� ������ � ����. �� ���� �������*/
void Tree::semRep ()
{
    Tree *c = Cur;
    //���� �� ����� �� ����� �������
    while (Cur != nullptr)
        if (Cur->Parent != nullptr)
            //�� ������ �������
            if (Cur->Parent->Right != Cur)
                Cur = Cur->Parent;
            else break;
        else Cur = Cur->Parent;

    //��������� �� ��������
    if (Cur != nullptr)
        Cur = Cur->Parent;
}

/*�������� ������ ������� ������*/
void Tree::semToRight()
{
    Cur->addRight(new Node ("", TypeEmpty));     //������ �������
    Cur = Cur->Right;
}

/*��������� ����� ���������� �������*/
void Tree::semInc (Tree *func)
{
    func->N->ParamCount++;
}

/*��������� ����� ���������� �������*/
void Tree::semInc ()
{
    F->N->ParamCount++;
}

/*��������� ��������� ������� � ������� ����������� �������*/
void Tree::semMas(int size){
    Cur->N->DimensionCount++;
    Cur->N->LowerBoundMas = size;

}

/*���������� ����� ����������*/
void Tree::semSetParAmount (Tree* f, int n)
{
    f->N->ParamCount = n;
}

/*����� ������� � �������*/
bool Tree::semSearch (Node* n)
{
    //���� ������
    Tree* ptr = Cur->Find(n);

    if (ptr == nullptr)
        return false;
    else
        return true;
}

/*����� �������������� � �������*/
bool Tree::semSearch (QString name)
{
    //���� ������
    Tree* ptr = Cur->Find(name);

    if (ptr == nullptr)
        return false;
    else
        return true;
}

/*�������� �� ������� ��������*/
bool Tree::semMasEl(QString name, int num)
{
    //���� ������
    Tree* ptr = Find(name);
    int LB = ptr->Cur->N->LowerBoundMas;

    cout << LB << endl;
    system("pause");

    if(num >= 0 && num < LB)
        return true;
    else
        return false;
}

/*����� ������������� ������*/
void Tree::semError(std::string err, Lexem* L)
{
    QTextStream cout (stdout);

    cout.setCodec(QTextCodec::codecForName("cp866"));

    std::cout<<"\n ������������� ������ � "<<L->str<<":"<<L->pos<<": "<<err;
}

/*�������� ���������� ��������*/
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
        semError("�������������� �����", l);
    if (o1 == TypeString && o2 == TypeInt)
        semError("�������������� �����", l);
    if (o1 == TypeString && o2 == TypeDouble)
        semError("�������������� �����", l);
    if (o1 == TypeDouble && o2 == TypeString)
        semError("�������������� �����", l);
    if (o1 != TypeUnKnown && o2 != TypeUnKnown)
        semError("�������������� �����", l);
    if (o1 == TypeUnKnown && o2 == TypeDouble)
        semError("�������������� �����", l);
    if (o1 == TypeDouble && o2 == TypeUnKnown)
        semError("�������������� �����", l);
    if (o1 == TypeInt && o2 == TypeUnKnown)
        semError("�������������� �����", l);
    if (o1 == TypeUnKnown && o2 == TypeInt)
        semError("�������������� �����", l);
    if (o1 == TypeChar && o2 == TypeUnKnown)
        semError("�������������� �����", l);
    if (o1 == TypeUnKnown && o2 == TypeChar)
        semError("�������������� �����", l);
    if (o1 == TypeString && o2 == TypeUnKnown)
        semError("�������������� �����", l);
    if (o1 == TypeUnKnown && o2 == TypeString)
        semError("�������������� �����", l);

    cout << endl << o1 << "   " << o2;

    return TypeUnKnown;
}
