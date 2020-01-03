#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;

using std::enable_if;
using std::is_same;

using std::iterator_traits;
using std::tuple;
using std::get;
using std::pair;
using std::vector;
using std::string;

using std::function;
using std::shared_ptr;
using std::make_shared;

template<typename T>
class _Stream;

template<typename T>
using Stream = shared_ptr<_Stream<T>>;

template<typename T>
using FStream = function<Stream<T>()>;

template<typename T>
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

template<typename T, class... Args>
Stream<T> stream(Args&&... args){
    return make_shared<_Stream<T>>(std::forward<Args>(args)...);
}

template<typename T>
Stream<T> _Stream<T>::Empty = stream<T>(T(), FStream<T>(), "empty", true);

template<typename T>
T &car(Stream<T> s){
    return s->car();
}

template<typename T>
Stream<T> cdr(Stream<T> s){
    return s->cdr();
}

template<typename T>
Stream<T> cons(T &car, Stream<T> cdr){
    return stream(car, [cdr](){return cdr;});
}

template<typename T>
Stream<T> slice(Stream<T> s, size_t n){
    return n == 0 ? s->Empty : stream<T>(car(s), [s, n](){return slice(cdr(s), n - 1);}, "slice");
}

template<typename T1, typename T2, typename T3 = decltype(T1() + T2())>
Stream<T3> add(Stream<T1> a, Stream<T2> b){
    return a->empty() || b->empty() ? _Stream<T3>::Empty
            : stream<T3>(car(a) + car(b), [=](){return add(cdr(a), cdr(b));}, "add");
}

template<typename T1, typename T2, typename T3 = decltype(T1() * T2())>
Stream<T3> scale(Stream<T1> a, const T2 &b){
    return a->empty() ? _Stream<T3>::Empty
            : stream<T3>(car(a) * b, [=](){return scale(cdr(a), b);}, "scale");
}

template<typename T1, typename T2, typename T3 = decltype(T1() * T2())>
Stream<T3> mul(Stream<T1> a, Stream<T2> b){
    return a->empty() || b->empty() ? _Stream<T3>::Empty
            : stream<T3>(car(a) * car(b), [=](){return add(scale(cdr(a), car(b)), mul(a, cdr(b)));}, "mul");
}

template<typename T>
Stream<T> constant(const T &c){
    Stream<T> ret = stream<T>(c, FStream<int>(), "constant");
    ret->cdr([ret](){return ret;});
    return ret;
}

Stream<int> integer(){
    Stream<int> ret = stream<int>(0, FStream<int>(), "integer");
    ret->cdr([ret](){return add(constant(1), ret);});
    return ret;
}

template<typename T>
vector<T> to_vector(Stream<T> s){
    vector<T> ret;
    while (!s->empty()){
        ret.push_back(car(s));
        s = cdr(s);
    }
    return ret;
}

template <typename T>
ostream &operator <<(ostream &os, const vector<T> &vec){
    os << "[";
    for (auto &elem: vec)
        os << elem << ",";
    return os << "]";
}

int main(){
    auto a = constant<int>(1);
    auto b = integer();
    auto c = scale(a, 2);
    auto d = mul(a, a);
    auto e = mul(a, d);

    cout << to_vector(slice(a, 10)) << endl;
    cout << to_vector(slice(b, 10)) << endl;
    cout << to_vector(slice(c, 10)) << endl;
    cout << to_vector(slice(d, 10)) << endl;
    cout << to_vector(slice(e, 10)) << endl;

    return 0;
}
