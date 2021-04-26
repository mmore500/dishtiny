import configparser
import json
import sys

config = configparser.ConfigParser()
config.read(sys.argv[1])
res = {
  'schemaVersion' : 1,
  'label' : 'version',
  'message' : config['bumpversion']['current_version'],
  'color' : 'orange',
}
print(json.dumps(res, sort_keys=True, indent=4))
