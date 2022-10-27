from datetime import datetime, timezone

def make_timestamp():
    return datetime.now().replace(
        tzinfo=timezone.utc,
    ).replace(
        microsecond=0,
    ).isoformat().replace('+', 'Z')
