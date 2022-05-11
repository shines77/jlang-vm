#!/usr/bin/python
# -*- coding: UTF-8 -*-

#coding=utf-8

import datetime
import re

def fibonacci(n):
    if (n >= 3):
        return fibonacci(n - 1) + fibonacci(n - 2);
    else:
        return 1;

print('');

# 'scanf() %u'
n = input('Input a number (1-50): ? ');
print('');

pattern = re.compile(r"\d+");
match = pattern.match(str(n));
if match:
    # print(match.group());

    print(datetime.datetime.now());
    fib_n = fibonacci(n);
    print(datetime.datetime.now());
    print('');

    print('fibonacci(%d) = %d' % (n, fib_n));
