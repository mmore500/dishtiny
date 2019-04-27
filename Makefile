# Project-specific settings
PROJECT := dishtiny
EMP_DIR := ../Empirical/source

DISHTINY_HASH := $(shell git rev-parse --short HEAD)
DISHTINY_DIRTY := $(shell \
    ( git diff-index --quiet HEAD -- && echo "-clean" || echo "-dirty" ) \
    | tr -d '\040\011\012\015' \
  )

EMPIRICAL_HASH := $(shell cd ../Empirical && git rev-parse --short HEAD)
EMPIRICAL_DIRTY := $(shell \
    cd ../Empirical \
    && ( git diff-index --quiet HEAD -- && echo "-clean" || echo "-dirty" ) \
    | tr -d '\040\011\012\015' \
  )

# Flags to use regardless of compiler
CFLAGS_all := -std=c++17 -Wall -Wno-unused-function \
                -I$(EMP_DIR)/ \
                -DDISHTINY_HASH_=$(DISHTINY_HASH)$(DISHTINY_DIRTY) \
                -DEMPIRICAL_HASH_=$(EMPIRICAL_HASH)$(EMPIRICAL_DIRTY)

# Native compiler information
CXX_nat := h5c++
CFLAGS_nat := -O3 -DNDEBUG $(CFLAGS_all)
CFLAGS_nat_ndata = $(CFLAGS_nat) -DNDATA
CFLAGS_nat_debug := -g -DEMP_TRACK_MEM $(CFLAGS_all)
CFLAGS_nat_sanitize := -fsanitize=address -fsanitize=undefined $(CFLAGS_nat_debug)
CFLAGS_nat_profile := -pg -DNDEBUG $(CFLAGS_all)

# Emscripten compiler information
CXX_web := em++
OFLAGS_web_all := -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" -s TOTAL_MEMORY=1073741824 --js-library $(EMP_DIR)/web/library_emp.js -s EXPORTED_FUNCTIONS="['_main', '_empCppCallback']" -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=1 -s ABORTING_MALLOC=0 -s "BINARYEN_TRAP_MODE='clamp'"#--preload-file state_grids #--embed-file configs
OFLAGS_web := -Oz -DNDEBUG
OFLAGS_web_debug := -g4 -Wno-dollar-in-identifier-extension -s DEMANGLE_SUPPORT=1 -s ASSERTIONS=1
#OFLAGS_web_debug := -g4 -DNDEBUG -Wno-dollar-in-identifier-extension -s DEMANGLE_SUPPORT=1

CFLAGS_web := $(CFLAGS_all) $(OFLAGS_web) $(OFLAGS_web_all) --preload-file request
CFLAGS_web_debug := $(CFLAGS_all) $(OFLAGS_web_debug) $(OFLAGS_web_all)


default: $(PROJECT)
native: $(PROJECT)
web: $(PROJECT).js
all: $(PROJECT) $(PROJECT).js

ndata: CXX_nat := g++
ndata: CFLAGS_nat := $(CFLAGS_nat_ndata)
ndata: $(PROJECT)

debug:	CFLAGS_nat := $(CFLAGS_nat_debug)
debug:	$(PROJECT)

sanitize: CFLAGS_nat := $(CFLAGS_nat_sanitize)
sanitize: $(PROJECT)

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
	rm -f $(PROJECT) web/$(PROJECT).js web/*.js.map web/*.js.mem web/*.wasm *~ source/*.o *.o debug_file

# Debugging information
print-%: ; @echo '$(subst ','\'',$*=$($*))'

serve:
	python3 -m http.server
