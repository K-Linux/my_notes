
 <font size="7"><center>**C++**</center></font>

___

[TOC]

___

## 一、基础语法 {#基础语法}

#### 类class

1. 关键字`class`类是一种构造类型，其成员可以是变量或者函数；函数可以在类里面定义，也可以在类外面定义；通过类定义出来的变量叫作对象；创建对象的过程叫作实例化
1. `::`表示作用域运算符或解引用；`:` 使用在类的继承，和构造函数初始化
1. 没用关键字修饰的变量，默认是`private`属性的
1. 用关键字`public` `protected` `private`修饰的函数和变量在自身类里可以相互调用，无区别，对于派生类和对象则有区别
1. 在类里(或者在同一作用域里)函数名相同，但是形参不同的函数叫做<font color="yellow">重载函数</font>。对象调用重载函数时，会根据形参的不同来判断调用的是哪个重载函数；析构函数不可被重载，构造函数可以被重载

#### new分配堆内存

1. `int *a = new int(6);`其中`new int(6)`会返回6的地址。`delete a;`释放。指针a的地址在栈区，6的地址在堆区
1. `int *arr = new int[6];` `new int[6]`会返回数组首地址。`delete[] arr;`释放。
1. `char *str = new char[10];` `new char[10]`会返回数组首地址。`delete[] str;`释放数组。
>注：释放数组要加 [ ]

#### 引用&

1. `int &b = a;`这时进行`b = 10`操作，就相当于`a = 10`操作。`int &b=a`相当于`int *const b = &a`
1. 引用在定义时必须初始化，且初始化后不可更改指向，但可以赋值(因为 * const 修饰禁止修改指针)
1. 函数`void swap(&a，&b)`，调用时写法为`swap(c，d)`，此时`a`和`b`修改值会反馈给`c`和`d`

#### 函数默认形参

1. 函数形参可以设置默认值，例如`void func(int a, int b=10)`，调用时为`func(30)`或`func(30，50)`（覆盖）
1. 某个形参设置默认值后，该形参后面的形参都必须要有默认值（默认形参是从右边开始初始化的）
1. 函数声明中的形参和函数定义中的形参，只能其中一个写默认形参（防止声明和定义的默认形参不相同）

#### 函数占位形参

1. 例如void func(int a，int)；调用时为func(30，1)；
1. 调用函数时，占位形参和普通形参一样，必须填写
1. 默认形参就是占位形参(当函数内部没有用到默认形参时)

  
### 1.1 构造函数和析构函数 {#1.1}

1. 构造函数和~析构函数是名称与类名相同，且没有返回值的函数。在实例化对象时若没有定义构造函数和析构函数，则会默认定义一个空的构造函数和析构函数
1. 构造函数有形参，可以被重载；析构函数无形参，不可被重载
1. 构造函数：实例化对象的一瞬间就会自动依次先后执行基类和派生类的构造函数
1. 析构函数：当对象生命周期结束时就会自动依次先后执行派生类和基类的析构函数
1. 定义对象同时传参给构造函数。①Dog dog1(4)；②Dog *dog2 = new Dog(5.5)
1. 定义对象后再传参给构造重载函数。①dog1.dog1_func(4)；②dog2->dog2_func(5.5)
1. 构造函数可以初始化类的成员变量，即<font color="yellow">初始化列表</font>。注意初始化与赋值不同，这意味着const修饰的成员也可以初始化

<div align=center><img src="img/2023-05-03-22-57-34.png" width="50%"></div>

8. 实例化对象时会先调用基类的构造函数，再调用派生类的构造函数；结束对象时先析构派生类再析构基类

### 1.2 拷贝构造函数 {#1.2}

1. 构造函数中，若形参为本类引用，则为拷贝构造函数
1. 编译器会自动给一个类添加无参构造函数、有参构造函数、拷贝构造函数。当手动添加拷贝构造函数时，编译器不再添加默认无参和有参构造函数。当手动添加有参构造函数时，编译器不再添加默认无参构造函数
1. 编译器自动添加的拷贝构造函数会复制形参给自己
1. 拷贝构造函数中，使用赋值号直接将形参(类)的指针成员复制，叫作浅拷贝（注：默认拷贝构造函数就是这样）
1. 拷贝构造函数中，在堆区new空间给形参(类)的指针成员，叫作深拷贝(需要手动new)

<div align=center><img src="img/2023-05-03-18-32-49.png" width="50%"></div>
.
<div align=center><img src="img/2023-05-03-18-33-32.png" width="50%"></div>

### 1.3 继承 {#1.3}

#### 单继承

1. 对象只能访问`public`成员
1. `public`继承时，派生类的成员只能访问基类中的`public`和`protected`，派生类的对象只能访问基类中的`public`成员
1. `protected`继承时，派生类的成员只能访问基类中`public`和`protected`，派生类的对象不能访问基类中的任何成员
1. `private`继承时，派生类的成员只能访问基类中的`public`和`protected`，派生类的对象不能访问基类中的任何成员
1. 
    - `protected`继承时，基类中`public`和`protected`继承到派生类变为`protected`
    - `private`继承时，基类中`public`和`protected`继承到派生类变为`private`(如下图)
    - 任何继承都无法访问基类的`private`成员
    - 所以只有`public`继承时派生类的对象才能访问基类的`public`成员

<div align=center><img src="img/2023-05-03-22-32-44.png"></div>

6. 派生类以任何方式继承基类的同时，也会继承基类的全部内存大小（注：静态成员不会增加类的内存。函数以及函数内部的局部变量也不会增加类的内存，因为是运行时分配）

<div align=center><img src="img/2023-05-05-22-00-57.png"></div>

7. 实例化对象时会先调用<font color="yellow">基类</font>的构造函数，再调用<font color="yellow">派生类</font>的构造函数；结束对象时先析构派生类再析构基类
8. 若派生类成员与基类成员同名，派生类优先级高。会屏蔽基类<font color="yellow">所有同名成员</font>(包括所有同名重载函数和同名静态成员)。只能使用作用域符号特指才能调用基类成员
9. 当静态成员同名时，既可以通过对象访问基类的静态成员，也可以通过类名访问基类的静态成员。使用类名访问可以不用实例化对象

<div align=center><img src="img/2023-05-08-20-03-58.png" width="60%"></div>
<div align=center><img src="img/2023-05-08-20-04-38.png" width="60%"></div>

#### 多继承

- 不推荐使用多继承

<div align=center><img src="img/2023-05-07-18-51-59.png" width="60%"></div>

#### 菱形继承

- 当`D`多继承了`B`和`C`，且`B`和`C`都继承了`A`，这时就形成了菱形继承

<div align=center><img src="img/2023-05-07-19-07-35.png" width="50%"></div>

#### 虚继承 virtual

在菱形继承中，`A`是相同的一份，`D`可以通过`B`或者`C`来反问`A`，没有一个标准。虚继承则可以使`B` `C` `D`指向相同的一份`A`，则`A`叫做虚基类

1. 虚继承时，`Base1`内自动创建一个指向`Base`的指针
2. 虚继承时，`Base1`的大小为1个指向`Base`的指针 + `Base`的大小
3. 实继承时`Base1`的大小为`Base`的大小
4. 此时`Derive`的大小为2个指向`Base`的指针 + `Base`的大小

<div align=center><img src="img/2023-05-07-20-36-03.png" width="60%"></div>

### 1.4 向上转型 {#向上转型}


类是一种数据类型，也可以发生数据类型转换，不过这种转换只有在基类和派生类之间才有意义，并且只能将派生类赋值给基类，包括将派生类对象赋值给基类对象、将派生类指针赋值给基类指针、将派生类引用赋值给基类引用，这在 C++ 中称为向上转型（Upcasting）。相应地，将基类赋值给派生类称为向下转型（Downcasting）。

##### 将派生类对象赋值给基类对象 w

将派生类对象`B`赋值给基类对象`A`时，只会将派生类对象`B`中的基类的成员变量赋值给基类对象`A`。且只能用派生类对象给基类对象赋值，而不能用基类对象给派生类对象赋值。

赋值的本质是将现有的数据写入已分配好的内存中，对象的内存只包含了成员变量，不包含成员函数，所以对象之间的赋值仅仅是成员变量的赋值。虽然有 `基类 = 派生类` 这样的赋值过程，但是<font color="yellow">基类调用的始终是基类的自身的函数</font>

<div align=center><img src="img/2023-05-13-12-39-53.png" width="70%"></div>
.
<div align=center><img src="img/2023-05-15-12-31-38.png" width="50%"></div>

##### 将派生类指针赋值给基类指针

将派生类指针`B`赋值给基类指针`A`时，此时基类指针`A`指向派生类`B`内的基类地址（实际是A == B）。相当于限制派生类只能调用基类的成员（注：只有使用虚函数时（即[多态](#多态)），基类才可以调用派生类的成员函数）

<div align=center><img src="img/2023-05-15-22-42-18.png" width="50%"></div>

### 1.5 多态 {#多态}

封装、继承和多态是C++面向对象三大特征。多态分为两类:

- 静态多态：函数重载和运算符重载属于静态多态，复用函数名
- 动态多态：派生类和虚函数实现运行时多态

静态多态的函数地址在编译阶段确定，动态多态的函数地址在运行阶段确定

#### 动态多态满足条件为

1. <font color="yellow">派生类要重写基类的虚函数</font>（注：重写即函数名和形参都要一样）
2. <font color="yellow">基类的指针或引用执行派生类的虚函数</font>

>注：在函数前加`virtual`即为虚函数。例如：virtual void func()
>构造函数不能是虚函数，因为派生类不能继承基类的构造函数，将构造函数声明为虚函数没有意义

#### 动态多态的原理

当基类创建虚函数的同时会创建一个占4字节的指针`vfptr`，指向虚函数表`vftable`，该虚函数表内记录着基类的虚函数地址`&Animal::speak`。当派生类继承基类时，指针`vfptr`和虚函数表`vftable`都会继承过来。当派生类重写虚函数时，虚函数表内由原来记录基类的虚函数地址`&Animal::speak`变为记录派生类的虚函数地址`&Cat::speak`
所以向上转型后，当基类调用虚函数时，因为虚函数表记录着派生类的虚函数地址，所以总是会调用派生类的虚函数
> 注：当实例化一个对象时，有一个派生类，派生类里面包含着一个基类

<div align=center><img src="img/2023-05-09-22-22-50.png" width="50%"></div>

#### 使用多态创建计算器案例

<div align=center><img src="img/2023-05-11-22-01-57.png" width="50%"></div>

上面的代码中，当`t1`指向不同的派生类对象`new Add;`或`new Mul;`时，`t1->get_result();`执行的结果是不一样的。<font color="yellow">同一条语句可以执行不同的操作，这就是多态</font>
C++提供多态的目的是通过基类指针对所有派生类（包括直接派生和间接派生）的成员变量和成员函数进行全方位的访问，尤其是成员函数。如果没有多态，我们只能通过[向上转型](#向上转型)访问派生类的成员变量

#### 纯虚函数和抽象类

一般我们不实例化基类对象，所以将基类变为抽象类，抽象类不可实例化对象（但可以实例化指针）。只要类中有了纯虚函数，那么这个类就是抽象类。抽象类的作用是防止我们实例化基类，以及提醒我们重写派生类的纯虚函数

1. <font color="yellow">在虚函数后面加`= 0`即为纯虚函数；当类中有了纯虚函数，这个类就被称为抽象类</font>
2. 抽象类无法实例化对象（Base t1；错误  ——  Base *t1；正确）
3. 派生类必须重写抽象类（基类）的纯虚函数，否则派生类也是纯虚函数，无法实例化对象

<div align=center><img src="img/2023-05-16-21-24-44.png" width="45%"></div>
.
<div align=center><img src="img/2023-05-16-21-44-24.png" width="45%"></div>

```C++
#include <iostream>
using namespace std;
//线
class Line{
public:
    Line(float len);
    virtual float area() = 0;
    virtual float volume() = 0;
protected:
    float m_len;
};
Line::Line(float len): m_len(len){ }
//矩形（未重写纯虚函数volume()，所以Rec也是抽象类）
class Rec: public Line{
public:
    Rec(float len, float width);
    float area();
protected:
    float m_width;
};
Rec::Rec(float len, float width): Line(len), m_width(width){ }
float Rec::area(){ return m_len * m_width; }
//长方体
class Cuboid: public Rec{
public:
    Cuboid(float len, float width, float height);
    float area();
    float volume();
protected:
    float m_height;
};
Cuboid::Cuboid(float len, float width, float height): Rec(len, width), m_height(height){ }
float Cuboid::area(){ return 2 * ( m_len*m_width + m_len*m_height + m_width*m_height); }
float Cuboid::volume(){ return m_len * m_width * m_height; }
//正方体
class Cube: public Cuboid{
public:
    Cube(float len);
    float area();
    float volume();
};
Cube::Cube(float len): Cuboid(len, len, len){ }
float Cube::area(){ return 6 * m_len * m_len; }
float Cube::volume(){ return m_len * m_len * m_len; }
int main(){
    Line *p = new Cuboid(10, 20, 30);
    cout<<"The area of Cuboid is "<<p->area()<<endl;
    cout<<"The volume of Cuboid is "<<p->volume()<<endl;
  
    p = new Cube(15);
    cout<<"The area of Cube is "<<p->area()<<endl;
    cout<<"The volume of Cube is "<<p->volume()<<endl;
    return 0;
}
```

- `Line`是一个抽象类，也是最底层的基类，在`Line`类中定义了两个纯虚函数 area() 和 volume()
- 在`Rec`类中，实现了`area()`的函数体。但没有实现继承来的 `volume()`的函数体，所以`Rec`也是抽象类，不能被实例化
- 在实际开发中，你可以定义一个抽象基类，只完成部分功能，未完成的功能交给派生类去实现（谁派生谁实现）。这部分未完成的功能，往往是基类不需要的，或者在基类中无法实现的。虽然抽象基类没有完成，但是却强制要求派生类完成，这就是抽象基类的"霸王条款"

```C++

#include <iostream>

using namespace std;
//CPU抽象类
class CPU { 
public:
    virtual void calculate() = 0;
};
//内存条抽象类
class Memory { 
public:
    virtual void storage() = 0;
};
//电脑派生类(作用是将基类指针指向派生类并执行派生类虚函数)
class Computer {
public:
    Computer(CPU *cpu, Memory *mem);
    ~Computer();
    void work();
private:
    CPU *m_cpu;
    Memory *m_mem;
};
//此处传入的形参应该是两种派生类
Computer::Computer(CPU *cpu, Memory *mem):m_cpu(cpu),m_mem(mem){}
//基类指针执行派生类的虚函数,即多态
void Computer::work(){
    m_cpu->calculate();
    m_mem->storage();
}
//释放传入的派生类的指针
Computer::~Computer(){
    if (m_cpu != NULL) {
        delete m_cpu;
        m_cpu = NULL;
    }
    if (m_mem != NULL) {
        delete m_mem;
        m_mem = NULL;
    }
}
//Intel厂商
class Intel_cpu:public CPU {
public:
    virtual void calculate() {
        cout << "CPU work of Intel" << endl;
    }
};
class Intel_memory:public Memory {
public:
    virtual void storage() {
        cout << "memory work of Intel" << endl;
    }
};
//Lenovo厂商
class Lenovo_cpu:public CPU {
public:
    virtual void calculate() {
        cout << "CPU work of Lenovo" << endl;
    }
};
class Lenovo_memory:public Memory {
public:
    virtual void storage() {
        cout << "memory work of Lenovo" << endl;
    }
};

int main()
{
    CPU *intel_cpu = new Intel_cpu;
    Memory *intel_mem = new Intel_memory;
    Computer *computer1 = new Computer(intel_cpu, intel_mem);
    computer1->work();  //打印CPU/memory work of Intel
    delete computer1;
    cout << "---------------------" << endl;
    Computer *computer2 = new Computer(new Lenovo_cpu, new Lenovo_memory);
    computer2->work();  // 打印CPU/memory work of Lenovo
    delete computer2;

    return 0;
}


```

#### 虚析构

当发生[向上转型](#向上转型)时，即派生类指针赋值给基类指针时，delete 释放基类并不会调用派生类的析构函数。因为这里的析构函数是非虚函数，通过指针访问非虚函数时，编译器会根据指针的类型来确定要调用的函数。例如`Base *p;`是基类的指针，所以不管它指向基类的对象还是派生类的对象，始终都是调用基类的析构函数。若`Derive *p;`是派生类的指针，编译器会根据它的类型匹配到派生类的析构函数，在执行派生类的析构函数时，又会调用基类的析构函数，这个过程是隐式完成的

将基类的析构函数声明为虚函数后，派生类的析构函数也会自动成为虚函数。这个时候编译器会忽略指针的类型，而根据指针的指向来选择函数。也就是说，指针指向基类对象就调用基类函数，指针指向派生类对象就调用派生类函数

<div align=center><img src="img/2023-05-22-12-26-13.png" width="50%"></div>

在实际开发中，我们必须将最底层的基类的析构函数声明为虚函数，否则就有内存泄露的风险

>注：多继承时，只要最底层基类的析构函数声明为虚函数即可

### 1.6 命名空间 namespace {#1.4}

大型软件由多名程序员共同开发，不可避免地会出现变量或函数的命名冲突。为了解决合作开发时的命名冲突问题，C++ 引入了命名空间的概念

1. 命名空间就是全局变量，命名空间`A`内的成员可以和命名空间`B`内的成员重名
1. 关键字`using`的功能是设置默认命名空间，无`using`关键字则是临时展开某个成员

早期的 C++ 使用的是C语言的库。例如，`stdio.h` `stdlib.h` `string.h` 等头文件。后来 C++ 开发了一些新的库，增加了自己的头文件，例如，`iostream.h`，`fstream.h` 等。再后来 C++ 引入了命名空间的概念，在原来库的基础上稍加修改后，将类、函数、宏等都纳入命名空间`std`(standard 标准命名空间)中。在`std`中将 C++ 旧头文件去掉后缀`.h` 。例如，`iostream` `fstream`。对于原来的C语言头文件去掉后缀`.h`且名字前添加`c`字母，例如`cstdio` `cstdlib` `cstring`。可以发现，对于不带`.h`的头文件都位于命名空间`std`中，使用时需要声明命名空间`std`

 <center>

```mermaid
graph LR
A(C语言头文件)
B(旧C++头文件)
A -->|去掉.h\n加上c字母| C{整合}
B -->|去掉.h| C{整合}
C --> D(std)

```

</center>

```C++
#include <iostream>

//默认使用命名空间std中的所有库
using namespace std;
//默认使用命名空间std中的cout库
using std::cout;

namespace first {
    void func(void) {
        //特指使用std的endl成员
        cout << "first" << std::endl;
    }
}
namespace second {
    namespace third {
        void func(void) {}
    }
}

//默认使用命名空间first中的所有库
using namespace first;
//特指使用命名空间first中的func()函数
using first::func;
int main()
{
    //默认调用first里的func()函数
    func();
    //特指使用second中third中的func()函数
    second::third::func();

    return 0;
}
```

```C++
#include <iostream>
using namespace std;

namespace my_space {
    int global = 10;
}

int global = 20;
using namespace my_space;
int main()
{
    //局部变量>全局变量>命名空间
    int global = 30;
    cout << global << endl;             //30
    cout << ::global << endl;           //20（全局变量）
    cout << my_space::global << endl;   //10

    return 0;
}
```

### 1.7 静态成员变量和静态成员函数（static） {#1.5}

1. <font color="yellow">静态成员函数</font>：**只能访问静态成员变量**（因为当有多个对象时静态函数能确定静态变量却不能确定动态变量）（注：[常对象只能调用常函数](#a2)）<a id="a1"></a>）
1. <font color="yellow">静态成员变量</font>：必须在类内声明，类外初始化（静态成员函数随意）
1. 静态变量在编译阶段分配内存
1. 类的成员变量存储在栈区，类的静态成员变量、函数都存储在全局区
1. 静态成员变量和函数在全局区，仅有一份，所有实例化出的对象共享这一份

<div align=center><img src="img/2023-05-03-23-02-51.png" width="45%"></div>

6. 空类在栈区仅占一个字节
7. 静态成员变量、静态成员函数和普通函数都不属于类的对象，即不增加类的对象的内存大小

<div align=center><img src="img/2023-05-03-23-09-31.png" width="45%"></div>

### 1.8 this 指针 {#1.6} 

1. 任何非静态成员函数都默认有`this`指针，且`this`指向调用该非静态成员函数的对象。例如`Person t1; t7.func(10)`。`func`内部的`this`指针指向`&t1`，`*this`表示`t1`
1. 空指针可以调用不含this的成员（因为空指针没有实体，就没有this）

```C++
class Person {
public:
    int a;
    //返回this本体，相当于Person &m = *this;
    Person& func() {
        this->a += 10;
        return *this;
    }
    //返回新对象new，相当于Person m = *this;即将*this的数据赋值给 m 并返回
    Person func() {
        this->a += 10;
        return *this;
    }
}
```

<div align=center><img src="img/2023-05-03-23-44-05.png" width="45%"></div>

3. `this`的原型是`Person *const this`，在函数后面加`const`叫<font color="yellow">常函数</font>，则`this`变为`const Person *const this`
4. `const Person t1`，在对象定义时加`const`叫<font color="yellow">常对象</font>。**常对象只能调用常函数**（注：[静态成员函数只能访问静态成员变量](#a1)）<a id="a2"></a>（const普通变量叫常变量）

<div align=center><img src="img/2023-05-03-23-47-01.png" width="30%"></div>

### 1.9 友元 friend {#1.7}

1. 全局函数声明为友元，则该全局函数就可以访问该类的所有成员（包括private成员）
1. 类声明为友元，则友元类中的所有成员函数都是另外一个类的友元函数
1. 类的成员函数声明为友元，则该函数可以访问该类的所有成员（包括private成员）

<div align=center><img src="img/2023-05-04-00-07-33.png" width="35%"></div>
.
<div align=center><img src="img/2023-05-04-00-08-56.png" width="70%"></div>
.
<div align=center><img src="img/2023-05-04-00-09-59.png" width="70%"></div>

### 1.10 运算符重载 {#运算符重载}

当运算符不仅可以实现`int`类型运算，还可以实现类对象的运算，则被称为<font color="yellow">运算符重载</font>。运算符重载其实就是定义一个函数，在函数体内实现想要的功能，当用到该运算符时，编译器会自动调用这个函数。也就是说，运算符重载本质上是函数重载

运算符重载的格式为：
>返回值类型 operator 运算符名称 (形参表列){
>  TODO
>}

运算符重载用于类对象之间的运算，`operator`是关键字，用于定义重载运算符的函数

```C++
#include <iostream>
#include <string>
using namespace std;

class Person {
public:
    Person();
    Person(string n, int a, int h);

    //operator是运算符重载关键字
    Person operator+(const Person &a) const;
    Person operator-(Person &a);
    void display() const;
private:
    string m_name;
    int m_age;
    int m_height;
};

Person::Person() { }
Person::Person(string n, int a, int h): m_name(n), m_age(a), m_height(h) { }

//重载加运算符
Person Person::operator+(const Person &a) const{
    Pacerson b;
    b.m_name = this->m_name;
    b.m_age = this->m_age + a.m_age;
    b.m_height = this->m_height + a.m_height;
    return b;
}
//重载减运算符
Person Person::operator-(Person &a) {
    Person b;
    b.m_name = this->m_name;
    b.m_age = this->m_age - a.m_age;
    b.m_height = this->m_height - a.m_height;
    return b;
}

void Person::display() const{
    cout << m_name;
    cout << "--> age: " << m_age;
    cout << " height: " << m_height <<endl;
}

int main(){
    Person t1("Li", 20, 150);
    Person t2("Hong", 30, 160);
    Person t3;
//80%的运算符都是自左向右运算
//编译器检测到+号左边t1是一个Person对象，就会调用t1的成员函数operator+()，将+号右边的t2作为实参，也就是转换为t3 = t1.operator+(t2);
    t3 = t1 + t2;
    t3.display();   //Li--> age: 50 height: 310
    t3 = t2 - t1;
    t3.display();   //Hong--> age: 10 height: 10

    return 0;
}
```




### 1.11 string

`string`类完全可以代替C语言中的字符串数组和字符串指针，使用`string`类需要包含头文件`<string>`

```C++
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

int main()
{
    // string的结尾没有'\0'
    string t1 = "Linux";
    // string定义的对象可以直接相互赋值,不需要使用strcpy函数
    string t2 = t1;
    // 初始化string类的构造函数。等价于 string t3 = "SSS"
    string t3(3, 'S');
    // 按下标来访问string字符串
    t1[0] = 'C';
    // string可以用 + 运算符来和任意字符串拼接
    char char_str[] = "ABC";
    string t4 = t1 + "China" + char_str + 'T';
    // 调用成员函数length(),返回字符串长度(无'\0)
    cout << t1.length() << endl;
    // 为了使用C语言中的fopen()函数打开文件,必须调用成员函数c_str(),将string字符串转换为C风格的字符串
    FILE *fp = fopen(t1.c_str(), "r+");
    // 成员函数insert(n, str),表示插入字符串str,从string的下标 n 前面
    t1.insert(0, "bhlk");
    // 成员函数erase(n, m),表示删除 m 个字符,从string的下标 n 开始(若 m 大于string则删除 n 后所有)
    // erase(n),表示删除所有字符,从string的下标 n 开始
    t1.erase(4, 5);
    // 成员函数substr(n, m),表示提取返回 m 个字符,从string的下标 n 开始(若 m 大于string则提取 n 后所有)
    // 成员函数substr(n),表示提取返回所有字符,从string的下标 n 开始
    cout << t1.substr(2, 1) << endl;
    // 成员函数find(str, n),表示查找并返回第1次出现字符串 str 的下标,从string的下标 n 开始
    // 成员函数find(str),表示查找并返回第1次出现字符串 str 的下标,从头开始
    t1 = "Linux China";
    cout << t1.find("Ch") << endl;  // 结果返回6

    return 0;
}
```

## 二、文件操作 {#文件操作}

C++对文件操作需要包含头文件`<fstream>`。文件类型分为两种：

1. 文本文件。文件以文本的ASCII码形式存储在计算机中
1. 二进制文件。文件以文本的二进制形式存储在计算机中

`ofstream`表示写操作；`ifstream`表示读操作；`fstream`表示读写操作（`f`表示file）

<center>读写ASCII码文件示例</center>

```C++
#include <iostream>
#include <string>
#include <fstream> //文件操作头文件

using namespace std;

int main()
{
    //创建流对象
    ofstream ofs;
    //指定写入的文件路径，以及out方式打开
    ofs.open("test.txt", ios::out);
    //利用 << 向文件中写数据
    ofs << "Linux" << endl;
    ofs << "China" << endl;
    //关闭文件
    ofs.close();

    string buf;
    ifstream ifs;
    //指定读取的文件路径，以及in方式打开
    ifs.open("test.txt", ios::in);
    //is_open() 返回0即文件打开失败，返回1即打开成功
    if (!ifs.is_open()) {
        cout << "file oepn failed" << endl;
    }
    //读取数据（getline()函数会将ifs读取到buf中）
    while (getline(ifs, buf)) {
        cout << buf << endl;
    }
    //关闭文件
    ifs.close();

    return 0;
}
```

## 三、模板 template {#模板}

C++ 除了面向对象的编程思想，还有一个泛型编程思想

泛型程序设计（generic programming），指的是算法只要实现一遍，就能适用于多种数据类型。泛型程序设计最成功的应用就是 C++ 的标准模板库STL（Standard Template Library，标准模板库）。在 C++ 中，模板分为<font color="yellow">函数模板</font>和<font color="yellow">类模板</font>两种

所谓函数模板，实际上是建立一个通用函数，它所用到的数据的类型（包括返回值类型、形参类型、局部变量类型）可以不具体指定，而是用一个虚拟的类型来代替（实际上是用一个标识符来占位），等发生函数调用时再根据传入的实参来逆推出真正的类型。这个通用函数就称为<font color="yellow">函数模板</font>（Function Template）

- `template`是定义函数模板的关键字（可以使用`class`替代关键字`template`）

- `typename`是另外一个关键字，用来声明具体的<font color="yellow">类型参数</font>（也可以说是虚拟的类型，或者说是类型占位符），这里的类型参数就是`T`。从整体上看，`template<typename T>` 被称为模板头

学习模板并不是为了写模板，而是在STL中能够用系统提供的模板

<font color="yellow">即使整个模板中没有出现`T`，编译器也必须要知道模板函数和模板类的类型参数`T`的数据类型。无论是通过自动推断`T`的类型还是指明`T`的类型</font>

### 3.1 函数模板

#### 函数模板基础

- 函数模板的类型参数 T 有自动推导和指明类型两种模式

```C++
#include <iostream>
#include <string.h>
using namespace std;

//函数模板定义
template<typename T> void func_swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

//若T2不在形参中，则需要在调用函数模板的时候指明数据类型
template<typename T1, typename T2> 
void func2(T1 a) {
    T2 b;
}

int main(void)
{
//1、自动推断T的数据类型
    int a = 2, b = 8;
    //a和b交换
    func_swap(a, b);    //形参是引用，需要传入变量，不能直接传入数字或字符串

    string str1 = "Linux", str2 = "China";
    //str1和str2交换
    func_swap(str1, str2);

//2、指明T的数据类型
    //给函数指明数据类型。T1的类型是int，T2的类型是string
    func2<int, string>(10);

    return 0;
}
```

#### 函数模板重载

```C++
#include <iostream>
#include <string.h>
using namespace std;

#include <iostream>
using namespace std;

// 函数名相同，形参不同的成员函数，称为重载函数
template<class T> void Swap(T &a, T &b);  //模板1：交换基本类型的值
template<typename T> void Swap(T a[], T b[], int len);  //模板2：交换两个数组
void printArray(int arr[], int len);  //打印每个数组元素
int main(){
//交换基本类型的值
    int m = 10, n = 99;
    Swap(m, n);  //匹配模板1
    cout<<m<<", "<<n<<endl;

//交换两个数组
    int a[5] = { 1, 2, 3, 4, 5 };
    int b[5] = { 10, 20, 30, 40, 50 };
    //数组名作为实参会自动转化为数组指针
    //sizeof只能通过数组名求得数组长度，不能通过数组指针求得数组长度
    int len = sizeof(a) / sizeof(int);  //数组长度
    Swap(a, b, len);  //匹配模板2
    printArray(a, len);
    printArray(b, len);
    return 0;
}
template<class T> void Swap(T &a, T &b){
    T temp = a;
    a = b;
    b = temp;
}
template<typename T> void Swap(T a[], T b[], int len){
    T temp;
    for(int i = 0; i < len; i++){
        temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}
// sizeof只能通过数组名求得数组长度，不能通过转换后的数组指针求得数组长度
void printArray(int arr[], int len){
    for(int i = 0; i < len; i++){
        cout << arr[i] << ", ";
    }
    //2个\b后面加一个空格，表示bakcspace
    cout << "\b\b " <<endl;
}
```

#### 函数模板显示具体化

- 让模板针对某种具体的类型使用不同的算法（函数体或类体不同），这种技术称为模板的<font color="yellow">显示具体化</font>。例如，函数模板无法进行类之间的比较，即判断`class1 == class2`。所以需要进行函数模板的显示具体化

 - 函数模板的显示具体化与普通函数的区别在于多了个空模板头
    - 普通函数：`void func(int a) { }`
    - 函数模板显示具体化：`template<> void func(int a) { }`

```C++
#include <iostream>
#include <string>
using namespace std;

class Base {
public:
    string m_name;
    int m_age;
    Base(string n, int a):m_name(n), m_age(a){}
};

template<typename T> void compare(T a, T b) {
    if (a == b)
        cout << "a == b" << endl;
}
//函数模板无法进行类之间的比较，即判断 class1 == class2
//所以需要进行函数模板compare的显示具体化
//函数模板的显示具体化与普通函数的区别在于多了个空模板头
template<> void compare(Base b1, Base b2) {
    if (b1.m_name == b2.m_name && b1.m_age == b2.m_age)
        cout << "b1 == b2" << endl;
    else
        cout << "b1 != b2" << endl;
}

int main()
{
    Base base1("China", 5000);
    Base base2("Linux", 100);

    compare(5, 5);          
    compare(base1, base2);  //调用具体化的模板

    return 0;
}
```

#### 函数模板的数组实参

通过函数实参来确定模板类型参数`T`的过程称为<font color="yellow">模板实参推断</font>
- 函数传入实参时，数组会转化成`int *`类型，所以函数形参类型`T`要凑成`int *`类型
- 当函数形参是引用类型时，数组不会转换为指针，依然是数组类型

```C++
#include <iostream>
#include <string.h>
using namespace std;
class Base {};

template<typename T> void func1(T *a) {
    cout << "T1 size of func1 is " << sizeof(T) << endl;
}
template<typename T> void func2(T a) {
    cout << "T2 size of func2 is " << sizeof(T) << endl;
}
template<typename T> void func3(const T &a) {
    cout << "T3 size of func3 is " << sizeof(T) << endl;
}
template<typename T> void func4(T &a) {
    cout << "T4 size of func4 is " << sizeof(T) << endl;
}

int main(void)
{
//函数传入实参时，数组会转化成 int * 类型，所以函数形参类型 T 要凑成 int * 类型
    int t1[20];
    Base base;

//t1 的类型从 int[20] 转换为 int*,所以 T 的类型为 int
    func1(t1);      // 4

//t1 的类型从 int[20] 换转换为 int *,所以 T 的类型为 int *
    func2(t1);      // 8

//T 的真实类型为 Base
    func3(base);    // 1

//当函数形参是引用类型时，数组不会转换为指针，依然是数组类型
//t1 的类型依然为 int[20]，不会转换为 int *，所以 T 的真实类型为 int[20]（注意int[10]和int[20]类型是不同的）
    func4(t1);      // 80

    return 0;
}
```

#### 函数模板的实参类型转换

<font color="yellow">建议使用显示指定类型的方式调用函数模板</font>

- 普通函数调用时会自动进行类型转换（隐式类型转换）
- 函数模板显示指定类型参数`T`时，也会进行类型转换。例如，只有一个`T`且指明`T`的类型为`int`，当同时传入两个实参为`char`和`int`时，则都会转为`int`
- 函数模板自动数据类型推导`T`时，不会进行类型转换。例如，只有一个`T`，当同时传入两个实参为`char`和`int`时，则会报错

```C++
#include <iostream>
using namespace std;

template<typename T> void func(T a, T b) {
    cout << a + b << endl;
}
int main()
{
    int a = 5;
    char b = '8';
    func(a, a);        //正确，T能明确数据类型
    func(a, b);        //错误，T的数据类型不明确
    func<int>(a, b);   //正确，指明了T的数据类型，char会转为int

    return 0;
}
```

#### 函数模板中的非类型参数

```C++
#include <iostream>
using namespace std;
//一般情况下给函数传入数组时会转化为int *类型，此时还需要传入数组长度
//使用非类型参数 + 引用的方式给函数传入数组时形参还是int[]类型,N就是长度
//a是一个引用，类型是数组T[N]
template<typename T, unsigned N> void Swap(T (&a)[N], T (&b)[N]);  //交换两个数组
template<typename T, unsigned N> void printArray(T (&arr)[N]);  //打印数组元素
int main(){
    //交换两个数组
    int a[5] = { 1, 2, 3, 4, 5 };
    int b[5] = { 10, 20, 30, 40, 50 };
    //此时T是int类型，N的值是3
    Swap(a, b);
    printArray(a);
    printArray(b);
    return 0;
}
template<typename T, unsigned N> void Swap(T (&a)[N], T (&b)[N]){
    T temp;
    for(int i = 0; i < N; i++) {
        temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}
template<typename T, unsigned N> void printArray(T (&arr)[N]){
    for(int i = 0; i < N; i++) {
        if(i == N-1){
            cout << arr[i] << endl;
        }else{
            cout << arr[i] << ", ";
        }
    }
}
```

#### 函数模板与普通函数重名


- 建议普通函数不要和函数模板重名
- 当函数模板和普通函数的函数名相同且形参类型也相同时，会屏蔽函数模板

```C++
#include <iostream>
using namespace std;

void func(int a) {
    cout << "common" << endl;
}
template<typename T> void func(T a) {
    cout << "template" << endl;
}
int main()
{
//当函数模板和普通函数的函数名相同且形参类型也相同时，会屏蔽函数模板
    func(6);        // 打印common
    func("Li");     // 打印template

//可以通过空模板参数列表强制调用函数模板
    func<>(6);      // 打印template
    func<int>(6);   // 打印template

    return 0;
}
```

### 3.2 类模板

#### 类模板基础

- 类模板实例化对象时必须指明类型参数`T`的数据类型（除非该类型参数设置了默认数据类型）
- 类模板的成员函数在类外实现时需要加模板的类型参数

```C++
#include <iostream>
using namespace std;

template<typename x_t, typename y_t>
class Point {
public:
    Point(x_t x, y_t y):m_x(x), m_y(y){}
    x_t get_x() const;
    y_t get_y() const;
    void set_xy(x_t x, y_t y);
private:
    x_t m_x;
    y_t m_y;
};
//类模板成员函数在类外定义时，类名Point后面要加类型参数
template<typename x_t, typename y_t>
x_t Point<x_t, y_t>::get_x() const {
    return m_x;
}
template<typename x_t, typename y_t>
y_t Point<x_t, y_t>::get_y() const {
    return m_y;
}
template<typename x_t, typename y_t>
void Point<x_t, y_t>::set_xy(x_t x, y_t y) {
    if (x != 0 || sizeof(x)) //sizeof判断空字符串
        m_x = x;
    if (y != 0 || sizeof(y))
        m_y = y;
}

int main()
{
    //使用类模板创建对象时，需要指明数据类型
    Point<int, float> p1(10, 23.5);
    //打印10  23.5
    cout << p1.get_x() << "  " << p1.get_y() << endl;
    p1.set_xy(0, 99);

    //使用类模板创建对象指针时，两边类型参数须保持一致
    Point<string, string> *p2 = new Point<string, string>("Linux", "China");
    //打印Linux  China
    cout << p2->get_x() << "  " << p2->get_y() << endl;

    return 0;
}
```



#### 类模板对象做函数参数

```C++
#include <iostream>
using namespace std;

template<typename T1, typename T2>
class Base {
public:
    Base(T1 n, T2 a):m_name(n), m_age(a){}
    void show() {
        cout << m_name << " " << m_age << endl;
    }
private:
    T1 m_name;
    T2 m_age;
};
//当实参是类模板时，函数形参也要指定类型参数T的数据类型
void func(Base<string, int> &p) {
    p.show();
}

int main()
{
    Base<string, int> t("Linux", 200);
    t.show();   //Linux  200

    return 0;
}
```

#### 类模板的继承

```C++
#include <iostream>
#include <string>
using namespace std;

template<typename T>
class Base {
    T m;
};
//当基类是一个类模板时，派生类要指定基类T的类型
class Derive1:public Base<int> {    //此时m是int类型
};

//当基类是一个类模板时，若派生类不指定基类T的类型，则派生类也必须是类模板
template<typename T1, typename T2>
class Derive2:public Base<T2> {     //此时m是T2类型
    T1 obj;
};

int main()
{
    Derive2<int, string> t;

    return 0;
}
```

### 3.3 函数模板与类模板的区别

1. 只有函数模板有自动类型推导，类模板必须指明类型参数`T`的数据类型
2. 只有类模板在参数列表中可以有默认数据类型

```C++
#include <iostream>
using namespace std;

//类型参数T可以设置默认数据类型
template<typename name_t, typename age_t = int>
class Base {
public:
    Base(name_t n, age_t a):m_name(n), m_age(a){}
private:
    name_t m_name;
    age_t m_age;
};

int main()
{

    //类模板实例化对象时必须指明类型参数
    //此处类型参数2存在默认类型，故可以不指明
    Base<string> t1("Linux", 100);

    return 0;
}
```


### 3.4 模板的实例化

- 模板仅仅是编译器用来生成函数或类的一张"图纸"。模板不会占用内存，最终生成的函数或者类才会占用内存。由模板生成函数或类的过程叫做<font color="yellow">模板的实例化</font>（Instantiate）
- 无论是函数模板，还是类模板中的成员函数，只要是模板，其中的函数都是在调用时才会实例化

#### 函数模板的实例化

```C++
template<typename T> void Swap(T &a, T &b){
    T temp = a;
    a = b;
    b = temp;
}
int main(){
    int n1 = 1, n2 = 2, n3 = 3, n4 = 4;
    string str1 = "Linux", str2 = "China";
   
    Swap(n1, n2);       //T为int，实例化出 void Swap(int &a, int &b);
    Swap(str1, str2);   //T为string，实例化出 void Swap(string &a, string &b);
    Swap(n3, n4);       //T为int，调用刚才生成的 void Swap(int &a, int &b);
    return 0;
}
```

#### 类模板的实例化

通过类模板创建对象时并不会实例化所有的成员函数，只有等到真正调用它们时才会被实例化。一般只需要实例化成员变量和构造函数。成员变量被实例化后就能够知道对象的大小了（占用的字节数），构造函数被实例化后就能够知道如何初始化了

```C++
#include <iostream>
using namespace std;

class Base1 {
public:
    void base1_show() {
        cout << "Base1" << endl;
    }
};
class Base2 {
public:
    void base2_show() {
        cout << "Base2" << endl;
    }
};
template<typename T>
class Derive {
public:
    T obj;
    void func1(){ obj.base1_show(); }
    void func2(){ obj.base2_show(); }
};

int main()
{
    //此时类模板只创建成员变量并不会创建成员函数
    Derive<Base1> t;
    //只有在调用func1时才会实例化成员函数func1（此时func2未实例化）
    t.func1();
    //只有在调用func2时才会实例化func2，但会报错
    t.func2();

    return 0;
}
```


### 3.5 将模板应用于多文件编程

工程中一般会包含两个源文件和一个头文件，`func.cpp`中定义函数和类，`func.h`中声明函数和类，`main.cpp`中调用函数和类，这是典型的将函数的声明和实现分离的编程模式，达到「模块化编程」的目的。 但是模板并不是真正的函数或类，它仅仅是用来生成函数或类的一张"图纸"，如果`main.cpp`只声明了`func.h`文件，则在编译的时候不会生成模板函数和模板类的实体，这样在链接的时候就会链接报错，只有将`func.cpp`包含到`main.cpp`中才正确。所以我们不能将模板的声明和定义分散到多个文件中，<font color="yellow">我们应该将模板的定义`func.cpp`和声明`func.h`都放到头文件`.hpp`中。</font>`.hpp`默认表示的是模板的头文件

## 四、C++11

### auto

在`C++11`之前的版本`C++98`中，定义变量或者声明变量之前都必须指明它的类型，比如`int`、`char` 等；`auto`关键字用来指明变量的存储类型，它和 static 关键字是相对的。auto 表示变量是自动存储的，这也是编译器的默认规则，所以写不写都一样，这使得 auto 关键字的存在变得非常鸡肋。为了让编译器（或者解释器）自己去推导数据类型，让编码更加方便，在`C++11`中使用`auto`关键字后，编译器会在编译期间做自动类型推导

#### auto的限制

1. auto 不能在函数的参数中使用
我们在定义函数的时候只是指明了参数的类型，但并没有给它赋值，只有在实际调用函数的时候才会给参数赋值；而 auto 要求必须对变量进行初始化，所以这是矛盾的。
1. auto 不能作用于类的非静态成员变量（也就是没有 static 关键字修饰的成员变量）中
1. auto 关键字不能定义数组，比如下面的例子就是错误的
`char array[];`
`auto  str[] = array;`  array是数组，所以不能使用 auto
1. auto 不能作用于模板参数。例如，`func<auto>();`是错误的

```C++
#include <iostream>
using namespace std;

int main()
{
    //关键字auto是在初始化时才进行数据类型推导的
    auto url = "192";   //"192"是const char*，auto是const char*类型  
    auto a = 5;         //5是int类型，auto是int类型
    auto b = 7.7;       //7.7是double类型，auto是double类型
    auto p1 = &a;       //&a是int* 类型，auto是int *类型
    auto *p2 = &a;      //&a是int* 类型，auto是int类型
    auto &c = a;        //&a是int* 类型，auto是int类型（int *const c = &a）
    auto d = c;         //c是int 类型，auto是int类型（c是解引用）

    //当未用引用时，auto类型推导时不保留const属性
    const auto e = a;   //auto是int类型
    auto f = e;         //auto是int类型
    //当使用引用时，auto类型推导时将保留const属性
    const auto &g = a;  //auto是int类型
    auto &h = g;        //g是const int *const类型，auto是const int类型

    return 0;
}
```

## 杂项

1. `string str;`相当于`char str[9];`
1. `srand(time(NULL));rand()%100;`生成一个随机数，范围0~99
1. int a = 156；将数字拆分(a%10会将a的个位数取出)
cout << (a / 1 % 10) << endl；	//6 
cout << (a / 10 % 10) << endl；	//5
cout << (a / 100 % 10) << endl；	//1
1. C++创建结构体变量时，"struct"可以省略
1. case内的代码过长，则要使用｛｝
1. C++中字符串指针存放在全局区，字符串数组在栈区 
1. switch的形参是int类型，不能判断数字范围

