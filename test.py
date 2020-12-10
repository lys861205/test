#/bin/python
# -*- coding: utf-8 -*-

from cfg_reader import *

if __name__=='__main__':
    h = {}
    cfgReader('homework.conf', h)
    print h