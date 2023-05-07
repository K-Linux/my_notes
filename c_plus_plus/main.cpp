#include <stdio.h>
#include <iostream>
#include <time.h>

using namespace std;

class Base {
public:
    int m_a;
    Base() {
        m_a = 10;
    }
    void func() {
        cout << "Base_func" << endl;
    }
};
class Derive : public Base{
public:
    //与基类同名
    int m_a;
    Derive() {
        m_a = 20;
    }
    void func() {
        cout << "Derive_func" << endl;
    }
};

int main(void)
{
    Derive t1;
    // 若派生类成员与基类成员同名，则屏蔽基类成员
    cout << t1.m_a << endl;         //20
    // 使用作用域符号特指调用基类成员变量
    cout << t1.Base::m_a << endl;   //10
    /* 若派生类成员函数与基类成员函数同名
     * 则屏蔽基类所有同名函数(包括重载函数) */
    t1.func();                      //Derive_func
    // 使用作用域符号特指调用基类成员函数
    t1.Base::func();                //Base_func

    return 0;
}

