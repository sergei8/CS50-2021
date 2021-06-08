#include <stdio.h>
#include <stdlib.h>
typedef struct person {
    struct person *parents[2];
} person;
person *create_family(int generations);
const int GENERATIONS = 3;
int main() {
    person *p = create_family(GENERATIONS);
  return 0;
}
person *create_family(int generations)
{
    person *p = (person*) malloc(sizeof(person));
    if (generations > 1) {
        generations--;
        p->parents[0] = create_family(generations);
        p->parents[1] = create_family(generations);
    }
    else {
        p->parents[0] = NULL;
        p->parents[1] = NULL;
    }
    return p;
}
