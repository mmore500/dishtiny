TARGET_NAMES += Animator
TARGET_NAMES += ControlPanel
TARGET_NAMES += GridViewer
TARGET_NAMES += ViewerCollection
TARGET_NAMES += ViewerPanel
TARGET_NAMES += WebInterface

TO_ROOT := $(shell git rev-parse --show-cdup)

test::
	npm install .

include $(TO_ROOT)/tests/MaketemplateWebRunning
