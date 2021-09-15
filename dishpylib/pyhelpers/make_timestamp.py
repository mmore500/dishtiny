from datetime import datetime, timezone
import deprecation

@deprecation.deprecated(details='use nbmetalog package instead')
def make_timestamp():
    return datetime.now().replace(
        tzinfo=timezone.utc,
    ).replace(
        microsecond=0,
    ).isoformat().replace('+', 'Z')
