
 <font size="7"><center>**临时记录**</center></font>
___

[TOC]

___

## <font color="1E90FF">int,short,char</font>

int、short、char都是数字类型，数字范围依次减少。但是char比较特殊，不但可以给char赋值0~127的数字，也可以赋值单个字符。打印的时候不但可以用%c也可以用%d打印。例如：char a = 55 和 char a = '7' 是等价的

## <font color="1E90FF">float a = 0.0</font>

float a = 0.0;会存在一个双精度浮点数（C里默认浮点常量是double的）转换为单精度的过程，防止被编译器优化掉这个过程。后面加f更靠谱，float a = 0.0f;

## <font color="1E90FF">井号`#`表示带格式输出</font>

`#`表示带格式输出
int a = 0x168;
printf("%#x", a); //打印`0x168`
printf("%x", a);  //打印`168`

## <font color="1E90FF">使用函数获取堆内存方式</font>

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//传入指针时只能改变解引用
void getmemory_error(char *p) {
    p = (char *)malloc(100);
}
//传入二重指针能改变原指针的值
void getmemory_ok(char **p) {
    *p = (char *)malloc(100);
}
//一般用此方式返回malloc内存
//函数结束时会释放栈区的p，堆区的首地址则返回
char *getmemory_yes(void) {
    char *p = (char *)malloc(100);
    return p;
}

int main(int argc, char *argv[])
{
    char *str;

//一、错误方式获取堆内存
    //相当于char *p = str;即让p指向str
    //然后p又指向了malloc,此时str什么都没得到
    getmemory_error(str);

//二、二重指针获取堆内存
    //将指针str的地址传入，函数里面就可以解引用二重指针来修改指针str的值了
    getmemory_ok(&str);
    strcpy(str, "china");
    printf("%s\n", str);    //china
    //清除堆内存数据"china"，但是堆内存地址一直都是存在的，且str还是指向该地址
    free(str);
    //虽然执行了free(str)，但是str还指向该堆内存的首地址，且该地址数据清零了，所以可以重复使用
    //但已经free了，就千万别再使用该地址了
    strcpy(str, "linux");
    printf("%s\n", str);    //linux

//三、使用return返回堆内存
    //一般用此方式返回malloc内存
    str = getmemory_yes();
    strcpy(str, "bhlk");
    printf("%s\n", str);    //bhlk

    return 0;
}
```

## <font color="1E90FF">ubuntu</font>

### <font color="1E90FF">修改ubuntu主机名称</font>

主机名存放在/etc/hostname文件中，修改主机名后重启系统后即可

### <font color="1E90FF">Alt + F7，拖动窗口</font>



## <font color="1E90FF">wps切换: ctrl+TAB</font>









