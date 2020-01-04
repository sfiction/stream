#pragma once

#include <iostream>
#include <exception>

namespace Fractional{

using std::ostream;
using std::exception;

template<class T>
class Frac;

template<class T>
Frac<T> operator +(const Frac<T> &a, const Frac<T> &b){
    T d = Frac<T>::gcd(a.denom, b.denom);
    Frac<T> ret(b.denom / d * a.numer + a.denom / d * b.numer, a.denom / d * b.denom);
    ret.refine();
    return ret;
}

template<class T>
Frac<T> operator *(const Frac<T> &a, const Frac<T> &b){
    Frac<T> ret(a.numer * b.numer, a.denom * b.denom);
    ret.refine();
    return ret;
}

template<class T>
Frac<T> operator /(const Frac<T> &a, const Frac<T> &b){
    Frac<T> ret(a.numer * b.denom, a.denom * b.numer);
    ret.refine();
    return ret;
}

template<class T>
bool operator ==(const Frac<T> &a, const Frac<T> &b){
    return a.numer == b.numer && a.denom == b.denom;
}

template<class T>
ostream &operator <<(ostream &os, const Frac<T> &a){
    if (a.numer == 0 || a.denom == 1)
        os << a.numer;
    else
        os << a.numer << "/" << a.denom;
    return os;
}

template<class T>
class Frac{
    T numer, denom;

    static T gcd(T a, T b){
        T t;
        while (b != T(0)){
            t = a;
            a = b;
            b = t % b;
        }
        return a;
    }
public:
    Frac(const T &numer, const T &denom): numer(numer), denom(denom){
        if (denom == 0)
            throw exception();
    }

    Frac(int x = 1): numer(x), denom(1){
    }

    void refine(){
        T d = gcd(numer, denom);
        numer /= d;
        denom /= d;
        if (denom < 0){
            numer = -numer;
            denom = -denom;
        }
    }

    Frac<T> operator -(){
        return Frac<T>(-numer, denom);
    }

    friend Frac<T> operator + <>(const Frac<T> &a, const Frac<T> &b);
    friend Frac<T> operator * <>(const Frac<T> &a, const Frac<T> &b);
    friend Frac<T> operator / <>(const Frac<T> &a, const Frac<T> &b);
    friend bool operator == <>(const Frac<T> &a, const Frac<T> &b);
    friend ostream &operator << <>(ostream &os, const Frac<T> &a);
};

}
