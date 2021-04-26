TARGET_NAMES += inlet=DequeIsend+outlet=BlockIrecv_t\:\:IdiObiDuct
TARGET_NAMES += inlet=Send+outlet=BlockIrecv_t\:\:IsObiDuct
#TARGET_NAMES += inlet=DequeRput+outlet=Window_t\:\:IdrOwDuct
#TARGET_NAMES += inlet=Put+outlet=Window_t\:\:IpOwDuct
#TARGET_NAMES += inlet=DequeIrsend+outlet=BlockIrecv_t\:\:IdirObiDuct
#TARGET_NAMES += inlet=Rsend+outlet=BlockIrecv_t\:\:IrObiDuct

TO_ROOT := $(shell git rev-parse --show-cdup)

PROC_MIN := 2

# TODO why does rdma hang with large proc counts on travis?
PROC_MAX := 4

include $(TO_ROOT)/tests/MaketemplateMultiproc
