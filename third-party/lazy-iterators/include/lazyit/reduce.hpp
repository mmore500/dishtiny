#pragma once
#ifndef INCLUDED_LAZYIT_REDUCE
#define INCLUDED_LAZYIT_REDUCE

/*
 * Chris Welshman 2014
 */

#include <iterator>
#include <type_traits>

namespace lazyit {

template<typename Range,typename F>
inline auto reduce( Range&& c, F&& f ) {
	auto it = begin(c);
	auto e = end(c);
	std::decay_t<decltype(*it)> x{};
	if( it != e ) {
		x = *it;
		for(++it;it!=e;++it) {
			x = f(x,*it);
		}
	}
	return x;
}

template<typename Range,typename F>
inline auto creduce( const Range& c, F&& f ) {
	auto it = cbegin(c);
	auto e = cend(c);
	std::decay_t<decltype(*it)> x{};
	if( it != e ) {
		x = *it;
		for(++it;it!=e;++it) {
			x = f(x,*it);
		}
	}
	return x;
}

template<typename Range,typename T,typename F>
inline auto reduce( Range&& c, T x, F&& f ) {
	auto it = begin(c);
	auto e = end(c);
	if( it != e ) {
		x = *it;
		for(++it;it!=e;++it) {
			x = f(x,*it);
		}
	}
	return x;
}

template<typename Range,typename T,typename F>
inline auto creduce( const Range& c, T x, F&& f ) {
	auto it = cbegin(c);
	auto e = cend(c);
	if( it != e ) {
		x = *it;
		for(++it;it!=e;++it) {
			x = f(x,*it);
		}
	}
	return x;
}

} // namespace lazyit

#endif // #ifndef INCLUDED_LAZYIT_REDUCE
