#pragma once
#ifndef INCLUDED_LAZYIT_DISTINCT_PAIRS
#define INCLUDED_LAZYIT_DISTINCT_PAIRS

/*
 * Chris Welshman 2014
 */

#include <iterator>
#include <utility>

namespace lazyit {

template<typename Iterator>
struct distinct_pairs_iterator {
	typedef typename std::iterator_traits<Iterator>::value_type        original_value_type;
	typedef typename std::iterator_traits<Iterator>::reference         original_reference;
	typedef typename std::iterator_traits<Iterator>::difference_type   difference_type;
	typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;

	typedef std::pair<original_value_type,original_value_type> value_type;
	typedef std::pair<original_reference,original_reference>   reference;
	typedef std::pair<Iterator,Iterator>                       pair_type;
	typedef const pair_type*                                   pointer;

	distinct_pairs_iterator() = default;

	explicit distinct_pairs_iterator( const pair_type& range ) : range(range) {
		pair.first = range.first;
		pair.second = range.first;
		if( range.first != range.second )
			++pair.second;
	}

	distinct_pairs_iterator( const Iterator& first, const Iterator& last ) : distinct_pairs_iterator(pair_type(first,last)) {}

	distinct_pairs_iterator( const pair_type& range, const pair_type& pair ) : range(range), pair(pair) {}

	reference operator*() const {
		return reference( *pair.first, *pair.second );
	}

	pointer operator->() const {
		return &pair;
	}

	original_reference first() const {
		return *pair.first;
	}

	original_reference second() const {
		return *pair.second;
	}

	distinct_pairs_iterator<Iterator>& operator++() {
		++pair.second;
		if( pair.second == range.second ) {
			++pair.first;
			pair.second = pair.first;
			++pair.second;
		}
		return *this;
	}

	distinct_pairs_iterator<Iterator> operator++(int) {
		distinct_pairs_iterator<Iterator> temp = *this;
		++(*this);
		return temp;
	}

	distinct_pairs_iterator<Iterator>& operator--() {
		--pair.second;
		if( pair.second == pair.first ) {
			--pair.first;
			pair.second = range.second;
			--pair.second;
		}
		return *this;
	}

	distinct_pairs_iterator<Iterator> operator--(int) {
		distinct_pairs_iterator<Iterator> temp = *this;
		--(*this);
		return temp;
	}

	distinct_pairs_iterator<Iterator>& operator+=( difference_type offset ) {
		difference_type N = std::distance( range.first, range.second );
		difference_type k = index(N) + offset;
		difference_type i = index_i( k, N );
		difference_type j = index_j( k, i, N );
		pair.first = range.first + i;
		pair.second = range.first + j;
		return *this;
	}

	distinct_pairs_iterator<Iterator> operator+( difference_type offset ) const {
		distinct_pairs_iterator<Iterator> temp = *this;
		return temp += offset;
	}

	distinct_pairs_iterator<Iterator>& operator-=( difference_type offset ) const {
		return *this += -offset;
	}

	distinct_pairs_iterator<Iterator> operator-( difference_type offset ) const {
		distinct_pairs_iterator<Iterator> temp = *this;
		return temp -= offset;
	}

	difference_type operator-( const distinct_pairs_iterator<Iterator>& rhs ) const {
		difference_type dfirst = std::distance( rhs.pair.first, pair.first );
		difference_type dsecond = std::distance( rhs.pair.second, pair.second );
		difference_type N = std::distance( range.first, range.second );
		difference_type sumfirst = std::distance( range.first, pair.first ) + std::distance( range.first, rhs.pair.first );

		return ( ( 2*(N-1) - 1 - sumfirst ) * dfirst ) / 2 + dsecond;
	}

	reference operator[]( difference_type offset ) const {
		return *(*this + offset);
	}

	difference_type index() const {
		difference_type N = std::distance( range.first, range.second );
		return index(N);
	}

	bool operator==( const distinct_pairs_iterator<Iterator>& rhs ) const {
		if( pair == rhs.pair )
			return true;
		if( pair.first == rhs.pair.second && pair.second == rhs.pair.first )
			return true;
		return false;
	}

	bool operator!=( const distinct_pairs_iterator<Iterator>& rhs ) const {
		return !(*this == rhs);
	}

	bool operator<( const distinct_pairs_iterator<Iterator>& rhs ) const {
		return rhs - *this > 0;
	}

	bool operator>( const distinct_pairs_iterator<Iterator>& rhs ) const {
		return rhs < *this;
	}

	bool operator<=( const distinct_pairs_iterator<Iterator>& rhs ) const {
		return !( *this > rhs );
	}

	bool operator>=( const distinct_pairs_iterator<Iterator>& rhs ) const {
		return !( *this < rhs );
	}

protected:
	pair_type range, pair;

	difference_type index( difference_type N ) const {
		return index(
			std::distance( range.first, pair.first ),
			std::distance( range.first, pair.second ),
			N
		);
	}

	difference_type index( difference_type i, difference_type j, difference_type N ) const {
		return j - i + ( i * ( 2*N - ( i + 1 ) ) ) / 2 - 1;
	}

	difference_type index_i( difference_type k, difference_type N ) const {
		difference_type d = N, total = 0;
		for(difference_type a=0;a<N;++a) {
			total += --d;
			if( total > k ) return a;
		}
		return N-1;
	}

	difference_type index_j( difference_type k, difference_type i, difference_type N ) const {
		return (k + i + 1) - ( i * ( 2*N - ( i + 1 ) ) ) / 2;
	}

};

template<typename Iterator>
struct distinct_pairs_range {
	typedef typename std::iterator_traits<Iterator>::value_type      value_type;
	typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
	typedef Iterator original_iterator;
	typedef distinct_pairs_iterator<original_iterator>     iterator;
	typedef std::reverse_iterator<iterator>                reverse_iterator;
	typedef std::pair<original_iterator,original_iterator> pair_type;

	explicit distinct_pairs_range( const pair_type& range ) : range(range) {}

	distinct_pairs_range( const Iterator& first, const Iterator& last ) : range(pair_type(first,last)) {}

	difference_type size() const {
		difference_type N = std::distance( range.first, range.second );
		return ( N * ( N - 1 ) ) / 2;
	}

	iterator begin() const {
		return iterator( range );
	}

	iterator end() const {
		pair_type temp = range;
		temp.first = range.second;
		if( range.first != range.second )
			--temp.first;
		return iterator( range, temp );
	}

protected:
	pair_type range;
};

template<typename Iterator>
inline distinct_pairs_range<Iterator> distinct_pairs( Iterator&& first, Iterator&& last ) {
	return distinct_pairs_range<Iterator>(
		std::make_pair(
			std::forward<Iterator>(first),
			std::forward<Iterator>(last)
		)
	);
}

template<typename Range>
inline auto distinct_pairs( Range&& r ) {
	return distinct_pairs(
		begin( std::forward<Range>(r) ),
		end( std::forward<Range>(r) )
	);
}

template<typename Range>
inline auto cdistinct_pairs( const Range& r ) {
	return distinct_pairs(
		cbegin( r ),
		cend( r )
	);
}

} // namespace lazyit

#endif // #ifndef INCLUDED_LAZYIT_DISTINCT_PAIRS
