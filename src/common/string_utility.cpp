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

double StringUtility::to_double(const std::string& arg) 
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
std::string StringUtility::to_string(int arg, int dec/*=1*/)
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

std::string StringUtility::to_string(float arg, int dec/*=2*/)
{
    return to_string((double)arg, dec);
}

std::string StringUtility::to_string(double arg, int dec/*=2*/)
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

std::string& StringUtility::Ltrim(std::string& str) { // NOLINT
    std::string::iterator it = find_if(str.begin(), str.end(), std::not1(std::ptr_fun(::isspace)));
    str.erase(str.begin(), it);
    return str;
}

std::string& StringUtility::Rtrim(std::string& str) { // NOLINT
    std::string::reverse_iterator it = find_if(str.rbegin(),
        str.rend(), std::not1(std::ptr_fun(::isspace)));

    str.erase(it.base(), str.end());
    return str;
}

std::string& StringUtility::Trim(std::string& str) { // NOLINT
    return Rtrim(Ltrim(str));
}

void StringUtility::Trim(std::vector<std::string>* str_list) {
    if (NULL == str_list) {
        return;
    }

    std::vector<std::string>::iterator it;
    for (it = str_list->begin(); it != str_list->end(); ++it) {
        *it = Trim(*it);
    }
}