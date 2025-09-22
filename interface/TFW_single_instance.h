/*
 * Copyright (c) 2025 TFW Team
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TFW_SINGLE_INSTANCE_H
#define TFW_SINGLE_INSTANCE_H

#ifndef TFW_EXPORT
#define TFW_EXPORT __attribute__ ((visibility ("default")))
#endif // TFW_EXPORT

namespace TFW {

#define TFW_DECLARE_SINGLE_INSTANCE_BASE(className)      \
public:                                                  \
    TFW_EXPORT static className &GetInstance();          \
                                                         \
private:                                                 \
    className(const className &) = delete;               \
    className &operator=(const className &) = delete;    \
    className(className &&) = delete;                    \
    className &operator=(className &&) = delete;

#define TFW_DECLARE_SINGLE_INSTANCE(className)  \
    TFW_DECLARE_SINGLE_INSTANCE_BASE(className) \
                                                \
private:                                        \
    className() = default;                      \
    ~className() = default;

#define TFW_IMPLEMENT_SINGLE_INSTANCE(className)     \
    TFW_EXPORT className &className::GetInstance()   \
    {                                                \
        static className instance;                   \
        return instance;                             \
    }
    
} // namespace TFW

#endif // TFW_SINGLE_INSTANCE_H