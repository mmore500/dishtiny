# Project-specific settings
PROJECT := SignalGP
EMP_DIR := ../Empirical/include
CEREAL_DIR := ../Empirical/third-party/cereal/include

# Flags to use regardless of compiler
CFLAGS_all := -Wall -Wno-unused-function -pedantic -std=c++17 -I$(EMP_DIR)/ -I./source/hardware/SignalGP/ -I$(CEREAL_DIR)/

# Native compiler information
CXX_nat := g++
CFLAGS_nat := -O3 -DNDEBUG $(CFLAGS_all)
CFLAGS_nat_debug := -g $(CFLAGS_all)

# Emscripten compiler information
CXX_web := emcc
OFLAGS_web_all := -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" -s TOTAL_MEMORY=67108864 --js-library $(EMP_DIR)/emp/web/library_emp.js -s EXPORTED_FUNCTIONS="['_main', '_empCppCallback']" -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=1 #--embed-file configs
OFLAGS_web := -Oz -DNDEBUG
OFLAGS_web_debug := -g4 -Oz -Wno-dollar-in-identifier-extension

CFLAGS_web := $(CFLAGS_all) $(OFLAGS_web) $(OFLAGS_web_all)
CFLAGS_web_debug := $(CFLAGS_all) $(OFLAGS_web_debug) $(OFLAGS_web_all)


default: $(PROJECT)
native: $(PROJECT)
web: $(PROJECT).js
all: $(PROJECT) $(PROJECT).js

debug:	CFLAGS_nat := $(CFLAGS_nat_debug)
debug:	$(PROJECT)

debug-web:	CFLAGS_web := $(CFLAGS_web_debug)
debug-web:	$(PROJECT).js

web-debug:	debug-web

$(PROJECT):	source/native/$(PROJECT).cc
	$(CXX_nat) $(CFLAGS_nat) source/native/$(PROJECT).cc -o $(PROJECT)
	@echo To build the web version use: make web

$(PROJECT).js: source/web/$(PROJECT)-web.cc
	$(CXX_web) $(CFLAGS_web) source/web/$(PROJECT)-web.cc -o web/$(PROJECT).js

.PHONY: clean test serve

serve:
	python3 -m http.server

clean:
	rm -f $(PROJECT) web/$(PROJECT).js web/*.js.map web/*.js.map *~ source/*.o web/*.wasm web/*.wast test_debug.out test_optimized.out unit_tests.gcda unit_tests.gcno
	rm -rf test_debug.out.dSYM

test: clean
test: tests/unit_tests.cc
	$(CXX_nat) $(CFLAGS_nat_debug) -I./source/ --coverage tests/unit_tests.cc -o test_debug.out
	./test_debug.out
	# $(CXX_nat) $(CFLAGS_nat) tests/unit_tests.cc -I./source/ -o test_optimized.out
	# ./test_optimized.out

# Debugging information
print-%: ; @echo '$(subst ','\'',$*=$($*))'
