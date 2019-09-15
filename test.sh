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

try 0 '0;'
try 42 '42;'
try 21 '5+20-4;'
try 41 " 12 + 34 - 5;"
try 47 '5+6*7;'
try 15 '5*(9-6);'
try 4 '(3+5)/2;'

try 10 '-10+20;'
try 10 '- -10;'
try 10 '- - +10;'
try 10 '+ - -10;'

try 0 '0==1;'
try 1 '34==34;'
try 1 '0!=1;'
try 0 '34!=34;'

try 1 '0<1;'
try 0 '1<1;'
try 0 '2<1;'
try 1 '0<=1;'
try 1 '1<=1;'
try 0 '2<=1;'

try 1 '1>0;'
try 0 '1>1;'
try 0 '1>2;'
try 1 '1>=0;'
try 1 '1>=1;'
try 0 '1>=2;'

try 7 '1+2; 3+4;'
try 8 'a=8;'
try 3 'b=1+2;'
try 3 'a=1;a+2;'

echo OK
