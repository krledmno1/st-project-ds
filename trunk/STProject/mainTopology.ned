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
        volatile double WakeUpDelay @unit(s) = default(uniform(10s, 30s));
        volatile double SleepDelay @unit(s) = default(uniform(60s, 120s));
        volatile double ReconnectDelay @unit(s) = default(uniform(2s,6s));
    @display("i=device/laptop");
    gates:
        input in;
        output out;
        input updIn; //used to communicate with the NS
}

simple Broker
{
    parameters:
        int nrInputGates = default(10);
        int nrOutputGates = default(10);
        volatile double WakeUpDelay @unit(s) = default(uniform(0s, 40s));
        volatile double SleepDelay @unit(s) = default(uniform(20s, 60s));
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
        @display("i=abstract/penguin");
    gates:
        input updIn;
}

network STNetwork
{
    parameters:
        int nrClients = default(4);
        int nrBrokers = default(3);

        @display("bgb=914,370");
    submodules:
        clients[nrClients]: Client;
        borkers[nrBrokers]: Broker;
        nameServer: NameServer;
    connections allowunconnected:
}
