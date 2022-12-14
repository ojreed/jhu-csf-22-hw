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
  //init impt vars
  Fixedpoint fp;
  fp.flag = 1;
  fp.whole = 0;
  fp.fractional = 0;
  const char *ptr = hex; // Pointer is memory address of first element
  int onto_frac = 0;
  char whole_arr[64];
  char frac_arr[64];
  int index = 0;
  int flow_ctr = 0;
  int whole_ctr = 0;
  int frac_ctr = 0;

  // Locate period, then divide into two parts
  for (uint64_t i = 0; i < strlen(hex); i++) {
    // Check for invalid character

    if (!(*ptr == 46) && !(*ptr == 45) && !(*ptr <= 70 && *ptr >= 65) && !(*ptr <= 102 && *ptr >= 97) && !(*ptr <= 57 && *ptr >= 48))  {
      fp.flag += 4; // Set error bit in flag
      return fp;
    }

    if(*ptr != '-') {//dont count bit size for -
      flow_ctr++;
    }

    if(*ptr == '.'){ // Returns 0 if identical
      onto_frac = 1;
      flow_ctr = 0;
      index = -1;
    } 

    // Check for overflow error
    if(onto_frac == 0 && flow_ctr > 16) { //
      fp.flag += 8;
      fp.flag += 4;
      return fp;
    } else if(onto_frac == 1 && flow_ctr > 16) {//check for underflow error
      fp.flag += 16;
      fp.flag += 4;
      return fp;
    }
    // Check if neg or pos value
    if(*ptr == '-') {
      fp.flag = 2; // Set flag if negative
    }

    if ((*ptr != '.')) { //dont count decimal as a number
      if (onto_frac == 0) { // add to whole
        whole_arr[index] = *ptr; 
        whole_ctr++; 
      } else if (onto_frac == 1) {//add to frac
        frac_arr[index] = *ptr;
        frac_ctr++;
      }
    }
    index++;
    ptr++; // Next element of char array
  }

  // Convert each half to decimal
  // Whole part
  fp.whole = whole_sum(whole_arr,whole_ctr);
  // Fractional part
  fp.fractional = frac_sum(frac_arr,frac_ctr);
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
  sum.flag = 0;
  sum.whole = 0;
  sum.fractional = 0;
  if ((left.flag & 3) == (right.flag & 3)) { // magnitudes increases ie. + and + or - and - NOTE: Bitwise and comparison
    if ((left.flag & 2) == 2) { //neg + neg --> sum is neg so set flag  NOTE: bitwise and comparison 
      sum.flag |= 2;
    } else {
      sum.flag |= 1;
    }
   //ADD COMPONENTS SEPERATLY 
    sum.whole = left.whole + right.whole;
    if ((sum.whole < left.whole) || (sum.whole < right.whole)) {//overflow check
      sum.flag |= 8;
    } 
    sum.fractional = left.fractional + right.fractional; 
    if (sum.fractional < left.fractional) {//carry check
      sum.whole += 1;
    }
    if (((sum.whole < left.whole) || (sum.whole < right.whole)) && ((sum.flag & 8) != 8)) {//overflow check
      sum.flag |= 8;
    } 
  } else { //magnitude decreases 
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
      if (sum.whole >= 1) {// flipped fracs
        sum.whole -= 1;
        sum.fractional =  ((uint64_t) - little.fractional) + big.fractional;
      } else { //zero edge case flipped fracs
        sum.flag |= 2;
        sum.fractional = little.fractional - big.fractional;
        return sum;
      }
    } else { //fractional component behaves as expected
      sum.fractional = big.fractional - little.fractional;
    }
    sum.flag = big.flag; //maintain sign of the larger magnitude component  
  }

  return sum;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  return fixedpoint_add(left,fixedpoint_negate(right)); //add the left to the negated right
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  if (fixedpoint_is_zero(val)) {
    return val;
  }
  if ((val.flag & 2) == 2) { //if negative make positive NOTE: we use "bitwise and" to see if the bit is set to high
    val.flag -= 1; //shift the flag in the 2s bit to the 1s 
  } else { //else make negative
    val.flag += 1;//shift ones bit one to twos
  }
  return val; //return flipped val
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  if ((val.fractional & 1) == 1) { //underflow exists
    val.flag |= 16;
  }
  val.fractional = (val.fractional >> 1); //divide frac by two
  if ((val.whole & 1) == 1) { //need to shift 1 to frac
    val.fractional |= (1UL<<63);
  }
  val.whole = (val.whole >> 1); //divide whole by two
  return val;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  if ((val.whole & (1UL<<63)) == (1UL<<63)) { //overflow exists
    val.flag += 8;
  }
  val.whole = val.whole << 1; //mult whole by two
  if ((val.fractional & (1UL<<63)) == (1UL<<63)) { //need to shift 1 to whole
    val.whole += 1;
  }
  val.fractional = (val.fractional << 1); //divide whole by two
  return val;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  if ((left.flag & 3) == (right.flag & 3)) { //same sign
    int output;
    if (left.whole > right.whole) { //left whole is bigger
      output = 1;
    } else if (left.whole < right.whole) { //right whole is bigger
      output = -1;
    } else { //wholes are the same need to compare fracs
      if (left.fractional > right.fractional) { //left frac is bigger 
        output = 1;
      } else if (left.fractional < right.fractional) { //right frac is bigger
        output = -1;
      } else { //fracs are the same --> all equal
        return 0;
      }
    }
    if ((left.flag & 3) == 1) {
      return output;
    } else {
      return (-1 * output);
    }
  } else { //diff sign
    if (fixedpoint_is_zero(left) && fixedpoint_is_zero(right)) {
      return 0;
    }
    if ((left.flag & 3) == 2) {//left is negative
      return -1;
    } 
    return 1; //right is negative
  }
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
  if ((val.flag & 4) == 4) { // if error bit set to high
    return 1;
  }
  return 0;
}

int fixedpoint_is_neg(Fixedpoint val) {
  if((val.flag & 2) == 2) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  if((val.flag & 10) == 10) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  if((val.flag & 9) == 9) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  if((val.flag & 18) == 18) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  if((val.flag & 17) == 17) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_valid(Fixedpoint val) {
  if ((val.flag <= 2) && (val.flag > 0)) { //only flags are pos/neg nothing else (and not both which shouldnt happen)
    return 1;
  }
  return 0;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  int string_ptr = 0;
  char *s = (char*) malloc(35*sizeof(char));
  if((val.flag & 2) == 2){ //insert negative sign if needed
    s[string_ptr] = '-';
    string_ptr++;
  }
  //init imporant vars
  uint64_t ptr = 1;
  ptr = (ptr<<63);
  int back_shift = 60;
  int leading_zero = 1;
  for(int i = 0; i < 16; i++) { //loop through all sets of 4 bits of whole
    uint64_t hex = 0;
    for(int j = 0; j < 4; j++){ //loop through chunk of 4 bits
      uint64_t temp = (val.whole & ptr);
      hex += (temp>>back_shift);
      ptr = ptr >> 1;
    }
    //convert hex to char
    if(hex >= 10 && hex <= 15) {// A - F
      hex += 87;
    } else { //digits
      hex += 48;
    }
    if (!((leading_zero == 1) && ((hex == 48)))) { //make sure we dont save leading zeros
      s[string_ptr] = (char) hex; //add correct char 
      string_ptr++;
      leading_zero = 0;
    }
    back_shift -= 4;
  }
  if (leading_zero == 1) { // kill leading zero stipulation
    s[string_ptr] = '0';
    string_ptr++;
    leading_zero = 0;
  }
  if(val.fractional != 0) { // only add fractional part if one exists 
    s[string_ptr] = '.'; //add decimal 
    string_ptr++;
    //init impt vars
    ptr = 1;
    ptr = (ptr<<63);
    back_shift = 60;
    int trail_zeros = 0;
    for(int i = 0; i < 16; i++) { //loop through 16 chunks of 4 bits
      uint64_t hex = 0;
      for(int j = 0; j < 4; j++){//loop through chunks of 4
        uint64_t temp = (val.fractional & ptr);
        hex += (temp>>back_shift);
        ptr = (ptr >> 1);
      }
      //convert hex to char
      if(hex >= 10 && hex <= 15) {// A and F
        hex += 87;
      } else {
        hex += 48;
      }

      if (hex != 48) { //only add a char directly if not a zero
        for (int i = 0; i <trail_zeros; i++) { //add saved up zeros
          s[string_ptr] = (char) 48;
          string_ptr++;
        }
        s[string_ptr] = (char) hex; //add correct char
        string_ptr++;
        trail_zeros = 0;
      } else { //if we find a zero tally it up in case we need to add it (if zeros arent trail)
        trail_zeros++;
      }
      back_shift -= 4;
    }
  }
  s[string_ptr] = '\0';//end string
  return s;
}

// Power function instead of using pow
uint64_t poww(uint64_t base, uint64_t power) {
  uint64_t ctr = 0;
  uint64_t result = 1;
  while(ctr < power) {
    result *= base;
    ctr++;
  }
  return result;
}

//Function to convert chars into the whole number 
uint64_t whole_sum(char whole_arr[], int length){
  uint64_t whole_sum = 0;
  uint64_t base = 1;
  for(int i = length-1; i>=0; i--) { // Traverse from end, sixeof
    if(whole_arr[i] >= 'A' && whole_arr[i] <= 'F'){ //uppercase hex
        whole_sum += (whole_arr[i] - 55) * base;
        base *= 16;
    } else if (whole_arr[i] >= 'a' && whole_arr[i] <= 'f'){ //lowercase hex
        whole_sum += (whole_arr[i] - 87) * base;
        base *= 16;
    } else if(whole_arr[i] >= '0' && whole_arr[i] <= '9') {//digits
        whole_sum += (whole_arr[i] - 48) * base;
        base *= 16;
    }
  }
  return whole_sum;
}

//Function to convert chars into the fractional number 
uint64_t frac_sum(char frac_arr[], int frac_ctr){
  uint64_t frac_sum = 0;
  uint64_t base = poww(16,15);
  for(int i = 0; i <= frac_ctr-1; i++) { // Traverse from end, sixeof
    if(frac_arr[i] >= 'A' && frac_arr[i] <= 'F'){ //uppercase hex
        frac_sum += (frac_arr[i] - 55) * base;
        base /= 16;
    } else if (frac_arr[i] >= 'a' && frac_arr[i] <= 'f'){  //lowercase hex
        frac_sum += (frac_arr[i] - 87) * base;
        base /= 16;
    } else if(frac_arr[i] >= '0' && frac_arr[i] <= '9') { //digits
        frac_sum += (frac_arr[i] - 48) * base;
        base /= 16;
    }
  }
  return frac_sum;
}

