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

package pre793;

//
// TODO auto-generated module
//
simple Router
{
    parameters:
        double latency @unit(s); // fixed time to process single packet 
        double throughput @unit(bps); // router throughput 
        int queueSize @unit(B); // size of router internal queue
        
        string priorities = default(""); // priorities
        
        string ignore = default(""); // clients to not retransmit
        
    @display("i=device/smallrouter");
    gates:
        inout port[];
}
