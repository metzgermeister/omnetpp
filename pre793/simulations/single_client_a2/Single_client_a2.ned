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

package pre793.simulations.single_client_a2;

import ned.DatarateChannel;
import pre793.Server;
import pre793.Router;
import pre793.Client;

//
// TODO Auto-generated network
//
network Single_client_a2
{
    @display("bgb=312,159");
    submodules:
        client: Client {
            @display("p=25,93");
        }
        router: Router {
            @display("p=119,92");
        }
        server: Server {
            @display("p=206,93");
        }
        A2: Client {
            @display("p=206,26");
        }
    connections:
        client.port <--> DatarateChannel <--> router.port++;
        router.port++ <--> DatarateChannel <--> server.port;
        A2.port <--> DatarateChannel <--> server.spam_port++;
}
