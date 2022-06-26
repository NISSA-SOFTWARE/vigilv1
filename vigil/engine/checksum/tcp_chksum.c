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
#include "tcp_chksum.h"
#include "../../packets/tcp.h"
#include "../../packets/ip_hdr.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
bool tcp_check_sum_passed(struct ip_hdr * ip_header, struct __tcp * tcp_hdr){
  register unsigned long sum = 0;
  unsigned short tcp_len = ntohs(ip_header->tot_len) - (ip_header->ihl<<2); 

  const unsigned long recv_chksum = tcp_hdr->check;

  unsigned short * ippayload = (unsigned short *)&tcp_hdr;
    tcp_hdr->check = 0x0000;
  /* adding psuedo header */
  sum += (ip_header->saddr >> 16) & 0xffff;
  sum += (ip_header->saddr) & 0xffff;
  sum += (ip_header->daddr >> 16) & 0xffff;
  sum += (ip_header->daddr) & 0xffff;
  sum += htons(IPPROTO_TCP);
  sum += htons(tcp_len);


  while(tcp_len > 1){
    sum += *(unsigned char*)tcp_hdr++;
    tcp_len -= 2;
  } 
  if(tcp_len > 0){
    // padding
    sum += ((*(unsigned char*)tcp_hdr));// & htons(0xffff));
  }
  while(sum >> 16){
    sum = (sum & 0xffff) + (sum >> 16);
  }
  sum = ~sum;


  tcp_hdr->check = (unsigned short)recv_chksum;
  printf("%02x -- %02x\n",ntohs(sum),ntohs(tcp_hdr->check));

  if(ntohs(sum) == ntohs(recv_chksum)) return true;
  return false;
  
}   