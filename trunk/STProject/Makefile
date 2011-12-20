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
    -IBrokers \
    -IBrokers/brokersContainer \
    -IBrokers/clientsContainer \
    -IClients \
    -IMessages \
    -IMessages/ConnectionRequestMessage \
    -IMessages/NSMsg \
    -IMessages/_Super \
    -INameServer \
    -INameServer/BrokersList \
    -IST \
    -IST/Node

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
    $O/Brokers/Broker.o \
    $O/Brokers/brokersContainer/BCNode.o \
    $O/Brokers/brokersContainer/BrokersContainer.o \
    $O/Clients/Client.o \
    $O/Messages/ConnectionRequestMessage/ConnectionRequestMessage.o \
    $O/Messages/NSMsg/NSMessage.o \
    $O/Messages/_Super/STMessage.o \
    $O/NameServer/NameServer.o \
    $O/NameServer/BrokersList/BrokersList.o \
    $O/NameServer/BrokersList/ListNode.o \
    $O/ST/Node/STNode.o

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
	-rm -f Brokers/*_m.cc Brokers/*_m.h
	-rm -f Brokers/brokersContainer/*_m.cc Brokers/brokersContainer/*_m.h
	-rm -f Brokers/clientsContainer/*_m.cc Brokers/clientsContainer/*_m.h
	-rm -f Clients/*_m.cc Clients/*_m.h
	-rm -f Messages/*_m.cc Messages/*_m.h
	-rm -f Messages/ConnectionRequestMessage/*_m.cc Messages/ConnectionRequestMessage/*_m.h
	-rm -f Messages/NSMsg/*_m.cc Messages/NSMsg/*_m.h
	-rm -f Messages/_Super/*_m.cc Messages/_Super/*_m.h
	-rm -f NameServer/*_m.cc NameServer/*_m.h
	-rm -f NameServer/BrokersList/*_m.cc NameServer/BrokersList/*_m.h
	-rm -f ST/*_m.cc ST/*_m.h
	-rm -f ST/Node/*_m.cc ST/Node/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc Brokers/*.cc Brokers/brokersContainer/*.cc Brokers/clientsContainer/*.cc Clients/*.cc Messages/*.cc Messages/ConnectionRequestMessage/*.cc Messages/NSMsg/*.cc Messages/_Super/*.cc NameServer/*.cc NameServer/BrokersList/*.cc ST/*.cc ST/Node/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/Brokers/Broker.o: Brokers/Broker.cc \
	Brokers/Broker.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Messages/NSMsg/NSMessage.h \
	Messages/_Super/STMessage.h \
	ST/Node/STNode.h
$O/Brokers/brokersContainer/BCNode.o: Brokers/brokersContainer/BCNode.cc \
	Brokers/brokersContainer/BCNode.h
$O/Brokers/brokersContainer/BrokersContainer.o: Brokers/brokersContainer/BrokersContainer.cc \
	Brokers/brokersContainer/BCNode.h \
	Brokers/brokersContainer/BrokersContainer.h
$O/Clients/Client.o: Clients/Client.cc \
	Brokers/Broker.h \
	Clients/Client.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Messages/NSMsg/NSMessage.h \
	Messages/_Super/STMessage.h \
	ST/Node/STNode.h
$O/Messages/ConnectionRequestMessage/ConnectionRequestMessage.o: Messages/ConnectionRequestMessage/ConnectionRequestMessage.cc \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Messages/_Super/STMessage.h
$O/Messages/NSMsg/NSMessage.o: Messages/NSMsg/NSMessage.cc \
	Brokers/Broker.h \
	Messages/NSMsg/NSMessage.h \
	Messages/_Super/STMessage.h \
	ST/Node/STNode.h
$O/Messages/_Super/STMessage.o: Messages/_Super/STMessage.cc \
	Messages/_Super/STMessage.h
$O/NameServer/NameServer.o: NameServer/NameServer.cc \
	Brokers/Broker.h \
	Clients/Client.h \
	Messages/NSMsg/NSMessage.h \
	Messages/_Super/STMessage.h \
	NameServer/BrokersList/BrokersList.h \
	NameServer/BrokersList/ListNode.h \
	NameServer/NameServer.h \
	ST/Node/STNode.h
$O/NameServer/BrokersList/BrokersList.o: NameServer/BrokersList/BrokersList.cc \
	Brokers/Broker.h \
	Messages/NSMsg/NSMessage.h \
	Messages/_Super/STMessage.h \
	NameServer/BrokersList/BrokersList.h \
	NameServer/BrokersList/ListNode.h \
	ST/Node/STNode.h
$O/NameServer/BrokersList/ListNode.o: NameServer/BrokersList/ListNode.cc \
	Brokers/Broker.h \
	Messages/NSMsg/NSMessage.h \
	Messages/_Super/STMessage.h \
	NameServer/BrokersList/ListNode.h \
	ST/Node/STNode.h
$O/ST/Node/STNode.o: ST/Node/STNode.cc \
	ST/Node/STNode.h

