#include <stdlib.h>
typedef struct node {
    int number;
    struct node *next;
} node;

int main() {
    node *list = NULL;
    node *p = malloc(sizeof(node));
    p->number = 100; p->next = NULL; list = p;
    p = malloc(sizeof(node));
    p->number = 200; p->next = NULL; list->next = p; p = NULL;
    p = malloc(sizeof(node));
    p->number = 300; p->next = NULL; list->next->next = p; p = NULL;
    p = &list[0];
    while(p->next != NULL){            
      p = p->next;
    }

    free(list);
    return 0;
}