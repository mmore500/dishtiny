TARGET_NAMES += AdjacencyFileTopologyFactory
TARGET_NAMES += CompleteTopologyFactory
TARGET_NAMES += DyadicTopologyFactory
TARGET_NAMES += LoopTopologyFactory
TARGET_NAMES += NavigableSmallWorldTopologyFactory
TARGET_NAMES += ProConTopologyFactory
TARGET_NAMES += RingTopologyFactory
TARGET_NAMES += SoftRandomGeometricTopologyFactory
TARGET_NAMES += SmallWorldGridFactory
TARGET_NAMES += ToroidalTopologyFactory
TARGET_NAMES += ToroidalGridTopologyFactory

TO_ROOT := $(shell git rev-parse --show-cdup)

default:
	test

staging:
	mkdir -p staging/assets

assets: staging
	mkdir assets
	python3 scripts/make_complete.py
	python3 scripts/make_dyadic.py
	python3 scripts/make_loop.py
	python3 scripts/make_navigable_small_world.py
	python3 scripts/make_procon.py
	python3 scripts/make_ring.py
	python3 scripts/make_toroidal_grid.py
	python3 scripts/make_toroidal.py
	python3 scripts/make_soft_random_geometric.py
	python3 scripts/make_small_world_grid.py

test:: assets
opt:: assets
fulldebug:: assets
cov:: assets

clean::
	rm -rf assets
	rm -rf staging

include $(TO_ROOT)/tests/MaketemplateUniproc
