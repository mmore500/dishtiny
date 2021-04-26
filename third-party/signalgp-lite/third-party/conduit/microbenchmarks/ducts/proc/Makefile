TARGET_NAMES += accumulating+type=fundamental
TARGET_NAMES += put=dropping+get=skipping+type=trivial
TARGET_NAMES += put=dropping+get=stepping+type=cereal
TARGET_NAMES += put=dropping+get=stepping+type=trivial
TARGET_NAMES += put=growing+get=skipping+type=trivial
TARGET_NAMES += put=growing+get=stepping+type=cereal
TARGET_NAMES += put=growing+get=stepping+type=trivial

TO_ROOT := $(shell git rev-parse --show-cdup)

include $(TO_ROOT)/microbenchmarks/MaketemplateForwarding
