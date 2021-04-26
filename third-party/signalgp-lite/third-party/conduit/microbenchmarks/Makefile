TARGET_NAMES += ducts
TARGET_NAMES += mpi

TO_ROOT := $(shell git rev-parse --show-cdup)

include $(TO_ROOT)/microbenchmarks/MaketemplateForwarding

bench:
	./script/fixup_and_validate.sh
	ls
	python3 script/consolidate.py $$(find ducts -name '*.json')
	ls
	for f in *run_type=iteration*.csv; do \
		python3 script/graph.py $$f; \
	done
	./script/upload.sh
