#pragma once
#ifndef INCLUDED_LAZYIT_INVERTIBLE_FUNCTION_SEQUENCE
#define INCLUDED_LAZYIT_INVERTIBLE_FUNCTION_SEQUENCE

/*
 * Chris Welshman 2014
 */

#include <iterator>
#include <utility>
#include <type_traits>

namespace lazyit {

template<typename F,typename Finverse,typename State>
struct invertible_function_sequence_iterator {

	typedef typename std::result_of<F(State)>::type value_type;
	typedef const value_type& reference;
	typedef const value_type* pointer;

	typedef ptrdiff_t difference_type;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef invertible_function_sequence_iterator<F,Finverse,State> iterator;

	invertible_function_sequence_iterator( const F& f, const Finverse& inverse ) : f(f), inverse(inverse), infinity(true) {}

	invertible_function_sequence_iterator( const F& f, const Finverse& inverse, const State& state ) : f(f), inverse(inverse), state(state), infinity(false) {
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
			++(*this);
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
	Finverse inverse;
	State state;
	value_type value;
	bool infinity;
};

template<typename F,typename Finverse,typename State>
struct invertible_function_sequence_range {

	typedef typename std::result_of<F(State)>::type value_type;
	typedef const value_type& reference;
	typedef const value_type* pointer;

	typedef ptrdiff_t difference_type;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef invertible_function_sequence_iterator<F,Finverse,State> iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;

	invertible_function_sequence_range( const F& f, const Finverse& inverse, const State& initial ) : f(f), inverse(inverse), initial(initial) {}

	iterator begin() const {
		return iterator( f, inverse, initial );
	}

	iterator end() const {
		return iterator( f, inverse );
	}

protected:
	F f;
	Finverse inverse;
	State initial;
};

template<typename F,typename Finverse,typename State>
invertible_function_sequence_range<F,Finverse,State> invertible_function_sequence( State&& initial, F&& f, Finverse&& inverse ) {
	return invertible_function_sequence_range<F,Finverse,State>(
		std::forward<F>(f),
		std::forward<Finverse>(inverse),
		std::forward<State>(initial)
	);
}

} // namespace lazyit

#endif // #ifndef INCLUDED_LAZYIT_INVERTIBLE_FUNCTION_SEQUENCE
