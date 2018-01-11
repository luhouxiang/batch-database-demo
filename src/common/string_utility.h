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


#ifndef _COMMON_STRING_UTILITY_H_
#define _COMMON_STRING_UTILITY_H_

#include <string>
#include <vector>
#include <json/json.h>

class StringUtility {
public:
    static void Split(const std::string& str,
        const std::string& delim,
        std::vector<std::string>& result);

    static std::string& Ltrim(std::string& str);
    static std::string& Rtrim(std::string& str);
    static std::string& Trim(std::string& str);
    static void Trim(std::vector<std::string>* str_list);

    static std::string to_string(int arg, int dec=1);
    static std::string to_string(float arg, int dec=2);
    static std::string to_string(double arg, int dec=2);
    static double to_double(const std::string& arg);
};


#endif // _COMMON_STRING_UTILITY_H_
