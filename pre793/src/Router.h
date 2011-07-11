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

#ifndef __PRE793_ROUTER_H_
#define __PRE793_ROUTER_H_

#include <omnetpp.h>
#include <queue>
#include "SimplePacket_m.h"

enum State {
	IDLE = 0, BUSY = 1
};

/**
 * TODO - Generated class
 */
class Router: public cSimpleModule {
protected:

	class SimplePacketCmp {
		std::map<std::string, int> *pri;
	public:
		SimplePacketCmp(std::map<std::string, int> *aPri) {
			this->pri = aPri;
		}
		SimplePacketCmp() {
			this->pri = NULL;
		}
		bool operator()(SimplePacket* a, SimplePacket* b) const {
			if (a->getKind() != b->getKind()) {
				return a->getKind() > b->getKind();
			}
			int aa = (*pri)[a->getName()];
			int bb = (*pri)[b->getName()];
			if (aa == 0) {
				aa = 1000;
			}
			if (bb == 0) {
				bb = 1000;
			}
			return aa > bb;
		}
	};

	cOutVector qsize;

	State state;
	SimplePacket* current;
	double latency;
	double throughput;

	int64 maxQueueSize;
	int64 currentQueueSize;

	std::map<std::string, int> destinations;
	//	std::map<std::string, cTopology*> routes;
	std::priority_queue<SimplePacket*, std::vector<SimplePacket*>,
			SimplePacketCmp> queue;

	std::map<std::string, int> pri;

	std::set<std::string> clientsToIgnore;

	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
};

#endif
