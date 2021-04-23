#pragma once
#include <array>
#include <cstddef>
#include <initializer_list>
#include <algorithm>
#include <iostream>

#include "utils/runtime_assert.hpp"


namespace JADA::Utils {

template <class T, size_t L, class S> struct MathVectorBase {
    
    static_assert(L > 0, "Math vector needs to be at least size 1.");
    static_assert(std::is_arithmetic<T>::value, "MathVector elements need to be arithmetic type.");

    // clang-format off

    //CRTP injection of access to the data, these must be implemented by all derived classes
    constexpr const T* get_ptr() const { return static_cast<const S*>(this)->get_ptr(); }
    constexpr       T* get_ptr()       { return static_cast<      S*>(this)->get_ptr(); }




    using iterator_t = T*;
    using const_iterator_t = const T*;
    using reverese_iterator_t = std::reverse_iterator<iterator_t>;
    using const_reverse_iterator_t = std::reverse_iterator<const_iterator_t>;



    template<class TT, size_t LL, class SS>
    friend std::ostream& operator<<(std::ostream& os, const MathVectorBase<TT,LL,SS>& v);


    inline constexpr size_t             size() const noexcept      {return L;}
    inline constexpr size_t             size()       noexcept      {return L;}

    inline constexpr T*                 data()      noexcept       {return get_ptr();}
    inline constexpr const T*           data()      const noexcept {return get_ptr();}
    inline constexpr iterator_t         begin()     noexcept       {return iterator_t(get_ptr());}
    inline constexpr const_iterator_t   begin()     const noexcept {return const_iterator_t(get_ptr());}
    inline constexpr const_iterator_t   cbegin()    const noexcept {return const_iterator_t(get_ptr());}
    inline constexpr iterator_t         end()       noexcept       {return iterator_t(get_ptr() + L);}
    inline constexpr const_iterator_t   end()       const noexcept {return const_iterator_t(get_ptr() + L);}
    inline constexpr const_iterator_t   cend()      const noexcept {return const_iterator_t(get_ptr() + L);}

    inline constexpr reverese_iterator_t        rbegin()          noexcept {return reverese_iterator_t(end());}
    inline constexpr const_reverse_iterator_t   rbegin()    const noexcept {return const_reverse_iterator_t(end());}
    inline constexpr const_reverse_iterator_t   crbegin()   const noexcept {return const_reverse_iterator_t(end());}

    inline constexpr reverese_iterator_t        rend()            noexcept {return reverese_iterator_t(begin());}
    inline constexpr const_reverse_iterator_t   rend()      const noexcept {return const_reverse_iterator_t(begin());}
    inline constexpr reverese_iterator_t        crend()     const noexcept {return const_reverse_iterator_t(begin());}


    inline constexpr const T& operator[](size_t idx) const { return get_ptr()[idx]; }
    inline constexpr     T& operator[](size_t idx)       { return get_ptr()[idx]; }



    inline constexpr T max() const {return *std::max_element(cbegin(), cend());}
    inline constexpr T min() const {return *std::min_element(cbegin(), cend());}
    inline constexpr T elementwise_product() const { return std::accumulate(begin(), end(), T(1), std::multiplies{});}
    inline constexpr T elementwise_sum() const { return std::accumulate(begin(), end(), T(0));}




    inline S abs() const { 
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr(); 
        for (size_t i = 0; i < L; i++) rp[i] = std::abs(tp[i]); 
        return r; 
    }

    bool non_zero() const {
        const T* tp = get_ptr();
        for (size_t i = 0; i < L; i++) { 
            if (tp[i] != T(0)) {return true;} 
        }
        return false;
    }

    template<size_t I>
    inline constexpr T get() const {return get_ptr()[I];}



    //set()
    inline constexpr void set(const T& a) {
        T* tp = get_ptr();
        for (size_t i = 0; i < L; i++) { tp[i] = a; };
    }
    inline constexpr void set(const T* ptr) {
        runtime_assert(ptr, "Invalid pointer"); T* tp = get_ptr();
        for (size_t i = 0; i < L; i++) { tp[i] = ptr[i]; };
    }

    template <class V> inline void set(const MathVectorBase<T, L, V>& v){ set(v.get_ptr()); }


    inline constexpr void set_zero() { set(0); }

    //positivate
    inline constexpr S operator+() const { return *this; }

    //negate
    inline constexpr S operator-() const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = -tp[i];
        return r;
    }

    // ================= scalar operators + - * / %
    inline constexpr S operator+(const T& a) const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] + a;
        return r;
    }
    inline constexpr S operator-(const T& a) const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] - a;
        return r;
    }
    inline constexpr S operator*(const T& a) const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] * a;
        return r;
    }
    inline constexpr S operator/(const T& a) const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] / a;
        return r;
    }
    inline constexpr S operator%(const T& a) const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] % a;
        return r;
    }

    // ================= scalar operators += -= *= /= %=
    inline constexpr S& operator=(const T& a)  { set(a);            return *static_cast<S*>(this); }
    inline constexpr S& operator+=(const T& a) { set(operator+(a)); return *static_cast<S*>(this); }
    inline constexpr S& operator-=(const T& a) { set(operator-(a)); return *static_cast<S*>(this); }
    inline constexpr S& operator*=(const T& a) { set(operator*(a)); return *static_cast<S*>(this); }
    inline constexpr S& operator/=(const T& a) { set(operator/(a)); return *static_cast<S*>(this); }
    inline constexpr S& operator%=(const T& a) { set(operator%(a)); return *static_cast<S*>(this); }
    
    
    // ================= vector operators += -= *= /= %=
    template <class V> inline constexpr S& operator=(const MathVectorBase<T, L, V>& v)  {set(v);            return *static_cast<S*>(this); }
    template <class V> inline constexpr S& operator+=(const MathVectorBase<T, L, V>& v) {set(operator+(v)); return *static_cast<S*>(this); }
    template <class V> inline constexpr S& operator-=(const MathVectorBase<T, L, V>& v) {set(operator-(v)); return *static_cast<S*>(this); }
    template <class V> inline constexpr S& operator*=(const MathVectorBase<T, L, V>& v) {set(operator*(v)); return *static_cast<S*>(this); }
    template <class V> inline constexpr S& operator/=(const MathVectorBase<T, L, V>& v) {set(operator/(v)); return *static_cast<S*>(this); }
    template <class V> inline constexpr S& operator%=(const MathVectorBase<T, L, V>& v) {set(operator%(v)); return *static_cast<S*>(this); }

    // ================= vector operators + - * / %
    template <class V>
    inline constexpr S operator+(const MathVectorBase<T, L, V>& v) const {
        const T* tp = get_ptr(); const T* vp = v.get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] + vp[i]; 
        return r;
    }

    template <class V>
    inline constexpr S operator-(const MathVectorBase<T, L, V>& v) const {
        const T* tp = get_ptr(); const T* vp = v.get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] - vp[i];
        return r;
    }

    template <class V>
    inline constexpr S operator*(const MathVectorBase<T, L, V>& v) const {
        const T* tp = get_ptr(); const T* vp = v.get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] * vp[i]; 
        return r;
    }

    template <class V>
    inline constexpr S operator/(const MathVectorBase<T, L, V>& v) const {
        const T* tp = get_ptr(); const T* vp = v.get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] / vp[i]; 
        return r;
    }
    
    template <class V>
    inline constexpr S operator%(const MathVectorBase<T, L, V>& v) const {
        const T* tp = get_ptr(); const T* vp = v.get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] % vp[i]; 
        return r;
    }

    // ================= vector comparison operators + - * / %
    template <class V>
    inline constexpr bool operator==(const MathVectorBase<T, L, V>& v) const {
        const T* tp = get_ptr(); const T* vp = v.get_ptr();
        for (size_t i = 0; i < L; i++) if (tp[i] != vp[i]) return false;
        return true;
    }

    template <class V>
    inline constexpr bool operator!=(const MathVectorBase<T, L, V>& v) const { return (!operator==(v)); }

    // clang-format on
};


template<class T, size_t L, class S>
std::ostream& operator<<(std::ostream& os, const MathVectorBase<T,L,S>& v) {

    os << "{";
    for (size_t i = 0; i < L; ++i){
        os << v[i] << " ";
    }
    os << "}"; 
    return os;

}






} // namespace JADA