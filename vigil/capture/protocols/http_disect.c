#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils.h"
#include "../../filter/parsing/rule.h"



void http_disect(const unsigned char * pkt, const struct rule_data * rdata){
  // printf("%s\n",pkt);
  // int loc = strloc((const char *)pkt,0x0d);
  // printf("Strloc successful\n");
  // if(!test_string_arr_print((const char *)pkt)) return;
  // char request_hdr[1024];
  // strncpy(request_hdr,(const char *)pkt,loc );

  // printf("%s\n",request_hdr);
  
}

