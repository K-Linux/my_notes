<font size="7"><center>**SHELL**</center></font>

___

[TOC]

___

## <font color="1E90FF">一、基础知识</font>

1. shell是个弱类型的语言，变量默认都是字符串类型（所以加双引号和没加都表示字符串，建议加影号）
1. 单影号是强引用，不会去识别特殊符号，即单引号内是纯字符串；双引号是弱引用，会去识别特殊符号
1. 变量不能以数字开头
1. 变量赋值两边不能有空格（Makefile可以有）
1. 变量解引用要用大括号（Makefile要用小括号）
1. <font color="yellow"> `sh script.sh`会创建一个新的进程执行脚本，`source script.sh` 或者 `./script.sh` 会在当前进程执行脚本</font>
1. <font color="yellow"> #!/bin/bash 和 #!/bin/sh 的区别时 bash 功能丰富，而 sh 是简易版</font>
1. 反引号的内容会被识别为命令，如 cmd=\`ls m.c\`，则 ${cmd} 就是执行 ls m.c 命令
1. `/dev/null` 是一个特殊的文件，被称为 "空设备文件"。这个文件会丢弃所有写入到它的数据

> shell赋值不能有空格，大括号解引用（Makefile反之，赋值有空格，小括号解引用）

> shell执行命令为 \$(ls m.c) ，Makefile执行命令为 \$(shell ls m.c)。两者都可以用 \`ls m.c\`


## <font color="1E90FF">二、命令</font>

|   |<div style="width:229px">命令</div>|<div style="width:121px">解释</div>|
|---|:---|:---|
|**01**|history|查看历史命令行以及其ID|
|**02**|history -c|清空历史命令|
|**03**|!30|感叹号+历史命令ID，快速执行历史命令|
|**04**|ctrl + l|快速清屏|
|**05**|ctrl + a/e|光标跳至最前/后|
|**06**|ctrl + u/k|清空光标前/后的所有字符|
|**07**|ctrl + w|清空光标前一个字符串|
|**07**|ctrl + p|上一个命令|

### <font color="1E90FF">wc命令</font>

wc命令是‌Linux系统中用于统计文件中的行数、字数、字节数的工具

‌统计文件的行数‌：`wc -l filename`
‌统计文件的最长那一行的长度‌：`wc -L filename`
‌统计文件的字数‌：`wc -w filename`
‌统计文件的字节数‌：`wc -c filename`
‌统计文件的字符数‌：`wc -m filename`

### <font color="1E90FF">echo命令</font>

`echo` 命令可以带有参数
`-n` 不换行输出（默认末尾是会换行的）
`-e` 解析字符串中的特殊符号
`\t` 制表符
`\b` 退格

```bash
echo "linux\nchina"     #输出linux\nchina
echo -e "linux\nchina"  #输出linux `换行` china
echo -e "\033[5;33mdebuglog:contains\033[0m"
```

### <font color="1E90FF">exec命令</font>

`exec` （execute执行）命令会在当前进程（终端）执行命令，且执行完后自动 exit 退出进程（终端）
若在终端执行则退出终端，若在脚本文件中执行则退出脚本进程（sh会新建一个进程执行）

```bash
exec date #输出日期信息后退出
```

## <font color="1E90FF">三、环境变量和内置变量</font>

<font color="1E90FF">内置变量</font>：是在当前进程（终端）中定义的变量，只在当前进程（终端）有效。可以通过`set`命令查看内置变量，并且可以用`unset`命令删除

<font color="1E90FF">环境变量</font>：是在操作系统级别定义的变量，对所有子进程都可见。可以通过`env`命令查看环境变量，并且可以使用`export`命令将内部变量导出为环境变量。
环境变量指的是用export命令导出的变量，例如`ls`的执行程序路径是在`PATH`变量中可找到

`export name` 将name导出为环境变量，使其在所有子进程（终端）中可见
`set` 输出当前进程（终端）中的环境变量和内置变量（即全有变量）`declare`和`set`命令是一样的
`unset name` 删除当前进程（终端）中的 name 变量
`env` 输出当前进程（终端）中的环境变量

```shell
var=china   #设置内置变量
export var  #导出到环境变量（子进程可访问）
echo $var   #打印变量
env         #显示输出环境变量
set         #显示输出环境变量和内置变量
unset var param   #删除var、param变量
```

### <font color="1E90FF">环境变量读取顺序</font>

- 每个用户都有自己的环境变量配置文件 `~/.bash_profile` 或者 `~/.bashrc`
- 若需要给所有用户配置环境变量则需要将变量写在 `/etc/profile`
- 每次开启终端时才会加载一次环境变量

环境变量的读取顺序为

1. 先读取 `/etc/profile` 加载所有用户公共的环境变量
1. 然后读取 `~/.bash_profile` 加载当前用户的环境变量（若有相同则覆盖）
1. 再者读取 `~/.bashrc`（若有相同则覆盖）
1. 最后读取 `/etc/bashrc`（若有相同则覆盖）


## <font color="1E90FF">四、shell入参</font>

`$0` 获取脚本文件名
`$n` 获取脚本第n个参数
`$#` 获取脚本参数总数（`$#`的数值不包括脚本，`argc`的数值包括程序）
`$@` 和 `$*` 没有双引号时，都表示将每个入参视为独立的字符串，并以列表的形式输；有双引号时 `"$*"` 表示将所有shell入参输出为一行
`$?` 输出上一次shell命令行的返回值，或者返回脚本文件`exit`和`return`退出的值
`$$` 输出当前脚本的进程ID（PID）

```bash
#for var in $*
#for var in "$@"
# $@ == $* == "$@" 循环输出每个shell入参给var变量
for var in $@   #for var in "$@"    #for var in $*   
do
    echo $var
done
# "$*" 将所有shell入参合并为一个变量赋值给var变量
for var in "$*"
do 
    echo $var 
done
```

>`$@` == `$*` == `"$@"` 即循环输出每个参数
>`"$*"` 是将所有参数输出为一行，即一个变量

## <font color="1E90FF">五、变量</font>

|   |<div style="width:229px">命令</div>|<div style="width:121px">解释</div>|
|---|:---|:---|
|**00**|`var=china_linux`|定义变量`var`|
|**01**|`${var}`|返回变量的值|
|**02**|`${#var}`|返回变量长度|
|**03**|`${var:3}`|从变量的第3个元素开始返回后面的所有值|
|**04**|`${var:3:1}`|从变量的第3个元素开始返回后面的1个值|
|**05**|`${var#"linux"}`|若变量首字符是`linux`，则删除`linux`并输出|
|**06**|`${var%"linux"}`|若变量末尾字符是`linux`，则删除`linux`并输出|
|**07**|`${var/linux/china}`|使用`china`替换第一个匹配到的`linux`|
|**08**|`${var//linux/china}`|使用`china`替换所有的`linux`（注意两斜杆）|
|**09**|`${para:-${var}}`|如果变量`para`为空，则返回变量`var`的值，否则返回`para`(para变量不需要解引用)|
|**10**|`${para:=${var}}`|如果变量`para`为空，则返回变量`var`的值，且将`var`赋值给`para`，否则返回`para`|
|**11**|`${para:="empty error"}`|如果变量`para`为空，则返回 "empty error"，且退出shell程序|

### <font color="1E90FF">变量应用</font>

**<font size="4" color="1E90FF">批量修改文件名</font>**

```shell
#将所有后缀.jpg的文件中的linux改成china
for var in `ls *.jpg`
do 
    mv $var ${var//linux/china}
done
```

**<font size="4" color="1E90FF">查找修改时间在7天之前的文件并删除</font>**

```shell

find ${param:="/mnt/mtd/"} -name "*.log" -mtine +7 | xargs rm -f

```

## <font color="1E90FF">六、运算符</font>

### <font color="1E90FF">|| 和 &&</font>

得出结论后就终止执行
|| 若①为真则整体为真，不执行②；若①为假则整体未知，执行②
&& 若①为真则整体未知，执行②； 若①为假则整体为假，不执行②

```shell
[ 6 -eq 5 ] || echo "linux"
#打印 linux
```

### <font color="1E90FF">逻辑与、逻辑或</font>

-o 逻辑或
-a 逻辑与

```shell
if [ 5 -eq 6 -o "a" = "a" ]; then
    echo "yes"
else
    echo "no"
fi
#打印 yes
```

### <font color="1E90FF"> if 运算符</font>

```shell
if [ -d file.c ]; then  # 判断目录存在
if [ -f file.c ]; then  # 判断文件存在
if [ "li" = "ch" ]; then # 判断字符或字符串相等
if [ -z $var ]; then    # 判断变量为空,是则返回1
if [ 6 -eq 5 ]; then    # 判断数字相等 equal
if [ 6 -gt 5 ]; then    # 判断数字大于 greater than
if [ 6 -ge 5 ]; then    # 判断数字大于等于 greater or equal
if [ 6 -lt 5 ]; then    # 判断数字小于 less than
if [ 6 -le 5 ]; then    # 判断数字小于等于 less or equal
    #todo
fi
```

### <font color="1E90FF"> for/while 循环</font>

```shell
for i in 1 2 3; do
    echo $i
done

i=0
while [ $i -lt 3 ]; do
    echo $i
    i=$(($i+1))     # 即i++
done
```


## <font color="1E90FF">七、命令</font>

### <font color="1E90FF">mount</font>

```shell
1. 挂载路径必须存在
1. 挂载路径下原有文件将被隐藏
mount 要挂载的设备 挂载路径
mount /dev/sdb1  ~/share/
```

1. Linux 只有一个根目录/。根目录以外的文件要想被访问，需要将其挂载到根目录下的某个目录来实现，这个目录就是挂载点
1. 插入了U盘，分了U盘区sdb1。它现在还不属于根目录/。因为 sdb1 还没被挂载
1. /dev/sdb1 不是目录，是一个类似指针的东西，指向这个分区的原始数据块。mount前，系统并不知道这个数据块哪部分数据代表文件，不知道如何对它们操作
1. 当使用 `mount /dev/sdb1 ~/share/`，把U盘的区 sdb1 挂载到工作目录的 ~/share/ 文件夹下，之后访问这个 ~/share/ 文件夹就相当于访问这个U盘的 sdb1 分区了
1. 所以Linux下，mount 挂载的作用，就是将一个设备（通常是存储设备）挂接到一个已存在的目录上。访问这个目录就是访问该存储设备。

### <font color="1E90FF">echo</font>

```shell
echo "linux" > a.c # 删除a.c文件，再创建a.c并输入打印
echo "linux" >> a.c # 有a.c文件则接续输入，无a.c文件则创建该文件
```



