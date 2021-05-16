import functools

def only_every_nth_stint_decorator_factory( n ):

    def only_every_nth_stint_decorator( func ):

        @functools.wraps( func )
        def only_every_nth_stint_wrapper(*args, **kwargs):
            if kwargs['stint'] % n == 0:
                return func(*args, **kwargs)
            else:
                print(f'only available every {n}th stint, passing')

        return only_every_nth_stint_wrapper

    return only_every_nth_stint_decorator
