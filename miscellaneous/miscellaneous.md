
 <font size="7"><center>**临时记录**</center></font>
___

[TOC]

___

## <font color="1E90FF">int,short,char</font>

int、short、char都是数字类型，数字范围依次减少。但是char比较特殊，不但可以给char赋值0~127的数字，也可以赋值单个字符。打印的时候不但可以用%c也可以用%d打印。例如：char a = 55 和 char a = '7' 是等价的
___

## <font color="1E90FF">float a = 0.0</font>

float a = 0.0;会存在一个双精度浮点数（C里默认浮点常量是double的）转换为单精度的过程，防止被编译器优化掉这个过程。后面加f更靠谱，float a = 0.0f;
___

## <font color="1E90FF">井号`#`表示带格式输出</font>

`#`表示带格式输出
int a = 0x168;
printf("%#x", a); //打印`0x168`
printf("%x", a);  //打印`168`
___

## <font color="1E90FF">映射网络驱动器</font>

<div align=center><img src="img/2023-06-26-22-21-02.png" width="100%"></div>
___

## <font color="1E90FF">OTG</font>
OTG即On-The-Go的英文缩写，是一项用于USB设备连接和数据交换的技术。凡具备OTG功能的设备，通过OTG接口转换器就可以连接并控制常规的USB设备，比如U盘、鼠标、键盘、打印机等。总之OTG功能就是用来连接USB设备的。
___

## <font color="1E90FF">串口、COM口、UART口, TTL、RS232、RS485区别详解</font>

串口、UART口、COM口、USB口是指的物理接口形式(硬件)。而TTL、RS232、RS485指的是电平标准(电信号)

串口是一个泛称，UART，TTL，RS232，RS485都遵循类似的通信时序协议，因此都被通称为串口。

嵌入式里面说的串口，一般是指UART口。UART有4个pin（VCC, GND, RX, TX）, 用的TTL电平, 低电平为0(0V)，高电平为1（3.3V或以上）

参考：<https://zhuanlan.zhihu.com/p/89240672>
___

## <font color="1E90FF">有源和无源蜂鸣器的区别</font>

有源蜂鸣器内部带震荡源，价格贵，发声频率固定。无源蜂鸣器内部不带震荡源，通过方波去驱动，发音频率可改变
___

## <font color="1E90FF">晶体振荡器</font>

晶振全名叫晶体振荡器，主要的特性就是通电后会产生机械震荡，可以给单片机提供稳定的时钟源，晶振提供时钟频率越高，单片机的运行速度也就越快

<font color="yellow">无源晶振</font>是有2个引脚的无极性元件，需要借助于时钟电路通电才能产生振荡信号，自身无法振荡起来

<font color="yellow">有源晶振</font>有4个引脚，是一个完整的振荡器，只需要电源，就可输出比较好的波形
___

## <font color="1E90FF">RGB屏和MCU屏</font>

MCU-LCD屏它与RGB-LCD屏主要区别在于显存的位置：
RGB-LCD的显存是由主机系统内存充当的,因此其大小只受限于系统内存的大小,这样RGB-LCD可以做出较大尺寸,象现在4.3"只能算入门级,而MID中7",10"的屏都开始大量使用

MCU-LCD的设计之初只要考虑单片机的内存较小,因此都是把显存内置在LCD模块内部.然后软件通过专门显示命令来更新显存,因此MCU屏往往不能做得很大.同时显示更新速度也比RGB-LCD慢.

<div align=center><img src="img/2023-06-30-12-40-29.png" width="50%"></div>

<div align=center><img src="img/2023-06-30-12-39-54.png" width="50%"></div>

MCU-LCD屏它与RGB-LCD屏量示数据传输模式也有差别：
        RGB屏只需显存组织好数据。启动显示后。LCD-DMA会自动把显存通过RGB接口送到LCM。
        MCU屏则需要发送画点的命令来修改MCU内部RAM。（即不能直接MCU屏RAM）所以RGB显示速度明显比MCU快,而且播放视频方面,MCU-LCD也比较慢.



参考：<https://blog.csdn.net/vrk731/article/details/85221189>
___

## <font color="1E90FF">上/下拉电阻</font>

上拉电阻就是接一个电阻后再接到VCC上，从而将a点拉成高电平。若按键开启，则a点为低电平。这样就可以确定a点的电平高低了

电路中加上拉电阻或下拉电阻的目的是确定某个状态电路中的高电平或低电平。简单来说，电源到器件引脚上的电阻叫上拉电阻，<font color="yellow">作用是平时使用该引脚为高电平</font>；地到器件引脚的电阻叫下拉电阻，<font color="yellow">作用是平时使该引脚为低电平</font>

不接上/下拉电阻，则称为悬空

<div align=center><img src="img/2023-06-30-21-37-37.png" width="30%"></div>

___

## <font color="1E90FF">三极管</font>

<div align=center><img src="img/2023-06-30-21-45-19.png" width="40%"></div>

NPN是基极电压大于(击穿)发射极时流通
PNP是发射极电压大于(击穿)基极时流通
基极 + 集电极 = 发射极
P：positive 正极
N：negative 负极

<div align=center><img src="img/2023-06-30-21-46-18.png" width="60%"></div>

___

## <font color="1E90FF">万用表</font>

V 为测电压
Ω 为测电阻。压下换色按键，若电阻为 0 则万用表报警，说明电路是接通的

___

## <font color="1E90FF">推挽输出和开漏输出</font>

推挽输出（Push-Pull Output）：的特点是可以真正的输出高电平和低电平，且这两种电平都具有驱动能力。输出多少伏就是多少伏，不会减少

开漏输出（Open Drain Output）：开漏输出无法真正输出高电平，即高电平时没有驱动能力，需要借助外部上拉电阻完成对外驱动

___

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

___

## <font color="1E90FF">ubuntu</font>

### <font color="1E90FF">修改ubuntu主机名称</font>

主机名存放在/etc/hostname文件中，修改主机名后重启系统后即可
___

### <font color="1E90FF">Alt + F7，拖动窗口</font>

## <font color="1E90FF">wps切换: ctrl+TAB</font>







