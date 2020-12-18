SHELL := /bin/bash

TO_ROOT := $(shell git rev-parse --show-cdup)

default: test

test:
	export TEST_MODE=debug && "./${TO_ROOT}/third-party/bats-core/bin/bats" *.bats

opt:
	"./${TO_ROOT}/third-party/bats-core/bin/bats" *.bats

coverage:
	echo "nop"

clean:
	echo "nop"
