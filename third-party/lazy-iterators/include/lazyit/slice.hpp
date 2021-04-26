#pragma once
#ifndef INCLUDED_LAZYIT_SLICE
#define INCLUDED_LAZYIT_SLICE

/*
 * Chris Welshman 2014
 */

#include <iterator>
#include <stdint.h>

namespace lazyit {

template<typename Iterator>
struct step_iterator {
	typedef typename std::iterator_traits<Iterator>::value_type        original_value_type;
	typedef typename std::iterator_traits<Iterator>::reference         original_reference;
	typedef typename std::iterator_traits<Iterator>::difference_type   difference_type;
	typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
	typedef original_value_type          value_type;
	typedef original_reference           reference;
	typedef Iterator                     pointer;
	typedef std::pair<Iterator,Iterator> range_type;

	step_iterator() = default;

	explicit step_iterator( const Iterator& it ) : it(it), step(1) {}

	step_iterator( const Iterator& it, difference_type step ) : it(it), step(step) {}

	reference operator*() const {
		return *it;
	}

	pointer operator->() const {
		return it;
	}

	step_iterator<Iterator>& operator++() {
		for(difference_type i=0;i<step;++i)
			++it;
		return *this;
	}

	step_iterator<Iterator> operator++(int) {
		step_iterator<Iterator> temp = *this;
		++(*this);
		return temp;
	}

	step_iterator<Iterator>& operator--() {
		for(difference_type i=0;i<step;++i)
			--it;
		return *this;
	}

	step_iterator<Iterator> operator--(int) {
		step_iterator<Iterator> temp = *this;
		--(*this);
		return temp;
	}

	step_iterator<Iterator>& operator+=( difference_type offset ) {
		it += offset * step;
		return *this;
	}

	step_iterator<Iterator> operator+( difference_type offset ) const {
		step_iterator<Iterator> temp = *this;
		return temp += offset;
	}

	step_iterator<Iterator>& operator-=( difference_type offset ) {
		return *this += -offset;
	}

	step_iterator<Iterator> operator-( difference_type offset ) const {
		step_iterator<Iterator> temp = *this;
		return temp -= offset;
	}

	difference_type operator-( const step_iterator<Iterator>& rhs ) const {
		return std::distance( rhs.it, it ) / step;
	}

	reference operator[]( difference_type offset ) const {
		return *(*this + offset);
	}

	bool operator==( const step_iterator<Iterator>& rhs ) const {
		return it == rhs.it;
	}

	bool operator!=( const step_iterator<Iterator>& rhs ) const {
		return !(*this == rhs);
	}

	bool operator<( const step_iterator<Iterator>& rhs ) const {
		return rhs - *this > 0;
	}

	bool operator>( const step_iterator<Iterator>& rhs ) const {
		return rhs < *this;
	}

	bool operator<=( const step_iterator<Iterator>& rhs ) const {
		return !( *this > rhs );
	}

	bool operator>=( const step_iterator<Iterator>& rhs ) const {
		return !( *this < rhs );
	}

protected:
	difference_type step;
	Iterator it;
};

template<typename Iterator>
struct slice_range {
	typedef typename std::iterator_traits<Iterator>::value_type      value_type;
	typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
	typedef Iterator original_iterator;
	typedef step_iterator<original_iterator>  iterator;
	typedef std::reverse_iterator<iterator>   reverse_iterator;
	typedef std::pair<Iterator,Iterator>      range_type;

	slice_range( const range_type& range, difference_type skip, difference_type count, difference_type step ) : range(range), skip(skip), count(count), step(step) {
		difference_type N = std::distance( range.first, range.second );
		count = std::min( N - skip, count );
	}

	slice_range( const range_type& range, difference_type skip, difference_type count ) : slice_range(range,skip,count,1) {}

	slice_range( const range_type& range, difference_type count ) : slice_range(range,0,count,1) {}

	iterator begin() const {
		return iterator( range.first + skip, step );
	}

	iterator end() const {
		return iterator( range.first + (skip + count), step );
	}

protected:
	range_type range;
	difference_type skip, count, step;
};

template<typename Iterator>
inline slice_range<Iterator> slice( Iterator&& first, Iterator&& last, uint32_t skip, uint32_t count, uint32_t step ) {
	return slice_range<Iterator>(
		std::make_pair(
			std::forward<Iterator>(first),
			std::forward<Iterator>(last)
		), skip, count, step
	);
}

template<typename Range>
inline auto slice( Range&& r, uint32_t skip, uint32_t count, uint32_t step ) {
	return slice(
		begin( std::forward<Range>(r) ),
		end( std::forward<Range>(r) ),
		skip, count, step
	);
}

template<typename Range>
inline auto cslice( const Range& r, uint32_t skip, uint32_t count, uint32_t step ) {
	return slice( cbegin(r), cend(r), skip, count, step );
}

template<typename Range>
inline auto slice( Range&& r, uint32_t skip, uint32_t count ) {
	return slice(
		begin( std::forward<Range>(r) ),
		end( std::forward<Range>(r) ),
		skip, count, 1
	);
}

template<typename Range>
inline auto cslice( const Range& r, uint32_t skip, uint32_t count ) {
	return slice( cbegin(r), cend(r), skip, count, 1 );
}

template<typename Range>
inline auto slice( Range&& r, uint32_t count ) {
	return slice(
		begin( std::forward<Range>(r) ),
		end( std::forward<Range>(r) ),
		0, count, 1
	);
}

template<typename Range>
inline auto cslice( const Range& r, uint32_t count ) {
	return slice( cbegin(r), cend(r), 0, count, 1 );
}

} // namespace lazyit

#endif // #ifndef INCLUDED_LAZYIT_SLICE
