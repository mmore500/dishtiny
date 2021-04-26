TARGET_NAMES += buffered+inlet=RingIsend+outlet=Iprobe_t\:\:BufferedIriOiDuct
#TARGET_NAMES += inlet=RingIrsend+outlet=RingIrecv_t\:\:IrirOriDuct
#TARGET_NAMES += inlet=RingIsend+outlet=RingIrecv_t\:\:IriOriDuct
#TARGET_NAMES += pooled+inlet=RingIsend+outlet=Iprobe_t\:\:PooledIriOiDuct

TO_ROOT := $(shell git rev-parse --show-cdup)

PROC_MIN := 2

# TODO why does rdma hang with large proc counts on travis?
PROC_MAX := 4

include $(TO_ROOT)/tests/MaketemplateMultiproc
