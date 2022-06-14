/*
 * Copyright 2021-2022 Conner Macolley
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdint.h>
#include <uchar.h>
#include <stdbool.h>
#include <time.h>
static __thread unsigned char buffer[128];

unsigned char * mac_ntoa( const uint8_t * hwaddr){
  
  unsigned char * __hwaddr = (unsigned char *)hwaddr;
  snprintf((char *)buffer, sizeof(buffer), "%02x:%02x:%02x:%02x:%02x:%02x",
      __hwaddr[0], __hwaddr[1], __hwaddr[2], __hwaddr[3], __hwaddr[4], __hwaddr[5]
  );
  return buffer;

}
unsigned char * uc_mac_ntoa(unsigned char * __hwaddr){
  snprintf((char *)buffer, sizeof(buffer), "%02x:%02x:%02x:%02x:%02x:%02x",  __hwaddr[0], __hwaddr[1], __hwaddr[2], __hwaddr[3], __hwaddr[4], __hwaddr[5]
  );
  return buffer;
}
unsigned char * u8_ipv4_ntoa(uint8_t * ip_addr){

  unsigned char * __ip_addr = (unsigned char *)ip_addr;
  snprintf((char *)buffer,sizeof(buffer),"%d.%d.%d.%d",
    __ip_addr[0],__ip_addr[1],__ip_addr[2],__ip_addr[3]
  );
  return buffer;
}
unsigned char * ipv4_ntoa( uint32_t  ip_addr){
  // char buffer[18];
  unsigned char * __ip_addr = (unsigned char *)&ip_addr;
  snprintf((char *)buffer,sizeof(buffer),"%d.%d.%d.%d",
    __ip_addr[0],__ip_addr[1],__ip_addr[2],__ip_addr[3]
  );
  return buffer;
  
}


unsigned char * inet6_ntoa(uint8_t * ip_bits){
  unsigned char *bytes;
  memset(buffer,0,128);
  memset(&bytes, 0, sizeof(bytes));
  bytes = (unsigned char *)ip_bits;
  snprintf((char *)buffer, sizeof(buffer),"%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
    ip_bits[0],ip_bits[1],ip_bits[2],ip_bits[3],
    ip_bits[4],ip_bits[5],ip_bits[6], ip_bits[7],
    ip_bits[8], ip_bits[9], ip_bits[10], ip_bits[11],
    ip_bits[12] ,ip_bits[13], ip_bits[14], ip_bits[15]
  );
  return buffer;
}


char * get_formated_time(){
  char *time__ = (char *)malloc(255);
  time_t t = time(NULL);
  struct tm __time = *localtime(&t);
  sprintf(time__,"%d-%02d-%02d %02d:%02d:%02d",
          __time.tm_year + 1900,
          __time.tm_mon + 1,
          __time.tm_mday,
          __time.tm_hour,
          __time.tm_min,
          __time.tm_sec);
  return time__;
}

void cb_get_formatted_time(char * time_buff){
  time_t t = time(NULL);
  struct tm __time = *localtime(&t);
  sprintf(time_buff,"%d-%02d-%02d %02d:%02d:%02d",
          __time.tm_year + 1900,
          __time.tm_mon + 1,
          __time.tm_mday,
          __time.tm_hour,
          __time.tm_min,
          __time.tm_sec);
}


int strloc(const char * haystack,const char needle){
  for(int i = 0; haystack[i] != '\0'; i++){
    if(haystack[i] == needle){
      return i;
    }
  }
  return -1;
}


void fail(const char * modname, const char * msg){
  printf("%s: %s\n",modname,msg);
}


bool char_is_printable(const char c){
  //                                      v test and maybe remove
  if(c < 32 || c > 127) return false;
  return true;
}

bool test_string_arr_print(const char * str){
  for(int i = 0; i < strlen(str); i++){
    if(!char_is_printable(str[i])) return false;
  }
  return true;
}

void hw_addr_to_bytes(const char * hw_addr, uint8_t * dst){
  char * p = strtok(hw_addr,":");
  int times = 0;
  
  while(p != NULL){
    dst[times++] = (uint8_t)strtol(p,NULL,16);
    p = strtok(NULL,":");
  }
}

int get_num_files(const char * path){
  struct dirent* dir;
  DIR * dr = opendir(path);
  if(dr == NULL){
    return -1;
  }
  int files_found = 0;
  while((dir = readdir(dr)) != NULL) files_found++;
  files_found -= 2; // account for . and ..

  closedir(dr);
  return files_found;
}