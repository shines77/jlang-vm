#!/usr/bin/python
# -*- coding: UTF-8 -*-

#coding=utf-8

import datetime
import time
import re
import os, sys

def fibonacci(n):
    if n >= 3:
        return fibonacci(n - 1) + fibonacci(n - 2)
    else:
        return 1

def python2_fibonacci_test():
    print('')

    # 'scanf() %u'
    num = raw_input('Input a number (n = 1-40): ? ');
    print('')

    pattern = re.compile(r"\d+")
    match = pattern.match(str(num))
    if match:
        # print(match.group())
        n = int(num)

        if n > 40:
            print('Error: The n value is bigger than 40.')
            os._exit(1)

        print(datetime.datetime.now())
        time_start = time.time()
        fib_n = fibonacci(n)
        time_end = time.time()
        print(datetime.datetime.now())
        print('')

        print('fibonacci(%d) = %d' % (n, fib_n))
        print('')
        print('elapsed time: %0.2f ms' % ((time_end - time_start) * 1000))
    else:
        print('Error: Please input a integer number.')

def python3_fibonacci_test():
    print('')

    # 'scanf() %u'
    num = input('Input a number (n = 1-40): ? ');
    print('')

    pattern = re.compile(r"\d+")
    match = pattern.match(str(num))
    if match:
        # print(match.group())
        n = int(num)

        if n > 40:
            print('Error: The n value is bigger than 40.')
            os._exit(1)

        print(datetime.datetime.now())
        time_start = time.time()
        fib_n = fibonacci(n)
        time_end = time.time()
        print(datetime.datetime.now())
        print('')

        print('fibonacci(%d) = %d' % (n, fib_n))
        print('')
        print('elapsed time: %0.2f ms' % ((time_end - time_start) * 1000))
    else:
        print('Error: Please input a integer number.')

if sys.version_info[0] >= 3:
    python3_fibonacci_test()
else:
    python2_fibonacci_test()
