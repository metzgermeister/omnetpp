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
import ned.IdealChannel;
import pre793.Server;
import pre793.Router;
import pre793.Client;


//
// TODO documentation
//
// @author User
//
network TwoRouters
{
    @display("bgb=596,383");
    submodules:
        client: Client {
            @display("p=42,68");
        }
        router: Router {
            @display("p=147,74");
        }
        router1: Router {
            @display("p=243,87");
        }
        server: Server {
            @display("p=353,120");
        }
        router2: Router {
            @display("p=216,170");
        }
        router3: Router {
            @display("p=429,193");
        }
        router4: Router {
            @display("p=395,57");
        }
        router5: Router {
            @display("p=471,111");
        }
        router6: Router {
            @display("p=90,200");
        }
        router7: Router {
            @display("p=489,35");
        }
        client1: Client {
            @display("p=515,185");
        }
        client2: Client {
            @display("p=63,138");
        }
        client3: Client {
            @display("p=287,234");
        }
        client4: Client {
            @display("p=281,20");
        }
        client5: Client {
            @display("p=540,77");
        }
        client6: Client {
            @display("p=414,276");
        }
    connections:
        client.port <--> DatarateChannel <--> router.port++;
        router.port++ <--> DatarateChannel <--> router1.port++;
        router1.port++ <--> DatarateChannel <--> server.port;
        router1.port++ <--> DatarateChannel <--> router4.port++;
        router4.port++ <--> DatarateChannel <--> router7.port++;
        client4.port <--> DatarateChannel <--> router7.port++;
        client5.port <--> DatarateChannel <--> router5.port++;
        router5.port++ <--> DatarateChannel <--> router4.port++;
        router5.port++ <--> DatarateChannel <--> router3.port++;
        client1.port <--> DatarateChannel <--> router3.port++;
        client6.port <--> DatarateChannel <--> router3.port++;
        client3.port <--> DatarateChannel <--> router2.port++;
        router1.port++ <--> DatarateChannel <--> router2.port++;
        router2.port++ <--> DatarateChannel <--> router3.port++;
        router6.port++ <--> DatarateChannel <--> router2.port++;
        client2.port <--> DatarateChannel <--> router6.port++;
}

