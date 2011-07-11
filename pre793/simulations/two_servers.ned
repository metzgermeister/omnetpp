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

package pre793.simulations;

import ned.DatarateChannel;
import pre793.Server;
import pre793.Router;
import pre793.Client;


network Two_servers
{
    @display("bgb=560,277");
    submodules:
        client: Client {
            @display("p=55,57");
        }
        router: Router {
            @display("p=174,108");
        }
        router1: Router {
            @display("p=316,108");
        }
        server: Server {
            @display("p=441,57;b=16,9");
        }
        client1: Client {
            @display("p=441,186");
        }
        server1: Server {
            @display("p=55,186");
        }
    connections:
        client.port <--> DatarateChannel <--> router.port++;
        router.port++ <--> DatarateChannel <--> router1.port++;
        router1.port++ <--> DatarateChannel <--> server.port;
        client1.port <--> DatarateChannel <--> router1.port++;
        router.port++ <--> DatarateChannel <--> server1.port;
}
