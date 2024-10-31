#include "Vector3.h"

int main()
{
    Vector3 v1(1, 2, 3);
    Vector3 v2(4, 5, 6);

    Vector3 v3 = v1 + v2;
    v3.print();

    Vector3 v4 = v1.cross(v2);
    v4.print();

    Vector3 v5 = v1.normalise();
    v5.print();

    return 0;
}