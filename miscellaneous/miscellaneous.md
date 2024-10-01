
 <font size="7"><center>**临时记录**</center></font>
___

[TOC]

___

## <font color="1E90FF">int，short，char</font>

int、short、char都是数字类型，数字范围依次减少。但是char比较特殊，不但可以给char赋值-128~127的数字，也可以赋值单个字符。<font color="yellow">当给char赋值数字时，是给ASCII的十进制赋值数字，当给char赋值字符时，是给ASCII的字符赋值字符</font>。打印的时候不但可以用%c也可以用%d打印

```C
//char a = 55 和 char a = '7' 是等价的
//用%d打印a是55，用%c打印a是字符7
char a = 55;
char b = '7';
printf("%d\n", a)   // 55
printf("%c\n", a)   // 7

printf("%d\n", b)   // 55
printf("%c\n", b)   // 7
```

有符号字符 char 值的范围是： -128 ~ 127
无符号字符 unsigned char 值的范围是：0 ~ 255
ASCII码值的范围是：0 ~ 127

___

## <font color="1E90FF">float a = 0.0</font>

float a = 0.0;会存在一个双精度浮点数（C里默认浮点常量是double的）转换为单精度的过程，防止被编译器优化掉这个过程。后面加`f`更靠谱，float a = 0.0f
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

RS-232 电平需要用 DB9 接口

串口是一个泛称，UART，TTL，RS232，RS485都遵循类似的通信时序协议，因此都被通称为串口。

嵌入式里面说的串口，一般是指UART口，通常也叫COM口。UART有4个pin（VCC, GND, RX, TX）

UART 一般的接口电平有 TTL 和 RS-232，其中 TXD 和 RXD 引脚，低电平表示逻辑 0，高电平（3.3V或以上）表示逻辑 1，这个就是 TTL 电平（电平信号）。RS-232 采用差分线（差分信号），-3~-15V 表示逻辑 1，+3~+15V 表示逻辑 0

参考：<https://zhuanlan.zhihu.com/p/89240672>
___

## <font color="1E90FF">有源和无源蜂鸣器的区别</font>

有源蜂鸣器内部带震荡源，价格贵，发声频率固定
无源蜂鸣器内部不带震荡源，通过方波去驱动，发音频率可改变
___

## <font color="1E90FF">晶体振荡器</font>

晶体振荡器 OSC（oscillator）简称晶振，主要的特性就是通电后会产生机械震荡，可以给单片机提供稳定的时钟源，晶振提供时钟频率越高，单片机的运行速度也就越快

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
Ω 为测电阻。压下黄色按键，若电阻为 0 则万用表报警，说明电路是接通的
___

## <font color="1E90FF">推挽输出和开漏输出</font>

推挽输出（Push-Pull Output）：的特点是可以真正的输出高电平和低电平，且这两种电平都具有驱动能力。输出多少伏就是多少伏，不会减少

开漏输出（Open Drain Output）：开漏输出无法真正输出高电平，即高电平时没有驱动能力，需要借助外部上拉电阻完成对外驱动
___

## <font color="1E90FF">进制缩写</font>

O ：Octet， 八进制
B ：Binary， 二进制
H ：Hex， 十六进制
D ：Decimal， 十进制
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

## <font color="1E90FF">extern</font>

其它文件使用全局变量时，需要用`extern`关键字声明全局变量

全局变量一定要定义在`.c`文件中

B要使用A的全局变量时，就在B的头文件中用`extern`关键字声明该全局变量
或者在`a.c`定义全局变量`int a;`，在`a.h`中用`extern int a;`声明，然后其它文件用`include`包含`a.h`头文件

`extern`声明全局结构体时，要包含结构体的成员定义（声明全局结构体时extern可以省略）

```C
// 声明不能赋值
extern int a;
```

```C
// main.c 文件定义全局结构体
struct test;
// main.h 文件声明全局结构体
#ifndef _MAIN_H_
#define _MAIN_H_
struct test {
    int b;
};
#endif
// b.c 文件 extern 全局结构体
extern struct test;
// b.h文件
#ifndef _B_H_
#define _B_H_
TODO
#endif

```

___

## <font color="1E90FF">int 强制转化为 int *</font>

当`int`类型强制转化为`int *`时，`int *`的值和`int`是一样的，值一样，仅数据类型变了

```C
int a = 0x8000;
//相当于 (int *)0x8000
printf("%p\n", (int *)a);   //打印0x8000
```

___

## <font color="1E90FF">int的二进制位移</font>

```C
int a = 9;
register = a >> 15;  /* 设置寄存器的高16位 */
register = a << 15;  /* 设置寄存器的底16位 */
```

___

## <font color="1E90FF">中断的CPSR和SPSR寄存器</font>

在马上要跳转模式时，cpsr会将当前模式的数据保存给spsr，然后改变cpsr的M位，转到另一个模式。
例如：IRQ模式下将cpsr数据保存给IRQ模式下的spsr，然后改变cpsr的M位，转到SVC模式，处理完事件后又将cpsr数据保存给SVC模式下的spsr，然后改变cpsr的M位，转到IRQ模式，然后将IRQ模式下的spsr恢复给cpsr

>注意：cpsr寄存器只有唯一的一个，cpsr的M[4:0]位可以任意改变模式

___

## <font color="1E90FF">子网掩码</font>

IP地址中32位实际包含2部分，即IP地址 = 网络(段)地址 + 主机地址。子网掩码只有一个作用，就是将IP地址划分为网络地址和主机地址。子网掩码为1的部分表示网络地址，为0部分表示主机地址

如何判断2个IP地址是否在同一子网内 ？

1. 2个IP地址的网络地址必须相同（主机地址可以不同）那么就处于同一网络。
1. 网络标识 = IP地址 & 子网掩码。2个IP地址的网络标识一样，那么就处于同一网络。

如果子网掩码为255.255.0.0时，
192.168.1.102 & 255.255.255.0 = 192.168.1.0
192.168.1.253 & 255.255.255.0 = 192.168.1.0
以上两个地址处于同一网络/网段

192.168.1.4和192.168.12.5
如果子网掩码是255.255.255.0那么不在同一网段。如果子网掩码是255.255.0.0那么就在同一个网段

<font color="yellow">子网掩码为1的部分相同，为0的部分随意，则在同一网段</font>

___

## <font color="1E90FF">ubuntu</font>

### <font color="1E90FF">修改ubuntu主机名称</font>

主机名存放在/etc/hostname文件中，修改主机名后重启系统后即可

### <font color="1E90FF">Alt + F7，拖动窗口</font>

___

## <font color="1E90FF">wps切换: ctrl+TAB</font>

___

## <font color="1E90FF">markdonw笔记规范</font>

1. 关键词用<font color="yellow">黄色高亮</font>
1. 两个段落之间用回车
1. 变量用字符串 `val_string`
1. 4 个 # 时，要调整字体大小。以下为模板
`#### <font color="1E90FF" size="3">contains</font>`

___

## <font color="1E90FF">彩色打印</font>

[^彩色输出]

```C
printf("\033[41;30mdebuglog\033:[0m %s:%d [contains]\n", __FILE__, __LINE__);
```

```sh
# shell脚本
echo -e "\033[5;33mdebuglog:${contains}\033[0m"
```

___

## <font color="1E90FF">NFS</font>

NFS[^NFS]（Network File System）即网络文件系统，它允许网络中的计算机之间通过 TCP/IP 网络共享资源。在NFS的应用中，本地NFS的客户端应用可以透明地读写位于远端NFS服务器上的文件，就像访问本地文件一样。这样我们开发板在 uboot 启动时就可以访问远程 ubuntu 中的根文件系统了

我们先在 ubuntu 中搭建 NFS 服务器：

1. `sudo apt install nfs-kernel-server  //安装 NFS 服务端`
1. `sudo systemctl status nfs-server    //检查nfs-server是否已经启动`
1. `sudo mkdir -p /home/k/nfs           //在根目录下创建NFS共享目录(目录随意)`
1. `sudo chown nobody:nogroup /home/k/nfs//权限不分组`
1. `sudo chmod -R 777 /home/k/nfs       //给客户端分配最高权限访问该共享目录`
1. `sudo vim /etc/exports               //编辑 /etc/exports 配置文件`

```C
/home/k/nfs 172.16.3.50(rw,sync,no_subtree_check)//设置哪些客户端IP可以访问该共享目录(可设置多个IP)
/home/k/nfs 172.16.3.*(rw,sync,no_subtree_check)//设置一个网段的客户端可访问（推荐用这个）
/home/k/nfs *(rw,sync,no_subtree_check)         //设置所有客户端可访问该共享目录
//rw 表示允许读写
//sync 表示文件同时写入硬盘和内存
//no_subtree_check 表示即使输出目录是子目录，nfs服务器也不检查其父目录的权限，这样可以提高效率
```

7. `sudo vim /etc/default/nfs-kernel-server    //从Ubuntu17.04开始，nfs默认只支持协议3和4，kernel默认支持nfs协议2`

```C
RPCNFSDOPTS="--nfs-version 2,3,4 --debug --syslog"  //添加这句话让ubuntu支持NFS协议2、3、4版本
```

8. `sudo exportfs -rv  导出共享目录`
9. `sudo /etc/init.d/nfs-kernel-server restart //重启NFS服务器`
10. `showmount -e   //查看共享目录`

___

## <font color="1E90FF">samba</font>

1. `sudo apt update`[^samba]
1. `sudo apt install samba -y`
1. `mkdir -p samba //创建需要共享的目录`
1. `chmod 0777 samba`
1. `sudo vim /etc/samba/smb.conf`

```C
//在该文件末尾配置以下信息
[ubuntu_22]                 //共享目录名称，在网络上访问该共享目录时使用
    comment = samba         //共享目录的简介描述
    path = /home/k/samba    //共享实际路径
    public = yes            //这表示该共享是否为公共共享，即是否允许匿名用户访问。       
    writable = yes          //表示是否允许用户在共享中创建、编辑和删除文件。
    available = yes         //表示该共享是否可用。
    browseable = yes        //表示该共享是否在网络上可以浏览。
    valid users = k         //当前 Ubuntu 系统的用户名
```

6. `sudo smbpasswd -a k //给k用户设置密码`
1. `systemctl restart smbd.service // 重启 Samba 服务器`
1. `systemctl enable smbd.service //密码验证时都是输入系统用户k的密码`
1. `systemctl status smbd.service //查看samba服务器运行状态`
1. 给ubuntu配置成静态IP
1. `win+r` 并输入 ubuntu 地址 `\\192.168.10.20`
1. 输入 samba 服务器帐号和密码
1. 右键 samba 共享目录，点击映射网络驱动器
1. 直接点击确定即完成 samba 配置

___

## <font color="1E90FF">vscode排除搜索文件</font>

```json
//在工作区的顶层目录创建  .vscode\settings.json
{
    "editor.codeActionsOnSave": {
    },
    "search.exclude": {
        "**/node_modules": true,
        "**/*.code-search": true,
    },
    "files.exclude": {
        "**/.git": true,
        "**/.svn": true,
        "**/Thumbs.db": true,
    }
}
```

___










[^彩色输出]:[printf 彩色输出](https://www.cnblogs.com/lewki/p/14343894.html)
[^NFS]:[良许搭建NFS服务器](https://www.lxlinux.net/6086.html)
[^samba]:[Samba服务器搭建教程](https://blog.csdn.net/qq_42417071/article/details/136328807)