/*     
    This file is part of Vigil.

    Vigil is free software: you can redistribute it and/or modify it under the terms of the 
    GNU General Public License as published by the Free Software Foundation, either version 3 
    of the License, or (at your option) any later version.

    Vigil is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
    without  even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
    See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with Vigil. 
    If not, see <https://www.gnu.org/licenses/>. 

*/
#ifndef VRMC_H
#define VRMC_H
#include <stdint.h>

#define STAGE_VERSION     0x01
#define STAGE_KEX         0x02
#define STAGE_SMY_KEX     0x03
#define STAGE_RTR         0x04

extern unsigned char *pubkey;
extern unsigned char *privkey;
extern int pubkey_len;
extern int privkey_len;
struct vrmc_ops{
  uint8_t opcode;
};

typedef struct{
  int fd;
  char sym_key[1024];
} connect_t;

void start_vrmc_server();
void __start_vrmc_server();

/**************** VRMC Protocol ****************

  client -> three way handshake -> server
 
  client -> send version
 
  server -> send version
  server: check version, in future version there will be a list of 
          unsupported version
  server -> send public key
  client -> recv public key and store
  server -> send randomly generated AES key
  client -> ack and store
  server -> store

  client -> switch to AES key 
  
  client -> send ready to recv opcode
  server -> send ready to recv opcode

  client -> send login opcode with logon information
  server -> check info
  server -> if info is correct, issue uuid cookie
  client -> ack that uuid cookie was recieved and send it back
  server -> check uuid cookie
  server -> send authenticated opcode
  
  if client was not correctly authenticated, server will send opcode saying so nd terminate the connection

*/

void version_exchange(connect_t *);
void handle_client(void *);
void read_pub_key();
void read_private_key();
void vkex(connect_t *);
void free_keys();
#endif /* VRMC_H */