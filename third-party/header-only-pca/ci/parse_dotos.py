import configparser
import json
import sys

res = {
  'schemaVersion' : 1,
  'label' : 'todos',
  'message' : sys.argv[1],
  'color' : 'orange',
}
print(json.dumps(res, sort_keys=True, indent=4))
