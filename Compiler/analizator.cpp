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
    scaner->toScan(lex);    //получаем список лексем
    QString type;

    //вывод лексемы и её типа
    cout << "Lexems:" << endl;
    for (int i = 0; i < (*lex).count(); i++)
    {
        type = scaner->Types.value((*lex)[i].type);
        cout << i + 1 << "   " << (*lex)[i].image.toLocal8Bit().data() << "<"<<type.toLocal8Bit().data() <<">" << endl;
        cout << "Строка - " << (*lex)[i].str << " | Позиция - " << (*lex)[i].pos << endl << endl;
    }
    T = new Tree ();
    T->Cur = T;

    //если лексический ошибок нет
    if(scaner->Errors.count() == 0)
        while ((*lex)[cur].type != Tend)
        {
            c = cur; // записываем номер лексемы

            //не функция, значит описанипе переменных
            if(!Function())
            {
                c = cur; //востанавливаем номер лексемы

                //не описание пременных - ошибка
                if(!VariableDescription())
                {
                    cur = c;
                    ErrorText = "Ожидается функция или описание переменных";
                    error_err
                    break;
                }

            }
        }

    cout.flush();

    if ((*lex)[cur].type == Tend)
    {
        {
            cout<<"\nСинтаксических ошибок не обнаружено\n";
        }
    }
    else
    {
        cout<<"\nОбнаружена ошибка!"<< endl;
        cout << ErrorText << "  " << (*lex)[cur].image.toLocal8Bit().data() << "<" << scaner->Types.value((*lex)[cur].type).toLocal8Bit().data() << ">  " << "Num-" << cur + 1 <<endl;

    }

    cout.flush();

    return;
}

bool Analizator::Function()
{
    //если не void
    if((*lex)[cur].type != Tvoid)
    {
        ErrorText = "Ожидается void";
        error_err
        return false;
    }
    cur++;

    //если не main - значит простая функция
    if((*lex)[cur].type != Tmain)
    {
        //если не ид-тор
        if((*lex)[cur].type != Tid)
        {

            ErrorText = "Ожидается идентификатор";
            error_err
            return false;
        }

        ///-----------------------------------------------------------
        ///дублирование функции
        if (!T->semFToTable((*lex)[cur].image))
        {
            T->semError("Дублирование идентификатора функции", &(*lex)[cur]);
        }
        ///-----------------------------------------------------------

        cur++;

        //если не (
        if((*lex)[cur].type != Tobracket )
        {

            ErrorText = "Ожидается '('";
            error_err
            return false;
        }
        cur++;

        //цикл запятой
        do
        {
            if((*lex)[cur].type == Tcomma) cur++;

            c = cur;
            //если тип

            int t1 = -1;

            if(Type(t1))
            {         
                //если не ид-тор
                if((*lex)[cur].type != Tid)
                {
                    ErrorText = "Ожидается идентификатор";
                    error_err
                    return false;
                }
                ///------------------------------------------------------------------
                /// заносим в дерево + увеличим число параметров
                else
                {
                    //создали вершину
                    Node *n = new Node ((*lex)[cur].image, t1);

                    //если на этом уровне нет
                    if (!T->Cur->FindOneLevel(n))
                    {
                        T->Cur->semToTable(n);   //помещаем в таблицу (дерево)
                        T->semInc();             //увеличим число параметров
                    }
                    //если на этом уровне есть
                    else
                    {
                        T->semError("Дублирование имени параметра", &(*lex)[cur]);
                    }
                }
                ///------------------------------------------------------------------
                cur++;
            }
            //если не было типа, но есть запятая
            else
            {
                cur = c;
                if((*lex)[cur].type == Tcomma)
                {
                    ErrorText = "Ожидается тип данных";
                    error_err
                    return false;
                }  
            }

        } while((*lex)[cur].type == Tcomma);

    }
    //если main
    else
    {
        cur++;
        ///-----------------------------------------------------------
        /// дублирование main
        if (!T->semFToTable((*lex)[cur].image))
        {
            T->semError("Дублирование main", &(*lex)[cur]);
        }
        ///-----------------------------------------------------------

        //если не (
        if((*lex)[cur].type != Tobracket)
        {

            ErrorText = "Ожидается '('";
            error_err
            return false;
        }
        cur++;
    }

    //если не )
    if((*lex)[cur].type != Tcbracket)
    {

        ErrorText = "Ожидается '('";
        error_err
        return false;
    }
    cur++;

    c = cur;

    //если не блок
    if(!Block())
    {
        cur = c;

        ErrorText = "Ожидается блок";
        error_err
                return false;
    }
    ///-----------------------------------------------------------
    ///если был блок. Восстановим уазатель
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

    //если не тип
    if(!Type(t1)){
        cur = c;

        ErrorText = "Ожидается тип данных";
        error_err
        return false;
    }

    do
    {
        if((*lex)[cur].type == Tcomma) cur++;

        //если не ид-тор
        if((*lex)[cur].type != Tid)
        {

            ErrorText = "Ожидается идентификатор";
            error_err
            return false;
        }
        ///-----------------------------------------------------------
        /// помещаем идентификатор в таблицу
        else
        {
            if (t1 == TypeDouble || t1 == TypeChar)
                //помещаем в таблицу (дерево)
                T->Cur->semToTable(new Node ((*lex)[cur].image, t1));
            else
                T->semError("Недопустимый тип. Требуется Double или Char.", &(*lex)[cur]);
        }
        ///-----------------------------------------------------------
        cur++;

        //если =
        if((*lex)[cur].type == Tsave)
        {
            cur++;
            c = cur;

            //если не выражение
            if(!Phrasing(t2))
            {
                cur = c;
                ErrorText = "Ожидается выражение";
                error_err
                return false;
            }
            ///-----------------------------------------------------------
            /// проверим типы при инициализации
            else
                T->semTypeRes(t1, t2, &(*lex)[cur]);
            ///-----------------------------------------------------------
        }

        //если [
        if((*lex)[cur].type == Tsqbbracket){

            cur++;
            c = cur;
            int val = cur;
            int t_m = -1;

            //если не выражение
            if(!Phrasing(t_m))
            {
                cur = c;

                ErrorText = "Ожидается выражение";
                error_err
                return false;
            }

            //если не ]
            if((*lex)[cur].type != Tsqcbracket){
                ErrorText = "Ожидается ']'";
                error_err
                return false;
            }
            ///-----------------------------------------------------------
            /// увеличиваем измерение и заносим размерность массива
            else
            {
                if ((*lex)[val].image.toInt() > 0 && t_m == TypeInt)
                    T->semMas((*lex)[val].image.toInt());
                else
                    T->semError("Недопустимый размер массива. Требуется int больше 0.", &(*lex)[val]);
            }
            ///-----------------------------------------------------------

            cur++;
        }

    }while((*lex)[cur].type == Tcomma);

    //если не ;
    if((*lex)[cur].type != Tsemicolon)
    {

        ErrorText = "Ожидается ';'";
        error_err
        return false;
    }
    cur++;

    return true;
}

bool Analizator::Block()
{
    //если не {
    if((*lex)[cur].type != Tobrace)
    {

        ErrorText = "Ожидается '{'";
        error_err
        return false;
    }
    ///-----------------------------------------------------------
    /// создаем пустую вершину слева
    else
    {
        T->Cur->addLeft(new Node ("", TypeEmpty));
        T->Cur = T->Cur->Left;
        T->Cur = T->Cur->Right;
    }
    ///-----------------------------------------------------------
    cur++;

    //если в блоке есть описание переменных или оператор
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

    //если не }
    if((*lex)[cur].type != Tcbrace)
    {

        ErrorText = "Ожидается '}'";
        error_err
        return false;
    }
    cur++;

    end_analiz = cur;

    ///-----------------------------------------------------------
    ///если был блок. Восстановим уазатель
    T->semRep();
    ///-----------------------------------------------------------

    return true;
}

bool Analizator::Operator()
{
    c = cur;

    //если не присваивание, то while
    if(!Assignment())
    {
        cur = c;

        //если не while, то блок
        if((*lex)[cur].type != Twhile)
        {
            c = cur;

            //если не блок, то вызов функции
            if(!Block())
            {
                int t = -1;
                //если не вызов функции, то ;
                if(!CallFunction(t))
                {
                    cur = c;

                    //если не ;
                    if((*lex)[cur].type != Tsemicolon)
                    {

                        ErrorText = "Ожидается ';'";
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
        //если while
        else
        {
            int t2 = -1;

            cur++;
            //если не (
            if((*lex)[cur].type != Tobracket)
            {

                ErrorText = "Ожидается '('";
                error_err
                return false;
            }
            cur++;

            c = cur;
            //если не выражение
            if(!Phrasing(t2))
            {
                cur = c;

                ErrorText = "Ожидается выражение";
                error_err
                return false;
            }

            //если не )
            if((*lex)[cur].type != Tcbracket)
            {

                ErrorText = "Ожидается ')'";
                error_err
                return false;
            }
            cur++;

            c = cur;
            //если не блок
            if(!Block())
            {
                cur = c;

                ErrorText = "Ожидается блок";
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
    //если не ид-тор
    if((*lex)[cur].type != Tid)
    {
        ErrorText = "Ожидается идентификатор";
        error_err
        return false;
    }
    ///-----------------------------------------------------------
    /// проверяем наличие функции
    else
    {
        if(!T->semSearch(new Node ((*lex)[cur].image, TypeFunc)))
            T->semError("Необъявленная функция", &(*lex)[cur]);
        else
        {
            Typ = T->Cur->Find((*lex)[cur].image)->N->TypeObj;
        }
    }
    cur++;
    ///-----------------------------------------------------------

    //если не (
    if((*lex)[cur].type != Tobracket)
    {

        ErrorText = "Ожидается '('";
        error_err
        return false;
    }
    cur++;

    //цикл запятой
    do
    {

        int t2 = -1;
        if((*lex)[cur].type == Tcomma)
        {
            cur++;
        }

        c = cur;

        //если не выражение
        if(!Phrasing(t2))
        {
            cur--;
            if((*lex)[cur].type == Tcomma)
            {

                ErrorText = "Ожидается выражение";
                error_err
                return false;
            }
            cur = c;
        }


    } while((*lex)[cur].type == Tcomma);

    //если не )
    if((*lex)[cur].type != Tcbracket)
    {

        ErrorText = "Ожидается ')'";
        error_err
        return false;
    }
    cur++;

    //если не ;
    if((*lex)[cur].type != Tsemicolon)
    {

        ErrorText = "Ожидается ';'";
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

    //если не ид-тор
    if((*lex)[cur].type != Tid)
    {
        ErrorText = "Ожидается идентификатор";
        error_err
        return false;
    }
    ///-----------------------------------------------------------
    /// ищем индентификатор
    else
    {
        if(!T->semSearch((*lex)[cur].image))
        {
            T->semError("Необъявленный идентификатор", &(*lex)[cur]);
        }
        else
        {
            lexem = (*lex)[cur].image;
            t1 = T->Cur->Find((*lex)[cur].image)->N->TypeObj;
        }
    }
    ///-----------------------------------------------------------
    cur++;

    //если [
    if((*lex)[cur].type == Tsqbbracket)
    {
        cur++;
        c = cur;

        int val = cur;
        int t_m = -1;

        //если не выражение
        if(!Phrasing(t_m))
        {
            cur = c;
            ErrorText = "Ожидается выражение";
            error_err
            return false;
        }

        //если не ]
        if((*lex)[cur].type != Tsqcbracket)
        {
            ErrorText = "Ожидается ']'";
            error_err
            return false;
        }
        ///-----------------------------------------------------------
        /// проверям на наличие элемента
        else
        {
            if (t_m != TypeInt && !T->semMasEl(lexem, (*lex)[val].image.toInt()))
                T->semError("Индекс элемента за границой массива.", &(*lex)[val]);
        }
        ///-----------------------------------------------------------
        cur++;
    }

    //если не =
    if((*lex)[cur].type != Tsave)
    {

        ErrorText = "Ожидается '='";
        error_err
        return false;
    }
    cur++;

    c = cur;
    //если не выражение
    if(!Phrasing(t2))
    {
        cur = c;

        ErrorText = "Ожидается выражение";
        error_err
        return false;
    }
    ///-----------------------------------------------------------
    /// проверка соответсвии типов
    else
    {
        T->semTypeRes(t1, t2, &(*lex)[cur - 1]);
    }
    ///-----------------------------------------------------------

    //если не ;
    if((*lex)[cur].type != Tsemicolon)
    {
        ErrorText = "Ожидается ';'";
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

    //знаки сравнения
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

        //если + или -
        if((*lex)[cur].type == Tplus || (*lex)[cur].type == Tminus) cur++;

        bool flag = false;
        // + и -
        do{
            if(flag)
            {
                cur++;
                flag = false;
            }

            // / и *
            do{
                if((*lex)[cur].type == Tdiv || (*lex)[cur].type == Tmult)
                    cur++;

                c = cur;
                //если не эл. выражение
                if(!BasicPhrasing(Typ)){
                    cur = c;
                    ErrorText = "Ожидается элементарное выражение";
                    error_err
                    return false;
                }
                ///-----------------------------------------------------------
                /// проверка соответсвии типов
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

    //если не ид-тор
    if((*lex)[cur].type != Tid)
    {
        //если не (
        if((*lex)[cur].type != Tobracket)
        {
            c = cur;

            //если не вызов функции
            if(!CallFunction(Typ))
            {
                cur = c;
                if((*lex)[cur].type != Tint10)
                {
                    if((*lex)[cur].type != Tstring)
                    {
                        if((*lex)[cur].type != Tcchar)
                        {
                            ErrorText = "Ожидается сивольная константа";
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

            //если не выражение
            if(!Phrasing(Typ))
            {
                cur = c;
                ErrorText = "Ожидается выражение";
                error_err
                return false;
            }

            //если не )
            if((*lex)[cur].type != Tcbracket)
            {

                ErrorText = "Ожидается ')'";
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
        /// проверям на идентификатор
        if(!T->semSearch((*lex)[cur].image))
        {
            T->semError("Необъявленный идентификатор", &(*lex)[cur]);
        }
        else
        {
            lexem = (*lex)[cur].image;
            Typ = T->Cur->Find((*lex)[cur].image)->N->TypeObj;
        }
        ///-----------------------------------------------------------

        cur++;

        //если [
        if((*lex)[cur].type == Tsqbbracket)
        {
            cur++;
            c = cur;

            int t_m = -1;
            int val = cur;

            //если не выражение
            if(!Phrasing(t_m))
            {
                cur = c;
                ErrorText = "Ожидается выражение";
                error_err
                return false;
            }

            //если не ]
            if((*lex)[cur].type != Tsqcbracket)
            {
                ErrorText = "Ожидается ']'";
                error_err
                return false;
            }
            ///-----------------------------------------------------------
            /// проверям на наличие элемента
            else
            {
                if (t_m != TypeInt && !T->semMasEl(lexem, (*lex)[val].image.toInt()))
                    T->semError("Индекс элемента за границой массива.", &(*lex)[val]);
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
    //если не double и char
    if((*lex)[cur].type != Tdouble && (*lex)[cur].type != Tchar)
    {

        ErrorText = "Ожидается double или char";
        error_err
        return false;
    }

    ///-----------------------------------------------------------
    /// определим тип параметра
    Typ = T->semType(&(*lex)[cur]);
    ///-----------------------------------------------------------

    cur++;

    return true;
}
