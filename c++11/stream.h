#include <bits/stdc++.h>

namespace StreamTool{

using std::cerr;
using std::endl;
using std::string;

using std::result_of;

using std::function;
using std::shared_ptr;
using std::make_shared;

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
    _Stream(const T &car = T(), FStream<T> compute_cdr = FStream<T>(), string tag = "stream", bool empty = false)
            :car_(car), compute_cdr(compute_cdr), tag(tag), empty_(empty), is_computed(false){
        cerr << "construct " << tag << ", " << (void*)this << ", car: " << car_ << endl;
    }

    _Stream(_Stream &&r): car_(r.car_), compute_cdr(r.compute_cdr), cdr_(r.cdr_){
    }

    ~_Stream(){
        cerr << "deconstruct " << tag << ", " << (void*)this << ", car: " << car_ << ", cdr: " << (void*)cdr_.get() <<endl;
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
Stream<T> cons(T &car, Stream<T> cdr){
    return stream(car, [cdr](){return cdr;}, "cons");
}

template<class F, class T, class R = typename result_of<F(T)>::type>
Stream<R> map(F func, Stream<T> s){
    return s->empty() ? _Stream<R>::Empty : stream<R>(func(car(s)), [=](){return map(func, cdr(s));}, "map");
}

template<class T>
Stream<T> slice(Stream<T> s, size_t n){
    return n == 0 ? s->Empty : stream<T>(car(s), [s, n](){return slice(cdr(s), n - 1);}, "slice");
}

template<class T1, class T2, class T3 = decltype(T1() + T2())>
Stream<T3> add(Stream<T1> a, Stream<T2> b){
    return a->empty() || b->empty() ? _Stream<T3>::Empty
            : stream<T3>(car(a) + car(b), [=](){return add(cdr(a), cdr(b));}, "add");
}

template<class T1, class T2, class T3 = decltype(T1() * T2())>
Stream<T3> scale(Stream<T1> a, const T2 &b){
    return a->empty() ? _Stream<T3>::Empty
            : stream<T3>(car(a) * b, [=](){return scale(cdr(a), b);}, "scale");
}

template<class T1, class T2, class T3 = decltype(T1() * T2())>
Stream<T3> scale2(Stream<T1> a, const T2 &b){
    return map([=](T1 x)->T3{return x * b;}, a);
}

template<class T1, class T2, class T3 = decltype(T1() * T2())>
Stream<T3> mul(Stream<T1> a, Stream<T2> b){
    return a->empty() || b->empty() ? _Stream<T3>::Empty
            : stream<T3>(car(a) * car(b), [=](){return add(scale(cdr(a), car(b)), mul(a, cdr(b)));}, "mul");
}

template<class T>
Stream<T> constant(const T &c){
    Stream<T> ret = stream<T>(c, FStream<T>(), "constant");
    ret->cdr([ret](){return ret;});
    return ret;
}

Stream<int> integer(){
    Stream<int> ret = stream<int>(0, FStream<int>(), "integer");
    ret->cdr([ret](){return add(constant(1), ret);});
    return ret;
}

}
