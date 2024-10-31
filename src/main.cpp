#include "Vector3.h"

int main()
{
    Vector3 v1(1, 2, 3);
    Vector3 v2(4, 5, 6);

    Vector3 v3 = v1 + v2;
    v3.print();

    return 0;
}