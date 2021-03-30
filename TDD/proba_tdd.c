#include <stdio.h>
#include "minunit.h"

 int tests_run = 0;
 
 int foo = 7;
 
 static char * test_foo() {
     mu_assert("error, foo != 7", foo == 7);
     return 0;
 }
  
 static char * all_tests() {
     mu_run_test(test_foo);
     return 0;
 }
 
 int main(int argc, char **argv) {
    //  char *result = all_tests();
     int result = mu_run_test(test_foo);
    //  char *result = mu_run_test(test_foo);
     if (result != 0) {
         printf("%i\n", result);
     }
     else {
         printf("ALL TESTS PASSED\n");
     }
 
     return result != 0;
 }

