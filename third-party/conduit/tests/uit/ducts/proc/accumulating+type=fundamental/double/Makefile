#TARGET_NAMES += aggregated+inlet=Accumulate+outlet=WithdrawingWindow_f\:\:AggregatedIaOwwDuct
#TARGET_NAMES += aggregated+inlet=Raccumulate+outlet=WithdrawingWindow_f\:\:AggregatedIrOwwDuct
#TARGET_NAMES += inlet=Accumulate+outlet=WithdrawingWindow_f\:\:IaOwwDuct
#TARGET_NAMES += inlet=Raccumulate+outlet=WithdrawingWindow_f\:\:IrOwwDuct

TO_ROOT := $(shell git rev-parse --show-cdup)

PROC_MIN := 2

# TODO why does rdma hang with large proc counts on travis?
PROC_MAX := 4

include $(TO_ROOT)/tests/MaketemplateMultiproc
