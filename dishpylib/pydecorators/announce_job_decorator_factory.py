import functools

def announce_job_decorator_factory( job_name ):

    def announce_job_decorator( func ):

        @functools.wraps( func )
        def announce_job_wrapper(*args, **kwargs):
            print()
            print( f'assembling {job_name}' )
            print( '---------------------------------------------------------' )
            return func(*args, **kwargs)

        return announce_job_wrapper

    return announce_job_decorator
