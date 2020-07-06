#include "analizator.h"

#define error_err  //cout << ErrorText << "  " << (*lex)[cur].image.toLocal8Bit().data() << "<" << scaner->Types.value((*lex)[cur].type).toLocal8Bit().data() << ">  " << "Num-" << cur + 1 <<endl;
int end_analiz = 0;

Analizator::Analizator()
{
    scaner = new Scanner ("input.txt");
    lex = new QList <Lexem> ();
    cur = 0;
    c = cur;

}

void Analizator::Programm()
{
    scaner->toScan(lex);    //�������� ������ ������
    QString type;

    //����� ������� � � ����
    cout << "Lexems:" << endl;
    for (int i = 0; i < (*lex).count(); i++)
    {
        type = scaner->Types.value((*lex)[i].type);
        cout << i + 1 << "   " << (*lex)[i].image.toLocal8Bit().data() << "<"<<type.toLocal8Bit().data() <<">" << endl;
        cout << "������ - " << (*lex)[i].str << " | ������� - " << (*lex)[i].pos << endl << endl;
    }
    T = new Tree ();
    T->Cur = T;

    //���� ����������� ������ ���
    if(scaner->Errors.count() == 0)
        while ((*lex)[cur].type != Tend)
        {
            c = cur; // ���������� ����� �������

            //�� �������, ������ ��������� ����������
            if(!Function())
            {
                c = cur; //�������������� ����� �������

                //�� �������� ��������� - ������
                if(!VariableDescription())
                {
                    cur = c;
                    ErrorText = "��������� ������� ��� �������� ����������";
                    error_err
                    break;
                }

            }
        }

    cout.flush();

    if ((*lex)[cur].type == Tend)
    {
        {
            cout<<"\n�������������� ������ �� ����������\n";
        }
    }
    else
    {
        cout<<"\n���������� ������!"<< endl;
        cout << ErrorText << "  " << (*lex)[cur].image.toLocal8Bit().data() << "<" << scaner->Types.value((*lex)[cur].type).toLocal8Bit().data() << ">  " << "Num-" << cur + 1 <<endl;

    }

    cout.flush();

    return;
}

bool Analizator::Function()
{
    //���� �� void
    if((*lex)[cur].type != Tvoid)
    {
        ErrorText = "��������� void";
        error_err
        return false;
    }
    cur++;

    //���� �� main - ������ ������� �������
    if((*lex)[cur].type != Tmain)
    {
        //���� �� ��-���
        if((*lex)[cur].type != Tid)
        {

            ErrorText = "��������� �������������";
            error_err
            return false;
        }

        ///-----------------------------------------------------------
        ///������������ �������
        if (!T->semFToTable((*lex)[cur].image))
        {
            T->semError("������������ �������������� �������", &(*lex)[cur]);
        }
        ///-----------------------------------------------------------

        cur++;

        //���� �� (
        if((*lex)[cur].type != Tobracket )
        {

            ErrorText = "��������� '('";
            error_err
            return false;
        }
        cur++;

        //���� �������
        do
        {
            if((*lex)[cur].type == Tcomma) cur++;

            c = cur;
            //���� ���

            int t1 = -1;

            if(Type(t1))
            {         
                //���� �� ��-���
                if((*lex)[cur].type != Tid)
                {
                    ErrorText = "��������� �������������";
                    error_err
                    return false;
                }
                ///------------------------------------------------------------------
                /// ������� � ������ + �������� ����� ����������
                else
                {
                    //������� �������
                    Node *n = new Node ((*lex)[cur].image, t1);

                    //���� �� ���� ������ ���
                    if (!T->Cur->FindOneLevel(n))
                    {
                        T->Cur->semToTable(n);   //�������� � ������� (������)
                        T->semInc();             //�������� ����� ����������
                    }
                    //���� �� ���� ������ ����
                    else
                    {
                        T->semError("������������ ����� ���������", &(*lex)[cur]);
                    }
                }
                ///------------------------------------------------------------------
                cur++;
            }
            //���� �� ���� ����, �� ���� �������
            else
            {
                cur = c;
                if((*lex)[cur].type == Tcomma)
                {
                    ErrorText = "��������� ��� ������";
                    error_err
                    return false;
                }  
            }

        } while((*lex)[cur].type == Tcomma);

    }
    //���� main
    else
    {
        cur++;
        ///-----------------------------------------------------------
        /// ������������ main
        if (!T->semFToTable((*lex)[cur].image))
        {
            T->semError("������������ main", &(*lex)[cur]);
        }
        ///-----------------------------------------------------------

        //���� �� (
        if((*lex)[cur].type != Tobracket)
        {

            ErrorText = "��������� '('";
            error_err
            return false;
        }
        cur++;
    }

    //���� �� )
    if((*lex)[cur].type != Tcbracket)
    {

        ErrorText = "��������� '('";
        error_err
        return false;
    }
    cur++;

    c = cur;

    //���� �� ����
    if(!Block())
    {
        cur = c;

        ErrorText = "��������� ����";
        error_err
                return false;
    }
    ///-----------------------------------------------------------
    ///���� ��� ����. ����������� ��������
    else
    {
        T->semRep();
    }
    ///-----------------------------------------------------------

    return true;
}

bool Analizator::VariableDescription()
{
    c = cur;
    int t1 = -1;
    int t2 = -1;

    //���� �� ���
    if(!Type(t1)){
        cur = c;

        ErrorText = "��������� ��� ������";
        error_err
        return false;
    }

    do
    {
        if((*lex)[cur].type == Tcomma) cur++;

        //���� �� ��-���
        if((*lex)[cur].type != Tid)
        {

            ErrorText = "��������� �������������";
            error_err
            return false;
        }
        ///-----------------------------------------------------------
        /// �������� ������������� � �������
        else
        {
            if (t1 == TypeDouble || t1 == TypeChar)
                //�������� � ������� (������)
                T->Cur->semToTable(new Node ((*lex)[cur].image, t1));
            else
                T->semError("������������ ���. ��������� Double ��� Char.", &(*lex)[cur]);
        }
        ///-----------------------------------------------------------
        cur++;

        //���� =
        if((*lex)[cur].type == Tsave)
        {
            cur++;
            c = cur;

            //���� �� ���������
            if(!Phrasing(t2))
            {
                cur = c;
                ErrorText = "��������� ���������";
                error_err
                return false;
            }
            ///-----------------------------------------------------------
            /// �������� ���� ��� �������������
            else
                T->semTypeRes(t1, t2, &(*lex)[cur]);
            ///-----------------------------------------------------------
        }

        //���� [
        if((*lex)[cur].type == Tsqbbracket){

            cur++;
            c = cur;
            int val = cur;
            int t_m = -1;

            //���� �� ���������
            if(!Phrasing(t_m))
            {
                cur = c;

                ErrorText = "��������� ���������";
                error_err
                return false;
            }

            //���� �� ]
            if((*lex)[cur].type != Tsqcbracket){
                ErrorText = "��������� ']'";
                error_err
                return false;
            }
            ///-----------------------------------------------------------
            /// ����������� ��������� � ������� ����������� �������
            else
            {
                if ((*lex)[val].image.toInt() > 0 && t_m == TypeInt)
                    T->semMas((*lex)[val].image.toInt());
                else
                    T->semError("������������ ������ �������. ��������� int ������ 0.", &(*lex)[val]);
            }
            ///-----------------------------------------------------------

            cur++;
        }

    }while((*lex)[cur].type == Tcomma);

    //���� �� ;
    if((*lex)[cur].type != Tsemicolon)
    {

        ErrorText = "��������� ';'";
        error_err
        return false;
    }
    cur++;

    return true;
}

bool Analizator::Block()
{
    //���� �� {
    if((*lex)[cur].type != Tobrace)
    {

        ErrorText = "��������� '{'";
        error_err
        return false;
    }
    ///-----------------------------------------------------------
    /// ������� ������ ������� �����
    else
    {
        T->Cur->addLeft(new Node ("", TypeEmpty));
        T->Cur = T->Cur->Left;
        T->Cur = T->Cur->Right;
    }
    ///-----------------------------------------------------------
    cur++;

    //���� � ����� ���� �������� ���������� ��� ��������
    bool VD, O;
    do
    {
        VD = O = false;
        c = cur;

        if(!VariableDescription()){
            cur = c;
        }
        else
        {
            VD = true;
            continue;
        }

        c = cur;
        if(!Operator()){
            cur = c;
        }
        else{
            O = true;
            continue;
        }

    } while(VD == true || O == true);

    //���� �� }
    if((*lex)[cur].type != Tcbrace)
    {

        ErrorText = "��������� '}'";
        error_err
        return false;
    }
    cur++;

    end_analiz = cur;

    ///-----------------------------------------------------------
    ///���� ��� ����. ����������� ��������
    T->semRep();
    ///-----------------------------------------------------------

    return true;
}

bool Analizator::Operator()
{
    c = cur;

    //���� �� ������������, �� while
    if(!Assignment())
    {
        cur = c;

        //���� �� while, �� ����
        if((*lex)[cur].type != Twhile)
        {
            c = cur;

            //���� �� ����, �� ����� �������
            if(!Block())
            {
                int t = -1;
                //���� �� ����� �������, �� ;
                if(!CallFunction(t))
                {
                    cur = c;

                    //���� �� ;
                    if((*lex)[cur].type != Tsemicolon)
                    {

                        ErrorText = "��������� ';'";
                        error_err
                        return false;
                    }
                    cur++;

                    return true;
                }
                else return true;
            }
            else return true;
        }
        //���� while
        else
        {
            int t2 = -1;

            cur++;
            //���� �� (
            if((*lex)[cur].type != Tobracket)
            {

                ErrorText = "��������� '('";
                error_err
                return false;
            }
            cur++;

            c = cur;
            //���� �� ���������
            if(!Phrasing(t2))
            {
                cur = c;

                ErrorText = "��������� ���������";
                error_err
                return false;
            }

            //���� �� )
            if((*lex)[cur].type != Tcbracket)
            {

                ErrorText = "��������� ')'";
                error_err
                return false;
            }
            cur++;

            c = cur;
            //���� �� ����
            if(!Block())
            {
                cur = c;

                ErrorText = "��������� ����";
                error_err
                return false;
            }  

            return true;
        }
    }
    else return true;

    return true;
}

bool Analizator::CallFunction(int& Typ)
{
    //���� �� ��-���
    if((*lex)[cur].type != Tid)
    {
        ErrorText = "��������� �������������";
        error_err
        return false;
    }
    ///-----------------------------------------------------------
    /// ��������� ������� �������
    else
    {
        if(!T->semSearch(new Node ((*lex)[cur].image, TypeFunc)))
            T->semError("������������� �������", &(*lex)[cur]);
        else
        {
            Typ = T->Cur->Find((*lex)[cur].image)->N->TypeObj;
        }
    }
    cur++;
    ///-----------------------------------------------------------

    //���� �� (
    if((*lex)[cur].type != Tobracket)
    {

        ErrorText = "��������� '('";
        error_err
        return false;
    }
    cur++;

    //���� �������
    do
    {

        int t2 = -1;
        if((*lex)[cur].type == Tcomma)
        {
            cur++;
        }

        c = cur;

        //���� �� ���������
        if(!Phrasing(t2))
        {
            cur--;
            if((*lex)[cur].type == Tcomma)
            {

                ErrorText = "��������� ���������";
                error_err
                return false;
            }
            cur = c;
        }


    } while((*lex)[cur].type == Tcomma);

    //���� �� )
    if((*lex)[cur].type != Tcbracket)
    {

        ErrorText = "��������� ')'";
        error_err
        return false;
    }
    cur++;

    //���� �� ;
    if((*lex)[cur].type != Tsemicolon)
    {

        ErrorText = "��������� ';'";
        error_err
        return false;
    }
    cur++;

    return true;
}

bool Analizator::Assignment()
{
    int t1 = -1;
    int t2 = -1;
    QString lexem;

    //���� �� ��-���
    if((*lex)[cur].type != Tid)
    {
        ErrorText = "��������� �������������";
        error_err
        return false;
    }
    ///-----------------------------------------------------------
    /// ���� ��������������
    else
    {
        if(!T->semSearch((*lex)[cur].image))
        {
            T->semError("������������� �������������", &(*lex)[cur]);
        }
        else
        {
            lexem = (*lex)[cur].image;
            t1 = T->Cur->Find((*lex)[cur].image)->N->TypeObj;
        }
    }
    ///-----------------------------------------------------------
    cur++;

    //���� [
    if((*lex)[cur].type == Tsqbbracket)
    {
        cur++;
        c = cur;

        int val = cur;
        int t_m = -1;

        //���� �� ���������
        if(!Phrasing(t_m))
        {
            cur = c;
            ErrorText = "��������� ���������";
            error_err
            return false;
        }

        //���� �� ]
        if((*lex)[cur].type != Tsqcbracket)
        {
            ErrorText = "��������� ']'";
            error_err
            return false;
        }
        ///-----------------------------------------------------------
        /// �������� �� ������� ��������
        else
        {
            if (t_m != TypeInt && !T->semMasEl(lexem, (*lex)[val].image.toInt()))
                T->semError("������ �������� �� �������� �������.", &(*lex)[val]);
        }
        ///-----------------------------------------------------------
        cur++;
    }

    //���� �� =
    if((*lex)[cur].type != Tsave)
    {

        ErrorText = "��������� '='";
        error_err
        return false;
    }
    cur++;

    c = cur;
    //���� �� ���������
    if(!Phrasing(t2))
    {
        cur = c;

        ErrorText = "��������� ���������";
        error_err
        return false;
    }
    ///-----------------------------------------------------------
    /// �������� ����������� �����
    else
    {
        T->semTypeRes(t1, t2, &(*lex)[cur - 1]);
    }
    ///-----------------------------------------------------------

    //���� �� ;
    if((*lex)[cur].type != Tsemicolon)
    {
        ErrorText = "��������� ';'";
        error_err
        return false;
    }
    cur++;

    return true;
}

bool Analizator::Phrasing(int& Typ)
{
    int t1 = -1;
    int t2 = -1;
    int t3 = -1;
    int counter = 0;

    //����� ���������
    do{
        if((*lex)[cur].type == Tmore ||
                (*lex)[cur].type == Tless ||
                (*lex)[cur].type == Teq_more ||
                (*lex)[cur].type == Teq_less ||
                (*lex)[cur].type == Teq ||
                (*lex)[cur].type == Tneq)
        {
            cur++;
        }

        //���� + ��� -
        if((*lex)[cur].type == Tplus || (*lex)[cur].type == Tminus) cur++;

        bool flag = false;
        // + � -
        do{
            if(flag)
            {
                cur++;
                flag = false;
            }

            // / � *
            do{
                if((*lex)[cur].type == Tdiv || (*lex)[cur].type == Tmult)
                    cur++;

                c = cur;
                //���� �� ��. ���������
                if(!BasicPhrasing(Typ)){
                    cur = c;
                    ErrorText = "��������� ������������ ���������";
                    error_err
                    return false;
                }
                ///-----------------------------------------------------------
                /// �������� ����������� �����
                else
                {
                    if(counter == 0)
                    {
                        t1 = Typ;
                        t1 = T->semTypeRes(t1, t1, &(*lex)[cur - 1]);
                        counter++;
                    }
                    else if(counter == 1)
                    {
                        t2 = Typ;
                        t3 = T->semTypeRes(t1, t2, &(*lex)[cur - 1]);
                        counter++;
                    }
                    else if(counter == 2)
                    {
                        t1 = t3;
                        t2 = Typ;
                        t3 = T->semTypeRes(t1, t2, &(*lex)[cur - 1]);
                    }
                }
                ///----------------------------------------------------------

            }while((*lex)[cur].type == Tdiv || (*lex)[cur].type == Tmult);

            if((*lex)[cur].type == Tplus || (*lex)[cur].type == Tminus)
                flag = true;

        }while((*lex)[cur].type == Tplus || (*lex)[cur].type == Tminus);
    }while((*lex)[cur].type == Tmore ||
           (*lex)[cur].type == Tless ||
           (*lex)[cur].type == Teq_more ||
           (*lex)[cur].type == Teq_less ||
           (*lex)[cur].type == Teq ||
           (*lex)[cur].type == Tneq);

    return true;
}

bool Analizator::BasicPhrasing(int& Typ)
{
    QString lexem;

    //���� �� ��-���
    if((*lex)[cur].type != Tid)
    {
        //���� �� (
        if((*lex)[cur].type != Tobracket)
        {
            c = cur;

            //���� �� ����� �������
            if(!CallFunction(Typ))
            {
                cur = c;
                if((*lex)[cur].type != Tint10)
                {
                    if((*lex)[cur].type != Tstring)
                    {
                        if((*lex)[cur].type != Tcchar)
                        {
                            ErrorText = "��������� ��������� ���������";
                            error_err
                            return false;
                        }
                        Typ = T->Cur->semType(&(*lex)[cur]);
                        cur++;
                        return true;
                    }
                    else
                    {
                        Typ = T->Cur->semType(&(*lex)[cur]);
                        cur++;
                        return true;
                    }
                }
                else
                {
                    Typ = T->Cur->semType(&(*lex)[cur]);
                    cur++;
                    return true;
                }
            }
            else return true;
        }
        else
        {
            cur++;  
            c = cur;

            //���� �� ���������
            if(!Phrasing(Typ))
            {
                cur = c;
                ErrorText = "��������� ���������";
                error_err
                return false;
            }

            //���� �� )
            if((*lex)[cur].type != Tcbracket)
            {

                ErrorText = "��������� ')'";
                error_err
                return false;
            }
            cur++;
            return true;
        }
    }
    else
    {
        ///-----------------------------------------------------------
        /// �������� �� �������������
        if(!T->semSearch((*lex)[cur].image))
        {
            T->semError("������������� �������������", &(*lex)[cur]);
        }
        else
        {
            lexem = (*lex)[cur].image;
            Typ = T->Cur->Find((*lex)[cur].image)->N->TypeObj;
        }
        ///-----------------------------------------------------------

        cur++;

        //���� [
        if((*lex)[cur].type == Tsqbbracket)
        {
            cur++;
            c = cur;

            int t_m = -1;
            int val = cur;

            //���� �� ���������
            if(!Phrasing(t_m))
            {
                cur = c;
                ErrorText = "��������� ���������";
                error_err
                return false;
            }

            //���� �� ]
            if((*lex)[cur].type != Tsqcbracket)
            {
                ErrorText = "��������� ']'";
                error_err
                return false;
            }
            ///-----------------------------------------------------------
            /// �������� �� ������� ��������
            else
            {
                if (t_m != TypeInt && !T->semMasEl(lexem, (*lex)[val].image.toInt()))
                    T->semError("������ �������� �� �������� �������.", &(*lex)[val]);
            }
            ///-----------------------------------------------------------
            cur++;
        }
        return true;
    }
    return true;
}

bool Analizator::Type(int& Typ)
{
    //���� �� double � char
    if((*lex)[cur].type != Tdouble && (*lex)[cur].type != Tchar)
    {

        ErrorText = "��������� double ��� char";
        error_err
        return false;
    }

    ///-----------------------------------------------------------
    /// ��������� ��� ���������
    Typ = T->semType(&(*lex)[cur]);
    ///-----------------------------------------------------------

    cur++;

    return true;
}
