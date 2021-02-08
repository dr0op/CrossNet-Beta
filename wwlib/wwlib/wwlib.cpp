// wwlib.cpp : 定义 DLL 的导出函数。
//

#include "pch.h"
#include "framework.h"
#include "wwlib.h"


// 这是导出变量的一个示例
WWLIB_API int nwwlib=0;

// 这是导出函数的一个示例。
WWLIB_API int fnwwlib(void)
{
    return 0;
}

// 这是已导出类的构造函数。
Cwwlib::Cwwlib()
{
    return;
}
