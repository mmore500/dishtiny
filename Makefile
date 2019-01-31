# Project-specific settings
PROJECT := dishtiny
EMP_DIR := ../Empirical/source

# Flags to use regardless of compiler
CFLAGS_all := -Wall -Wno-unused-function -I$(EMP_DIR)/

# Native compiler information
CXX_nat := g++
CFLAGS_nat := -O3 -std=c++17 -DNDEBUG $(CFLAGS_all)
CFLAGS_nat_debug := -g -pedantic -DEMP_TRACK_MEM $(CFLAGS_all)
CFLAGS_nat_profile := -pg -pedantic -DNDEBUG $(CFLAGS_all)

# Emscripten compiler information
CXX_web := emcc
OFLAGS_web_all := -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" -s TOTAL_MEMORY=134217728 --js-library $(EMP_DIR)/web/library_emp.js -s EXPORTED_FUNCTIONS="['_main', '_empCppCallback']" -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=1 -s ABORTING_MALLOC=0 #--preload-file state_grids #--embed-file configs
OFLAGS_web := -Oz -DNDEBUG -std=c++14
OFLAGS_web_debug := -g4 -pedantic -Wno-dollar-in-identifier-extension -s DEMANGLE_SUPPORT=1 -s ASSERTIONS=1
#OFLAGS_web_debug := -g4 -DNDEBUG -pedantic -Wno-dollar-in-identifier-extension -s DEMANGLE_SUPPORT=1

CFLAGS_web := $(CFLAGS_all) $(OFLAGS_web) $(OFLAGS_web_all)
CFLAGS_web_debug := $(CFLAGS_all) $(OFLAGS_web_debug) $(OFLAGS_web_all)


default: $(PROJECT)
native: $(PROJECT)
web: $(PROJECT).js
all: $(PROJECT) $(PROJECT).js

debug:	CFLAGS_nat := $(CFLAGS_nat_debug)
debug:	$(PROJECT)

profile:	CFLAGS_nat := $(CFLAGS_nat_profile)
profile:	$(PROJECT)

debug-web:	CFLAGS_web := $(CFLAGS_web_debug)
debug-web:	$(PROJECT).js

web-debug:	debug-web

$(PROJECT):	source/native/$(PROJECT).cc
	$(CXX_nat) $(CFLAGS_nat) source/native/$(PROJECT).cc -o $(PROJECT)
	@echo To build the web version use: make web

$(PROJECT).js: source/web/$(PROJECT)-web.cc
	$(CXX_web) $(CFLAGS_web) source/web/$(PROJECT)-web.cc -o web/$(PROJECT).js

clean:
	rm -f $(PROJECT) web/$(PROJECT).js web/*.js.map web/*.js.mem web/*.wasm *~ source/*.o debug_file

# Debugging information
print-%: ; @echo '$(subst ','\'',$*=$($*))'

serve:
	python3 -m http.server
