#pragma once
#ifndef INCLUDED_LAZYIT_PRODUCT
#define INCLUDED_LAZYIT_PRODUCT

/*
 * Chris Welshman 2014
 */

#include <type_traits>
#include <utility>
#include <iterator>

namespace lazyit {

template<typename It1,typename It2>
struct product_iterator {
	typedef typename std::iterator_traits<It1>::value_type        value_type_1;
	typedef typename std::iterator_traits<It1>::reference         reference_1;
	typedef typename std::iterator_traits<It1>::difference_type   difference_type_1;
	typedef typename std::iterator_traits<It1>::iterator_category iterator_category_1;
	typedef std::pair<It1,It1>                                    pair_type_1;

	typedef typename std::iterator_traits<It2>::value_type        value_type_2;
	typedef typename std::iterator_traits<It2>::reference         reference_2;
	typedef typename std::iterator_traits<It2>::difference_type   difference_type_2;
	typedef typename std::iterator_traits<It2>::iterator_category iterator_category_2;
	typedef std::pair<It2,It2>                                    pair_type_2;

	typedef std::pair<value_type_1,value_type_2> value_type;
	typedef std::pair<reference_1,reference_2>   reference;
	typedef std::pair<It1,It2>                   pair_type;
	typedef const pair_type*                     pointer;
	typedef std::pair<pair_type_1,pair_type_2>   range_type;

	//typedef typename std::common_type<difference_type_1,difference_type_2>::type difference_type;  // fatal error C1001: An internal error has occurred in the compiler.
	typedef difference_type_2   difference_type;
	typedef iterator_category_2 iterator_category;

	product_iterator() = default;

	explicit product_iterator( const range_type& range ) : range(range) {
		pair.first = range.first.first;
		pair.second = range.second.first;
	}

	product_iterator( const pair_type_1& range_1, const pair_type_2& range_2 ) : product_iterator(range_type(range_1,range_2)) {}

	product_iterator( const range_type& range, const pair_type& pair ) : range(range), pair(pair) {}

	reference operator*() const {
		return reference( *pair.first, *pair.second );
	}

	pointer operator->() const {
		return &pair;
	}

	reference_1 first() const {
		return reference_1( *pair.first );
	}

	reference_2 second() const {
		return reference_2( *pair.second );
	}

	product_iterator<It1,It2>& operator++() {
		++pair.second;
		if( pair.second == range.second.second ) {
			++pair.first;
			pair.second = range.second.first;
		}
		return *this;
	}

	product_iterator<It1,It2> operator++(int) {
		product_iterator<It1,It2> temp = *this;
		++(*this);
		return temp;
	}

	product_iterator<It1,It2>& operator--() {
		if( pair.second == range.second.first ) {
			pair.second = range.second.second;
			--pair.first;
		} else {
			--pair.second;
		}
		return *this;
	}

	product_iterator<It1,It2> operator--(int) {
		product_iterator<It1,It2> temp = *this;
		--(*this);
		return temp;
	}

	product_iterator<It1,It2>& operator+=( difference_type offset ) {
		difference_type N2 = std::distance( range.second.first, range.second.second );
		difference_type k = index(N2) + offset;
		difference_type i = k / N2;
		difference_type j = k % N2;
		pair.first = range.first.first + i;
		pair.second = range.second.first + j;
		return *this;
	}

	product_iterator<It1,It2> operator+( difference_type offset ) const {
		product_iterator<It1,It2> temp = *this;
		return temp += offset;
	}

	product_iterator<It1,It2>& operator-=( difference_type offset ) {
		return *this += -offset;
	}

	product_iterator<It1,It2> operator-( difference_type offset ) const {
		product_iterator<It1,It2> temp = *this;
		return temp -= offset;
	}

	difference_type operator-( const product_iterator<It1,It2>& rhs ) const {
		difference_type dfirst = std::distance( rhs.pair.first, pair.first );
		difference_type dsecond = std::distance( rhs.pair.second, pair.second );
		difference_type N2 = std::distance( range.second.first, range.second.second );
		return dfirst * N2 + dsecond;
	}

	reference operator[]( difference_type offset ) const {
		return *(*this + offset);
	}

	difference_type index() const {
		difference_type N2 = std::distance( range.second.first, range.second.second );
		return index(N2);
	}

	bool operator==( const product_iterator<It1,It2>& rhs ) const {
		return pair == rhs.pair;
	}

	bool operator!=( const product_iterator<It1,It2>& rhs ) const {
		return !(*this == rhs);
	}

	bool operator<( const product_iterator<It1,It2>& rhs ) const {
		return rhs - *this > 0;
	}

	bool operator>( const product_iterator<It1,It2>& rhs ) const {
		return rhs < *this;
	}

	bool operator<=( const product_iterator<It1,It2>& rhs ) const {
		return !( *this > rhs );
	}

	bool operator>=( const product_iterator<It1,It2>& rhs ) const {
		return !( *this < rhs );
	}

protected:
	range_type range;
	pair_type pair;

	difference_type index( difference_type N2 ) const {
		return index(
			std::distance( range.first.first, pair.first ),
			std::distance( range.second.first, pair.second ),
			N2
		);
	}

	difference_type index( difference_type i, difference_type j, difference_type N2 ) const {
		return N2*i + j;
	}

};

template<typename It1,typename It2>
struct product_range {
	typedef typename std::iterator_traits<It1>::value_type      value_type_1;
	typedef typename std::iterator_traits<It2>::value_type      value_type_2;
	typedef typename std::iterator_traits<It1>::difference_type difference_type_1;
	typedef typename std::iterator_traits<It2>::difference_type difference_type_2;
	typedef typename std::common_type<difference_type_1,difference_type_2>::type difference_type;
	typedef It1 iterator_1;
	typedef It2 iterator_2;
	typedef product_iterator<iterator_1,iterator_2> iterator;
	typedef std::reverse_iterator<iterator>         reverse_iterator;
	typedef std::pair<It1,It1>                      pair_type_1;
	typedef std::pair<It2,It2>                      pair_type_2;
	typedef std::pair<pair_type_1,pair_type_2>      range_type;
	typedef std::pair<value_type_1,value_type_2>    value_type;

	explicit product_range( const range_type& range ) : range(range) {}

	product_range( const pair_type_1& range_1, const pair_type_2& range_2 ) : range(range_type(range_1,range_2)) {}

	difference_type size() const {
		difference_type N1 = std::distance( range.first.first, range.first.second );
		difference_type N2 = std::distance( range.second.first, range.second.second );
		return N1 * N2;
	}

	iterator begin() const {
		return iterator( range );
	}

	iterator end() const {
		return iterator(
			range,
			std::make_pair( range.first.second, range.second.first )
		);
	}

protected:
	range_type range;
};

template<typename It1,typename It2>
inline product_range<It1,It2> product( It1&& first_1, It1&& last_1, It2&& first_2, It2&& last_2 ) {
	return product_range<It1,It2>(
		std::make_pair(
			std::forward<It1>(first_1),
			std::forward<It1>(last_1)
		),
		std::make_pair(
			std::forward<It2>(first_2),
			std::forward<It2>(last_2)
		)
	);
}

template<typename R1,typename R2>
inline auto product( R1&& r1, R2&& r2 ) {
	return product(
		begin( std::forward<R1>(r1) ),
		end( std::forward<R1>(r1) ),
		begin( std::forward<R2>(r2) ),
		end( std::forward<R2>(r2) )
	);
}

template<typename R1,typename R2>
inline auto cproduct( const R1& r1, const R2& r2 ) {
	return product(
		cbegin( r1 ),
		cend( r1 ),
		cbegin( r2 ),
		cend( r2 )
	);
}

template<typename Range>
inline auto pairs( Range&& r ) {
	return product( std::forward<Range>(r), std::forward<Range>(r) );
}

template<typename Range>
inline auto cpairs( const Range& r ) {
	return cproduct( r, r );
}

} // namespace lazyit

#endif // #ifndef INCLUDED_LAZYIT_PRODUCT
