#pragma once

#include <type_traits>

namespace Monitoring {

template<class, class = void> struct needs_checker : std::false_type
{
};
template<class T> struct needs_checker<T, std::void_t<typename std::decay_t<T>::needs_checker>> : std::true_type
{
};
template<typename T> inline constexpr bool needs_checker_v = needs_checker<T>::value;

template<class, class = void> struct is_condition_checker : std::false_type
{
};
template<class T>
struct is_condition_checker<T, std::void_t<typename std::decay_t<T>::is_condition_checker>> : std::true_type
{
};
template<typename T> inline constexpr bool is_condition_checker_v = is_condition_checker<T>::value;

}// namespace Monitoring