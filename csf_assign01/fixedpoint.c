#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

// You can remove this once all of the functions are fully implemented
static Fixedpoint DUMMY;

Fixedpoint fixedpoint_create(uint64_t whole) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.fractional = 0;
  fp.flag = 1; // Only set first bit (valid non-neg number)
  return fp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint fp = fixedpoint_create(whole);
  fp.fractional = frac;
  return fp;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) { // Hex to decimal
  Fixedpoint fp;
  int *ptr = hex; // Pointer is memory address of first element
  int period;
  int onto_frac = 0;
  char whole_arr[64];
  char frac_arr[64];
  int index = 0;

  // Locate period, then divide into two parts
  While (ptr != NULL) {
    if(strcmp(ptr, "-") == 0) {
      fp.flag |= (1 << 1); // Set flag if negative
    }

    if(strcmp(ptr, ".") == 0){ // Returns 0 if identical
      onto_frac = 0;
    } 
    
    if (onto_frac == 0) {
      whole_arr[index] = ptr;
    } else if (onto_frac == 1) {
      frac_arr[index] = ptr;
    }

    index++;
    ptr++; // Next element of char array
    return fp;
  }

  // Convert each half to decimal
  // Whole part
  ptr = whole_arr; // Element of string
  int power = 0; // See if we can reuse other variable later
  int num;
  int whole_sum;
  for(int i = sizeof(whole_arr); i > 0; i--){ // Traverse from end
    num = ptr;
    if((int)ptr >= 97 && (int)ptr <= 102){ // a-f
      num = (int)ptr - 55;
    }
    whole_sum += num * pow(16, power);
    power++;
  }
  fp.whole = whole_sum;

  // Fractional part
  int frac_sum;
  ptr = frac_arr; // Element of string
  power = -1; 

  for(int i = 0; i < sizeof(frac_arr); i++){ // Traverse from end
    num = ptr;
    if((int)ptr >= 97 && (int)ptr <= 102){ // a-f
      num = (int)ptr - 55;
    }
    frac_sum += num * pow(16, power);
    power--;
  }
  fp.fractional = frac_sum;

  // Return Fixedpoint value
  return fp;
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole; //returns the whole part
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.fractional; //returns the fractional part
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  Fixedpoint sum;
  if (left.flag == right.flag) { // magnitudes increases 
    if (left.flag == 2) { //neg + neg --> sum is neg
      sum.flag = 2;
    }
    /* TODO
    1) overflow of whole --> throw an overflow flag
    2) overflow of fractional --> carry 1 to whole part
    */
    sum.whole = left.whole + right.whole;
    sum.fractional = left.fractional + right.fractional; 
    if (pow(2,64) - left.fractional < right.fractional) {//carry check
      sum.whole + 1;
    }
    if (pow(2,64) - left.whole < right.whole) {//overflow check
      sum.flag = 8;
    } 
  } else { //magnitude decreases 
    /*
      1) if neg > pos --> neg - pos and set sum to negative maybe recursive addition call?? -fp_subtract(neg,pos)
      2) borrowing from fractional 
    */
    if (right.flag == 2) { //right is negative
      sum.whole = left.whole - right.whole;
      sum.fractional = left.fractional - right.fractional;
    } else { //left is negative
      sum.whole = right.whole - left.whole;
      sum.fractional = right.fractional - left.fractional;
    }
  }
  
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  return fixedpoint_add(left,fixedpoint_negate(right)); //add the left to the negated right
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  if (val.flag == 2) { //if negative make positive
    val.flag = 1;
  } else { //else make negative
    val.flag = 2;
  }
  return val; //return flipped val
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_zero(Fixedpoint val) {
  if (val.flag <= 2) { //verify val is in the valid range of flags
    if (val.whole == 0 && val.fractional == 0) { //if both the whole and fractional parts are zero than the FP is zero
      return 1;
    }
  }
  return 0;
}

int fixedpoint_is_err(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_valid(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  // TODO: implement
  assert(0);
  char *s = malloc(20);
  strcpy(s, "<invalid>");
  return s;
}
