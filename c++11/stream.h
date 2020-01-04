#pragma once

#include <bits/stdc++.h>

namespace StreamTool{

using std::cerr;
using std::endl;
using std::string;

using std::result_of;

using std::function;
using std::shared_ptr;
using std::make_shared;

using std::exception;

template<class T>
class _Stream;

template<class T>
using Stream = shared_ptr<_Stream<T>>;

template<class T>
using FStream = function<Stream<T>()>;

template<class T>
class _Stream{
    T car_;
    FStream<T> compute_cdr;
    Stream<T> cdr_;
    string tag;
    bool empty_, is_computed;
    
public:
    _Stream(const T &car = T(), FStream<T> compute_cdr = FStream<T>(), const string &tag = "stream", bool empty = false)
            :car_(car), compute_cdr(compute_cdr), tag(tag), empty_(empty), is_computed(false){
        cerr << "construct " << typeid(T).name() << " " << tag << ", " << (void*)this << ", car: " << car_ << endl;
    }

    _Stream(_Stream &&r): car_(r.car_), compute_cdr(r.compute_cdr), cdr_(r.cdr_){
    }

    ~_Stream(){
        cerr << "deconstruct " << typeid(T).name() << " " << tag << ", " << (void*)this << ", car: " << car_ << ", cdr: " << (void*)cdr_.get() <<endl;
    }

    T &car(){
        return car_;
    }

    void cdr(FStream<T> compute_cdr){
        this->compute_cdr = compute_cdr;
    }

    Stream<T> cdr(){
        if (!is_computed){
            cdr_ = compute_cdr();
            is_computed = true;
        }
        return cdr_;
    }

    bool empty() const{
        return empty_;
    }

    static Stream<T> Empty;
};

template<class T, class... Args>
Stream<T> stream(Args&&... args){
    return make_shared<_Stream<T>>(std::forward<Args>(args)...);
}

template<class T>
Stream<T> _Stream<T>::Empty = stream<T>(T(), FStream<T>(), "empty", true);

template<class T>
T &car(Stream<T> s){
    return s->car();
}

template<class T>
Stream<T> cdr(Stream<T> s){
    return s->cdr();
}

template<class T>
Stream<T> cons(const T &car, Stream<T> cdr){
    return stream<T>(car, [=](){return cdr;}, "cons");
}

template<class F, class T, class R = typename result_of<F(T)>::type>
Stream<R> map(F func, Stream<T> s){
    return s->empty() ? _Stream<R>::Empty
            : stream<R>(func(car(s)), [=](){return map(func, cdr(s));}, "map_unary");
}

template<class F, class T1, class T2, class R = typename result_of<F(T1, T2)>::type>
Stream<R> map(F func, Stream<T1> a, Stream<T2> b){
    return a->empty() || b->empty() ? _Stream<R>::Empty
            : stream<R>(func(car(a), car(b)), [=](){return map(func, cdr(a), cdr(b));}, "map_binary");
}

template<class T>
Stream<T> slice(Stream<T> s, size_t begin, size_t end, size_t step = 1){
    if (step == 0)
        throw exception();
    if (begin >= end)
        return _Stream<T>::Empty;
    end -= begin;
    for (; begin && !s->empty(); --begin)
        s = cdr(s);
    return s->empty() ? _Stream<T>::Empty
            : stream<T>(car(s), [=](){return slice(s, step, end, step);}, "slice");
}

template<class T>
Stream<T> slice(Stream<T> s, size_t n){
    return s->empty() || n == 0 ? s->Empty
            : stream<T>(car(s), [=](){return slice(cdr(s), n - 1);}, "slice_n");
}

template<class T>
Stream<T> unit(const T &c = T()){
    return stream<T>(c, [](){return _Stream<T>::Empty;}, "unit");
}

template<class T>
Stream<T> constant(const T &c){
    Stream<T> ret = stream<T>(c, FStream<T>(), "constant");
    ret->cdr([=](){return ret;});
    return ret;
}

template<class T1, class T2, class T3 = decltype(T1() + T2())>
Stream<T3> add2(Stream<T1> a, Stream<T2> b){
    return map([=](const T1 &x, const T2 &y){return x + y;}, a, b);
}

template<class T1, class T2, class T3 = decltype(T1() + T2())>
Stream<T3> add2(Stream<T1> a, const T2 &b){
    return map([=](const T1 &x)->T3{return x + b;}, a);
}

template<class T1, class T2, class T3 = decltype(T1() * T2())>
Stream<T3> scale2(Stream<T1> a, const T2 &b){
    return map([=](const T1 &x)->T3{return x * b;}, a);
}

template<class T1, class T2, class T3 = decltype(T1() - T2())>
Stream<T3> sub2(Stream<T1> a, Stream<T2> b){
    return map([=](const T1 &x, const T2 &y){return x - y;}, a, b);
}

/* arithmetic operations */

template<class T>
Stream<T> zero(){
    static Stream<T> ret;
    if (!ret)
        ret = constant<T>(0);
    return ret;
}

template<class T1, class T2, class T3 = decltype(T1() + T2())>
Stream<T3> add(Stream<T1> a, Stream<T2> b){
    return a->empty() ? b
            : b->empty() ? a
            : stream<T3>(car(a) + car(b), [=](){return add(cdr(a), cdr(b));}, "add");
}

template<class T1, class T2, class T3 = decltype(T1() * T2())>
Stream<T3> dot(Stream<T1> a, Stream<T2> b){
    return a->empty() || b->empty() ? _Stream<T3>::Empty
            : stream<T3>(car(a) * car(b), [=](){return dot(cdr(a), cdr(b));}, "dot");
}

template<class T1, class T2, class T3 = decltype(T1() * T2())>
Stream<T3> dot2(Stream<T1> a, Stream<T2> b){
    return map([=](const T1 &x, const T2 &y){return x * y;}, a, b);
}

template<class T1, class T2, class T3 = decltype(T1() * T2())>
Stream<T3> dotr(Stream<T1> a, Stream<T2> b){
    if (a->empty())
        return _Stream<T3>::Empty;
    if (b->empty())
        throw exception();
    return stream<T3>(car(a) / car(b), [=](){return dotr(cdr(a), cdr(b));}, "dotr");
}

template<class T1, class T2, class T3 = decltype(T1() + T2())>
Stream<T3> add(Stream<T1> a, const T2 &b){
    return a->empty() ? constant(b)
            : stream<T3>(car(a) + b, [=](){return scale(cdr(a), b);}, "add_c");
}

template<class T1, class T2, class T3 = decltype(T1() * T2())>
Stream<T3> scale(Stream<T1> a, const T2 &b){
    return a->empty() || b == T2() ? _Stream<T3>::Empty
            : stream<T3>(car(a) * b, [=](){return scale(cdr(a), b);}, "scale");
}

template<class T1, class T2, class T3 = decltype(T1() * T2())>
Stream<T3> scale3(Stream<T1> a, const T2 &b){
    return dot(a, constant(b));
}

template<class T1, class T2, class T3 = decltype(T1() - T2())>
Stream<T3> sub(Stream<T1> a, Stream<T2> b){
    return a->empty() ? scale(b, -1)
            : b->empty() ? a
            : stream<T3>(car(a) - car(b), [=](){return sub(cdr(a), cdr(b));}, "sub");
}

template<class T1, class T2, class T3 = decltype(T1() + T2())>
Stream<T3> sub3(Stream<T1> a, Stream<T2> b){
    return add(a, scale(b, -1));
}

template<class T1, class T2, class T3 = decltype(T1() * T2())>
Stream<T3> mul(Stream<T1> a, Stream<T2> b){
    return a->empty() || b->empty() ? _Stream<T3>::Empty
            : stream<T3>(car(a) * car(b), [=](){return add(scale(cdr(a), car(b)), mul(a, cdr(b)));}, "mul");
}

template<class T1, class T2, class T3 = decltype(T1() / T2())>
Stream<T3> div(Stream<T1> a, Stream<T2> b){
    if (b->empty())
        throw exception();
    if (a->empty())
        return _Stream<T3>::Empty;
    T3 val = car(a) / car(b);
    return stream<T3>(val, [=](){return div(sub(cdr(a), scale(cdr(b), val)), b);}, "div");
}

template<class T1, class T2, class T3 = decltype(T1() / T2())>
Stream<T3> div(Stream<T1> a, T2 b){
    if (b == T2())
        throw exception();
    return a->empty() ? _Stream<T3>::Empty
            : stream<T3>(car(a) / b, [=](){return div(cdr(a), b);}, "div");
}

template<class T1, class T2 = decltype(1.0 / T1())>
Stream<T2> inv(Stream<T1> a){
    return div(unit(1.0), a);
}

bool is_empty_range(int begin, int end, int step){
    if (step == 0)
        throw exception();
    return begin == end || (step > 0) ^ (begin < end);
}

Stream<int> range(int begin, int end, int step = 1){
    return is_empty_range(begin, end, step) ? _Stream<int>::Empty
            : stream<int>(begin, [=](){return range(begin + step, end, step);}, "range");
}

Stream<int> range(int end){
    return range(0, end, 1);
}

template<class T = int>
Stream<T> integer(const T &start = 0){
    return stream<T>(start, [=](){return integer<T>(start + T(1));}, "integer");
}

Stream<int> range2(int begin, int end, int step = 1){
    return is_empty_range(begin, end, step) ? _Stream<int>::Empty
            : add(scale(slice(integer(), (end - begin) / step), step), begin);
}

template<class T = int>
Stream<T> integer2(const T &start = 0){
    Stream<T> ret = stream<T>(start, FStream<T>(), "integer2");
    ret->cdr([=](){return add(constant<T>(1), ret);});
    return ret;
}

Stream<int> fibonacci(int a = 0, int b = 1){
    return stream<int>(a, [=](){return fibonacci(b, a + b);}, "fibonacci");
}

Stream<int> fibonacci2(int a = 0, int b = 1){
    Stream<int> B = stream<int>(b, FStream<int>(), "fibonacci2");
    Stream<int> A = stream<int>(a, [=](){return B;}, "fibonacci2");
    B->cdr([=](){return add(A, B);});
    return A;
}

template<class T>
Stream<T> integral(Stream<T> a){
    return a->empty() ? _Stream<T>::Empty
            : cons<T>(T(0), dotr(a, integer<T>(1)));
}

template<class T>
Stream<T> derivation(Stream<T> a){
    return a->empty() ? _Stream<T>::Empty
            : dot<T>(cdr(a), integer<T>(1));
}

template<class T>
Stream<T> sin();

template<class T>
Stream<T> cos(){
    static Stream<T> ret;
    if (!ret)
        ret = stream<T>(T(1), [](){return cdr(scale(integral(sin<T>()), -T(1)));}, "cos");
    return ret;
}

template<class T>
Stream<T> sin(){
    static Stream<T> ret;
    if (!ret)
        ret = stream<T>(T(0), [](){return cdr(integral(cos<T>()));}, "sin");
    return ret;
}

}
