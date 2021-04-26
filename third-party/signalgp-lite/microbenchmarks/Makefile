TARGET_NAMES += SignalGP
TARGET_NAMES += signalgp-lite

TO_ROOT := $(shell git rev-parse --show-cdup)

include $(TO_ROOT)/microbenchmarks/MaketemplateForwarding

bench:
	./script/fixup_and_validate.sh
	python3 script/consolidate.py $$(find SignalGP signalgp-lite -name '*.json')
	for f in *run_type=iteration*.csv; do \
		python3 script/graph.py $$f; \
		python3 script/graph_speedup.py $$f; \
	done
	./script/upload.sh
