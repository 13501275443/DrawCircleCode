#include "stdafx.h"
#include <windows.h>
#include "DrawCircle.h"
int _tmain(int argc, _TCHAR* argv[])
{

    CDrawCircle cDrawCircle(10, 30);
    cDrawCircle.DrawCircle();
    
    getchar();
    return 0;
}