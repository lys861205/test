#!/bin/python
# -*- coding: utf-8 -*-

import os
import time
import sys



# 模块的开始
STATE_SECTION_FLAG=0
# 模块数组的开始
STATE_SECTION_ARRAY_FLAG=1
# key -- value
STATE_SECTION_VALUE=2
# 数组
STATE_SECTION_ARRAY_VALUE=3

# 解析状态
g_status = STATE_SECTION_FLAG

# 文件行数计数器
g_line = 0

MAX_DEPTH = 10

def stringStrip(str, seplist):
    tmpStr = str
    for i in range (len(seplist)):
        tmpStr = tmpStr.strip(seplist[i])

    return tmpStr

def stringSplit(data, sep):
    try:
        arr = data.split(sep)
        if len(arr) >= 2:
            return arr[0].strip(' '), arr[1].strip(' ')
        elif len(arr) == 1:
            return arr[0].strip(' '), ""
        else:
            return "", ""
    except:
        return "", ""

def getLine(fp):
    global  g_line
    line = fp.next()
    g_line += 1
    data = stringStrip(line, ['\t', '\n', ' '])
    if len(data) == 0 or data[0] == '#':
        return False, data
    else:
        return True, data

#brief 解析tag信息
def parseTag(data):
    d = data
    depth = 0
    idx = 0
    global g_status
    for i in range(len(d)):
        if d[i] == '.':
            depth += 1
            idx   += 1
        else:
            break
    if d[idx] == '@':  # section
        g_status = STATE_SECTION_ARRAY_FLAG
        d = d[idx+1:]
    else:                # section array
        g_status = STATE_SECTION_FLAG
        d = d[idx:]
    key, value = stringSplit(d, ']')
    return depth, key

# brief   解析一行数据
# param   line 数据
# return  返回参数3个，配置块的深度，key， value
def parseLine(line):
    try:
        global  g_status
        data = line
        if data[0] == '[':      # sub
            depth, key = parseTag(data[1:])
            return depth, key, ''

        elif data[0] == '@':    # array
            g_status = STATE_SECTION_ARRAY_VALUE
            data=data[1:]
            k, v = stringSplit(data, '=')
            return MAX_DEPTH, k, v

        else:     # k-v
            g_status = STATE_SECTION_VALUE
            k, v = stringSplit(data, '=')
            return MAX_DEPTH, k, v
    except:
        pass

#breif 解析数组
def getArrayList(fp, depth, a_list=[]):
    try:
        while True:
            ret, data = getLine(fp)
            if ret == False:
                continue
            dep, key, value = parseLine(data)
            if g_status == STATE_SECTION_ARRAY_VALUE:
                a_list.append(value)
            else:
                return dep, key, value
    except StopIteration, e:
        pass
    except Exception, e:
        pass

#brief 解析子配置块
def parseSection(fp, depth, s_hash={}):
    try:
        while True:
            ret, data = getLine(fp)
            if ret==False:
                continue
            dep, key, value = parseLine(data)
            if g_status == STATE_SECTION_VALUE:
                s_hash[key] = value
                continue
            elif g_status == STATE_SECTION_ARRAY_VALUE:
                s_hash[key] = [value]
                dep, key, value = getArrayList(fp, depth, s_hash[key])
            while dep > depth:
                if g_status == STATE_SECTION_FLAG:
                    s_hash[key] = {}
                    dep, key, value = parseSection(fp, dep, s_hash[key])
                elif g_status == STATE_SECTION_ARRAY_FLAG:
                    if s_hash.has_key(key) == False:
                        s_hash[key] = []
                    dep, key, value = parseSecArr(fp, dep, s_hash[key])
            return dep, key, value
    except StopIteration, e:
        return -1, "", ""
    except Exception, e:
        return -1, "", ""

#brief 解析子配置块数组
def parseSecArr(fp, depth, s_list=[]):
    s_hash = {}
    dep=-1
    key=''
    value=''
    try:
        while True:
            ret, data = getLine(fp)
            if ret==False:
                continue
            dep, key, value = parseLine(data)
            if g_status == STATE_SECTION_VALUE:
                s_hash[key] = value
                continue
            elif g_status == STATE_SECTION_ARRAY_VALUE:
                s_hash[key] = [value]
                dep, key, value = getArrayList(fp, depth, s_hash[key])

            while dep > depth:
                if g_status == STATE_SECTION_FLAG:
                    s_hash[key] = {}
                    dep, key, value = parseSection(fp, dep, s_hash[key])
                elif g_status == STATE_SECTION_ARRAY_FLAG:
                    if s_hash.has_key(key) == False:
                        s_hash[key] = []
                    dep, key, value = parseSecArr(fp, dep, s_hash[key])
            s_list.append(s_hash)
            return dep, key, value
    except StopIteration:
        s_list.append(s_hash)
        return -1, "", ""
    except Exception, e:
        s_list.append(s_hash)
        return -1, "", ""

def cfgReader(file_in, cfg_hash={}):
    ret = None
    if len(file_in) == 0:
        return False
    fp = None
    try:
        isQuit = False
        fp = open(file_in, "r")
        while isQuit==False:
            r, data = getLine(fp)
            if r == False:
                continue
            depth, key, value = parseLine(data)
            isQuit = True
            while True:
                cfg_hash[key] = {}
                depth, key, value = parseSection(fp, depth, cfg_hash[key])
                if depth == -1:
                    ret = True
                    break
            break
    except  StopIteration, e:
       ret = True
    except Exception, e:
        print e
        ret = False
    finally:
        if fp:
            fp.close()
        return ret
