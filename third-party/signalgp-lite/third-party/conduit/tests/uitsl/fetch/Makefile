TARGET_NAMES += autoinstall
TARGET_NAMES += detect_gz
TARGET_NAMES += detect_tar
TARGET_NAMES += fetch
TARGET_NAMES += inflate
TARGET_NAMES += untar

TO_ROOT := $(shell git rev-parse --show-cdup)

default: web

clean::
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f *.txt
	rm -f *.profdata
	rm -f *.profraw
	rm -f ./Coverage*
	rm -rf ./temp

web: $(addprefix web-, $(TARGET_NAMES))

web-%: %.cpp
	source $(TO_ROOT)/third-party/emsdk/emsdk_env.sh && \
	emcc -std=c++17 -pipe -DCATCH_CONFIG_MAIN -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" -s EXPORTED_FUNCTIONS="['_main']" -s DISABLE_EXCEPTION_CATCHING=0 -s FETCH=1 -s USE_ZLIB=1 -s EXIT_RUNTIME=1 --embed-file assets -o $@.js $<
	npm install
	node $@.js
	rm -rf $@.js

test:: web

clean::
	rm -f *.js
	rm -f *.wasm
	rm -f twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son
	rm -rf beautiful-is-better-than-ugly-explicit-is-better-than-implicit-simple-is-better-than-complex-complex-is-better-than-complicated-flat-is-better-than-nested
	rm -rf never
	rm -rf empty
