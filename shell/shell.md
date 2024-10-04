<font size="7"><center>**SHELL**</center></font>

___

[TOC]

___

## <font color="1E90FF">一、基础知识</font>

1. shell是个弱类型的语言，变量默认都是字符串类型（所以加双引号和没加都表示字符串，建议加影号）
1. 变量不能以数字开头
1. 变量赋值不能有空格（Makefile可以有）
1. 变量解引用要用大括号（Makefile要用小括号）
1. 单影号不能识别解引用，即单引号内是纯字符串
1. `/bin/bash` 会创建一个新的进程执行脚本，`source shell.sh` 或者 `./shell.sh` 会在当前进程执行脚本
1. 反引号的内容会被识别为命令，如 cmd=`pwd`，则 ${cmd} 就是执行 pwd 命令

> shell和Makefile变量赋值都不用空格，shell用大括号解引用，Makefile用小括号解引用


## <font color="1E90FF">二、命令</font>

|   |<div style="width:229px">命令</div>|<div style="width:521px">解释</div>|
|---|:---|:---|
|**01**|history|查看历史命令|
|**02**|history -c|清空历史命令|
|**03**|!30|感叹号+历史命令ID，快速执行历史命令|
|**04**|ctrl + l|快速清屏|
|**05**|ctrl + a/e|光标跳至最前/后|
|**06**|ctrl + u/k|清空光标前/后的所有字符|
|**07**|ctrl + w|清空光标前一个字符串|


## <font color="1E90FF">三、环境变量</font>

每个用户都有自己的环境变量配置文件 `~/.bash_profile` 或者 `~/.bashrc`

若需要给所有用户配置环境变量则需要将变量写在 `/etc/profile` 

每次开启终端时才会加载一次环境变量

`export` 命令用于将变量导出为全局变量，使其在当前shell会话中可见，并且可以在其他子进程中访问
`set` 输出当前shell会话中的所有变量（包括全局和局部变量）
`env` 输出当前shell会话中的全局变量

### <font color="1E90FF">环境变量读取顺序</font>

1. 先读取 `/etc/profile` 加载所有用户公共的环境变量
1. 然后读取 `~/.bash_profile` 加载当前用户的环境变量（若有相同则覆盖）
1. 再者读取 `~/.bashrc`
1. 最后读取 `/etc/bashrc`
