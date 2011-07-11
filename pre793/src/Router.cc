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

#include <string>
#include <sstream>
#include "Router.h"
#include "SimplePacket_m.h"

Define_Module(Router)
;

void Router::initialize() {

	qsize.setName("Queue size");

	queue = std::priority_queue<SimplePacket*, std::vector<SimplePacket*>,
			SimplePacketCmp>(SimplePacketCmp(&pri),
			std::vector<SimplePacket*>());

	std::string priorities = par("priorities").stdstringValue();
	std::stringstream ss(priorities);
	std::string s;
	int cc = 0;
	while (ss >> s) {
		pri[s] = ++cc;
	}

	throughput = par("throughput").doubleValue();
	latency = par("latency").doubleValue();
	maxQueueSize = par("queueSize").longValue();
	currentQueueSize = 0;

	//	// determine gate-module coincidence
	//	for (int i = 0; i < gateSize("port$o"); ++i) {
	//		cGate *t = gate("port$o", i);
	//		std::string name = t->getNextGate()->getOwner()->getName();
	//		ev << name << endl;
	//		destinations[name] = t->getId();
	//	}

	state = IDLE;

	/////////////////////

	cTopology targets;
	targets.extractByNedTypeName(
			cStringTokenizer("pre793.Client pre793.Server").asVector());

	for (int i = 0; i < targets.getNumNodes(); i++) {
		cTopology::Node *node = targets.getNode(i);
		ev << node->getModule()->getName() << endl;
		cTopology topo;
		topo.extractByNedTypeName(cStringTokenizer(
				"pre793.Router pre793.Client pre793.Server").asVector());

		cTopology::Node *nodeCopy = topo.getNodeFor(node->getModule());
		topo.calculateUnweightedSingleShortestPathsTo(nodeCopy);
		cTopology::Node *current = topo.getNodeFor(this);

		ev << current->getModule()->getName() << endl;

		if (current == NULL) {
			ev << "We (" << getFullPath()
					<< ") are not included in the topology.\n";
		} else if (current->getNumPaths() == 0) {
			ev << "No path to destination.\n";
		} else {
			cTopology::LinkOut *path = current->getPath(0);
			destinations[node->getModule()->getName()] = path->getLocalGateId();
			//			while (node != topo.getTargetNode()) {
			//				ev << "We are in " << node->getModule()->getFullPath() << endl;
			//				ev << node->getDistanceToTarget() << " hops to go\n";
			//				ev << "There are " << node->getNumPaths()
			//						<< " equally good directions, taking the first one\n";
			//				cTopology::LinkOut *path = node->getPath(0);
			//				ev << "Taking gate " << path->getLocalGate()->getFullName()
			//						<< " we arrive in "
			//						<< path->getRemoteNode()->getModule()->getFullPath()
			//						<< " on its gate "
			//						<< path->getRemoteGate()->getFullName() << endl;
			//				node = path->getRemoteNode();
			//			}
		}
	}

	///////////// COPYPASTE
	ev << "ignore = " << par("ignore").stdstringValue() << endl;
	std::stringstream ss2(par("ignore").stdstringValue());
	std::string s2;
	while (ss2 >> s2) {
		clientsToIgnore.insert(s2);
	}
	///////////// COPYPASTE
}

void Router::handleMessage(cMessage *msg) {
	//	delete msg;
	//	return;

	qsize.record(currentQueueSize);
	ev << "queue size before event = " << currentQueueSize << " B" << endl;
	if (dynamic_cast<SimplePacket *> (msg) != NULL) {
		// packet
		ev << "state: " << state << endl;
		ev << "packet from " << msg->getName() << " id=" << msg->getId()
				<< endl;
		SimplePacket *pkt = (SimplePacket *) msg;

		if (currentQueueSize + pkt->getByteLength() <= maxQueueSize) {
			if (queue.size() == 0 && state == IDLE) {
				scheduleAt(simTime(),
						new cMessage((std::string(this->getName())
								+ " self-message loop").c_str()));
			}
			currentQueueSize += pkt->getByteLength();
			queue.push(pkt);
			ev << "pushed!" << endl;
		} else {
			// drop packet
			ev << "dropped!" << endl;
			delete pkt;
		}
	} else {
		// self message
		ev << "state: " << state << endl;
		ev << msg->getName() << " id=" << msg->getId() << endl;

		//		ev << "self-message" << endl;
		if (state == IDLE) {
			if (queue.size() > 0) {
				current = queue.top();
				queue.pop();
				currentQueueSize -= current->getByteLength();
				// process
				double sendingDelay = current->getBitLength() / throughput;
				//				ev << sendingDelay << endl;
				simtime_t processTime = simTime() + exponential(sendingDelay)
						+ exponential(latency);
				scheduleAt(processTime, msg);
				state = BUSY;
			} else {
				// queue is empty
				//				ev << "warning: empty queue in IDLE state" << endl;
				delete msg;
			}

		} else if (state == BUSY) {
			int gateId = destinations[current->getDest()];
			;
			cChannel *ch = gate(gateId)->getChannel();
			if (ch->isBusy()) {
				// wait
				ev << "waiting for the free channel" << endl;
				simtime_t gateFreeTime = ch->getTransmissionFinishTime();
				scheduleAt(gateFreeTime, msg);
			} else {
				// sending

				if (clientsToIgnore.count(current->getName()) == 0) {
					ev << "sending packet from " << current->getName()
							<< " id=" << current->getId() << endl;
					send(current, gateId);
				} else {
					delete current;
				}
				//				queue.pop();
				state = IDLE;
				scheduleAt(simTime(), msg);
			}
		} else {
			ev << "unknown state: " << state << endl;
			throw 1;
		}
	}
}

