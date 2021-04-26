#pragma once
#ifndef INCLUDED_LAZYIT_MAP
#define INCLUDED_LAZYIT_MAP

/*
 * Chris Welshman 2014
 */

#include <iterator>
#include <utility>
#include <type_traits>

namespace lazyit {

template<typename F,typename Iterator>
struct map_iterator {
	typedef typename std::iterator_traits<Iterator>::value_type        original_value_type;
	typedef typename std::iterator_traits<Iterator>::difference_type   difference_type;
	typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename std::result_of<F(original_value_type)>::type      value_type;
	typedef std::pair<Iterator,Iterator> range_type;
	typedef const value_type* pointer;
	typedef value_type reference;

	map_iterator() = default;

	map_iterator( const F& f, const range_type& range ) : f(f), range(range), it(range.first) {}

	map_iterator( const F& f, const range_type& range, const Iterator& it ) : f(f), range(range), it(it) {}

	map_iterator( const F& f, const Iterator& first, const Iterator& last ) : map_iterator(f,range_type(first,last)) {}

	map_iterator( const F& f, const Iterator& first, const Iterator& last, const Iterator& it ) : map_iterator(f,range_type(first,last),it) {}

	value_type operator*() const {
		return f(*it);
	}

	map_iterator<F,Iterator>& operator++() {
		++it;
		return *this;
	}

	map_iterator<F,Iterator> operator++(int) {
		map_iterator<F,Iterator> temp = *this;
		++(*this);
		return temp;
	}

	map_iterator<F,Iterator>& operator--() {
		--it;
		return *this;
	}

	map_iterator<F,Iterator> operator--(int) {
		map_iterator<F,Iterator> temp = *this;
		--(*this);
		return temp;
	}

	map_iterator<F,Iterator>& operator+=( difference_type offset ) {
		it += offset;
		return *this;
	}

	map_iterator<F,Iterator> operator+( difference_type offset ) const {
		map_iterator<F,Iterator> temp = *this;
		return temp += offset;
	}

	map_iterator<F,Iterator>& operator-=( difference_type offset ) {
		return *this += -offset;
	}

	map_iterator<F,Iterator> operator-( difference_type offset ) const {
		map_iterator<F,Iterator> temp = *this;
		return temp -= offset;
	}

	difference_type operator-( const map_iterator<F,Iterator>& rhs ) const {
		return std::distance( rhs.it, it );
	}

	value_type operator[]( difference_type offset ) const {
		return *(*this + offset);
	}

	difference_type index() const {
		return N = std::distance( range.first, it );
	}

	bool operator==( const map_iterator<F,Iterator>& rhs ) const {
		return it == rhs.it;
	}

	bool operator!=( const map_iterator<F,Iterator>& rhs ) const {
		return !(*this == rhs);
	}

	bool operator<( const map_iterator<F,Iterator>& rhs ) const {
		return rhs - *this > 0;
	}

	bool operator>( const map_iterator<F,Iterator>& rhs ) const {
		return rhs < *this;
	}

	bool operator<=( const map_iterator<F,Iterator>& rhs ) const {
		return !( *this > rhs );
	}

	bool operator>=( const map_iterator<F,Iterator>& rhs ) const {
		return !( *this < rhs );
	}

protected:
	range_type range;
	Iterator it;
	F f;
};

template<typename F,typename Iterator>
struct map_range {
	typedef typename std::iterator_traits<Iterator>::value_type      original_value_type;
	typedef typename std::result_of<F(original_value_type)>::type    value_type;
	typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
	typedef Iterator original_iterator;
	typedef map_iterator<F,original_iterator> iterator;
	typedef std::reverse_iterator<iterator>   reverse_iterator;
	typedef std::pair<Iterator,Iterator>      range_type;

	map_range( const F& f, const range_type& range ) : f(f), range(range) {}

	map_range( const F& f, const Iterator& first, const Iterator& last ) : map_range(f,range_type(first,last)) {}

	difference_type size() const {
		return std::distance( range.first, range.second );
	}

	iterator begin() const {
		return iterator( f, range );
	}

	iterator end() const {
		return iterator( f, range, range.second );
	}

protected:
	range_type range;
	F f;
};

template<typename F,typename Iterator>
inline map_range<F,Iterator> map( Iterator&& first, Iterator&& last, F&& f ) {
	return map_range<F,Iterator>( std::forward<F>(f),
		std::make_pair(
			std::forward<Iterator>(first),
			std::forward<Iterator>(last)
		)
	);
}

template<typename Range,typename F>
inline auto map( Range&& r, F&& f ) {
	return map(
		begin( std::forward<Range>(r) ),
		end( std::forward<Range>(r) ),
		std::forward<F>(f)
	);
}

template<typename Range,typename F>
inline auto cmap( const Range& r, F&& f ) {
	return map(
		cbegin( r ),
		cend( r ),
		std::forward<F>(f)
	);
}

} // namespace lazyit

#endif // #ifndef INCLUDED_LAZYIT_MAP
