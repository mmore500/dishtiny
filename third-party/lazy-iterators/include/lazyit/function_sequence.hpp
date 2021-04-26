#pragma once
#ifndef INCLUDED_LAZYIT_FUNCTION_SEQUENCE
#define INCLUDED_LAZYIT_FUNCTION_SEQUENCE

/*
 * Chris Welshman 2014
 */

#include <iterator>
#include <utility>
#include <type_traits>

namespace lazyit {

template<typename F,typename State>
struct function_sequence_iterator {

	typedef typename std::result_of<F(State)>::type value_type;
	typedef const value_type& reference;
	typedef const value_type* pointer;

	typedef ptrdiff_t difference_type;
	typedef std::forward_iterator_tag iterator_category;
	typedef function_sequence_iterator<F,State> iterator;

	explicit function_sequence_iterator( const F& f ) : f(f), infinity(true) {}

	function_sequence_iterator( const F& f, const State& state ) : f(f), state(state), infinity(false) {
		this->operator++();
	}

	reference operator*() const {
		return value;
	}

	pointer operator->() const {
		return &value;
	}

	iterator& operator++() {
		value = f(state);
		return *this;
	}

	iterator operator++(int) {
		iterator temp = *this;
		++(*this);
		return temp;
	}

	iterator& operator--() {
		value = inverse(state);
		return *this;
	}

	iterator operator--(int) {
		iterator temp = *this;
		--(*this);
		return temp;
	}

	iterator& operator+=( difference_type offset ) {
		for(difference_type i=0;i<offset;++i)
			++(*this);
		for(difference_type i=0;i>offset;--i)
			--(*this);
		return *this;
	}

	iterator operator+( difference_type offset ) const {
		iterator temp = *this;
		return temp += offset;
	}

	iterator& operator-=( difference_type offset ) {
		return *this += -offset;
	}

	iterator operator-( difference_type offset ) const {
		iterator temp = *this;
		return temp -= offset;
	}

	reference operator[]( difference_type offset ) const {
		return *(*this + offset);
	}

	bool operator==( const iterator& rhs ) const {
		return infinity == rhs.infinity && ( infinity || state == rhs.state );
	}

	bool operator!=( const iterator& rhs ) const {
		return !(*this == rhs);
	}

protected:

	F f;
	State state;
	value_type value;
	bool infinity;
};

template<typename F,typename State>
struct function_sequence_range {

	typedef typename std::result_of<F(State)>::type value_type;
	typedef const value_type& reference;
	typedef const value_type* pointer;

	typedef ptrdiff_t difference_type;
	typedef std::forward_iterator_tag iterator_category;
	typedef function_sequence_iterator<F,State> iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;

	function_sequence_range( F&& f, const State& initial ) : f(f), initial(initial) {}

	iterator begin() const {
		return iterator( f, initial );
	}

	iterator end() const {
		return iterator(f);
	}

protected:
	F f;
	State initial;
};

template<typename F,typename State>
function_sequence_range<F,State> function_sequence( State&& initial, F&& f ) {
	return function_sequence_range<F,State>( std::forward<F>(f), std::forward<State>(initial) );
}

} // namespace lazyit

#endif // #ifndef INCLUDED_LAZYIT_FUNCTION_SEQUENCE
