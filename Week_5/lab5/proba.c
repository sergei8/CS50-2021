#include <stdlib.h>
typedef struct person{
    struct person *parents[2];
    // char alleles[2];
} person;
person* create_family(int gen);

int main(){ create_family(2);return(0); }

person* create_family(int gen){
  person *p = (person*) malloc(sizeof(person));
  if (gen > 1){
    gen--;
    person *pa = create_family(gen); 
    // create_family(gen);
  }
  else {
  }
}