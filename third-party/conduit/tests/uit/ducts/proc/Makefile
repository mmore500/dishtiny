TARGET_NAMES += accumulating+type=fundamental
TARGET_NAMES += accumulating+type=span
TARGET_NAMES += accumulating+type=spanfundamental
TARGET_NAMES += accumulating+type=trivial
TARGET_NAMES += impl
TARGET_NAMES += put=dropping+get=skipping+type=span
TARGET_NAMES += put=dropping+get=skipping+type=trivial
TARGET_NAMES += put=dropping+get=stepping+type=cereal
TARGET_NAMES += put=dropping+get=stepping+type=span
TARGET_NAMES += put=dropping+get=stepping+type=trivial
TARGET_NAMES += put=growing+get=skipping+type=trivial
TARGET_NAMES += put=growing+get=stepping+type=cereal
TARGET_NAMES += put=growing+get=stepping+type=trivial

TO_ROOT := $(shell git rev-parse --show-cdup)

include $(TO_ROOT)/tests/MaketemplateForwarding
