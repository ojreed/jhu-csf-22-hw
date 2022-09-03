/*
CSF Assignement 1 - Fixedpoint Number Representation
Maddie Estey and Owen Reed
mestey1@jhu.edu and oreed2@jhu.edu
Functions to implement desired functionality of the Fixedpoint struct
*/

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
  int flow_ctr = 0;

  // Locate period, then divide into two parts
  while (ptr != NULL) {
    // Check for invalid character
    if (!46 || !45 || !(ptr < 102 && ptr > 97) || !(ptr < 57 || ptr > 48))  {
      fp.flag += 4; // Set error bit in flag
      break;
    }

    // Check for overflow
    if(onto_frac == 0 && flow_ctr > 8) {
      fp.flag += 8;
    } else if(onto_frac == 1 && flow_ctr > 8) {
      fp.flag += 16;
    }

    // Check if neg or pos value
    if(strcmp(ptr, "-") == 0) {
      fp.flag |= (1 << 1); // Set flag if negative
    } else {
      fp.flag |= 1;
    }

    if(strcmp(ptr, ".") == 0){ // Returns 0 if identical
      onto_frac = 1;
      flow_ctr = 0;
    } 
    
    if (onto_frac == 0) {
      whole_arr[index] = ptr;
    } else if (onto_frac == 1) {
      frac_arr[index] = ptr;
    }

    index++;
    ptr++; // Next element of char array
  }

  // Convert each half to decimal
  // Whole part
  ptr = whole_arr; // Element of string
  uint64_t power = 0; // See if we can reuse other variable later
  uint64_t num;
  uint64_t whole_sum;
  for(int i = sizeof(whole_arr); i > 0; i--) { // Traverse from end
    num = ptr;
    if((uint64_t)ptr >= 97 && (uint64_t)ptr <= 102){ // a-f
      num = (uint64_t)ptr - 55;
    }
    if((uint64_t)ptr >= 48 && (uint64_t)ptr <= 57){ // a-f
      num = (uint64_t)ptr - 48;
    }
    whole_sum += num * pow(16, power);
    power++;
    flow_ctr++;
  }
  fp.whole = whole_sum;

  // Fractional part
  uint64_t frac_sum;
  ptr = frac_arr; // Element of string
  power = -1; 

  for(int i = 0; i < sizeof(frac_arr); i++) { // Traverse from end
    num = ptr;
    if((uint64_t)ptr >= 97 && (uint64_t)ptr <= 102){ // a-f
      num = (uint64_t)ptr - 55;
    }
    if((uint64_t)ptr >= 48 && (uint64_t)ptr <= 57){ // a-f
      num = (uint64_t)ptr - 48;
    }
    frac_sum += num * pow(16, power);
    power--;
    flow_ctr++;
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
  if (left.flag & 3 == right.flag & 3) { // magnitudes increases ie. + and + or - and - NOTE: Bitwise and comparison
    if (left.flag & 2 == 2) { //neg + neg --> sum is neg so set flag  NOTE: bitwise and comparison 
      sum.flag = 2;
    }
    /* TODO: 
    1) overflow of whole --> throw an overflow flag | DONE
    2) overflow of fractional --> carry 1 to whole part | DONE
    */
   //ADD COMPONENTS SEPERATLY 
    sum.whole = left.whole + right.whole;
    sum.fractional = left.fractional + right.fractional; 

    //RESOLVE ISSUES
    //TODO: do we need the final - 1??? 
    if (((1<<64)-1) - left.fractional < right.fractional - 1) {//carry check
      sum.whole + 1;
    }
    if (((1<<64)-1)  - left.whole < right.whole - 1) {//overflow check
      sum.flag += 8;
    } 
  } else { //magnitude decreases 
    /*
      1) if neg > pos | DONE
      2) borrowing from fractional | DONE

    */
    //SETUP
    //Subtract smaller value from the larger 
    Fixedpoint big, little;
    if (left.whole<right.whole || (left.whole == right.whole && left.fractional < right.fractional)) { //right has the larger magnitude 
     big = right;
     little = left;
    } else { // right has the higher magnitude 
     big = left;
     little = right;
    }
    //COMPUTATION
    sum.whole = big.whole-little.whole;
    if (big.fractional < little.fractional) {//need carry in fractional side
      sum.whole -= 1;
      sum.fractional = ((1<<64)-1) - (little.fractional - big.fractional) + 1;//do I need the plus 1?
    } else { //fractional component behaves as expected
      sum.fractional = big.fractional - little.fractional;
    }
    sum.flag = big.flag; //maintain sign of the larger magnitude component  
  }
  
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  return fixedpoint_add(left,fixedpoint_negate(right)); //add the left to the negated right
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  if (val.flag & 2 == 2) { //if negative make positive NOTE: we use "bitwise and" to see if the bit is set to high
    val.flag -= 1; //shift the flag in the 2s bit to the 1s 
  } else { //else make negative
    val.flag += 1;//shift ones bit one to twos
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
  if (val.flag & 4 == 4) { // if error bit set to high
    return 1;
  }
  return 0;
}

int fixedpoint_is_neg(Fixedpoint val) {
  if(val.flag & 2 == 2) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  if(val.flag & 10 == 10) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  if(val.flag & 9 == 9) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  if(val.flag & 18 == 18) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  if(val.flag & 17 == 17) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_valid(Fixedpoint val) {
  if (val.flag <= 2 && val.flag > 0) { //only flags are pos/neg nothing else (and not both which shouldnt happen)
    return 1;
  }
  return 0;
}

// Return a dynamically allocated C character string with the representation of
// the given valid Fixedpoint value.  The string should start with "-" if the
// value is negative, and should use the characters 0-9 and a-f to represent
// each hex digit of the whole and fractional parts. As a special case, if the
// Fixedpoint value represents an integer (i.e., the fractional part is 0),
// then no "decimal point" ('.') should be included.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   dynamically allocated character string containing the representation
//   of the Fixedpoint value
char *fixedpoint_format_as_hex(Fixedpoint val) {
  // TODO: implement
  //assert(0);
  //char *s = malloc(20);
  //strcpy(s, "<invalid>");
  //return s;
}

// Power function instead of using pow
uint64_t pow(uint64_t base, uint64_t power) {
  int ctr = 0;
  int result = 1;
  while(ctr < power) {
    result *= base;
    ctr++;
  }
  return result;
}
