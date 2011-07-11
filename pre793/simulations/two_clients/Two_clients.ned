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

package pre793.simulations.two_clients;

import ned.DatarateChannel;
import pre793.Server;
import pre793.Client;
import pre793.Router;

network Two_clients
{
    @display("bgb=537,302");
    submodules:
        router: Router {
            @display("p=131,91");
        }
        client1: Client {
            @display("p=42,34");
        }
        client2: Client {
            @display("p=42,137");
        }
        server: Server {
            @display("p=310,92");
        }
    connections:
        client1.port <--> DatarateChannel <--> router.port++;
        client2.port <--> DatarateChannel <--> router.port++;
        router.port++ <--> DatarateChannel <--> server.port;
}
