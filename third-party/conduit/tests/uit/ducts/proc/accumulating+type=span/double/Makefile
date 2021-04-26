TARGET_NAMES += buffered+inlet=BufferedIsend+outlet=Irecv_s\:\:BufferedIbiOiDuct
TARGET_NAMES += inlet=Isend+outlet=Irecv_s\:\:IiOiDuct

TO_ROOT := $(shell git rev-parse --show-cdup)

PROC_MIN := 2

# TODO why does rdma hang with large proc counts on travis?
PROC_MAX := 4

include $(TO_ROOT)/tests/MaketemplateMultiproc
