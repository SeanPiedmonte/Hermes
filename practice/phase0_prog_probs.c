#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void print_base(uint64_t v) {
  uint64_t val = v;
  int i;
  const unsigned char hex_digits[16] = "0123456789ABCDEF";
  printf("DECIMAL: %ld\n", v);

  /* Print Hex */
  printf("0x");
  for (i=15; i >= 0; i--) {
    printf("%c", hex_digits[(val >> (i*4)) & 0xF]);
  }
  printf("\n");

  /* Print Binary */
  for (i=63; i >= 0; i--) {
    (val >> i) & 0x1 ? printf("1") : printf("0");
  } 
  printf("\n");

}

unsigned char *show_bytes_ui32(uint32_t v) {
  int i;
  const unsigned char hex_digits[16] = "0123456789ABCDEF";
  unsigned char *u32 = malloc(9);

  for (i=7; i >= 0; i--) { 
    byte_string[i] = hex_digit[v >> (i*4)) & 0xF];
  }
  byte_string[8] = '\0';

  return byte_string;
}

unsigned char *show_bytes_ui64(uint64_t v) {
  int i;
  const unsigned char hex_digits[16] = "0123456789ABCDEF";
  unsigned char *u32 = malloc(17);

  for (i=15; i >= 0; i--) { 
    byte_string[i] = hex_digit[v >> (i*4)) & 0xF];
  }
  byte_string[16] = '\0';

  return byte_string;
}

unsigned char *show_bytes_f64(double v) {
  int i;
  uint64_t d_val = (uint64_t)v;
  const unsigned char hex_digits[16] = "0123456789ABCDEF";
  unsigned char *u32 = malloc(17);

  for (i=15; i >= 0; i--) { 
    byte_string[i] = hex_digit[v >> (i*4)) & 0xF];
  }
  byte_string[16] = '\0';

  return byte_string;
}

int main(void) {
}
