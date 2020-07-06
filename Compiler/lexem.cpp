#include "lexem.h"

Lexem::Lexem()
{

}

Lexem::Lexem(int t, QString i, int s, int p)
{
    image = i;
    type = t;
    str = s;
    pos = p;
}
