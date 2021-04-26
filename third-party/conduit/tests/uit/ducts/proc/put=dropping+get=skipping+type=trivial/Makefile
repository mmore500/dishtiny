TARGET_NAMES += inlet=RingIsend+outlet=BlockIrecv_t\:\:IriObiDuct
TARGET_NAMES += inlet=RingRput+outlet=Window_t\:\:IrrOwDuct
#TARGET_NAMES += pooled+inlet=RingIsend+outlet=BlockIrecv_t\:\:PooledIriObiDuct

TO_ROOT := $(shell git rev-parse --show-cdup)

PROC_MIN := 2

include $(TO_ROOT)/tests/MaketemplateMultiproc
