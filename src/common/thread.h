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


#ifndef _TS_COMMON_THREAD_H_
#define _TS_COMMON_THREAD_H_

#include <pthread.h>

class Thread
{
public:
    Thread();
    virtual ~Thread();

    virtual void Run() = 0;

    bool Start();

    bool Join();

    void Exit();
private:
    ::pthread_t m_thread_id;
};

#endif // _TS_COMMON_THREAD_H_

