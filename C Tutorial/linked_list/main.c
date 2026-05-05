#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node {
    int value;
    struct node *next;
}   node_t;

void print_list(node_t *head) {
    node_t *current = head;

    while (current != NULL) {
        printf("%d", current->value);

        if (current->next != NULL) {
            printf(" -> "); // separator between nodes
        }

        current = current->next;
    }

    printf("\n"); // newline at the end
}

void push(node_t * head, int val) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    /* now we can add a new variable */
    current->next = (node_t *) malloc(sizeof(node_t));
    current->next->value = val;
    current->next->next = NULL;
}

int remove_last(node_t * head) {
    int retval = 0;
    /* if there is only one item in the list, remove it */
    if (head->next == NULL) {
        retval = head->value;
        free(head);
        return retval;
    }

    /* get to the second to last node in the list */
    node_t * current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    /* now current points to the second to last item of the list, so let's remove current->next */
    retval = current->next->value;
    free(current->next);
    current->next = NULL;
    return retval;

}

int pop(node_t **head) {
    if (*head == NULL) return -1;

    node_t *temp = *head;
    int value = temp->value;

    *head = temp->next;
    free(temp);

    return value;
}

int remove_by_index(node_t ** head, int n) {
    int i = 0;
    int retval = -1;
    int pop(node_t **head);
    node_t * current = *head;
    node_t * temp_node = NULL;

    if (n == 0) {
        return pop(head);
    }

    for (i = 0; i < n-1; i++) {
        if (current->next == NULL) {
            return -1;
        }
        current = current->next;
    }

    if (current->next == NULL) {
        return -1;
    }

    temp_node = current->next;
    retval = temp_node->value;
    current->next = temp_node->next;
    free(temp_node);

    return retval;

}


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

    /* Assign a new value to the linked list so the 4th will be null */
    push(head, 3);
    print_list(head);

    /* Remove the last value from the linked list so the third will be null */
    remove_last(head);
    print_list(head);


    push(head, 3);
    push(head, 4);
    push(head, 5);

    remove_by_index(&head, 2);
    print_list(head);
    
    return 0;
}