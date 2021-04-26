#TARGET_NAMES += inlet=RingIrend+outlet=BlockIrecv_s\:\:IrirObiDuct
#TARGET_NAMES += inlet=RingIrsend+outlet=BlockIrecv_s\:\:IrirObiDuct

TO_ROOT := $(shell git rev-parse --show-cdup)

PROC_MIN := 2

# TODO why does rdma hang with large proc counts on travis?
PROC_MAX := 4

include $(TO_ROOT)/tests/MaketemplateMultiproc
