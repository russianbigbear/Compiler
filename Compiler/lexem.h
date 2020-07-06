#ifndef LEXEM_H
#define LEXEM_H
#include <QString>
#include <QStringBuilder>
#include <QList>

class Lexem
{
public:
    int type;
    QString image;
    Lexem();
    Lexem(int t, QString i, int s, int p);
    Lexem(int t, QList <char> i);
    int str;
    int pos;
};

#endif // LEXEM_H
