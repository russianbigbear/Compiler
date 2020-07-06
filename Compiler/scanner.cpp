#include "scanner.h"

Scanner::Scanner()
{

}
Scanner::Scanner(QString fileName)      //получить текст из файла
{
    file = new QFile (fileName);
    file->open(QIODevice::ReadOnly);

    Types.insert(1, "Tvoid");
    Types.insert(2, "Tmain");
    Types.insert(3, "Twhile");
    Types.insert(4, "Tdouble");
    Types.insert(5, "Tchar");
    Types.insert(10, "Tid");
    Types.insert(20, "Tstring");
    Types.insert(21, "Tcchar");
    Types.insert(22, "Tint10");
    Types.insert(30, "Tcomma");
    Types.insert(31, "Tsemicolon");
    Types.insert(32, "Tobracket");
    Types.insert(33, "Tcbracket");
    Types.insert(34, "Tobrace");
    Types.insert(35, "Tcbrace");
    Types.insert(36, "Tsqbbracket");
    Types.insert(37, "Tsqcbracket");
    Types.insert(40, "Tplus");
    Types.insert(41, "Tminus");
    Types.insert(42, "Tmult");
    Types.insert(43, "Tdiv");
    Types.insert(44, "Tmod");
    Types.insert(45, "Tsave");
    Types.insert(46, "Tless");
    Types.insert(47, "Tmore");
    Types.insert(48, "Teq_less");
    Types.insert(49, "Teq_more");
    Types.insert(50, "Teq");
    Types.insert(51, "Tneq");
    Types.insert(100, "Tend");
    Types.insert(200, "Terr");

    KeyWords.insert(1, "void");
    KeyWords.insert(2, "main");
    KeyWords.insert(3, "while");
    KeyWords.insert(4, "double");
    KeyWords.insert(5, "char");

    Symbols.insert(30, ',');
    Symbols.insert(31, ';');
    Symbols.insert(32, '(');
    Symbols.insert(33, ')');
    Symbols.insert(34, '{');
    Symbols.insert(35, '}');
    Symbols.insert(36, '[');
    Symbols.insert(37, ']');

    Symbols.insert(40, '+');
    Symbols.insert(41, '-');
    Symbols.insert(42, '*');
    Symbols.insert(43, '/');
    Symbols.insert(44, '%');
    Symbols.insert(45, '=');
    Symbols.insert(46, '<');
    Symbols.insert(47, '>');

    IgnoreSymbols.insert(1, ' ');
    IgnoreSymbols.insert(2, '\n');
    IgnoreSymbols.insert(3, '\r');
    IgnoreSymbols.insert(4, '\t');

    text = new QString ();
    text->append(file->readAll());
    text->append('\0');
    file->close();
}

void Scanner::toScan (QList <Lexem> *lex)
{
    QTextStream cout (stdout);
    int uk = 0;
    int str = 1;
    int pos = 1;

    QString LexIm; //изображение лексемы

    //проходим весь текст
    while (uk < text->length())
    {
        lexLenth = 0;
        LexIm.clear();

        //пропускаем незн. символы
        while (IgnoreSymbols.key((*text)[uk]) > 0)
        {
            if ((*text)[uk++]=='\n')
            {
                str++;
                pos = 0;
            }
        }

        //конец модуля
        if ((*text)[uk]=='\0')
        {
            lex->append(*new Lexem (Tend, "", str, pos));
            return;
        }

        //если встречается слеш, смотрим коментарий ли
        else if ((*text)[uk] == '/')
        {
            //второй слеш - значит комментарий
            if ((*text)[uk+1] == '/')
            {
                pos += 2;
                uk += 2;

                //пропускаем все символы, кроме конца строки и модуля
                while ((*text)[uk] != '\n' && (*text)[uk] != '\0')
                {
                    uk++;
                    pos++;
                }
            }
            //если не комментарий переходим дальше
            else
            {
                goto next;
            }
        }

        //если встретили цифру 0, то проверям единственная или нет
        else
next:
            if ((*text)[uk] == '0')
            {
                LexIm.append(((*text)[uk]));
                uk++;
                pos++;
                lexLenth++;

                if(Symbols.key((*text)[uk]) > 0)
                {
                    //x10 число равное 0
                    lex ->append (*new Lexem (Tint10, LexIm, str, pos));
                }
                else
                    goto errors;

            }

        //если число не с 0, то int10
            else if ((*text)[uk].isDigit() && (*text)[uk] != '0')
            {
                while ((*text)[uk].isDigit())
                {
                    if (lexLenth <= MaxLenth)
                        LexIm.append((*text)[uk++]);
                    else
                        goto errors;

                    lexLenth++;
                    pos++;
                }

                //x10 число
                lex->append (*new Lexem (Tint10, LexIm, str, pos));
            }
        //если буква
            else if ((*text)[uk].isLetter() || (*text)[uk] == '_')
            {
                //считываем буквы и цифры
                while ((*text)[uk].isLetter() || (*text)[uk].isDigit() || (*text)[uk] == '_')
                {
                    //ограничение вдвое больше чем для чисел
                    if (lexLenth <= MaxLenth * 2)
                        LexIm.append((*text)[uk++]);
                    else
                        goto errors;
                    pos++;
                    lexLenth++;
                }
                //получаем ключ ключевого слова
                int typeKeyWord = KeyWords.key(LexIm);

                if (typeKeyWord > 0)
                {
                    //некоторое ключевое слово
                    lex->append (*new Lexem (typeKeyWord, LexIm, str, pos));
                }
                else
                    //идентификатор
                    lex->append (*new Lexem (Tid, LexIm, str, pos));
            }

        //проверяем на char
            else if ((*text)[uk] == '\'')
            {
                LexIm.append((*text)[uk++]);
                LexIm.append((*text)[uk++]);

                pos +=2;
                LexIm.append((*text)[uk++]);

                //если тоже кавычка, то char
                if ((*text)[uk-1] == '\'')
                {
                    lex->append (*new Lexem  (Tcchar, LexIm, str, pos));
                }
                //ошибка
                else
                {
                    goto errors;
                }

            }
        //проверяем на string
            else if ((*text)[uk] == '\"')
            {
                LexIm.append((*text)[uk++]);
                LexIm.append((*text)[uk++]);
                pos +=2;

                while((*text)[uk] != '\"')
                {
                    if((*text)[uk] != '\n')
                    {
                        LexIm.append((*text)[uk++]);
                        pos++;
                    }
                    else
                    {
                        goto errors;
                    }
                }
                LexIm.append((*text)[uk++]);

                lex->append (*new Lexem  (Tstring, LexIm, str, pos));
                pos++;

            }
        //знак операции <
            else if ((*text)[uk] == '<')
            {
                LexIm.append((*text)[uk++]);
                pos++;

                if ((*text)[uk] == '=')
                {
                    LexIm.append((*text)[uk++]);
                    pos++;
                    lex->append (*new Lexem  (Teq_less, LexIm, str, pos));
                }

                else
                {
                    lex->append (*new Lexem  (Tless, LexIm, str, pos));
                }
            }
        //знак операции >
            else if ((*text)[uk] == '>')
            {
                LexIm.append((*text)[uk++]);
                pos++;

                if ((*text)[uk] == '=')
                {
                    LexIm.append((*text)[uk++]);
                    pos++;
                    lex->append (*new Lexem  (Teq_more, LexIm, str, pos));
                }

                else
                {
                    lex->append (*new Lexem  (Tmore, LexIm, str, pos));
                }
            }
        //знак операции =
            else if ((*text)[uk] == '=')
            {
                LexIm.append((*text)[uk++]);
                pos++;

                if ((*text)[uk] == '=')
                {
                    LexIm.append((*text)[uk++]);
                    pos++;
                    lex->append (*new Lexem  (Teq, LexIm, str, pos));
                }

                else
                {
                    lex->append (*new Lexem  (Tsave , LexIm, str, pos));
                }
            }
        //знак операции !
            else if ((*text)[uk] == '!')
            {
                LexIm.append((*text)[uk++]);
                pos++;

                if ((*text)[uk] == '=')
                {
                    LexIm.append((*text)[uk++]);
                    pos++;
                    lex->append (*new Lexem  (Tneq , LexIm, str, pos));
                }

                else
                {
                    goto errors;
                }
            }
        //знак операции
            else if (Symbols.key((*text)[uk]) > 0)
            {
                LexIm = (*text)[uk];
                //знак и его номер
                lex->append (*new Lexem  (Symbols.key((*text)[uk++]), LexIm, str, pos));
                pos++;
            }
        //ошибка
            else
            {
                goto errors;
            }
    } // конец Scaner

    goto N1;

errors:
    //очередная ошибка
    Errors.append(new int [2]{str, pos});
    //пока не попался игнорируемый символ
    while ( !(IgnoreSymbols.key((*text)[uk]) > 0) && (*text)[uk] != '\0')
    {
        LexIm.append((*text)[uk++]);
        pos++;
    }
    lex->append (*new Lexem  (Terr, LexIm, str, pos));

N1:
   lex->append (*new Lexem  (Tend, "", str, pos));
}



