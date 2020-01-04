#include <bits/stdc++.h>
#include "stream.h"
#include "fractional.h"

using namespace StreamTool;

using Fractional::Frac;

using std::vector;
using std::cout;
using std::endl;
using std::ostream;

template<typename T>
vector<T> to_vector(Stream<T> s, size_t limit = 10){
    vector<T> ret;
    for (; !s->empty() && limit > 0; --limit){
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

void test_tool(){
    auto a = constant(1);
    auto b = integer();
    auto c = scale(a, 2);
    auto c2 = scale2(a, 2);
    auto d = add(b, c);
    auto d2 = add2(b, c);
    auto e = dot(d, d);
    auto e2 = dot2(d, d);
    auto f = sub(d, c);
    auto f2 = sub2(d, c);
    auto f3 = sub3(d, c);
    auto g = mul(f, f);

    cout << "a " << to_vector(slice(a, 10)) << endl;
    cout << "b " << to_vector(slice(b, 10)) << endl;
    cout << "c " << to_vector(slice(c, 10)) << endl;
    cout << "c2 " << to_vector(slice(c2, 10)) << endl;
    cout << "d " << to_vector(slice(d, 10)) << endl;
    cout << "d2 " << to_vector(slice(d2, 10)) << endl;
    cout << "e " << to_vector(slice(e, 10)) << endl;
    cout << "e2 " << to_vector(slice(e2, 10)) << endl;
    cout << "f " << to_vector(slice(f, 10)) << endl;
    cout << "f2 " << to_vector(slice(f2, 10)) << endl;
    cout << "f3 " << to_vector(slice(f3, 10)) << endl;
    cout << "g " << to_vector(slice(g, 10)) << endl;
}

void test_type(){
    auto a = constant(1.1);
    auto b = mul(a, a);

    cout << to_vector(slice(b, 10)) << endl;
}

void test_range(){
    auto a = range(10);
    auto b = range(1, 12, 3);
    auto b2 = range(1, 12, 3);
    auto c = range(1, -12, -3);
    auto c2 = range(1, -12, -3);

    cout << "a " << to_vector(a) << endl;
    cout << "b " << to_vector(b) << endl;
    cout << "b2 " << to_vector(b2) << endl;
    cout << "c " << to_vector(c) << endl;
    cout << "c2 " << to_vector(c2) << endl;
}

void test_fibonacci(){
    auto a = fibonacci();
    auto b = fibonacci2();

    cout << "a " << to_vector(a) << endl;
    cout << "b " << to_vector(slice(b, 10)) << endl;
}

void test_slice(){
    auto a = range(1, 12, 3);
    auto b = slice(integer(), 1, 12, 3);

    cout << "a " << to_vector(a) << endl;
    cout << "b " << to_vector(b) << endl;
}

void test_div(){
    auto a = unit(1.0);
    auto b = div(a, a);
    auto c = cons(1, unit(-1));
    auto d = inv(c);
    auto e = cons(1, unit(1));
    auto f = inv(e);
    auto g = cons(1.0, unit(-2.01));
    auto h = inv(g);

    cout << "a " << to_vector(a) << endl;
    cout << "b " << to_vector(b) << endl;
    cout << "c " << to_vector(c) << endl;
    cout << "d " << to_vector(d) << endl;
    cout << "e " << to_vector(e) << endl;
    cout << "f " << to_vector(f, 20) << endl;
    cout << "g " << to_vector(g) << endl;
    cout << "h " << to_vector(h) << endl;
}

void test_sin(){
    auto a = constant(1.0);
    auto b = integral(a);
    auto c = derivation(b);
    auto d = sin<double>();
    auto e = cos<double>();
    auto f = sin<Frac<long long>>();
    auto g = cos<Frac<long long>>();

    cout << "a " << to_vector(a) << endl;
    cout << "b " << to_vector(b) << endl;
    cout << "c " << to_vector(c) << endl;
    cout << "d " << to_vector(d) << endl;
    cout << "e " << to_vector(e) << endl;
    cout << "f " << to_vector(f) << endl;
    cout << "g " << to_vector(g) << endl;
}

int main(){
    test_type();
    test_tool();
    test_range();
    test_fibonacci();
    test_slice();
    test_div();
    test_sin();

    return 0;
}
