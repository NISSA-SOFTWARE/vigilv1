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
#include "base64.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/* Everything is this file is a modified version of what was found on stackoverflow */
char * base64_encode(const unsigned char * data, size_t input_len){
  int output_len = 4 *((input_len + 2) / 3);

  char *encoded_data = malloc(output_len);
  if(encoded_data == NULL) return NULL;

  for(int i = 0, j = 0; i < input_len;){
    uint32_t octet_a = i < input_len ? (unsigned char *)data[i++] : 0;
    uint32_t octet_b = i < input_len ? (unsigned char *)data[i++] : 0;
    uint32_t octet_c = i < input_len ? (unsigned char *)data[i++] : 0;
    
    uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

    encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3f];
    encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3f];
    encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3f];
    encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3f];
  }
  for( int i = 0; i < mod_table[input_len % 3]; i++)
    encoded_data[output_len -1 - i] = '=';
  return encoded_data;
}
unsigned char * base64_decode(const char * data, size_t input_len){
  if(decoding_table == NULL) build_decoding_table();

  if(input_len % 4 != 0) return NULL;

  int output_len = input_len / 4 * 3;

  if(data[input_len - 1] == '=') (output_len)--;
  if(data[input_len - 2] == '=') (output_len)--;

  unsigned char * decoded_data = malloc(output_len);
  if(decoded_data == NULL) return NULL;

  for(int i = 0, j = 0; i < input_len;){
    uint32_t sextext_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
    uint32_t sextext_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
    uint32_t sextext_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
    uint32_t sextext_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

    uint32_t triple = (sextext_a << 3 * 6) +
                      (sextext_b << 2 * 6) + 
                      (sextext_c << 1 * 6) +
                      (sextext_d << 0 * 6);
    if(j < output_len) decoded_data[j++] = (triple >> 2 * 8) & 0xff;
    if(j < output_len) decoded_data[j++] = (triple >> 1 * 8) & 0xff;
    if(j < output_len) decoded_data[j++] = (triple >> 0 * 8) & 0xff;
  }
  return decoded_data;

}
void build_decoding_table(){
  decoding_table = malloc(256);
  for(int i = 0; i < 64; i++)
    decoding_table[(unsigned char)encoding_table[i]] = i;
}
void base64_cleanup(){
  free(decoding_table);
}
