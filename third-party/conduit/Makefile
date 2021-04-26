SHELL := /bin/bash

# Project-specific settings
PROJECT := conduit
EMP_DIR := third-party/Empirical/include/emp

# Flags to use regardless of compiler
CFLAGS_all := -pipe -Wall -Wno-unused-function -std=c++17 -Iinclude/ -fopenmp

# Native compiler information
CFLAGS_nat := -O3 -DNDEBUG -O3 -flto -march=native $(CFLAGS_all)
CFLAGS_nat_debug := -g $(CFLAGS_all)
UIT_MPICXX ?= mpicxx
UIT_MPIEXEC ?= mpiexec

# Emscripten compiler information
CXX_web := emcc
OFLAGS_web_all := -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" -s TOTAL_MEMORY=67108864 --js-library $(EMP_DIR)/web/library_emp.js -s EXPORTED_FUNCTIONS="['_main', '_empCppCallback', '_empDoCppCallback']" -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=1 #--embed-file configs
OFLAGS_web := -Oz -DNDEBUG
OFLAGS_web_debug := -g4 -Oz -Wno-dollar-in-identifier-extension

CFLAGS_web := $(CFLAGS_all) $(OFLAGS_web) $(OFLAGS_web_all)
CFLAGS_web_debug := $(CFLAGS_all) $(OFLAGS_web_debug) $(OFLAGS_web_all)


default: $(PROJECT)
native: $(PROJECT)
web: $(PROJECT).js documentation-coverage-badge.json version-badge.json doto-badge.json

omp: CFLAGS_nat := $(CFLAGS_nat) -fopenmp
omp: $(PROJECT)

debug: CFLAGS_nat := $(CFLAGS_nat_debug)
debug: $(PROJECT)

debug-web: CFLAGS_web := $(CFLAGS_web_debug)
debug-web: $(PROJECT).js

web-debug: debug-web

$(PROJECT): source/native/$(PROJECT).cpp
	$(UIT_MPICXX) $(CFLAGS_nat) source/native/$(PROJECT).cpp -lstdc++fs -lbenchmark -lmetis -lpthread -o $(PROJECT)
	@echo To build the web version use: make web

$(PROJECT).js: source/web/$(PROJECT)-web.cpp
	source third-party/emsdk/emsdk_env.sh; \
	$(CXX_web) $(CFLAGS_web) source/web/$(PROJECT)-web.cpp -o web/$(PROJECT).js

serve:
	python3 -m http.server

install-coverage-dependencies:
	git submodule init && git submodule update \
		&& cd third-party/force-cover && make

cov: install-coverage-dependencies
	cd tests && make cov

documentation-coverage:
	cd docs && make coverage

documentation-coverage-badge.json: documentation-coverage
	python3 ci/parse_documentation_coverage.py docs/_build/doc-coverage.json > web/documentation-coverage-badge.json

version-badge.json:
	python3 ci/parse_version.py .bumpversion.cfg > web/version-badge.json

doto-badge.json:
	python3 ci/parse_dotos.py $$(./ci/grep_dotos.sh) > web/doto-badge.json

clean:
	rm -f $(PROJECT) web/$(PROJECT).js web/*.js.map web/*.js.map *~ source/*.o web/*.wasm web/*.wast
	rm -rf coverage_include
	cd docs && make clean
	cd demos && make clean
	cd macrobenchmarks && make clean
	cd microbenchmarks && make clean
	cd tests && make clean

docs:
	cd docs && make html

demos:
	cd demos && make

macrobenchmark:
	cd macrobenchmarks && make bench

microbenchmark:
	cd microbenchmarks && make bench

benchmark: macrobenchmark microbenchmark

test-readme-snippets:
	python3 ci/test_readme_snippets.py

test-source: debug debug-web
	$(UIT_MPIEXEC) -n 1 ./conduit | grep -q '>>> end <<<' && echo 'matched!' || exit 1
	npm install
	echo "const puppeteer = require('puppeteer'); var express = require('express'); var app = express(); app.use(express.static('web')); app.listen(3000); express.static.mime.types['wasm'] = 'application/wasm'; function sleep(millis) { return new Promise(resolve => setTimeout(resolve, millis)); } async function run() { const browser = await puppeteer.launch(); const page = await browser.newPage(); await page.goto('http://localhost:3000/conduit.html'); await sleep(1000); const html = await page.content(); console.log(html); browser.close(); process.exit(0); } run();" | node | tr -d '\n' | grep -q "Hello, browser!" && echo "matched!" || exit 1
	echo "const puppeteer = require('puppeteer'); var express = require('express'); var app = express(); app.use(express.static('web')); app.listen(3000); express.static.mime.types['wasm'] = 'application/wasm'; function sleep(millis) { return new Promise(resolve => setTimeout(resolve, millis)); } async function run() { const browser = await puppeteer.launch(); const page = await browser.newPage(); page.on('console', msg => console.log(msg.text())); await page.goto('http://localhost:3000/conduit.html'); await sleep(1000); await page.content(); browser.close(); process.exit(0); } run();" | node | grep -q "Hello, console!" && echo "matched!"|| exit 1

test:
	test $TRAVIS && mkdir -p "~/.openmpi/" && touch "~/.openmpi/mca-params.conf" && grep -qxF 'btl_base_warn_component_used = 0' ~/.openmpi/mca-params.conf || echo 'btl_base_warn_component_used = 0' >> ~/.openmpi/mca-params.conf
	cd tests && make

test-opt:
	test $TRAVIS && mkdir -p "~/.openmpi/" && touch "~/.openmpi/mca-params.conf" && grep -qxF 'btl_base_warn_component_used = 0' ~/.openmpi/mca-params.conf || echo 'btl_base_warn_component_used = 0' >> ~/.openmpi/mca-params.conf
	cd tests && make opt

test-fulldebug:
	test $TRAVIS && mkdir -p "~/.openmpi/" && touch "~/.openmpi/mca-params.conf" && grep -qxF 'btl_base_warn_component_used = 0' ~/.openmpi/mca-params.conf || echo 'btl_base_warn_component_used = 0' >> ~/.openmpi/mca-params.conf
	cd tests && make fulldebug

test-all: test test-opt test-fulldebug

.PHONY: \
	benchmark \
	clean \
	cov \
	demos \
	docs \
	install-coverage-dependencies \
	native \
	macrobenchmark \
	microbenchmark \
	serve \
	test \
	tests \
	web \


# Debugging information
print-%: ; @echo '$(subst ','\'',$*=$($*))'
