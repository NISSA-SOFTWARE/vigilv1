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
#include <pcap.h>
#include <netinet/ether.h>
#include <net/if_arp.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "l2pktmgr.h"
#include "protocols.h"
#include "utils.h"
#include <pthread.h>
#include "colors.h"
#include "database/update_db.h"
#include "nethdr/arp-hdr.h"
#include "arpcache.h"
#include "nethdr/ctp.h"
#include "globals.h"


/*
  *-*-*-*- l2pktmgr.c -*-*-*-*
  @purpose Decode various layer two protocols
  @supports
    - ARP
    - CTP

    ==> decode arp packets based on data in the /src/packets/arp-hdr.h struct
    Will also interact with the sqlite database if it is specified to do so 
    in the config file.
  
  @TODO: Add decoding for more layer two protocols(especially distance protocols and STP)
  
*/


void arpdecode(const unsigned char * pkt, const int len){
  if(packet_print) printf("%s",__ARP_BOTH);

  struct arp_hdr * arp = (struct arp_hdr *)(pkt + ETH_HDR_SZ);

  switch(arp->opcode){
    case 0x0100:
    case 6144:{ // request:
      // printf("arp request\n");
      char dest_ip[32];
      char src_ip[32];
      strncpy(dest_ip,(char *)u8_ipv4_ntoa((uint8_t *)&arp->dst_ip),sizeof(dest_ip));
      strncpy(src_ip,(char *)u8_ipv4_ntoa((uint8_t *)&arp->src_ip),sizeof(src_ip));

      if(!strncmp(src_ip,"0.0.0.0",5) ){
        if(packet_print) printf("PROTO ARP: PROBE: Who has %s?\n",dest_ip);
        // break;
      } else {
        if(packet_print) printf("PROTO ARP: Who is at %s? Tell %s\n",dest_ip, src_ip);
      }
      break;
    }

    case 0x0002:
    case 512:{ // reply
      // printf("arp reply\n");
      char src_mac[64];
      char src_ip[32];
      // char * src_mac = mac_ntoa((uint8_t)*arp->src_mac);
      strncpy(src_mac,(char*)mac_ntoa(arp->src_mac),sizeof(src_mac));
      strncpy(src_ip, (char*)u8_ipv4_ntoa((uint8_t *)&arp->src_ip),sizeof(src_ip));


      if(entry_exists((char *)&src_ip,(char *)&src_mac) != -1){
        compare_entries((char *)&src_ip,(char *)&src_mac);
      } else {
        if(use_sqlite){
          pthread_t pthrd;
          update_db_t update;
          update.update_type = ARP_UP_T;
          strcpy(update.ip_addr,src_ip);
          strcpy(update.mac_addr,src_mac);
          pthread_create(&pthrd,NULL,update_db,&update);
        }
        add_entry((char *)&src_ip,(char *)&src_mac);

      }
      
      
      
      if(packet_print) printf("PROTO ARP: REPLY: %s is at %s\n",src_ip,src_mac);
      break;
    }
    default:{
      printf("Unknown ARP opcode: %d\n",arp->opcode);
      break;
    }
  }
  printf("%s",__END_COLOR_STREAM);

}


void loopback_ctp_decode(const unsigned char * pkt){
  struct config_test_proto * ctp_data = (struct config_test_proto *)(pkt + ETH_HDR_SZ);
  struct ethhdr * ethernet_hdr = (struct ethhdr *)(pkt);
  char src_mac[24];
  char dest_mac[24];
  strncpy(src_mac, (char*)uc_mac_ntoa(ethernet_hdr->h_source), sizeof(src_mac));
  strncpy(dest_mac,(char*)uc_mac_ntoa(ethernet_hdr->h_dest),sizeof(dest_mac));
  if(packet_print) printf("LOOP %s -> %s",src_mac,dest_mac);
  
  switch(ctp_data->relevant_func){
    case 1:
      if(packet_print) printf(" REPLY \n");
      break;
    default:
      if(packet_print) printf(" UNKNOWN LOOP FUNCTION:%d \n",ctp_data->relevant_func);
      break;
  }
}
