SHELL := /bin/bash

TARGET_NAMES += inlet=DequeIsend+outlet=RingIrecv_t\:\:IdiOriDuct
#TARGET_NAMES += inlet=Rsend+outlet=RingIrecv_t\:\:IrOri
#TARGET_NAMES += inlet=Send+outlet=RingIrecv_t\:\:IsOriDuct
#TARGET_NAMES += inlet=DequeIrsend+outlet=RingIrecv_t\:\:IdirOriDuct

TO_ROOT := $(shell git rev-parse --show-cdup)

PROC_MIN := 2

include $(TO_ROOT)/tests/MaketemplateMultiproc
