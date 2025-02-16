<font size="7"><center>**Makefile**</center></font>

___

[TOC]

___

## <font color="1E90FF">一、规则</font>


1. Makefile中以 [Tab] 字符开始的，都会作为命令行来处理
1. 一条规则由目标、先决条件和命令组成。target prerequisite command
1. 一个 Makefile 中可以定义多个目标。第一个目标all不会被生成文件，其它目标会被生成
1. 第一个目标all不会被创建，所以需要用命令创建第一个目标，否则第一个目标永远不存在，则永远会执行一遍第一个目标
1. 当没有给 make 命令指明具体的目标时，那么 make 将以 Makefile 中定义的第一个目标作为本次运行的目标
1. 目标和先决条件之间是依赖关系。在构建目标之前，必须保证先决条件被构建
1. 构建一个目标的顺序是，先构建先决条件（若没有则不构建），然后执行规则命令
1. 当一个目标有多个先决条件时，会从左到右顺序依次构建先决条件
1. 先决条件什么情况下会被构建？一个目标的多个先决条件中，哪个先决条件比目标的时间戳更新，就会被重新构建
1. 目标什么情况下会被构建？目标不存在或先决条件比目标时间戳更新时就会构建目标
1. 如果当前路径存在一个文件名称为`clean`，此时执行`make clean`，则会显示`xxx is up to date`。因为目标`clean`已经被构建。此时需要用`.PHONY:clean`声明假目标`clean`。假目标会脱离与文件的关联

```Makefile
目标:先决条件
    命令
%.o:%.c

########################### 为什么要用 %.o:%.c ###########################
OBJS1 := main.o a.o b.o
OBJS2 := c.o d.o
all: $(OBJS1) $(OBJS2)
# 无论 $(OBJS1) 和 $(OBJS2) 的值是什么，都在目标 %.o 的范围内
%.o:%.c

############ 为什么先决条件中要加入头文件；为什么要用 $< #########
# 添加头文件 $(INCLUDE) 是因为需要将先决条件和目标形成依赖关系，这样当改动头文件时目标才会被重新生成
# 必须要用 $< 是因为不希望gcc编译头文件为.o文件，否则会报错
%.o : %.c $(INCLUDE)
    gcc -c $< -o $@

#################### 怎么将 .o 文件生成在指定的目录中 #######################
DIR := objs                           # 生成目录名
OBJS := main.o a.o b.o
OBJS := $(addprefix $(DIR)/, $(OBJS)) # 给.o文件加上目录前缀
all: $(DIR) $(OBJS)
$(DIR)%.o:%.c                         # 生成带目录的目标
    #这里gcc生成的 .o 文件就会在 DIR 目录中
$(DIR):
    mkdir -p $@

#################### 怎么自动找到 .c 文件所包含的头文件 ##################
# gcc 添加 -M 参数会将源文件里面所包含依赖的头文件列出来
# -MM 表示仅列出用户定义的头文件，不显示系统头文件
gcc -MM main.c 或 gcc -M main.c

#################### %.o:%c 逻辑是什么 ##################
%.o:%.c
    gcc -c $^ -o $@
# 实际就是循环开启一个shell并执行 gcc 命令，如下
    gcc -c 1.c -o 1.o
    gcc -c 2.c -o 2.o
    gcc -c n.c -o n.o
# 每次都是在当前目录开启shell
    #嵌套创建b
    mkdir a; cd a; touch b
    #在当前目录创建b
    mkdir a
    cd a
    touch b




```

## <font color="1E90FF">二、变量</font>

1. make命令行中定义的变量会覆盖Makefile中同名的变量值。例 `make var=linux`
1. Makefile可以直接使用环境变量（使用`export`shell命令导出的变量即为环境变量）
1. 执行命令为`$(shell ls m.c)` 或 \`ls m.c\`
1. 变量解引用为`$(val)`或`${val}`

> shell赋值不能有空格，大括号解引用（Makefile反之，赋值有空格，小括号解引用）

> shell执行命令为 \$(ls m.c) ，Makefile执行命令为 \$(shell ls m.c)。两者都可以用 \`ls m.c\`

### <font color="1E90FF">自动变量</font>

1. 输出纯字符`$@`的方式为`\$$@`

|<div style="width:36px">命令</div>|<div style="width:121px">解释</div>|
|:---|:---|
|$@|所有目标|
|$^|所有先决条件|
|$<|第一个先决条件|

### <font color="1E90FF">标准变量</font>

|<div style="width:36px">命令</div>|<div style="width:121px">解释</div>|
|:---|:---|
|CURDIR|即 pwd 命令|
|MAKE|即 make 命令|
|MAKECMDGOALS|输入的目标|
|CC|编译器（例：make CC=arm-linux-gcc）|

### <font color="1E90FF">赋值号</font>

|<div style="width:36px">命令</div>|<div style="width:121px">解释</div>|
|:---|:---|
|:=|赋值上面最后的变量值|
|=|赋值下面最后的变量值|
|?=|若变量为空则赋值，否则不赋值|
|+=|接续赋值|
|$(var:.c=.o)|将var中的所有 .c 后缀改成 .o，然后返回（禁止空格）|



### <font color="1E90FF">通配符</font>

|<div style="width:36px">命令</div>|<div style="width:121px">解释</div>|
|:---|:---|
|%|通配符|
|*|通配符|
|*.c|通配所有 .c 文件|
|??.c|通配两个字符名的 xx.c 文件|
|[ab].c|通配 a.c 和 b.c|

### <font color="1E90FF">编译参数</font>

|<div style="width:36px">命令</div>|<div style="width:121px">解释</div>|
|:---|:---|
|-c|只编译不链接|
|-o|设置编译或链接生成的文件名|
|-nostdlib|不使用标准库（不用标准printf）|
|-nostdinc|不使用标准头文件|
|-I pathname|编译包含头文件|
|-wall|显示所有警告|
|-o2|二级编译优化|
|-C|切换目录|
|-MM|输出依赖文件|
|-E|预处理|

## <font color="1E90FF">三、函数</font>

### <font color="1E90FF">wildcard</font>

```Makefile
$(wildcard ./*.h) # 输出 ./xx.h ./xxx.h ./xxxx.h
#返回当前目录中的所有 .h 后缀文件（返回的每项输出都会带有 ./ 前缀）
```

### <font color="1e90ff">patsubst</font>

```makefile
# 参数1中通配符 % 匹配到什么，则下一个参数中 % 就表示什么 

var = a.c b.c
$(patsubst %.c, %.o, $(var)) # 输出 a.o b.o
# 将变量 var 中所有后缀为 .c 的文件替换为 .o 后缀

$(patsubst %, pre_%.o, $(var)) # 输出 pre_a.c.o pre_b.c.o
# 将变量 var 中所有文件添加前缀 pre_ 和 后缀 .o
```

### <font color="1E90FF">subst</font>

```Makefile
var = a.cc bcc.cc
$(subst cc, oo, $(var)) # 输出 a.oo 未知
#将变量 var 中的所有 cc 改成 oo，然后返回（注意会将所有cc改成oo）
```

### <font color="1E90FF">foreach</font>

```Makefile
var = a.c b.c
$(foreach tmp, $(var), $(tmp).o) # 输出 a.c.o b.c.o
#依次将变量 var 中的每个值赋值给变量 tmp，然后执行参数3所在的表达式并返回
```

### <font color="1E90FF">addprefix</font>

```Makefile
var = a.c b.c
$(addprefix ssl_, $(var)) # 输出 ssl_a.c ssl_b.c
#给变量 var 添加前缀 ssl_
```

### <font color="1E90FF">dir</font>

```Makefile
var = ./demo/m.c
$(dir $(var)) # 输出 ./demo/
#删除最后一个 / 后面的字符串并返回
```

### <font color="1e90ff">findstring</font>

```makefile
var = a.c b.c
find = a.c
$(findstring $(find), $(var)) # 输出 a.c
#若在变量 var 中找到变量 find，则返回变量 find，否则返回空
```

### <font color="1e90ff">filter 和 filter-out</font>

```makefile
var = a.c b.c a.h b.h
find = %.c

$(filter $(find), $(var)) # 输出 a.c b.c
#若在变量 var 中找到变量 find，则返回变量 find，否则返回空

$(filter-out $(find), $(var)) # 输出 a.h b.h
#返回变量 find 以外的字符串
```

### <font color="1e90ff">if</font>

```makefile
true = yes
false = no
var = linux
$(if $(var), $(true), $(false)) # 输出 yes
#若变量 var 为真，则返回参数2，否则返回参数3
```

### <font color="1e90ff">ifeq 和 ifneq</font>

```makefile
#ifeq 若相等则执行
#ifneq 若不相等则执行
var = linux
ifeq ($(var), linux)    # ifeq 后面必须空格
    out = linux
else ifeq ($(var), china)
    out = china
else
    out = other
endif
#若变量 var 的值和 linux 相等，则给变量 out 赋值 linux
```

### <font color="1e90ff">ifdef 和 ifndef</font>

```makefile
#ifdef 若真则执行
#ifndef 若假则执行
var = yes
ifdef var
    out = linux
else
    out = china
endif

#若变量 var 为真，则执行
```

> 注意区分if条件语句、ifeq比较语句、ifdef条件编译




## <font color="1E90FF">四、关键词</font>

1. `export` 导入到环境变量
1. `include` 包含 .mk 文件
1. `override` 该关键词修饰的变量不会被make命令参数覆盖


