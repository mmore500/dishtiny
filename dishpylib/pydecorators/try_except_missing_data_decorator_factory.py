import functools

def try_except_missing_data_decorator_factory( job_name ):

    def try_except_missing_data_decorator( func ):

        @functools.wraps( func )
        def try_except_missing_data_wrapper(*args, **kwargs):
            try:
                return func(*args, **kwargs)
            except ValueError:
                print(f'missing {job_name}, skipping')

        return try_except_missing_data_wrapper

    return try_except_missing_data_decorator
