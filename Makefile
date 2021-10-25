SHELL := /bin/bash

# Project-specific settings
PROJECT := dishtiny
EMP_DIR := third-party/Empirical/include/emp

DISHTINY_HASH := $(shell git rev-parse --short HEAD)
DISHTINY_DIRTY := $(shell \
		( git diff-index --quiet HEAD -- && echo "-clean" || echo "-dirty" ) \
		| tr -d '\040\011\012\015' \
	)
DISHTINY_SOURCE_DIR := $(shell test -d /opt/dishtiny && echo /opt/dishtiny || pwd)
# to compile different metrics/selecctors
# make ARGS="-DMETRIC=streak -DSELECTOR=roulette"

# Flags to use regardless of compiler
CFLAGS_all := $(CXXFLAGS) -std=c++17 -Wall -Wno-unused-function -Wno-unused-private-field -Wno-empty-body \
	-Iinclude -Ithird-party/ \
	-DDISHTINY_HASH_=$(DISHTINY_HASH)$(DISHTINY_DIRTY) \
	-DDISHTINY_SOURCE_DIR_=$(DISHTINY_SOURCE_DIR)\
	-fno-signaling-nans -fno-trapping-math \
	$(ARGS)

# Native compiler information
DISH_MPICXX ?= mpicxx.mpich

IS_CLANG := $(shell ($(DISH_MPICXX) --version | grep -q clang); echo $$?)
ifeq (${IS_CLANG},0)
	OMP_FLAG := -fopenmp=libomp
	OMP_LINKER_FLAG := -fopenmp=libiomp5
	# adapted from https://unix.stackexchange.com/a/530726
	CFLAGS_march_native := -march=$(shell cat /sys/devices/cpu/caps/pmu_name)
else
	OMP_FLAG := -fopenmp
	OMP_LINKER_FLAG := -fopenmp
	# adapted from https://stackoverflow.com/a/18963118
	CFLAGS_march_native := $(shell g++ -\#\#\# -E - -march=native 2>&1 | sed -r '/cc1/!d;s/(")|(^.* - )//g' )
endif

CFLAGS_nat := -O3 $(CFLAGS_march_native) -flto -DNDEBUG $(CFLAGS_all) $(OMP_FLAG)
CFLAGS_nat_production := $(CFLAGS_nat) -g -fdebug-prefix-map=$(PWD)=.
CFLAGS_nat_ndata = $(CFLAGS_nat) -DNDATA
CFLAGS_nat_debug := -g -DEMP_TRACK_MEM -D_GLIBCXX_DEBUG -D_LIBCPP_DEBUG $(OMP_FLAG) $(CFLAGS_all)
CFLAGS_nat_sanitize := -fsanitize=address -fsanitize=undefined $(CFLAGS_nat_debug)
CFLAGS_nat_profile := -pg -DNDEBUG $(OMP_FLAG) $(CFLAGS_all)

# Emscripten compiler information
CXX_web := emcc
OFLAGS_web_all := $(CXXFLAGS) -Wno-mismatched-tags -Wno-empty-body -s USE_ZLIB=1 -s USE_LIBLZMA=1 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" -s TOTAL_MEMORY=671088640 --js-library $(EMP_DIR)/web/library_emp.js -s EXPORTED_FUNCTIONS="['_main', '_empCppCallback', '_empDoCppCallback']" -s NO_EXIT_RUNTIME=1 -s ABORTING_MALLOC=0 -I/usr/lib/x86_64-linux-gnu/openmpi/include/
OFLAGS_web_pthread := -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD=1
OFLAGS_web := -O3 -DNDEBUG -s DISABLE_EXCEPTION_CATCHING=1
OFLAGS_web_debug := -g4 -Oz -Wno-dollar-in-identifier-extension -s DEMANGLE_SUPPORT=1 -s ASSERTIONS=2 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=2 -D_GLIBCXX_DEBUG -D_LIBCPP_DEBUG -fexceptions

CFLAGS_web := $(CFLAGS_all) $(OFLAGS_web) $(OFLAGS_web_all)
CFLAGS_web_pthread := $(CFLAGS_all) $(OFLAGS_web) $(OFLAGS_web_pthread) $(OFLAGS_web_all)
CFLAGS_web_debug := $(CFLAGS_all) $(OFLAGS_web_debug) $(OFLAGS_web_pthread) $(OFLAGS_web_all)

default: $(PROJECT)
native: $(PROJECT)
production: $(PROJECT)
web: $(PROJECT).js web/index.html
web-no-pthread: $(PROJECT)-no-pthread.js web/index.html
web-deploy: web web-no-pthread

all: $(PROJECT) $(PROJECT).js web/index.html

debug: CFLAGS_nat := $(CFLAGS_nat_debug)
debug: $(PROJECT)

production: CFLAGS_nat := $(CFLAGS_nat_production)
production: $(PROJECT)

sanitize: CFLAGS_nat := $(CFLAGS_nat_sanitize)
sanitize: $(PROJECT)

debug-web: CFLAGS_web := $(CFLAGS_web_debug)
debug-web: $(PROJECT).js web/index.html

web-debug: debug-web

debug-web-no-pthread: CFLAGS_web := $(CFLAGS_web_debug)
debug-web-no-pthread: $(PROJECT)-no-pthread.js web/index.html

web-debug-no-pthread: debug-web-no-pthread

# see https://stackoverflow.com/a/57760267 RE: -lstdc++fs
$(PROJECT): source/native.cpp include/
	@echo DISH_MPICXX $(DISH_MPICXX)
	@echo MPICH_CXX $(MPICH_CXX)
	@echo OMPI_CXX $(OMPI_CXX)
	# use ccache for compile stage
	time ccache $(DISH_MPICXX) $(CFLAGS_nat) -c source/native.cpp
	# perform link stage
	time $(DISH_MPICXX) $(CFLAGS_nat) -o run$(PROJECT) native.o $(OMP_LINKER_FLAG) -lstdc++fs -lmetis -lz -lcurl -lsfml-graphics -ldw -llzma
	@echo To build the web version use: make web

$(PROJECT).js: source/web.cpp include/
	cd third-party/emsdk && . ./emsdk_env.sh && cd - && $(CXX_web) $(CFLAGS_web_pthread) source/web.cpp -o web/$(PROJECT).js

$(PROJECT)-no-pthread.js: source/web.cpp include/
	cd third-party/emsdk && . ./emsdk_env.sh && cd - && $(CXX_web) $(CFLAGS_web) source/web.cpp -o web/$(PROJECT)-no-pthread.js

web/index.html: web/includes
	python3 web/make_html.py

docs:
	$(MAKE) html -C docs/

serve:
	python3 -m http.server

docs/_build/doc-coverage.json:
	$(MAKE) coverage -C docs/

documentation-coverage-badge.json: docs/_build/doc-coverage.json
	python3 ci/parse_documentation_coverage.py docs/_build/doc-coverage.json > web/documentation-coverage-badge.json

version-badge.json:
	python3 ci/parse_version.py .bumpversion.cfg > web/version-badge.json

doto-badge.json:
	python3 ci/parse_dotos.py $$(./ci/grep_dotos.sh) > web/doto-badge.json

badges: documentation-coverage-badge.json version-badge.json doto-badge.json

clean:
	rm -rf run$(PROJECT) web/$(PROJECT).js web/*.js.map web/*.js.map *~ source/*.o web/*.wasm web/*.wast coverage_include web/*.json native.o.ccache*
	$(MAKE) clean -C docs/
	$(MAKE) clean -C fuzzing/
	$(MAKE) clean -C microbenchmarks/
	$(MAKE) clean -C tests/

test-source: debug web
	timeout 90 ./rundishtiny -N_CELLS 100 2>&1 >/dev/null | tee /dev/stderr | grep -q '>> update ' && echo 'matched!' || exit 1
	npm install .
	echo "const puppeteer = require('puppeteer'); var express = require('express'); var app = express(); app.use(express.static('web')); app.listen(3000); express.static.mime.types['wasm'] = 'application/wasm'; function sleep(millis) { return new Promise(resolve => setTimeout(resolve, millis)); } async function run() { const browser = await puppeteer.launch({ headless: true, args: [ '--disable-gpu', '--disable-dev-shm-usage', '--disable-setuid-sandbox', '--no-sandbox' ] }); const page = await browser.newPage(); page.on('console', msg => console.log(msg.text())); await page.goto('http://localhost:3000/index.html'); await sleep(30000); await page.content(); browser.close(); process.exit(0); } run();" | node | tee /dev/stderr | grep -q "web viewer load complete" && echo "matched!"|| exit 1
	echo "const puppeteer = require('puppeteer'); var express = require('express'); var app = express(); app.use(express.static('web')); app.listen(3000); express.static.mime.types['wasm'] = 'application/wasm'; function sleep(millis) { return new Promise(resolve => setTimeout(resolve, millis)); } async function run() { const browser = await puppeteer.launch({ headless: true, args: [ '--disable-gpu', '--disable-dev-shm-usage', '--disable-setuid-sandbox', '--no-sandbox' ] }); const page = await browser.newPage(); await page.goto('http://localhost:3000/index.html'); await sleep(30000); const html = await page.content(); console.log(html); browser.close(); process.exit(0); } run();" | node | tee /dev/stderr | grep -q "Kin Group ID" && echo "matched!" ||  exit 1

tests:
	$(MAKE) -C tests/
	$(MAKE) opt -C tests/
	$(MAKE) fulldebug -C tests/

coverage:
	$(MAKE) coverage -C tests/

install-test-dependencies:
	git submodule update --init && cd third-party && bash ./install_emsdk.sh && bash ./install_force_cover.sh

.PHONY: all
.PHONY: clean
.PHONY: cleanall
.PHONY: cleanlogs
.PHONY: debug
.PHONY: debug-web
.PHONY: default
.PHONY: docs
.PHONY: documentation-coverage
.PHONY: documentation-coverage-badge.json
.PHONY: doto-badge.json
.PHONY: install-test-dependencies
.PHONY: native
.PHONY: ndata
.PHONY: profile
.PHONY: sanitize
.PHONY: serve
.PHONY: static
.PHONY: test-source
.PHONY: tests
.PHONY: version-badge.json
.PHONY: web
.PHONY: web-debug
