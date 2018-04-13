OPL_DIR := $(call select_from_ports,openpowerlink)

INC_DIR += $(OPL_DIR)/openpowerlink $(OPL_DIR)/openpowerlink/include $(OPL_DIR)/openpowerlink/contrib/ $(OPL_DIR)/openpowerlink/common/ $(OPL_DIR)/openpowerlink/app/ $(OPL_DIR)/openpowerlink/arch/ $(OPL_DIR)/openpowerlink/app/common $(OPL_DIR)/openpowerlink/app/common/obdcreate

#Uncomment for CN
INC_DIR += $(OPL_DIR)/openpowerlink/proj/cn
INC_DIR += $(OPL_DIR)/openpowerlink/app/common/objdicts/CiA401_CN
INC_DIR += $(OPL_DIR)/openpowerlink/app/demo_cn

#Uncomment for MN
#INC_DIR += $(OPL_DIR)/openpowerlink/proj/mn
#INC_DIR += $(OPL_DIR)/openpowerlink/app/common/objdicts/CiA302-4_MN
#INC_DIR += $(OPL_DIR)/openpowerlink/app/demo_mn

LIBS += libc libc_lwip base pthread stdcxx

CC_DEF += -DWITH_EC -DWITH_SOCKS -DWITH_THREADING

SHARED_LIB = yes

CC_OPT += -O2

SRC_COMMON = /user/api/generic.c \
/user/api/processimage.c \
/user/api/sdotest.c \
/user/api/service.c \
/user/obd/obdu.c \
/user/obd/obdal.c \
/user/dll/dllucal.c \
/user/event/eventu.c \
/user/nmt/nmtu.c \
/user/nmt/nmtcnu.c \
/user/nmt/nmtmnu.c \
/user/nmt/identu.c \
/user/nmt/statusu.c \
/user/nmt/syncu.c \
/user/pdo/pdou.c \
/user/pdo/pdoucal.c \
/user/sdo/sdotest-com.c \
/user/sdo/sdotest-seq.c \
/user/sdo/sdocom-dummy.c \
/user/sdo/sdocom.c \
/user/sdo/sdocom-std.c \
/user/sdo/sdocomsrv.c \
/user/sdo/sdocomclt.c \
/user/sdo/sdoseq.c \
/user/sdo/sdoasnd.c \
/user/sdo/sdoudp.c \
/user/timesync/timesyncu.c \
/user/errhnd/errhndu.c \
/user/ctrl/ctrlu.c \
/user/sdo/sdoudp-linux.c \
/user/ctrl/ctrlucal-direct.c \
/user/dll/dllucal-circbuf.c \
/user/errhnd/errhnducal-local.c \
/user/event/eventucal-nooscircbuf.c \
/user/event/eventucalintf-circbuf.c \
/user/pdo/pdoucalmem-local.c \
/user/pdo/pdoucal-triplebufshm.c \
/user/timesync/timesyncucal-local.c \
/user/timer/timer-generic.c \
/kernel/dll/dllk.c \
/kernel/dll/dllkfilter.c \
/kernel/dll/dllkstatemachine.c \
/kernel/dll/dllkevent.c \
/kernel/dll/dllkframe.c \
/kernel/dll/dllknode.c \
/kernel/dll/dllkcal.c \
/kernel/event/eventk.c \
/kernel/nmt/nmtk.c \
/kernel/pdo/pdok.c \
/kernel/pdo/pdokcal.c \
/kernel/pdo/pdoklut.c \
/kernel/timesync/timesynck.c \
/kernel/errhnd/errhndk.c \
/kernel/errhnd/errhndkcal.c \
/kernel/ctrl/ctrlk.c \
/kernel/led/ledk.c \
/kernel/led/ledktimer.c \
/kernel/ctrl/ctrlkcal-direct.c \
/kernel/dll/dllkcal-circbuf.c \
/kernel/errhnd/errhndkcal-local.c \
/kernel/event/eventkcal-direct.c \
/kernel/event/eventkcalintf-circbuf.c \
/kernel/pdo/pdokcalmem-local.c \
/kernel/pdo/pdokcal-triplebufshm.c \
/kernel/timesync/timesynckcal-local.c \
/kernel/timer/hrestimer-genode.c \
/kernel/edrv/edrvcyclic.c \
/kernel/edrv/edrv-genode.c \
/user/obd/obdconfcrc-generic.c \
/common/circbuf/circbuffer.c \
/common/circbuf/circbuf-noos.c \
/common/memmap/memmap-null.c \
/common/ami/amix86.c \
/kernel/edrv/genode_wrapper.cpp \
/common/debugstr.c \
/arch/L4-FOC/target-foc.c \
/arch/L4-FOC/target-mutex.c \
/contrib/trace/trace-printf.c \
/app/common/eventlog/eventlogstring.c \
/app/common/eventlog/eventlog.c \
/app/common/system/system-linux.c \
/app/common/obdcreate/obdcreate.c

SRC_MN = /user/cfmu.c \
/user/obd/obdcdc.c \

SRC_MN_APP = /app/demo_mn/app.c \
/app/demo_mn/event.c \
/app/demo_mn/main.c \
/app/common/firmwaremanager/firmwaremanager.c \
/app/common/firmwaremanager/firmwarecheck.c \
/app/common/firmwaremanager/firmwareinfo.c \
/app/common/firmwaremanager/firmwareinfodecode-ascii.c \
/app/common/firmwaremanager/firmwarestore-file.c \
/app/common/firmwaremanager/firmwareupdate.c \
/app/common/objdicts/CiA302-4_MN/obdpi.c

SRC_CN_APP = /app/demo_cn/main.c \
/app/demo_cn/app.c \
/app/demo_cn/event.c

#SRC_CC = $(addprefix $(OPL_DIR)/openpowerlink, $(SRC_COMMON)) $(addprefix $(OPL_DIR)/openpowerlink, $(SRC_MN)) $(addprefix $(OPL_DIR)/openpowerlink, $(SRC_MN_APP))
SRC_CC = $(addprefix $(OPL_DIR)/openpowerlink, $(SRC_COMMON)) $(addprefix $(OPL_DIR)/openpowerlink, $(SRC_MN)) $(addprefix $(OPL_DIR)/openpowerlink, $(SRC_CN_APP))

