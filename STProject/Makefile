#
# OMNeT++/OMNEST Makefile for STProject
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out
#

# Name of target to be created (-o option)
TARGET = STProject$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -IMessages \
    -IMessages/ConnectionRequestMessage \
    -IMessages/DisconnectionRequestMessage \
    -IMessages/NSMsg \
    -IMessages/_Super \
    -INodes \
    -INodes/Brokers \
    -INodes/Brokers/NeighboursMap \
    -INodes/Clients \
    -INodes/NameServer \
    -INodes/_STNode

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/Messages/ConnectionRequestMessage/ConnectionRequestMessage.o \
    $O/Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.o \
    $O/Messages/NSMsg/NSMessage.o \
    $O/Messages/_Super/STMessage.o \
    $O/Nodes/Brokers/Broker.o \
    $O/Nodes/Brokers/NeighboursMap/NeighbourEntry.o \
    $O/Nodes/Brokers/NeighboursMap/NeighboursMap.o \
    $O/Nodes/Clients/Client.o \
    $O/Nodes/NameServer/NameServer.o \
    $O/Nodes/_STNode/STNode.o

# Message files
MSGFILES =

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f STProject STProject.exe libSTProject.so libSTProject.a libSTProject.dll libSTProject.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f Messages/*_m.cc Messages/*_m.h
	-rm -f Messages/ConnectionRequestMessage/*_m.cc Messages/ConnectionRequestMessage/*_m.h
	-rm -f Messages/DisconnectionRequestMessage/*_m.cc Messages/DisconnectionRequestMessage/*_m.h
	-rm -f Messages/NSMsg/*_m.cc Messages/NSMsg/*_m.h
	-rm -f Messages/_Super/*_m.cc Messages/_Super/*_m.h
	-rm -f Nodes/*_m.cc Nodes/*_m.h
	-rm -f Nodes/Brokers/*_m.cc Nodes/Brokers/*_m.h
	-rm -f Nodes/Brokers/NeighboursMap/*_m.cc Nodes/Brokers/NeighboursMap/*_m.h
	-rm -f Nodes/Clients/*_m.cc Nodes/Clients/*_m.h
	-rm -f Nodes/NameServer/*_m.cc Nodes/NameServer/*_m.h
	-rm -f Nodes/_STNode/*_m.cc Nodes/_STNode/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc Messages/*.cc Messages/ConnectionRequestMessage/*.cc Messages/DisconnectionRequestMessage/*.cc Messages/NSMsg/*.cc Messages/_Super/*.cc Nodes/*.cc Nodes/Brokers/*.cc Nodes/Brokers/NeighboursMap/*.cc Nodes/Clients/*.cc Nodes/NameServer/*.cc Nodes/_STNode/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/Messages/ConnectionRequestMessage/ConnectionRequestMessage.o: Messages/ConnectionRequestMessage/ConnectionRequestMessage.cc \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Messages/_Super/STMessage.h \
	Nodes/_STNode/STNode.h
$O/Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.o: Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.cc \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Messages/_Super/STMessage.h \
	Nodes/_STNode/STNode.h
$O/Messages/NSMsg/NSMessage.o: Messages/NSMsg/NSMessage.cc \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Messages/NSMsg/NSMessage.h \
	Messages/_Super/STMessage.h \
	Nodes/Brokers/Broker.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/_STNode/STNode.h
$O/Messages/_Super/STMessage.o: Messages/_Super/STMessage.cc \
	Messages/_Super/STMessage.h
$O/Nodes/Brokers/Broker.o: Nodes/Brokers/Broker.cc \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Messages/NSMsg/NSMessage.h \
	Messages/_Super/STMessage.h \
	Nodes/Brokers/Broker.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/_STNode/STNode.h
$O/Nodes/Brokers/NeighboursMap/NeighbourEntry.o: Nodes/Brokers/NeighboursMap/NeighbourEntry.cc \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Nodes/_STNode/STNode.h
$O/Nodes/Brokers/NeighboursMap/NeighboursMap.o: Nodes/Brokers/NeighboursMap/NeighboursMap.cc \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/_STNode/STNode.h
$O/Nodes/Clients/Client.o: Nodes/Clients/Client.cc \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Messages/NSMsg/NSMessage.h \
	Messages/_Super/STMessage.h \
	Nodes/Brokers/Broker.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Clients/Client.h \
	Nodes/_STNode/STNode.h
$O/Nodes/NameServer/NameServer.o: Nodes/NameServer/NameServer.cc \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Messages/NSMsg/NSMessage.h \
	Messages/_Super/STMessage.h \
	Nodes/Brokers/Broker.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Clients/Client.h \
	Nodes/NameServer/NameServer.h \
	Nodes/_STNode/STNode.h
$O/Nodes/_STNode/STNode.o: Nodes/_STNode/STNode.cc \
	Nodes/_STNode/STNode.h
