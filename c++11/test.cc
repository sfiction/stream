#include <bits/stdc++.h>
#include "stream.h"

using namespace StreamTool;

using std::vector;
using std::cout;
using std::endl;
using std::ostream;

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
    auto a = constant(1);
    auto b = integer();
    auto c = scale(a, 2);
    auto d = mul(a, a);
    auto e = mul(a, d);

    auto f = constant(1.1);
    auto g = scale(f, 2);
    auto h = mul(g, g);

    cout << to_vector(slice(a, 10)) << endl;
    cout << to_vector(slice(b, 10)) << endl;
    cout << to_vector(slice(c, 10)) << endl;
    cout << to_vector(slice(d, 10)) << endl;
    cout << to_vector(slice(e, 10)) << endl;

    cout << to_vector(slice(h, 10)) << endl;

    return 0;
}
