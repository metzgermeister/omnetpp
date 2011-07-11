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

#include "ACKGenerator.h"
#include "SimplePacket_m.h"

Define_Module(ACKGenerator);

void ACKGenerator::initialize()
{
	rate = par("sendingRate").doubleValue();
	dest = par("destination").stdstringValue();
	if(rate > 0) {
		sendingDelay = 8.0 * 4 / rate;
		scheduleAt(simTime(), new cMessage("self-message loop"));
	}
}

void ACKGenerator::handleMessage(cMessage *msg)
{
	cChannel *ch = gate("port$o")->getChannel();
	if (ch->isBusy()) {
		// wait
		ev << "waiting for free channel" << endl;
		simtime_t gateFreeTime = ch->getTransmissionFinishTime();
		scheduleAt(gateFreeTime, msg);
	} else {
		// send a packet
		SimplePacket *ack = new SimplePacket(this->getName(), ACK);
		ack->setDest(dest.c_str());
		ack->setByteLength(4);
		send(ack, gate("port$o"));

		// schedule next event
		simtime_t processTime = simTime() + exponential(sendingDelay);
		scheduleAt(processTime, msg);
	}
}
