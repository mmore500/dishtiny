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
                -I$(EMP_DIR)/../third-party/cereal/include \
                -DDISHTINY_HASH_=$(DISHTINY_HASH)$(DISHTINY_DIRTY) \
                -DEMPIRICAL_HASH_=$(EMPIRICAL_HASH)$(EMPIRICAL_DIRTY) \
								$(ARGS)
# to compile different metrics/selecctors
# make ARGS="-DMETRIC=streak -DSELECTOR=roulette"

# Native compiler information
CXX_nat := h5c++
CFLAGS_nat := -O3 -DNDEBUG $(CFLAGS_all) -fopenmp
CFLAGS_nat_ndata = $(CFLAGS_nat) -DNDATA
CFLAGS_nat_debug := -g -DEMP_TRACK_MEM -fopenmp $(CFLAGS_all)
CFLAGS_nat_sanitize := -fsanitize=address -fsanitize=undefined $(CFLAGS_nat_debug)
CFLAGS_nat_profile := -pg -DNDEBUG -fopenmp $(CFLAGS_all)

# Emscripten compiler information
CXX_web := em++
OFLAGS_web_all := -s ALLOW_MEMORY_GROWTH=1 -s USE_ZLIB=1 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" -s TOTAL_MEMORY=5242880 --js-library $(EMP_DIR)/web/library_emp.js -s EXPORTED_FUNCTIONS="['_main', '_empCppCallback']" -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=1 -s ABORTING_MALLOC=0 -s "BINARYEN_TRAP_MODE='clamp'"#--preload-file state_grids #--embed-file configs
OFLAGS_web := -O3 -DNDEBUG
OFLAGS_web_debug := -g4 -Wno-dollar-in-identifier-extension -s DEMANGLE_SUPPORT=1 -s ASSERTIONS=1
#OFLAGS_web_debug := -g4 -DNDEBUG -Wno-dollar-in-identifier-extension -s DEMANGLE_SUPPORT=1

CFLAGS_web := $(CFLAGS_all) $(OFLAGS_web) $(OFLAGS_web_all)
CFLAGS_web_debug := $(CFLAGS_all) $(OFLAGS_web_debug) $(OFLAGS_web_all)


default: $(PROJECT)
native: $(PROJECT)
web: $(PROJECT).js $(PROJECT).html
all: $(PROJECT) $(PROJECT).js

ndata: CXX_nat := g++
ndata: CFLAGS_nat := $(CFLAGS_nat_ndata)
ndata: $(PROJECT)

debug:	CFLAGS_nat := $(CFLAGS_nat_debug)
debug:	$(PROJECT)

static: CFLAGS_nat := -static $(CFLAGS_nat)
static: $(PROJECT)

sanitize: CFLAGS_nat := $(CFLAGS_nat_sanitize)
sanitize: $(PROJECT)

profile:	CFLAGS_nat := $(CFLAGS_nat_profile)
profile:	$(PROJECT)

debug-web:	CFLAGS_web := $(CFLAGS_web_debug)
debug-web:	$(PROJECT).js

web-debug:	debug-web

$(PROJECT):	source/native/$(PROJECT).cc
	$(CXX_nat) $(CFLAGS_nat) source/native/$(PROJECT).cc -lstdc++fs -o $(PROJECT)
	@echo To build the web version use: make web

$(PROJECT).js: source/web/$(PROJECT)-web.cc
	$(CXX_web) $(CFLAGS_web) source/web/$(PROJECT)-web.cc -o web/$(PROJECT).js

$(PROJECT).html: web/includes
	python3 web/make_html.py

clean:
	rm -f $(PROJECT) web/$(PROJECT).js web/*.js.map web/*.js.mem web/*.wasm *~ source/*.o *.o debug_file web/$(PROJECT).html

# Debugging information
print-%: ; @echo '$(subst ','\'',$*=$($*))'

serve:
	python3 -m http.server
