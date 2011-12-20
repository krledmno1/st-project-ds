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
    -INameServer \
    -INameServer/BrokersList \
    -INameServer/msg \
    -INode \
    -INode/msg

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
    $O/NameServer/NameServer.o \
    $O/NameServer/BrokersList/BrokersList.o \
    $O/NameServer/BrokersList/ListNode.o \
    $O/NameServer/msg/NSMessage.o \
    $O/Node/STNode.o \
    $O/Node/msg/ConnectionRequestMessage.o

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
	-rm -f NameServer/*_m.cc NameServer/*_m.h
	-rm -f NameServer/BrokersList/*_m.cc NameServer/BrokersList/*_m.h
	-rm -f NameServer/msg/*_m.cc NameServer/msg/*_m.h
	-rm -f Node/*_m.cc Node/*_m.h
	-rm -f Node/msg/*_m.cc Node/msg/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc Brokers/*.cc Brokers/brokersContainer/*.cc Brokers/clientsContainer/*.cc Clients/*.cc NameServer/*.cc NameServer/BrokersList/*.cc NameServer/msg/*.cc Node/*.cc Node/msg/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/Brokers/Broker.o: Brokers/Broker.cc \
	Brokers/Broker.h \
	NameServer/msg/NSMessage.h \
	Node/STNode.h \
	Node/msg/ConnectionRequestMessage.h
$O/Brokers/brokersContainer/BCNode.o: Brokers/brokersContainer/BCNode.cc \
	Brokers/brokersContainer/BCNode.h
$O/Brokers/brokersContainer/BrokersContainer.o: Brokers/brokersContainer/BrokersContainer.cc \
	Brokers/brokersContainer/BCNode.h \
	Brokers/brokersContainer/BrokersContainer.h
$O/Clients/Client.o: Clients/Client.cc \
	Brokers/Broker.h \
	Clients/Client.h \
	NameServer/msg/NSMessage.h \
	Node/STNode.h \
	Node/msg/ConnectionRequestMessage.h
$O/NameServer/NameServer.o: NameServer/NameServer.cc \
	Brokers/Broker.h \
	Clients/Client.h \
	NameServer/BrokersList/BrokersList.h \
	NameServer/BrokersList/ListNode.h \
	NameServer/NameServer.h \
	NameServer/msg/NSMessage.h \
	Node/STNode.h
$O/NameServer/BrokersList/BrokersList.o: NameServer/BrokersList/BrokersList.cc \
	Brokers/Broker.h \
	NameServer/BrokersList/BrokersList.h \
	NameServer/BrokersList/ListNode.h \
	NameServer/msg/NSMessage.h \
	Node/STNode.h
$O/NameServer/BrokersList/ListNode.o: NameServer/BrokersList/ListNode.cc \
	Brokers/Broker.h \
	NameServer/BrokersList/ListNode.h \
	NameServer/msg/NSMessage.h \
	Node/STNode.h
$O/NameServer/msg/NSMessage.o: NameServer/msg/NSMessage.cc \
	Brokers/Broker.h \
	NameServer/msg/NSMessage.h \
	Node/STNode.h
$O/Node/STNode.o: Node/STNode.cc \
	Node/STNode.h
$O/Node/msg/ConnectionRequestMessage.o: Node/msg/ConnectionRequestMessage.cc \
	Node/msg/ConnectionRequestMessage.h

