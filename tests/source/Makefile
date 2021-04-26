SHELL := /bin/bash

TO_ROOT := $(shell git rev-parse --show-cdup)

default: test

test:
	export TEST_MODE=debug && "./${TO_ROOT}/third-party/bats-core/bin/bats" native.bats

opt:
	unset TEST_MODE && "./${TO_ROOT}/third-party/bats-core/bin/bats" native.bats native_opt.bats

coverage:
	echo "nop"

clean:
	rm -rf *.csv *.cfg *.bin *.json out* rundishtiny
	rm -rf first first.tar second second.tar
