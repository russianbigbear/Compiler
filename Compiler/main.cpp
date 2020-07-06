#include <QCoreApplication>
#include <lexem.h>
#include <scanner.h>
#include <analizator.h>
#include <QList>
#include <iostream>
#include <QTextStream>
#include <stdio.h>
#include <locale.h>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL, "rus");
    Analizator analizator;
    analizator.Programm();
    system("pause");
    return 0;
}
