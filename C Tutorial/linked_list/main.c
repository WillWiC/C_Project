#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node {
    int value;
    struct node *next;
}   node_t;



int main(){

    // Linked List
    node_t *head = NULL;
    head = (node_t *) malloc (sizeof(node_t));
    if (head == NULL) {
        return 1;
    }

    /* Assign head as 1 and next is 2 and the third is null */
    head->value = 1;
    head->next = (node_t *) malloc(sizeof(node_t));
    head->next->value = 2;
    head->next->next = NULL;

    push(head, 3);
    print_list(head);


    return 0;
}