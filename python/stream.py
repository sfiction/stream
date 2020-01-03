#!/bin/env python3

from functools import *
from itertools import *

class Stream(object):
    def __init__(self, car, cdr=None, empty=False):
        """
        Args:
            car: any
            cdr: lambda: Stream
        """
        self.car = car
        self.cdr_ = cdr
        self.empty = empty

        self.computed_ = False

    @property
    def cdr(self):
        if not self.computed_:
            self.cdr_ = self.cdr_()
            self.computed_ = True
        return self.cdr_

    def __add__(self, s):
        return add(self, s)

    def __mul__(self, s):
        return mul(self, s)

    def __iter__(self):
        return to_iter(self)

emtpy = Stream(None, None, True)

def cons_stream(car, cdr):
    """
    Args:
        car: any
        cdr: Stream
    """
    return Stream(car, lambda: cdr)

def to_iter(s):
    while not s.empty:
        yield s.car
        s = s.cdr

def map(f, s):
    return Stream(f(s.car), lambda: map(f, s.cdr))

def map2(f, s):
    return fmap(f, s)

def starmap(f, s):
    return Stream(f(*s.car), lambda: starmap(f, s.cdr))

def zip(*args):
    return Stream((s.car for s in args), lambda: zip(s.cdr for s in args))

def fmap(f, *args):
    return starmap(zip(*args))

def slice(s, end):
    return empty if n == 0 else Stream(s.car, lambda: slice(s.cdr, n - 1))

# 
def add(a, b):
    return Stream(a.car + b.car, lambda: add(a.cdr, b.cdr))

def add2(a, b):
    return fmap(lambda x, y: x + y, a, b)

def scale(s, e):
    return Stream(s.car * e, lambda: scale(s.cdr, e))

def scale2(s, e):
    return map(lambda x: x * e, s)

def mul(a, b):
    return Stream(a.car * b.car, lambda: scale(a.cdr, b.car) + mul(a, b.cdr))

def display(s):
    def f(x):
        print(x)
        return x
    return map(f, s)

def constant(c):
    ret = Stream(c)
    ret.cdr_ = lambda: ret
    return ret

def natural_number():
    ret = Stream(0)
    ret.cdr_ = lambda: add(ret, constant(1))
    return ret

# results
def fibonacci():
    a, b = Stream(0), Stream(1)
    a.cdr_ = lambda: b
    b.cdr_ = lambda: add(a, b)
    return a

# tests
def tslice(s, *args):
    if not args:
        args = [10]
    return list(islice(iter(s), *args))

def test_constant():
    s = constant(7)
    assert tslice(s, 10) == [7] * 10

def test_scale():
    a = natural_number()
    e = 2
    b = scale(a, e)
    c = scale2(a, e)
    assert tslice(b) == [x * e for x in tslice(a)]
    assert tslice(c) == [x * e for x in tslice(a)]

def test_natural_number():
    s = natural_number()
    assert tslice(s, 10) == [x for x in range(10)]

def test_fibonacci():
    s = fibonacci()
    assert tslice(s, 10) == [0, 1, 1, 2, 3, 5, 8, 13, 21, 34]

def test_mul():
    s = constant(1)
    m = display(mul(s, s))
    n = natural_number()
    assert tslice(m, 10) == tslice(n, 1, 11)

if __name__ == '__main__':
    test_constant()
    test_scale()
    test_natural_number()
    test_fibonacci()
    test_mul()

    print(tslice(constant(1), 10))
    print(tslice(constant(1) * constant(1), 10))
    print(tslice(constant(1) * natural_number(), 10))
    print(tslice(natural_number() * natural_number(), 10))
