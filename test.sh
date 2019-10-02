#!/bin/bash
try() {
    expected="$1"
    input="$2"

    ./tcc "$input" > tmp.s
    gcc -static -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
	echo "$input => $actual"
    else
	echo "$expected expected, but got $actual"
	exit 1
    fi
}

try 0 'return 0;'
try 42 'return 42;'
try 21 'return 5+20-4;'
try 41 "return  12 + 34 - 5;"
try 47 'return 5+6*7;'
try 15 'return 5*(9-6);'
try 4 'return (3+5)/2;'

try 10 'return -10+20;'
try 10 'return - -10;'
try 10 'return - - +10;'
try 10 'return + - -10;'

try 0 'return 0==1;'
try 1 'return 34==34;'
try 1 'return 0!=1;'
try 0 'return 34!=34;'

try 1 'return 0<1;'
try 0 'return 1<1;'
try 0 'return 2<1;'
try 1 'return 0<=1;'
try 1 'return 1<=1;'
try 0 'return 2<=1;'

try 1 'return 1>0;'
try 0 'return 1>1;'
try 0 'return 1>2;'
try 1 'return 1>=0;'
try 1 'return 1>=1;'
try 0 'return 1>=2;'

try 7 '1+2; return 3+4;'
try 8 'a=8; return a;'
try 3 'b=1+2; return b;'
try 3 'a=1; return a+2;'

try 10 'r25=10; return r25;'
try 3 'foo=1;bar=2; return foo+bar;'

echo OK
