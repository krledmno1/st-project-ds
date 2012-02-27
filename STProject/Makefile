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
    -IMessages/NewBrokerNotificationMessage \
    -IMessages/ProtocolMessages \
    -IMessages/ProtocolMessages/ConnectMessage \
    -IMessages/ProtocolMessages/ExpandMessage \
    -IMessages/ProtocolMessages/MWOEMessage \
    -IMessages/ProtocolMessages/UpdateMessage \
    -IMessages/PublishMessage \
    -IMessages/SubscriptionMessage \
    -IMessages/UnsubscriptionMessage \
    -IMessages/_Super \
    -INodes \
    -INodes/Brokers \
    -INodes/Brokers/AcyclicBroker \
    -INodes/Brokers/NeighboursMap \
    -INodes/Brokers/OptimalBroker \
    -INodes/Clients \
    -INodes/NameServer \
    -INodes/SubscriptionMonitor \
    -INodes/_STNode \
    -IUtilities \
    -IUtilities/LinkedList \
    -IUtilities/NetworkConditionTable \
    -IUtilities/Stack

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
    $O/Messages/NewBrokerNotificationMessage/NewBrokerNotificationMessage.o \
    $O/Messages/ProtocolMessages/ConnectMessage/ConnectMessage.o \
    $O/Messages/ProtocolMessages/ExpandMessage/ExpandMessage.o \
    $O/Messages/ProtocolMessages/MWOEMessage/MWOEMessage.o \
    $O/Messages/ProtocolMessages/UpdateMessage/UpdateMessage.o \
    $O/Messages/PublishMessage/PublishMessage.o \
    $O/Messages/SubscriptionMessage/SubscriptionMessage.o \
    $O/Messages/UnsubscriptionMessage/UnsubscriptionMessage.o \
    $O/Messages/_Super/STMessage.o \
    $O/Nodes/Brokers/Broker.o \
    $O/Nodes/Brokers/AcyclicBroker/AcyclicBroker.o \
    $O/Nodes/Brokers/NeighboursMap/NeighboursMap.o \
    $O/Nodes/Brokers/NeighboursMap/NeighbourEntry.o \
    $O/Nodes/Brokers/OptimalBroker/OptimalBroker.o \
    $O/Nodes/Clients/Client.o \
    $O/Nodes/NameServer/NameServer.o \
    $O/Nodes/SubscriptionMonitor/SubscriptionMonitor.o \
    $O/Nodes/_STNode/STNode.o \
    $O/Utilities/NetworkConditionTable/Map.o \
    $O/Utilities/NetworkConditionTable/NetworkConditionTable.o \
    $O/Utilities/NetworkConditionTable/KeyValue.o \
    $O/Utilities/Stack/Stack.o

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
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

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

$O/%.o: %.cc
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
	-rm -f Messages/NewBrokerNotificationMessage/*_m.cc Messages/NewBrokerNotificationMessage/*_m.h
	-rm -f Messages/ProtocolMessages/*_m.cc Messages/ProtocolMessages/*_m.h
	-rm -f Messages/ProtocolMessages/ConnectMessage/*_m.cc Messages/ProtocolMessages/ConnectMessage/*_m.h
	-rm -f Messages/ProtocolMessages/ExpandMessage/*_m.cc Messages/ProtocolMessages/ExpandMessage/*_m.h
	-rm -f Messages/ProtocolMessages/MWOEMessage/*_m.cc Messages/ProtocolMessages/MWOEMessage/*_m.h
	-rm -f Messages/ProtocolMessages/UpdateMessage/*_m.cc Messages/ProtocolMessages/UpdateMessage/*_m.h
	-rm -f Messages/PublishMessage/*_m.cc Messages/PublishMessage/*_m.h
	-rm -f Messages/SubscriptionMessage/*_m.cc Messages/SubscriptionMessage/*_m.h
	-rm -f Messages/UnsubscriptionMessage/*_m.cc Messages/UnsubscriptionMessage/*_m.h
	-rm -f Messages/_Super/*_m.cc Messages/_Super/*_m.h
	-rm -f Nodes/*_m.cc Nodes/*_m.h
	-rm -f Nodes/Brokers/*_m.cc Nodes/Brokers/*_m.h
	-rm -f Nodes/Brokers/AcyclicBroker/*_m.cc Nodes/Brokers/AcyclicBroker/*_m.h
	-rm -f Nodes/Brokers/NeighboursMap/*_m.cc Nodes/Brokers/NeighboursMap/*_m.h
	-rm -f Nodes/Brokers/OptimalBroker/*_m.cc Nodes/Brokers/OptimalBroker/*_m.h
	-rm -f Nodes/Clients/*_m.cc Nodes/Clients/*_m.h
	-rm -f Nodes/NameServer/*_m.cc Nodes/NameServer/*_m.h
	-rm -f Nodes/SubscriptionMonitor/*_m.cc Nodes/SubscriptionMonitor/*_m.h
	-rm -f Nodes/_STNode/*_m.cc Nodes/_STNode/*_m.h
	-rm -f Utilities/*_m.cc Utilities/*_m.h
	-rm -f Utilities/LinkedList/*_m.cc Utilities/LinkedList/*_m.h
	-rm -f Utilities/NetworkConditionTable/*_m.cc Utilities/NetworkConditionTable/*_m.h
	-rm -f Utilities/Stack/*_m.cc Utilities/Stack/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc Messages/*.cc Messages/ConnectionRequestMessage/*.cc Messages/DisconnectionRequestMessage/*.cc Messages/NSMsg/*.cc Messages/NewBrokerNotificationMessage/*.cc Messages/ProtocolMessages/*.cc Messages/ProtocolMessages/ConnectMessage/*.cc Messages/ProtocolMessages/ExpandMessage/*.cc Messages/ProtocolMessages/MWOEMessage/*.cc Messages/ProtocolMessages/UpdateMessage/*.cc Messages/PublishMessage/*.cc Messages/SubscriptionMessage/*.cc Messages/UnsubscriptionMessage/*.cc Messages/_Super/*.cc Nodes/*.cc Nodes/Brokers/*.cc Nodes/Brokers/AcyclicBroker/*.cc Nodes/Brokers/NeighboursMap/*.cc Nodes/Brokers/OptimalBroker/*.cc Nodes/Clients/*.cc Nodes/NameServer/*.cc Nodes/SubscriptionMonitor/*.cc Nodes/_STNode/*.cc Utilities/*.cc Utilities/LinkedList/*.cc Utilities/NetworkConditionTable/*.cc Utilities/Stack/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/Messages/ConnectionRequestMessage/ConnectionRequestMessage.o: Messages/ConnectionRequestMessage/ConnectionRequestMessage.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Utilities/LinkedList/LinkedList.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h
$O/Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.o: Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Messages/_Super/STMessage.h
$O/Messages/NSMsg/NSMessage.o: Messages/NSMsg/NSMessage.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Messages/NSMsg/NSMessage.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Nodes/Brokers/Broker.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Messages/NewBrokerNotificationMessage/NewBrokerNotificationMessage.o: Messages/NewBrokerNotificationMessage/NewBrokerNotificationMessage.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Messages/NewBrokerNotificationMessage/NewBrokerNotificationMessage.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Messages/NSMsg/NSMessage.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Nodes/Brokers/Broker.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Messages/ProtocolMessages/ConnectMessage/ConnectMessage.o: Messages/ProtocolMessages/ConnectMessage/ConnectMessage.cc \
	Messages/ProtocolMessages/ConnectMessage/ConnectMessage.h \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Messages/NSMsg/NSMessage.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Nodes/Brokers/Broker.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Messages/ProtocolMessages/ExpandMessage/ExpandMessage.o: Messages/ProtocolMessages/ExpandMessage/ExpandMessage.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/Stack/Stack.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h \
	Messages/ProtocolMessages/ExpandMessage/ExpandMessage.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Messages/NSMsg/NSMessage.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Nodes/Brokers/Broker.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Messages/ProtocolMessages/MWOEMessage/MWOEMessage.o: Messages/ProtocolMessages/MWOEMessage/MWOEMessage.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Messages/ProtocolMessages/MWOEMessage/MWOEMessage.h \
	Utilities/Stack/Stack.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Messages/NSMsg/NSMessage.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Nodes/Brokers/Broker.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Messages/ProtocolMessages/UpdateMessage/UpdateMessage.o: Messages/ProtocolMessages/UpdateMessage/UpdateMessage.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h \
	Messages/ProtocolMessages/UpdateMessage/UpdateMessage.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Messages/NSMsg/NSMessage.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Nodes/Brokers/Broker.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Messages/PublishMessage/PublishMessage.o: Messages/PublishMessage/PublishMessage.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/LinkedList/LinkedList.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h
$O/Messages/SubscriptionMessage/SubscriptionMessage.o: Messages/SubscriptionMessage/SubscriptionMessage.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Utilities/LinkedList/LinkedList.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h
$O/Messages/UnsubscriptionMessage/UnsubscriptionMessage.o: Messages/UnsubscriptionMessage/UnsubscriptionMessage.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/LinkedList/LinkedList.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Messages/_Super/STMessage.o: Messages/_Super/STMessage.cc \
	Messages/_Super/STMessage.h
$O/Nodes/Brokers/Broker.o: Nodes/Brokers/Broker.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/Clients/Client.h \
	Messages/NewBrokerNotificationMessage/NewBrokerNotificationMessage.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Messages/NSMsg/NSMessage.h \
	Nodes/SubscriptionMonitor/SubscriptionMonitor.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Nodes/Brokers/Broker.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Nodes/Brokers/AcyclicBroker/AcyclicBroker.o: Nodes/Brokers/AcyclicBroker/AcyclicBroker.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h \
	Nodes/Brokers/AcyclicBroker/AcyclicBroker.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Messages/NewBrokerNotificationMessage/NewBrokerNotificationMessage.h \
	Nodes/Clients/Client.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Messages/NSMsg/NSMessage.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Nodes/SubscriptionMonitor/SubscriptionMonitor.h \
	Nodes/Brokers/Broker.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Nodes/Brokers/NeighboursMap/NeighboursMap.o: Nodes/Brokers/NeighboursMap/NeighboursMap.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Nodes/Clients/Client.h \
	Messages/NewBrokerNotificationMessage/NewBrokerNotificationMessage.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Messages/NSMsg/NSMessage.h \
	Nodes/SubscriptionMonitor/SubscriptionMonitor.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Nodes/Brokers/Broker.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Nodes/Brokers/NeighboursMap/NeighbourEntry.o: Nodes/Brokers/NeighboursMap/NeighbourEntry.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/LinkedList/LinkedList.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Nodes/_STNode/STNode.h
$O/Nodes/Brokers/OptimalBroker/OptimalBroker.o: Nodes/Brokers/OptimalBroker/OptimalBroker.cc \
	Utilities/NetworkConditionTable/Map.h \
	Messages/ProtocolMessages/ConnectMessage/ConnectMessage.h \
	Utilities/Stack/Stack.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Messages/ProtocolMessages/MWOEMessage/MWOEMessage.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/Brokers/OptimalBroker/OptimalBroker.h \
	Nodes/_STNode/STNode.h \
	Messages/ProtocolMessages/UpdateMessage/UpdateMessage.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h \
	Nodes/Brokers/AcyclicBroker/AcyclicBroker.h \
	Messages/ProtocolMessages/ExpandMessage/ExpandMessage.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Messages/NSMsg/NSMessage.h \
	Nodes/SubscriptionMonitor/SubscriptionMonitor.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Nodes/Brokers/Broker.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Nodes/Clients/Client.o: Nodes/Clients/Client.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Nodes/Clients/Client.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Messages/NewBrokerNotificationMessage/NewBrokerNotificationMessage.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Messages/NSMsg/NSMessage.h \
	Nodes/SubscriptionMonitor/SubscriptionMonitor.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Nodes/Brokers/Broker.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Nodes/NameServer/NameServer.o: Nodes/NameServer/NameServer.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Nodes/NameServer/NameServer.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h \
	Messages/_Super/STMessage.h \
	Messages/PublishMessage/PublishMessage.h \
	Messages/SubscriptionMessage/SubscriptionMessage.h \
	Messages/ConnectionRequestMessage/ConnectionRequestMessage.h \
	Nodes/Clients/Client.h \
	Messages/NewBrokerNotificationMessage/NewBrokerNotificationMessage.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/Brokers/NeighboursMap/NeighboursMap.h \
	Nodes/Brokers/NeighboursMap/NeighbourEntry.h \
	Messages/NSMsg/NSMessage.h \
	Messages/DisconnectionRequestMessage/DisconnectionRequestMessage.h \
	Nodes/SubscriptionMonitor/SubscriptionMonitor.h \
	Nodes/Brokers/Broker.h \
	Messages/UnsubscriptionMessage/UnsubscriptionMessage.h
$O/Nodes/SubscriptionMonitor/SubscriptionMonitor.o: Nodes/SubscriptionMonitor/SubscriptionMonitor.cc \
	Nodes/SubscriptionMonitor/SubscriptionMonitor.h
$O/Nodes/_STNode/STNode.o: Nodes/_STNode/STNode.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Utilities/LinkedList/LinkedList.h \
	Nodes/_STNode/STNode.h
$O/Utilities/NetworkConditionTable/KeyValue.o: Utilities/NetworkConditionTable/KeyValue.cc \
	Utilities/NetworkConditionTable/KeyValue.h
$O/Utilities/NetworkConditionTable/NetworkConditionTable.o: Utilities/NetworkConditionTable/NetworkConditionTable.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h \
	Utilities/LinkedList/LinkedList.h \
	Utilities/NetworkConditionTable/NetworkConditionTable.h \
	Nodes/_STNode/STNode.h
$O/Utilities/NetworkConditionTable/Map.o: Utilities/NetworkConditionTable/Map.cc \
	Utilities/NetworkConditionTable/Map.h \
	Utilities/NetworkConditionTable/KeyValue.h
$O/Utilities/Stack/Stack.o: Utilities/Stack/Stack.cc \
	Utilities/Stack/Stack.h \
	Utilities/LinkedList/LinkedList.h

