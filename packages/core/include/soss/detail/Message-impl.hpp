/*
 * Copyright (C) 2018 Open Source Robotics Foundation
 *
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
 *
*/

#ifndef SOSS__DETAIL__MESSAGEIMPL_HPP
#define SOSS__DETAIL__MESSAGEIMPL_HPP

#include <soss/Message.hpp>

#include <type_traits>
#include <soss/utilities.hpp>

namespace soss {

namespace detail {

//==============================================================================
template<typename T>
using Raw_t = std::remove_cv_t<std::remove_reference_t<T>>;

} // namespace detail

//==============================================================================
template<typename T>
void Field::set(T&& data)
{
  // converts the native type to soss type before putting it in the field, not doing
  // so may cause later conversions to fail.
  auto* soss_val = new SossType<T>();
  try
  {
    soss::Convert<T>::to_soss(std::forward<T>(data), *soss_val);

    _set(typeid(detail::Raw_t<SossType<T>>),
         soss_val,
         [](const void* other) -> void* {
           return new detail::Raw_t<SossType<T>>(
             *static_cast<const detail::Raw_t<SossType<T>>*>(other));
         },
         [](void* expired) { delete static_cast<detail::Raw_t<SossType<T>>*>(expired); });
  }
  catch (...)
  {
    delete soss_val;
    throw;
  }
}

//==============================================================================
template<typename T>
T* Field::cast()
{
  return static_cast<T*>(_cast(typeid(detail::Raw_t<T>)));
}

//==============================================================================
template<typename T>
const T* Field::cast() const
{
  return static_cast<const T*>(_cast(typeid(detail::Raw_t<T>)));
}

} // namespace soss

#endif // SOSS__DETAIL__MESSAGEIMPL_HPP
