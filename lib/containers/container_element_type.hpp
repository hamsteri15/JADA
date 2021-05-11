#pragma once

namespace JADA {

template <class T> struct ContainerElementType {

    using type = typename T::value_type; 
        
        
        
        // typename
                    // std::remove_reference<decltype(*std::begin(c))>::type

    //    using T = std::remove_reference<decltype(vec[0])>::type;
};

} // namespace JADA