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

#include "Client.h"
#include "SimplePacket_m.h"

Define_Module(Client);

void Client::initialize() {

	acks.setName("ACKs to receive");

	destination = par("destination").stringValue();
	int64 fileSize = par("fileSize").longValue();
	packetSize = par("packetSize").longValue();
	ev << packetSize << endl;
	ev << par("sendingRate").doubleValue() << endl;
	acksToReceive = (fileSize + packetSize - 1) / packetSize; // ceiling
	sendingDelay = 8.0 * packetSize / par("sendingRate").doubleValue();

	ev << sendingDelay << endl;
	scheduleAt(simTime(), new cMessage("self-message loop"));
}

void Client::handleMessage(cMessage *msg) {

	acks.record(acksToReceive);
	if (dynamic_cast<SimplePacket *> (msg) != NULL) {
		// packet received
		SimplePacket *pkt = (SimplePacket *) msg;
		if (pkt->getKind() == ACK) {
			--acksToReceive;
			ev << "received ACK, acksToReceive=" << acksToReceive << endl;
			if(acksToReceive == 0) {
//				emit(0, true);
				recordScalar("Finish time", simTime());
			}
		} else {
			ev << "error: received non-ACK packet id=" << pkt->getId() << endl;
			throw 2;
		}
		delete pkt;
	} else {
		// self message
		if (acksToReceive > 0) {
			cChannel *ch = gate("port$o")->getChannel();
			if (ch->isBusy()) {
				// wait
				ev << "waiting for free channel" << endl;
				simtime_t gateFreeTime = ch->getTransmissionFinishTime();
				scheduleAt(gateFreeTime, msg);
			} else {
				ev << "sending data, acksToReceive=" << acksToReceive << endl;

				// send a packet
				SimplePacket *pkt = new SimplePacket(this->getName(), DATAGRAM);
				pkt->setDest(destination.c_str());
				pkt->setByteLength(packetSize);

				send(pkt, "port$o");

				// schedule next call
				simtime_t processTime = simTime() + exponential(sendingDelay);
				scheduleAt(processTime, msg);
			}
		}
		else {
			delete msg;
		}
	}
}
