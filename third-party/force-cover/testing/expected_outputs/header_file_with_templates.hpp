/**
 *  @note This file is part of Empirical, https://github.com/devosoft/Empirical
 *  @copyright Copyright (C) Michigan State University, MIT Software license; see doc/LICENSE.md
 *  @date 2016-2018
 *
 *  @file vector.h
 *  @brief A drop-in wrapper for std::vector; adds on bounds checking in debug mode.
 *  @note Status: BETA
 *
 *  This class is a drop-in wrapper for std::vector, adding on bounds checking.
 *  If EMP_NDEBUG is set then it reverts back to std::vector.
 *
 *  @todo Debug code: member functions that take iterators should also take emp iterators that verify
 *        whether those iterators are valid.
 */


#ifndef EMP_VECTOR_H
#define EMP_VECTOR_H

#include <initializer_list>
#include <iterator>
#include <utility>
#include <vector>

#include "assert.h"

#ifdef EMP_NDEBUG

// Seemlessly translate emp::vector to std::vector
namespace emp {
  template <typename... Ts> using vector = std::vector<Ts...>;
}


#else // #EMP_NDEBUG *not* set

namespace emp {

  /// Build a debug wrapper emp::vector around std::vector.
  template <typename T, typename... Ts>
  class vector : public std::vector<T,Ts...> {
  private:
    using this_t = emp::vector<T,Ts...>;
    using stdv_t = std::vector<T,Ts...>;

    /// Setup a threshold; if we try to make a vector bigger than MAX_SIZE, throw a warning.
    constexpr static const size_t MAX_SIZE = 2000000001; // 2x10^9 + 1

  public:
    /// Setup a revision number - iterators must match the revision of their vector.
    int revision;

    /// Setup an iterator wrapper to make sure that they're not used again after a vector changes.
    template<typename ITERATOR_T>
    struct iterator_wrapper : public ITERATOR_T {
      using this_t = iterator_wrapper<ITERATOR_T>;
      using wrapped_t = ITERATOR_T;
      using vec_t = emp::vector<T,Ts...>;

      /// What vector and revision was this iterator created from?
      const vec_t * v_ptr;
      int revision;

      // @CAO: For the moment cannot create an emp iterator from a base since we don't know vector to use.
      // iterator_wrapper(const ITERATOR_T & _in)
      //   : ITERATOR_T(_in), v_ptr(nullptr), revision(0) { ; }
      iterator_wrapper(const ITERATOR_T & _in, const vec_t * _v)
        : ITERATOR_T(_in), v_ptr(_v), revision(_v->revision) {/*_FORCE_COVER_START_*/ ; }/*_FORCE_COVER_END_*/
      /*_FORCE_COVER_START_*/iterator_wrapper(const this_t &) = default/*_FORCE_COVER_END_*/;
      /*_FORCE_COVER_START_*/iterator_wrapper(this_t &&) = default/*_FORCE_COVER_END_*/;
      ~iterator_wrapper() {/*_FORCE_COVER_START_*/ ; }/*_FORCE_COVER_END_*/

      // Debug tools to make sure this iterator is okay.
      bool OK(bool begin_ok=true, bool end_ok=true) const {/*_FORCE_COVER_START_*/
        if (v_ptr == nullptr) return false;                // Invalid vector
        if (v_ptr->revision == 0) return false;            // Vector has been deleted!
        if (revision != v_ptr->revision) return false;     // Vector has changed memory!
        size_t pos = (size_t) (*this - v_ptr->begin());
        if (pos > v_ptr->size()) return false;             // Iterator out of range.
        if (!begin_ok && pos == 0) return false;           // Iterator not allowed at beginning.
        if (!end_ok && pos == v_ptr->size()) return false; // Iterator not allowed at end.
        return true;
      }/*_FORCE_COVER_END_*/

      /*_FORCE_COVER_START_*/this_t & operator=(const this_t &) = default/*_FORCE_COVER_END_*/;
      /*_FORCE_COVER_START_*/this_t & operator=(this_t &&) = default/*_FORCE_COVER_END_*/;

      operator ITERATOR_T() {/*_FORCE_COVER_START_*/ return *this; }/*_FORCE_COVER_END_*/
      operator const ITERATOR_T() const {/*_FORCE_COVER_START_*/ return *this; }/*_FORCE_COVER_END_*/

      auto & operator*() {/*_FORCE_COVER_START_*/
        emp_assert(OK(true, false));  // Ensure vector hasn't changed since making iterator.
        return wrapped_t::operator*();
      }/*_FORCE_COVER_END_*/
      const auto & operator*() const {/*_FORCE_COVER_START_*/
        emp_assert(OK(true, false));  // Ensure vector hasn't changed since making iterator.
        return wrapped_t::operator*();
      }/*_FORCE_COVER_END_*/

      auto operator->() {/*_FORCE_COVER_START_*/
        emp_assert(OK(true, false));  // Ensure vector hasn't changed since making iterator.
        return wrapped_t::operator->();
      }/*_FORCE_COVER_END_*/
      const auto operator->() const {/*_FORCE_COVER_START_*/
        emp_assert(OK(true, false));  // Ensure vector hasn't changed since making iterator.
        return wrapped_t::operator->();
      }/*_FORCE_COVER_END_*/

      this_t & operator++() {/*_FORCE_COVER_START_*/ emp_assert(OK(true,false)); wrapped_t::operator++(); return *this; }/*_FORCE_COVER_END_*/
      this_t operator++(int x) {/*_FORCE_COVER_START_*/ emp_assert(OK(true,false)); return this_t(wrapped_t::operator++(x), v_ptr); }/*_FORCE_COVER_END_*/
      this_t & operator--() {/*_FORCE_COVER_START_*/ emp_assert(OK(false,true)); wrapped_t::operator--(); return *this; }/*_FORCE_COVER_END_*/
      this_t operator--(int x) {/*_FORCE_COVER_START_*/ emp_assert(OK(false,true)); return this_t(wrapped_t::operator--(x), v_ptr); }/*_FORCE_COVER_END_*/

      auto operator+(int in) {/*_FORCE_COVER_START_*/ emp_assert(OK()); return this_t(wrapped_t::operator+(in), v_ptr); }/*_FORCE_COVER_END_*/
      auto operator-(int in) {/*_FORCE_COVER_START_*/ emp_assert(OK()); return this_t(wrapped_t::operator-(in), v_ptr); }/*_FORCE_COVER_END_*/
      auto operator-(const this_t & in) {/*_FORCE_COVER_START_*/ emp_assert(OK()); return ((wrapped_t) *this) - (wrapped_t) in; }/*_FORCE_COVER_END_*/

      this_t & operator+=(int in) {/*_FORCE_COVER_START_*/ emp_assert(OK()); wrapped_t::operator+=(in); return *this; }/*_FORCE_COVER_END_*/
      this_t & operator-=(int in) {/*_FORCE_COVER_START_*/ emp_assert(OK()); wrapped_t::operator-=(in); return *this; }/*_FORCE_COVER_END_*/
      auto & operator[](int offset) {/*_FORCE_COVER_START_*/ emp_assert(OK()); return wrapped_t::operator[](offset); }/*_FORCE_COVER_END_*/

      //bool operator==(const wrapped_t & in) const { emp_assert(OK()); return wrapped_t::operator==(in); }
      //bool operator!=(const wrapped_t & in) const { emp_assert(OK()); return wrapped_t::operator!=(in); }

      //bool operator< (const wrapped_t & in) const { emp_assert(OK()); return wrapped_t::operator< (in); }
      //bool operator<=(const wrapped_t & in) const { emp_assert(OK()); return wrapped_t::operator<=(in); }
      //bool operator> (const wrapped_t & in) const { emp_assert(OK()); return wrapped_t::operator> (in); }
      //bool operator>=(const wrapped_t & in) const { emp_assert(OK()); return wrapped_t::operator>=(in); }
    };

    using iterator = iterator_wrapper< typename stdv_t::iterator >;
    using const_iterator = iterator_wrapper< typename stdv_t::const_iterator >;
    using reverse_iterator = iterator_wrapper< typename stdv_t::reverse_iterator >;
    using const_reverse_iterator = iterator_wrapper< typename stdv_t::const_reverse_iterator >;
    using value_type = T;
    using size_type = typename stdv_t::size_type;
    using reference = typename stdv_t::reference;
    using const_reference = typename stdv_t::const_reference;

    vector() : stdv_t(), revision(1) {/*_FORCE_COVER_START_*/}/*_FORCE_COVER_END_*/;
    vector(const this_t & _in) : stdv_t(_in), revision(1) {/*_FORCE_COVER_START_*/}/*_FORCE_COVER_END_*/;
    vector(size_t size) : stdv_t(size), revision(1) {/*_FORCE_COVER_START_*/ emp_assert(size < MAX_SIZE, size); }/*_FORCE_COVER_END_*/
    vector(size_t size, const T & val) : stdv_t(size, val), revision(1) {/*_FORCE_COVER_START_*/ emp_assert(size < MAX_SIZE, size); }/*_FORCE_COVER_END_*/
    vector(std::initializer_list<T> in_list) : stdv_t(in_list), revision(1) {/*_FORCE_COVER_START_*/ ; }/*_FORCE_COVER_END_*/
    vector(const stdv_t & in) : stdv_t(in), revision(1) {/*_FORCE_COVER_START_*/ ; }/*_FORCE_COVER_END_*/         // Emergency fallback conversion.
    template <typename InputIt>
    vector(InputIt first, InputIt last) : stdv_t(first, last), revision(1) {/*_FORCE_COVER_START_*//*_FORCE_COVER_START_*/ ; }/*_FORCE_COVER_END_*//*_FORCE_COVER_END_*/
    ~vector() {/*_FORCE_COVER_START_*/ revision = 0; }/*_FORCE_COVER_END_*/ // Clear out revision when vector is deleted.

    size_t size() const {/*_FORCE_COVER_START_*/ return stdv_t::size(); }/*_FORCE_COVER_END_*/

    iterator begin() noexcept {/*_FORCE_COVER_START_*/ return iterator(stdv_t::begin(), this); }/*_FORCE_COVER_END_*/
    const_iterator begin() const noexcept {/*_FORCE_COVER_START_*/ return const_iterator(stdv_t::begin(), this); }/*_FORCE_COVER_END_*/
    iterator end() noexcept {/*_FORCE_COVER_START_*/ return iterator(stdv_t::end(), this); }/*_FORCE_COVER_END_*/
    const_iterator end() const noexcept {/*_FORCE_COVER_START_*/ return const_iterator(stdv_t::end(), this); }/*_FORCE_COVER_END_*/

    // operator stdv_t &() { return v; }
    // operator const stdv_t &() const { return v; }

    void resize(size_t new_size) {/*_FORCE_COVER_START_*/
      emp_assert(new_size < MAX_SIZE, new_size);
      stdv_t::resize(new_size);
      revision++;
    }/*_FORCE_COVER_END_*/
    void resize(size_t new_size, const T & val) {/*_FORCE_COVER_START_*/
      emp_assert(new_size < MAX_SIZE, new_size);
      stdv_t::resize(new_size, val);
      revision++;
    }/*_FORCE_COVER_END_*/
    /*_FORCE_COVER_START_*/this_t & operator=(const this_t &) = default/*_FORCE_COVER_END_*/;

    T & operator[](size_t pos) {/*_FORCE_COVER_START_*/
      emp_assert(pos < stdv_t::size(), pos, stdv_t::size());
      return stdv_t::operator[](pos);
    }/*_FORCE_COVER_END_*/

    const T & operator[](size_t pos) const {/*_FORCE_COVER_START_*/
      emp_assert(pos < stdv_t::size(), pos, stdv_t::size());
      return stdv_t::operator[](pos);
    }/*_FORCE_COVER_END_*/

    T & back() {/*_FORCE_COVER_START_*/ emp_assert(stdv_t::size() > 0); return stdv_t::back(); }/*_FORCE_COVER_END_*/
    const T & back() const {/*_FORCE_COVER_START_*/ emp_assert(stdv_t::size() > 0); return stdv_t::back(); }/*_FORCE_COVER_END_*/
    T & front() {/*_FORCE_COVER_START_*/ emp_assert(stdv_t::size() > 0); return stdv_t::front(); }/*_FORCE_COVER_END_*/
    const T & front() const {/*_FORCE_COVER_START_*/ emp_assert(stdv_t::size() > 0); return stdv_t::front(); }/*_FORCE_COVER_END_*/

    template <typename... PB_Ts>
    void push_back(PB_Ts &&... args) {/*_FORCE_COVER_START_*//*_FORCE_COVER_START_*/
      stdv_t::push_back(std::forward<PB_Ts>(args)...);
      revision++;
    }/*_FORCE_COVER_END_*//*_FORCE_COVER_END_*/

    void pop_back() {/*_FORCE_COVER_START_*/
      emp_assert(stdv_t::size() > 0, stdv_t::size());
      stdv_t::pop_back();
      revision++;           // Technically reducing size can cause memory reallocation, but less likely.
    }/*_FORCE_COVER_END_*/

    template <typename... ARGS>
    iterator insert(ARGS &&... args) {/*_FORCE_COVER_START_*//*_FORCE_COVER_START_*/
      ++revision;
      return iterator( stdv_t::insert(std::forward<ARGS>(args)...), this );
    }/*_FORCE_COVER_END_*//*_FORCE_COVER_END_*/

    template <typename... ARGS>
    iterator erase(ARGS &&... args) {/*_FORCE_COVER_START_*//*_FORCE_COVER_START_*/
      ++revision;
      return iterator( stdv_t::erase(std::forward<ARGS>(args)...), this );
    }/*_FORCE_COVER_END_*//*_FORCE_COVER_END_*/

    template <typename... ARGS>
    iterator emplace(ARGS &&... args) {/*_FORCE_COVER_START_*//*_FORCE_COVER_START_*/
      ++revision;
      return iterator( stdv_t::emplace(std::forward<ARGS>(args)...), this );
    }/*_FORCE_COVER_END_*//*_FORCE_COVER_END_*/

    template <typename... ARGS>
    void emplace_back(ARGS &&... args) {/*_FORCE_COVER_START_*//*_FORCE_COVER_START_*/
      stdv_t::emplace_back(std::forward<ARGS>(args)...);
      revision++;
    }/*_FORCE_COVER_END_*//*_FORCE_COVER_END_*/
  };

  /// Build a specialized debug wrapper for emp::vector<bool>
  template <typename... Ts>
  class vector<bool, Ts...> : public std::vector<bool,Ts...> {
  private:
    using this_t = emp::vector<bool,Ts...>;
    using stdv_t = std::vector<bool,Ts...>;

    /// Setup a threshold; if we try to make a vector bigger than MAX_SIZE, throw a warning.
    constexpr static const size_t MAX_SIZE = 2000000001;

  public:
    using iterator = typename stdv_t::iterator;
    using const_iterator = typename stdv_t::const_iterator;
    using value_type = bool;
    using size_type = typename stdv_t::size_type;
    using reference = typename stdv_t::reference;
    using const_reference = typename stdv_t::const_reference;

    vector() : stdv_t() {/*_FORCE_COVER_START_*/}/*_FORCE_COVER_END_*/;
    vector(const this_t & _in) : stdv_t(_in) {/*_FORCE_COVER_START_*/}/*_FORCE_COVER_END_*/;
    vector(size_t size) : stdv_t(size) {/*_FORCE_COVER_START_*/ emp_assert(size < MAX_SIZE, size); }/*_FORCE_COVER_END_*/
    vector(size_t size, bool val) : stdv_t(size, val) {/*_FORCE_COVER_START_*/ emp_assert(size < MAX_SIZE, size); }/*_FORCE_COVER_END_*/
    vector(std::initializer_list<bool> in_list) : stdv_t(in_list) {/*_FORCE_COVER_START_*/ ; }/*_FORCE_COVER_END_*/
    vector(const stdv_t & in) : stdv_t(in) {/*_FORCE_COVER_START_*/ ; }/*_FORCE_COVER_END_*/         // Emergency fallback conversion.
    template <typename InputIt>
    vector(InputIt first, InputIt last) : stdv_t(first, last){/*_FORCE_COVER_START_*//*_FORCE_COVER_START_*/;}/*_FORCE_COVER_END_*//*_FORCE_COVER_END_*/


    // operator stdv_t &() { return v; }
    // operator const stdv_t &() const { return v; }

    void resize(size_t new_size) {/*_FORCE_COVER_START_*/ emp_assert(new_size < MAX_SIZE, new_size); stdv_t::resize(new_size); }/*_FORCE_COVER_END_*/
    void resize(size_t new_size, bool val) {/*_FORCE_COVER_START_*/
      emp_assert(new_size < MAX_SIZE, new_size);
      stdv_t::resize(new_size, val);
    }/*_FORCE_COVER_END_*/
    /*_FORCE_COVER_START_*/this_t & operator=(const this_t &) = default/*_FORCE_COVER_END_*/;

    auto operator[](size_t pos) -> decltype(stdv_t::operator[](pos)) {/*_FORCE_COVER_START_*/
      emp_assert(pos < stdv_t::size(), pos, stdv_t::size());
      return stdv_t::operator[](pos);
    }/*_FORCE_COVER_END_*/

    bool operator[](size_t pos) const {/*_FORCE_COVER_START_*/
      emp_assert(pos < stdv_t::size(), pos, stdv_t::size());
      return stdv_t::operator[](pos);
    }/*_FORCE_COVER_END_*/

    auto & back() {/*_FORCE_COVER_START_*/ emp_assert(stdv_t::size() > 0); return stdv_t::back(); }/*_FORCE_COVER_END_*/
    bool back() const {/*_FORCE_COVER_START_*/ emp_assert(stdv_t::size() > 0); return stdv_t::back(); }/*_FORCE_COVER_END_*/
    auto & front() {/*_FORCE_COVER_START_*/ emp_assert(stdv_t::size() > 0); return stdv_t::front(); }/*_FORCE_COVER_END_*/
    bool front() const {/*_FORCE_COVER_START_*/ emp_assert(stdv_t::size() > 0); return stdv_t::front(); }/*_FORCE_COVER_END_*/

    void pop_back() {/*_FORCE_COVER_START_*/
      emp_assert(stdv_t::size() > 0, stdv_t::size());
      stdv_t::pop_back();
    }/*_FORCE_COVER_END_*/
  };

}

// A crude, generic printing function for vectors.
template <typename T, typename... Ts>
std::ostream & operator<<(std::ostream & out, const emp::vector<T,Ts...> & v) {/*_FORCE_COVER_START_*/
  for (const T & x : v) out << x << " ";
  return out;
}/*_FORCE_COVER_END_*/

template <typename T, typename... Ts>
std::istream & operator>>(std::istream & is, emp::vector<T,Ts...> & v) {/*_FORCE_COVER_START_*/
  for (T & x : v) is >> x;
  return is;
}/*_FORCE_COVER_END_*/

#endif

// namespace emp {
//   template<typename T, typename... Ts> struct TypeID< emp::vector<T,Ts...> > {
//     static std::string GetName() {
//       using simple_vt = emp::vector<T>;
//       using full_vt = emp::vector<T,Ts...>;
//       if (std::is_same<simple_vt,full_vt>::value) {
//         return "emp::vector<" + TypeID<T>::GetName() + ">";
//       }
//       return "emp::vector<" + TypeID<TypePack<T,Ts...>>::GetTypes() + ">";
//     }
//   };
// }

#endif
