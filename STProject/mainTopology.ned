//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package stproject;



simple Client
{
    parameters:
        volatile double WakeUpDelay @unit(s) = default(uniform(0s, 40s));
        volatile double SleepDelay @unit(s) = default(uniform(60s, 120s));
        volatile double ReconnectDelay @unit(s) = default(uniform(2s,6s));
        volatile double SubscriptionPeriod @unit(s) = uniform(5s, 15s);
        volatile double UnSubscriptionPeriod @unit(s) = uniform(10s, 20s);
        volatile double PublishPeriod @unit(s) = uniform(2s, 10s);
        @statistic[PublishDelay](title="Publish Delay";record=max,timeavg,vector;unit=second);

        int nrInputGates = default(2);
    @display("i=device/laptop");
    gates:
        input in[nrInputGates];
        output out;
        input updIn; //used to communicate with the NS
}

//Big Huge Problem: if you use inheritance in NED, the virtual table is not dynamic anymore... a sort of.
//That means, if I put this as a Broker (as it was) and create a new "simple AcyclicBroker extends Broker" to inherit the properties, and instantiate a number of AcyclicBrokers
//	I will get in the end Brokers in the STNetwork, NOT AcyclicBrokers as I wished. VERY VERY STRANGE BUG
simple OptimalBroker {
    parameters:
        int nrInputGates = default(10);
        int nrOutputGates = default(10);
        volatile double WakeUpDelay @unit(s) = default(uniform(0s, 30s));
        volatile double ConverganceDelay @unit(s) = default(uniform(1s, 1s));
        volatile double SleepDelay @unit(s) = default(uniform(120s, 240s));
        @display("i=device/router");
    gates:
        input in[nrInputGates];
        output out[nrOutputGates];
        input updIn; //used to communicate with the NS
}

simple NameServer
{
    parameters:
        volatile double random = default(uniform(0,1));
        volatile double minDelay = default(uniform(0.01 , 0.5));
        volatile double maxDelay = default(uniform(0.95, 1));  
        @display("i=abstract/penguin");
    gates:
        input updIn;
}

network STNetwork
{
    
    parameters:
        int nrClients = default(4);
        int nrBrokers = default(4);
        bool newBrokerNotification = default(true);
        @display("bgb=914,370");
    submodules:
        clients[nrClients]: Client;
        borkers[nrBrokers]: OptimalBroker;
        nameServer: NameServer;
    connections allowunconnected:
}
