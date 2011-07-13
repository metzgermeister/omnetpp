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

Define_Module(ACKGenerator)
;

void ACKGenerator::initialize() {
	rate = par("sendingRate").doubleValue();
	dest = par("destination").stdstringValue();
	if (rate > 0) {
		sendingDelay = 8.0 * 4 / rate;
		scheduleAt(simTime(), new cMessage("self-message loop"));
	}
	finish = false;
}

void ACKGenerator::handleMessage(cMessage *msg) {
	if(msg->getName() == std::string("FINISH")) {
		finish = true;
		delete msg;
		return;
	}
	simtime_t delay = 0;
	cChannel *ch = gate("port$o")->getChannel();
	if (ch->isBusy()) {
		simtime_t gateFreeTime = ch->getTransmissionFinishTime();
		delay = gateFreeTime - simTime();
	}
	// send a packet
	SimplePacket *ack = new SimplePacket(this->getName(), ACK);
	ack->setDest(dest.c_str());
	ack->setByteLength(4);
	sendDelayed(ack, delay, gate("port$o"));

	// schedule next event
	simtime_t processTime = simTime() + exponential(sendingDelay);
	if(!finish) {
		scheduleAt(processTime, msg);
	} else {
		delete msg;
	}
}
