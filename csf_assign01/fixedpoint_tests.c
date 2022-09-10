/*
CSF Assignement 1 - Fixedpoint Number Representation
Maddie Estey and Owen Reed
mestey1@jhu.edu and oreed2@jhu.edu
Functions to test the functionality of the Fixedpoint struct
*/

#include <stdio.h>
#include <stdlib.h>
#include "fixedpoint.h"
#include "tctest.h"

// Test fixture object, has some useful values for testing
typedef struct {
  Fixedpoint zero;
  Fixedpoint one;
  Fixedpoint one_half;
  Fixedpoint one_fourth;
  Fixedpoint large1;
  Fixedpoint large2;
  Fixedpoint max;

  // TODO: add more objects to the test fixture
} TestObjs;

// functions to create and destroy the test fixture
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// test functions
void test_whole_part(TestObjs *objs);
void test_frac_part(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_negate(TestObjs *objs);
void test_add(TestObjs *objs);
void test_sub(TestObjs *objs);
void test_is_overflow_pos(TestObjs *objs);
void test_is_err(TestObjs *objs);
void test_compare(TestObjs *objs);
void test_halving(TestObjs *objs);
// TODO: add more test functions

int main(int argc, char **argv) {
  // if a testname was specified on the command line, only that
  // test function will be executed
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();
  //SIMPLIFIED MAIN FOR DEBUGGING:
  // TEST(debug_create_from_hex);
  // TEST(debug_add);
  // TEST(debug_add2);
  TEST(test_whole_part);
  TEST(test_frac_part);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_negate);
  TEST(test_add);
  TEST(test_sub);
  TEST(test_is_overflow_pos);
  TEST(test_is_err);
  TEST(test_is_zero);
  TEST(test_compare);
  TEST(test_halving);

  // IMPORTANT: if you add additional test functions (which you should!),
  // make sure they are included here.  E.g., if you add a test function
  // "my_awesome_tests", you should add
  //
  //   TEST(my_awesome_tests);
  //
  // here. This ensures that your test function will actually be executed.

  TEST_FINI();
}

TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));

  objs->zero = fixedpoint_create(0UL);
  objs->one = fixedpoint_create(1UL);
  objs->one_half = fixedpoint_create2(0UL, 0x8000000000000000UL);
  objs->one_fourth = fixedpoint_create2(0UL, 0x4000000000000000UL);
  objs->large1 = fixedpoint_create2(0x4b19efceaUL, 0xec9a1e2418UL);
  objs->large2 = fixedpoint_create2(0xfcbf3d5UL, 0x4d1a23c24fafUL);
  objs->max = fixedpoint_create2(0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL);

  return objs;
}

void cleanup(TestObjs *objs) {
  free(objs);
}

void test_whole_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
}

void test_frac_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void debug_create_from_hex(TestObjs *objs) {
  Fixedpoint test1 = fixedpoint_create_from_hex("a.0");
  printf("a = %lu \n",fixedpoint_whole_part(test1));
  printf("0 = %lu \n",fixedpoint_frac_part(test1));

  Fixedpoint test2 = fixedpoint_create_from_hex("14.0005");
  printf("14 = %lu \n",fixedpoint_whole_part(test2));
  printf("0005 = %lu \n",fixedpoint_frac_part(test2));

  Fixedpoint test3 = fixedpoint_create_from_hex("1.0");
  printf("1 = %lu \n",fixedpoint_whole_part(test3));
  printf("0 = %lu \n",fixedpoint_frac_part(test3));
}

void create_from_hex_segfault(TestObjs *objs) {
  Fixedpoint test1 = fixedpoint_create_from_hex("a.0");
  printf("a = %lu \n",fixedpoint_whole_part(test1));
  printf("0 = %lu \n",fixedpoint_frac_part(test1));

  Fixedpoint test2 = fixedpoint_create_from_hex("14.0005");
  printf("14 = %lu \n",fixedpoint_whole_part(test2));
  printf("0005 = %lu \n",fixedpoint_frac_part(test2));

  Fixedpoint test3 = fixedpoint_create_from_hex("1.0");
  printf("1 = %lu \n",fixedpoint_whole_part(test3));
  printf("0 = %lu \n",fixedpoint_frac_part(test3));
}

void debug_add2(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");

  sum = fixedpoint_add(lhs, rhs);

  printf("a = %lu.%lu \n",fixedpoint_whole_part(lhs),fixedpoint_frac_part(lhs));
  printf("b = %lu.%lu \n",fixedpoint_whole_part(rhs),fixedpoint_frac_part(rhs));
  printf("|a+b| = %lu.%lu \n",fixedpoint_whole_part(sum),fixedpoint_frac_part(sum));
  printf("|a+b| actual = 3503398930554254.35895529729925907281");
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));
}

void debug_add(TestObjs *objs) {
  Fixedpoint a = fixedpoint_create2(1,0);
  Fixedpoint b = fixedpoint_create2(1,1);
  Fixedpoint c = fixedpoint_create2(5,0);

  printf("\n");
  printf("a = %i.%i \n",fixedpoint_whole_part(a),fixedpoint_frac_part(a));
  printf("b = %i.%i \n",fixedpoint_whole_part(b),fixedpoint_frac_part(b));
  printf("c = %i.%i \n",fixedpoint_whole_part(c),fixedpoint_frac_part(c));
  
  Fixedpoint d = fixedpoint_add(a,b);
  printf("a+b = %i.%i \n",fixedpoint_whole_part(d),fixedpoint_frac_part(d));
  ASSERT(fixedpoint_whole_part(d) == 2);
  ASSERT(fixedpoint_frac_part(d) == 1);
  ASSERT(!fixedpoint_is_neg(d));

  Fixedpoint e = fixedpoint_add(a,fixedpoint_negate(b));
  printf("b.flag = %d\n",b.flag);
  printf("-b.flag = %d\n",fixedpoint_negate(b).flag);
  printf("|a-b| = %i.%i \n",fixedpoint_whole_part(e),fixedpoint_frac_part(e));
  ASSERT(fixedpoint_whole_part(e) == 0);
  ASSERT(fixedpoint_frac_part(e) == 1);
  printf("e.flag = %d\n",e.flag);
  ASSERT(fixedpoint_is_neg(e));

  Fixedpoint f = fixedpoint_add(e,c);
  printf("e+c = %lu.%lu \n",fixedpoint_whole_part(f),fixedpoint_frac_part(f));
  ASSERT(fixedpoint_whole_part(f) == 4);
  ASSERT(fixedpoint_frac_part(f) == 18446744073709551614);//dumb because of how FP representation works
  ASSERT(!fixedpoint_is_neg(f));
  
  
}
void test_create_from_hex(TestObjs *objs) {
  (void) objs;

  Fixedpoint val1 = fixedpoint_create_from_hex("f6a5865.00f2");

  ASSERT(fixedpoint_is_valid(val1));

  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val1));

  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val1));
}

void test_is_zero(TestObjs *objs) {
  Fixedpoint whole, frac, both, zero;
  zero = fixedpoint_create2(0,0);
  whole = fixedpoint_create2(1,0);
  both = fixedpoint_create2(1,1);
  frac = fixedpoint_create2(0,1); 
  ASSERT(fixedpoint_is_zero(zero));
  ASSERT(!fixedpoint_is_zero(whole));
  ASSERT(!fixedpoint_is_zero(both));
  ASSERT(!fixedpoint_is_zero(frac));
}

void test_format_as_hex(TestObjs *objs) {
  char *s;

  s = fixedpoint_format_as_hex(objs->zero);
  ASSERT(0 == strcmp(s, "0"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one);
  ASSERT(0 == strcmp(s, "1"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_half);
  ASSERT(0 == strcmp(s, "0.8"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_fourth);
  ASSERT(0 == strcmp(s, "0.4"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large1);
  ASSERT(0 == strcmp(s, "4b19efcea.000000ec9a1e2418"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large2);
  ASSERT(0 == strcmp(s, "fcbf3d5.00004d1a23c24faf"));
  free(s);
}

void test_negate(TestObjs *objs) {
  // none of the test fixture objects are negative
  ASSERT(!fixedpoint_is_neg(objs->zero));
  ASSERT(!fixedpoint_is_neg(objs->one));
  ASSERT(!fixedpoint_is_neg(objs->one_half));
  ASSERT(!fixedpoint_is_neg(objs->one_fourth));
  ASSERT(!fixedpoint_is_neg(objs->large1));
  ASSERT(!fixedpoint_is_neg(objs->large2));

  // negate the test fixture values
  Fixedpoint zero_neg = fixedpoint_negate(objs->zero);
  Fixedpoint one_neg = fixedpoint_negate(objs->one);
  Fixedpoint one_half_neg = fixedpoint_negate(objs->one_half);
  Fixedpoint one_fourth_neg = fixedpoint_negate(objs->one_fourth);
  Fixedpoint large1_neg = fixedpoint_negate(objs->large1);
  Fixedpoint large2_neg = fixedpoint_negate(objs->large2);

  // zero does not become negative when negated
  ASSERT(!fixedpoint_is_neg(zero_neg));

  // all of the other values should have become negative when negated
  ASSERT(fixedpoint_is_neg(one_neg));
  ASSERT(fixedpoint_is_neg(one_half_neg));
  ASSERT(fixedpoint_is_neg(one_fourth_neg));
  ASSERT(fixedpoint_is_neg(large1_neg));
  ASSERT(fixedpoint_is_neg(large2_neg));

  // magnitudes should stay the same
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_add(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));
}

void test_sub(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, diff;

  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa496c124UL == fixedpoint_whole_part(diff));
  ASSERT(0x0905000000000000UL == fixedpoint_frac_part(diff));
}

void test_is_overflow_pos(TestObjs *objs) {
  Fixedpoint sum;

  sum = fixedpoint_add(objs->max, objs->one);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->one, objs->max);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  Fixedpoint negative_one = fixedpoint_negate(objs->one);

  sum = fixedpoint_sub(objs->max, negative_one);
  ASSERT(fixedpoint_is_overflow_pos(sum));
}

void test_is_err(TestObjs *objs) {
  (void) objs;

  // too many characters
  Fixedpoint err1 = fixedpoint_create_from_hex("88888888888888889.6666666666666666");
  ASSERT(fixedpoint_is_err(err1));

  // too many characters
  Fixedpoint err2 = fixedpoint_create_from_hex("6666666666666666.88888888888888889");
  ASSERT(fixedpoint_is_err(err2));

  // this one is actually fine
  Fixedpoint err3 = fixedpoint_create_from_hex("-6666666666666666.8888888888888888");
  ASSERT(fixedpoint_is_valid(err3));
  ASSERT(!fixedpoint_is_err(err3));

  // whole part is too large
  Fixedpoint err4 = fixedpoint_create_from_hex("88888888888888889");
  ASSERT(fixedpoint_is_err(err4));

  // fractional part is too large
  Fixedpoint err5 = fixedpoint_create_from_hex("7.88888888888888889");
  ASSERT(fixedpoint_is_err(err5));

  // invalid hex digits in whole part
  Fixedpoint err6 = fixedpoint_create_from_hex("123xabc.4");
  ASSERT(fixedpoint_is_err(err6));

  // invalid hex digits in fractional part
  Fixedpoint err7 = fixedpoint_create_from_hex("7.0?4");
  ASSERT(fixedpoint_is_err(err7));
}

// TODO: implement more test functions
void test_compare(TestObjs *objs) {
  Fixedpoint a = fixedpoint_create2(56,0);
  Fixedpoint b = fixedpoint_create2(78,15);
  Fixedpoint c = fixedpoint_create2(78,15);
  ASSERT(fixedpoint_compare(a, b) != 0);
  ASSERT(fixedpoint_compare(b, c) == 0);
}

void test_halving(TestObjs *objs) {
  Fixedpoint pt = fixedpoint_create2(1,0); //should end up as 
  Fixedpoint correct = fixedpoint_create(0,((uint64_t)-1));
  Fixedpoint result = fixedpoint_halve(pt);
  ASSERT(0 == fixedpoint_compare(result, correct));
}