#pragma once

#include <concepts>

//TODO: check if this could be replaced with something from standard

namespace JADA{

/*
template<typename T>
struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value>::type>
{
   static constexpr bool value = true;
};
*/


template <typename T> concept Iterator = requires(T iter) {
    //requires std::iterator_traits<T>::value_type;
    { *iter }->auto;
    requires std::incrementable<T>;
    //{ std::end(range) }->std::same_as<Iterator_type<T>>;
    //requires std::same_as<std::iterator<Iterator_type<T>>>;
    //requires std::iterator<Iterator_type<T>>>;
};


}