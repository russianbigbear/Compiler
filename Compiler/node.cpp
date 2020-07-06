#include "node.h"

Node::Node(QString I, int T, int PC, int DC, int LB)
{
    Id = I;
    TypeObj = T;
    ParamCount = PC;
    DimensionCount = DC;
    LowerBoundMas = LB;
}
