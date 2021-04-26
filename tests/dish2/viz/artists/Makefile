TARGET_NAMES += Artist
TARGET_NAMES += DummyArtist
TARGET_NAMES += IsAliveArtist
TARGET_NAMES += KinGroupIDArtist
TARGET_NAMES += PhylogeneticRootArtist
TARGET_NAMES += QuorumBitsArtist
TARGET_NAMES += ResourceStockpileArtist

TO_ROOT := $(shell git rev-parse --show-cdup)

test::
	npm install .

include $(TO_ROOT)/tests/MaketemplateWebRunning
