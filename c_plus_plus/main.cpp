#include <stdio.h>
#include <iostream>
#include <time.h>

using namespace std;

class Base {
public:
    int m_a;
    static int m_b;
    Base() {
        m_a = 10;
    }
    void func() {
        cout << "Base_func" << endl;
    }
};
int Base::m_b = 100;
class Derive : public Base{
public:
    //成员与基类同名
    int m_a;
    static int m_b;
    Derive() {
        m_a = 20;
    }
    void func() {
        cout << "Derive_func" << endl;
    }
};
int Derive::m_b = 200;

int main(void)
{
    Derive t1;
    // 若派生类成员与基类成员同名，则屏蔽基类成员
    cout << t1.m_a << endl;         //20
    // 使用作用域符号特指调用基类成员变量
    cout << t1.Base::m_a << endl;   //10
    /* 若派生类成员函数与基类成员函数同名
     * 则屏蔽基类所有同名函数(包括重载函数和静态成员)*/
    t1.func();                      //Derive_func
    // 使用作用域符号特指调用基类成员函数
    t1.Base::func();                //Base_func
    /* 当静态成员同名时，可以通过对象访问（同上）
     * 也可以通过类名访问静态成员（如下） */
    cout << Derive::m_b << endl;        //200
    cout << Derive::Base::m_b << endl;  //100
    /* 以上通过派生类访问基类
     * 以下直接调用基类，与派生类无关(即无同名) */
    cout << Base::m_b << endl;          //100

    return 0;
}

