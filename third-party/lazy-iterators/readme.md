Lazy Iterators
==============

C++ iterators for iterating over various sets where the elements are computed on demand, without the need to precompute or store the set. Features sets of pairs (Cartesian product, distinct pairs, zip), subsets (filter, slice), and function applications (map, reduce, function_sequence).

A longer introduction can be read here:

https://cwzx.wordpress.com/2014/04/15/iterating-over-distinct-pairs-in-cpp/

### Product

product(X,Y) is the Cartesian product of X and Y, which is the set of all pairs (x,y). There are N*M pairs, where N is the size of X and M is the size of Y.

pairs(X) = product(X,X). There are N^2 pairs.

### Distinct Pairs

distinct_pairs(X) is the set of all pairs (x,y) such that x and y are different instances and the order doesn't matter, so (x,y) is the same distinct pair as (y,x). These are known in combinatorics as '2-combinations'. There are 'N choose 2' distinct pairs, which is N(N-1)/2.

### Zip

zip(X,Y) is the set of element-wise pairings, e.g.

    zip( (1,2,3), (4,5,6) ) = { (1,4), (2,5), (3,6) }

The number of pairs is min(M,N), i.e. if one of the lists is longer than the other, its extra elements will be ignored.

### Filter

filter(X,f) is the subset of elements satisfying the predicate f.

    filter( {1,2,3}, f ) = { 1, 3 }, where f(x) = ( x % 2 != 0 ).

### Slice

slice(X,skip,count,step) is a subset of X. The first skip elements are skipped, the following count elements are iterated through with a step size. Defaults: skip=0, step=1.

### Map

map(X,f) is the set of element-wise evaluations of the function f.

    map( {1,2,3}, f ) = { f(1), f(2), f(3) }

### Reduce

reduce(X,f) is the single value obtained by repeated application of the binary function f.

    reduce( {1,2,3}, f ) = f( f(1,2), 3 ).

### Integer Interval

integer_interval(a,b) is a closed interval of integers, [a..b]. The integer type is templated, so you can use any data type that behaves like an integer.

### Function Sequence

function_sequence(initial,f) is a sequence produced by repeated application of a function f to an initial state. Each application mutates the state and returns a value. The sequence can only be iterated forward.

### Invertible Function Sequence

invertible_function_sequence(initial,f,finv) is a sequence produced by repeated application of an invertible function f with inverse finv to an initial state. Each application mutates the state and returns a value. The sequence supports bidirectional iteration.


Usage Notes
-----------

The pairs iterators' dereference operations return a pair of references, not a reference. Therefore you should receive this by-value, not by-reference (e.g. in the range-based for loop).

Examples
--------

### Fibonacci

```cpp
auto fibonacci = function_sequence( make_pair(1,0),
	[]( auto& p ) {
		auto temp = p.first + p.second;
		p.first = p.second;
		return p.second = temp;
	}
);

for( auto f : fibonacci ) {
	if( f > 1000 ) break;
	cout << f << endl;
}

/* Output:
1
1
2
3
5
8
13
21
34
55
89
144
233
377
610
987
*/
```

### Primes

```cpp
auto primes( int lower, int upper ) {
	upper = max(upper,2); lower = min(max(lower,2),upper);
	return filter( integer_interval( lower, upper ),
		[]( auto i ) {
			auto tests = integer_interval( 2, max((int)sqrt(i),2) );
			auto results = map( tests, [i]( auto j ) { return (i % j) != 0; } );
			return reduce( results, []( bool x, bool y ) { return x && y; } );
		}
	);
}

for( auto p : primes(100,150) ) {
	cout << p << endl;
}

/* Output:
101
103
107
109
113
127
131
137
139
149
*/
```

### Pythagorean Triples

```cpp
auto range = integer_interval( 1, 100 );

auto triples = product( range, distinct_pairs(range) ); // (z,(x,y))

auto pythagorean_triples = filter( triples,
	[]( auto t ) {
		return t.second.first*t.second.first + t.second.second*t.second.second == t.first*t.first;
	}
);

for( auto t : pythagorean_triples ) {
	cout << "( " << t.second.first << ", " << t.second.second << ", " << t.first << " )" << endl;
}

/* Output:
( 3, 4, 5 )
( 6, 8, 10 )
( 5, 12, 13 )
( 9, 12, 15 )
( 8, 15, 17 )
( 12, 16, 20 )
( 7, 24, 25 )
( 15, 20, 25 )
( 10, 24, 26 )
( 20, 21, 29 )
( 18, 24, 30 )
( 16, 30, 34 )
( 21, 28, 35 )
( 12, 35, 37 )
( 15, 36, 39 )
( 24, 32, 40 )
( 9, 40, 41 )
( 27, 36, 45 )
( 14, 48, 50 )
( 30, 40, 50 )
( 24, 45, 51 )
( 20, 48, 52 )
( 28, 45, 53 )
( 33, 44, 55 )
( 40, 42, 58 )
( 36, 48, 60 )
( 11, 60, 61 )
( 16, 63, 65 )
( 25, 60, 65 )
( 33, 56, 65 )
( 39, 52, 65 )
( 32, 60, 68 )
( 42, 56, 70 )
( 48, 55, 73 )
( 24, 70, 74 )
( 21, 72, 75 )
( 45, 60, 75 )
( 30, 72, 78 )
( 48, 64, 80 )
( 18, 80, 82 )
( 13, 84, 85 )
( 36, 77, 85 )
( 40, 75, 85 )
( 51, 68, 85 )
( 60, 63, 87 )
( 39, 80, 89 )
( 54, 72, 90 )
( 35, 84, 91 )
( 57, 76, 95 )
( 65, 72, 97 )
( 28, 96, 100 )
( 60, 80, 100 )
*/
```

### Distinct pairs of distinct pairs

```cpp
auto v = integer_interval( 1, 4 );

for( auto p : distinct_pairs(distinct_pairs(v)) ) {
    cout << "( ( " << p.first.first << ", " << p.first.second << " ), ( " << p.second.first << ", " << p.second.second << " ) )" << endl;
}

/* Output:
( ( 1, 2 ), ( 1, 3 ) )
( ( 1, 2 ), ( 1, 4 ) )
( ( 1, 2 ), ( 2, 3 ) )
( ( 1, 2 ), ( 2, 4 ) )
( ( 1, 2 ), ( 3, 4 ) )
( ( 1, 3 ), ( 1, 4 ) )
( ( 1, 3 ), ( 2, 3 ) )
( ( 1, 3 ), ( 2, 4 ) )
( ( 1, 3 ), ( 3, 4 ) )
( ( 1, 4 ), ( 2, 3 ) )
( ( 1, 4 ), ( 2, 4 ) )
( ( 1, 4 ), ( 3, 4 ) )
( ( 2, 3 ), ( 2, 4 ) )
( ( 2, 3 ), ( 3, 4 ) )
( ( 2, 4 ), ( 3, 4 ) )
*/
```
