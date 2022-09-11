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
  fp.flag = 1;
  char *ptr = hex; // Pointer is memory address of first element
  int period;
  int onto_frac = 0;
  char whole_arr[64];
  char frac_arr[64];
  int index = 0;
  int flow_ctr = 0;
  int whole_ctr = 0;
  int frac_ctr = 0;

  // Locate period, then divide into two parts
  for (int i = 0; i < strlen(hex); i++) {
    // Check for invalid character
    if (!(*ptr == 46) && !(*ptr == 45) && !(*ptr <= 102 && *ptr >= 97) && !(*ptr <= 57 && *ptr >= 48))  {
      fp.flag += 4; // Set error bit in flag
      return fp;
    }

    if(*ptr != '-') {
      flow_ctr++;
    }

    if(*ptr == '.'){ // Returns 0 if identical
      onto_frac = 1;
      flow_ctr = 0;
      index = -1;
    } 

    // printf("\nfinish error check\n",*ptr);
    // Check for overflow
    if(onto_frac == 0 && flow_ctr > 16) {
      fp.flag += 8;
      fp.flag += 4;
      return fp;
    } else if(onto_frac == 1 && flow_ctr > 16) {
      fp.flag += 16;
      fp.flag += 4;
      return fp;
    }
    // printf("\nfinish overflow check\n",*ptr);
    // Check if neg or pos value
    if(*ptr == '-') {
      fp.flag = 2; // Set flag if negative
    }
    // printf("\nfinish negation check\n",*ptr);
    
    // printf("\nfinish decimal check\n",*ptr);
    if ((*ptr != '.')) {
      if (onto_frac == 0) {
        whole_arr[index] = *ptr;
        whole_ctr++; 
      } else if (onto_frac == 1) {
        frac_arr[index] = *ptr;
        frac_ctr++;
      }
    }

    // printf("\nfinish add to array\n",*ptr);
    index++;
    ptr++; // Next element of char array
  }

  // Convert each half to decimal
  // Whole part
  // Test value: char whole_arr[2] = "2b";
  int length = whole_ctr;
  uint64_t whole_sum = 0;
  uint64_t base = 1;
  // printf("\n%d\n",length);
  for(int i = length-1; i>=0; i--) { // Traverse from end, sixeof
    if(whole_arr[i] >= 'A' && whole_arr[i] <= 'F'){ 
        whole_sum += (whole_arr[i] - 55) * base;
        base *= 16;
    } else if (whole_arr[i] >= 'a' && whole_arr[i] <= 'f'){ 
        whole_sum += (whole_arr[i] - 87) * base;
        base *= 16;
    } else if(whole_arr[i] >= '0' && whole_arr[i] <= '9') {
        whole_sum += (whole_arr[i] - 48) * base;
        base *= 16;
    }
  }
  // printf("%lu",whole_sum);
  fp.whole = whole_sum;
  // Fractional part
  uint64_t frac_sum = 0;
  base = pow(16,15);
  // printf("\n%lu\n",base);
  // printf("BEFORE LOOP\n");
  for(int i = 0; i <= frac_ctr-1; i++) { // Traverse from end, sixeof
    // printf("IN LOOP\n");
    // printf("\n%c\n",frac_arr[i]);
    if(frac_arr[i] >= 'A' && frac_arr[i] <= 'F'){ 
        frac_sum += (frac_arr[i] - 55) * base;
        base /= 16;
    } else if (frac_arr[i] >= 'a' && frac_arr[i] <= 'f'){ 
        frac_sum += (frac_arr[i] - 87) * base;
        base /= 16;
    } else if(frac_arr[i] >= '0' && frac_arr[i] <= '9') {
        frac_sum += (frac_arr[i] - 48) * base;
        base /= 16;
    }
  }
  // printf("\n%lu\n",frac_sum);
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
  // printf("Running add\n");
  // printf("Flags Right = %d, Left = %d\n",right.flag,left.flag);
  if ((left.flag & 3) == (right.flag & 3)) { // magnitudes increases ie. + and + or - and - NOTE: Bitwise and comparison
    if ((left.flag & 2) == 2) { //neg + neg --> sum is neg so set flag  NOTE: bitwise and comparison 
      sum.flag = 2;
    } else {
      sum.flag = 1;
    }
    // printf("Running mag increase\n");
    /* TODO: 
    1) overflow of whole --> throw an overflow flag | DONE
    2) overflow of fractional --> carry 1 to whole part | DONE
    */
   //ADD COMPONENTS SEPERATLY 
    sum.whole = left.whole + right.whole;
    sum.fractional = left.fractional + right.fractional; 

    //RESOLVE ISSUES
    //TODO: do we need the final - 1??? 
    if (sum.fractional < left.fractional) {//carry check
      sum.whole += 1;
    }
    if (sum.whole < left.whole) {//overflow check
      sum.flag += 8;
    } 
  } else { //magnitude decreases 
    // printf("Running mag decrease (correct)\n");
    /*
      1) if neg > pos | DONE
      2) borrowing from fractional | DONE

    */
    //SETUP
    //Subtract smaller value from the larger 
    Fixedpoint big, little;
    if (left.whole<right.whole || (left.whole == right.whole && left.fractional < right.fractional)) { //right has the larger magnitude 
    //  printf("Right Bigger\n");
     big = right;
     little = left;
    } else { // right has the higher magnitude 
    // printf("Left Bigger (correct for test)\n");
     big = left;
     little = right;
    }
    //COMPUTATION
    sum.whole = big.whole-little.whole;
    // printf("Added whole\n");
    if (big.fractional < little.fractional) {//need carry in fractional side
      // printf("Flipped Fractional\n");
      sum.whole -= 1;
      sum.fractional =  (((uint64_t)-1) - little.fractional) + big.fractional;//do I need the plus 1?
    } else { //fractional component behaves as expected
      // printf("Regular Fractional\n");
      sum.fractional = big.fractional - little.fractional;
    }
    sum.flag = big.flag; //maintain sign of the larger magnitude component  
    // printf("FINISHED\n");
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
    val.flag += 16;
  }
  val.fractional = val.fractional >> 1; //divide frac by two
  if ((val.whole & 1) == 1) { //need to shift 1 to frac
    val.fractional += (1<<63);
  }
  val.whole = val.whole >> 1; //divide whole by two
  return val;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  if ((val.whole & (1<<63)) == (1<<63)) { //overflow exists
    val.flag += 8;
  }
  val.whole = val.whole << 1; //mult whole by two
  if ((val.fractional & (1<<63)) == (1<<63)) { //need to shift 1 to whole
    val.whole = val.whole | 1;
  }
  val.fractional = val.fractional << 1; //divide whole by two
  return val;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  if ((left.flag & 3) == (right.flag & 3)) { //same sign
    if (left.whole > right.whole) { //left whole is bigger
      return 1;
    } else if (left.whole < right.whole) { //right whole is bigger
      return -1;
    } else { //wholes are the same need to compare fracs
      if (left.fractional > right.fractional) { //left frac is bigger 
        return 1;
      } else if (left.fractional < right.fractional) { //right frac is bigger
        return -1;
      } else { //fracs are the same --> all equal
        return 0;
      }
    }
  } else { //diff sign
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
  int string_ptr = 0;
  printf("START\n");
  char *s = (char*) malloc(34*sizeof(char));
  if((val.flag & 2) == 2){
    printf("INSIDE NEG\n");
    s[string_ptr] = '-';
    string_ptr++;
    printf("\n%s\n",s);
  }
  printf("NEG CHECKED\n");
  uint64_t ptr = 1;
  ptr = (ptr<<63);
  uint64_t back_shift = 1;
  back_shift = (back_shift<<59);
  int leading_zero = 1;
  for(int i = 0; i < 16; i++) { //67
    uint64_t hex = 0;
    for(int j = 0; j < 4; j++){
      printf("#%li BIT VAL: %li ",ptr, ((uint64_t) (val.whole & ptr) >> (uint64_t)back_shift));
      hex += ((uint64_t) (val.whole & ptr) >> back_shift);
      ptr = ptr >> 1;
    }
    //convert hex to char
    if(hex >= 10 && hex <= 15) {// A and F
      hex += 87;
    } else {
      hex += 48;
    }
    // printf("%d\n",hex);
    if (!((leading_zero == 1) && ((hex == 48)))) {
      s[string_ptr] = (char) hex;
      string_ptr++;
      leading_zero = 0;
    }
    back_shift = (back_shift >> 4);
  }
  if (leading_zero == 1) {
    s[string_ptr] = '0';
    string_ptr++;
    leading_zero = 0;
  }
  printf("WHOLE COMPLETE\n");
  if(val.fractional != 0) {
    s[string_ptr] = '.';
    string_ptr++;
    ptr = 1;
    ptr = (ptr<<63);
    back_shift = 1;
    back_shift = (back_shift<<59);
    for(int i = 0; i < 16; i++) { //67
      uint64_t hex = 0;
      for(int j = 0; j < 4; j++){
        printf("#%li BIT VAL: %li ",ptr, ((uint64_t)(val.fractional & ptr) >> (uint64_t) back_shift));
        hex += ((uint64_t) (val.fractional & ptr) >> back_shift);
        ptr = (ptr >> 1);
      }
      //convert hex to char
      if(hex >= 10 && hex <= 15) {// A and F
        hex += 87;
      } else {
        hex += 48;
      }
      printf("%c",hex);
      s[string_ptr] = (char) hex;
      string_ptr++;
      back_shift = (back_shift >> 4);
    }
  }
  printf("FRAC COMPLETE\n");
  printf("\n%s\n",s);
  return s;
}

// Power function instead of using pow
uint64_t pow(uint64_t base, uint64_t power) {
  int ctr = 0;
  uint64_t result = 1;
  while(ctr < power) {
    result *= base;
    ctr++;
  }
  return result;
}
