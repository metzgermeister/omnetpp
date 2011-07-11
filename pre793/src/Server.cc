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

#include "Server.h"
#include "SimplePacket_m.h"

Define_Module(Server);

void Server::initialize()
{
	throughput = par("throughput").doubleValue();
	latency = par("latency").doubleValue();
	maxQueueSize = par("queueSize").longValue();

	///////////// COPYPASTE
	std::stringstream ss(par("ignore").stdstringValue());
	std::string s;
	while(ss >> s) {
		clientsToIgnore.insert(s);
	}
	///////////// COPYPASTE

	state = IDLE;
}

void Server::handleMessage(cMessage *msg)
{

	//	delete msg;
	//	return;

	ev << "queue size before = " << queue.size() << endl;
	if (dynamic_cast<SimplePacket *> (msg) != NULL) {
		// packet

//		delete msg;

		ev << "state: " << state << endl;
		ev << "received packet from " << msg->getName() << " id=" << msg->getId() << endl;
//		ev << "packet from " << msg->getName() << " id=" << msg->getId() << endl;
		SimplePacket *pkt = (SimplePacket *) msg;

		if ((int) queue.size() < maxQueueSize) {
			if (queue.size() == 0 && state == IDLE) {
				scheduleAt(simTime(), new cMessage((std::string(this->getName()) + " self-message loop").c_str()));		}
			queue.push(pkt);
		} else {
			// drop packet
			delete pkt;
		}
	} else {
		// self message
		ev << "state: " << state << endl;
		ev << msg->getName() << " id=" << msg->getId() << endl;

//		ev << "self-message" << endl;
		if(state == IDLE) {
			if (queue.size() > 0) {
				current = queue.front();
				queue.pop();
				// process
				double sendingDelay = current->getBitLength() / throughput;
//				ev << sendingDelay << endl;
				simtime_t processTime = simTime() + exponential(sendingDelay) + exponential(latency);
				scheduleAt(processTime, msg);
				state = BUSY;
			} else {
				// queue is empty
//				ev << "warning: empty queue in IDLE state" << endl;
				delete msg;
			}

		}
		else if(state == BUSY) {
//			SimplePacket *pkt = queue.top();
//			int gateId = gate("port$o").get
			cChannel *ch = gate("port$o")->getChannel();
			if (ch->isBusy()) {
				// wait
				ev << "waiting for the free channel" << endl;
				simtime_t gateFreeTime = ch->getTransmissionFinishTime();
				scheduleAt(gateFreeTime, msg);
			} else {
				// sending
				if(clientsToIgnore.count(current->getName()) == 0) {
					ev << "sending ACK to " << current->getName() << endl;
					SimplePacket *ack = new SimplePacket(this->getName(), ACK);
					ack->setDest(current->getName());
					ack->setByteLength(4);
					send(ack, gate("port$o"));
				}

				delete current;

				state = IDLE;
				scheduleAt(simTime(), msg);
			}
		}
		else {
			ev << "unknown state: " << state << endl;
			throw 1;
		}
	}

}
