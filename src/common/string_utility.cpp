/*
 * Tsfree is pleased to support the open source community by making Tsbase available.
 * Copyright (C) 2016 THL A29 Limited, a Tsfree company. All rights reserved.
 * Licensed under the MIT License (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 *
 */

#include <string.h>
#include <algorithm>
#include <iostream>  

#include "string_utility.h"

double StringUtility::to_double(const string& arg) 
{
    double d = 0.0;
    try
    {
        d = atof(arg.c_str());
    }
    catch(...)
    {

    }
    return d;
}
string StringUtility::to_string(int arg, int dec/*=1*/)
{
    char buf[30] = { 0 };
    try
    {
        sprintf(buf, "%0*d", dec, arg);
    }
    catch (...)
    {
    }
    return buf;
}

string StringUtility::to_string(float arg, int dec/*=2*/)
{
    return to_string((double)arg, dec);
}

string StringUtility::to_string(double arg, int dec/*=2*/)
{
    char decbuf[5] = { 0 };   
    char buf[30] = { 0 };
    try
    {
        sprintf(decbuf, "%%.%df", dec);
        sprintf(buf, decbuf, arg);
    }
    catch (...)
    {
    }
    return buf;
}

string& StringUtility::Ltrim(string& str) { // NOLINT
    string::iterator it = find_if(str.begin(), str.end(), std::not1(std::ptr_fun(::isspace)));
    str.erase(str.begin(), it);
    return str;
}

string& StringUtility::Rtrim(std::string& str) { // NOLINT
    string::reverse_iterator it = find_if(str.rbegin(),
        str.rend(), std::not1(std::ptr_fun(::isspace)));

    str.erase(it.base(), str.end());
    return str;
}

string& StringUtility::Trim(string& str) { // NOLINT
    return Rtrim(Ltrim(str));
}

void StringUtility::Trim(vector<string>* str_list) {
    if (NULL == str_list) {
        return;
    }

    vector<string>::iterator it;
    for (it = str_list->begin(); it != str_list->end(); ++it) {
        *it = Trim(*it);
    }
}

string StringUtility::replace_all(const string& src, const string& old_str, 
                                  const string& new_str)
{
    string name = src;
    string::size_type startpos = 0;
    while (startpos != string::npos)
    {
        startpos = name.find(old_str, startpos);
        if (startpos != string::npos) 
        {
            name.replace(startpos, old_str.size(), new_str);
            startpos += new_str.size();
        }
    }
    return name;
}