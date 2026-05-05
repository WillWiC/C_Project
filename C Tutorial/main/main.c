#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct person {
    char * name;
    int age;
}   person_t;

typedef struct node {
    int value;
    struct node *next;
}   node_t;

int sum(int num_1) {
    static int sum;
    sum += num_1;

    return sum;
}

unsigned int factorial(unsigned int num){

    if (num == 0){
        return 1;
    }

    else if (num == 1){
        return num;
    }

    else {
        return num * factorial(num-1);
    }
}

void guessNumber(int guess){
    if (guess == 555) {
        printf("Correct, You guess it! \n");
    } else if ( guess < 555) {
        printf("Your guess is too low \n");
    } else if ( guess > 555) {
        printf("Your guess is too high \n");
    }
}

void print_big(int number){
    if (number > 10){
        printf("%d is big number. \n", number);
    } else {
        printf("Invalid big number. \n");

    }
}

void birthday(int *age) {
    (*age)++;
}

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

int main(){
    
    // Hello world 
    printf("Hello World!\n");

    // Integer Variable 
    short number_8 = 8;        // small integer (>=16 bits)
    int number_16 = 16;        // standard integer (usually 32 bits)
    long number_32 = 32;       // large integer (>=32 bits)
    long long number_64 = 64;  // very large integer (>=64 bits)


    int sum_number = number_8 + number_16 / number_32 * number_64;

    printf("%d\n", sum_number);


    // Array 
    int a_grades[3]; // if is 3 then 0,1,2 inclusively, defined by how many numbers inside the array
    int a_average;

    a_grades[0] = 80;
    a_grades[1] = 85;
    a_grades[2] = 90;

    a_average = (a_grades[0]+ a_grades[1]+ a_grades[2])/3;

    printf("The average of the 3 grades is %d\n", a_average);


    // Multidimensional arrays 
    // multidimensional is like matrix/matrices in math
    float m_average;
    int m_grades[2][5];
    int students;
    int subjects;

    // 1st Subject
    m_grades[0][0] = 80;
    m_grades[0][1] = 70;
    m_grades[0][2] = 65;
    m_grades[0][3] = 89;
    m_grades[0][4] = 90;

    // 2nd Subject
    m_grades[1][0] = 85;
    m_grades[1][1] = 80;
    m_grades[1][2] = 80;
    m_grades[1][3] = 82;
    m_grades[1][4] = 87;

    for (subjects = 0; subjects < 2 ; subjects++) {
        m_average = 0;
        for (students = 0; students < 5 ; students++) {
            m_average += m_grades[subjects][students];
        }
        printf("The average marks obtained in subject %d is: %.2f\n", subjects, m_average);
    }

    //Condition 
    guessNumber(500);
    guessNumber(600);
    guessNumber(555);

    
    // String 
    char first_name[20] = "John";
    char last_name[20] = " Doe";
    char * whole_name = "John Doe";
    strncat(first_name,last_name, 4);
    printf("%s and %s\n", first_name, whole_name);
    
    // For Loops

    int array[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int final_result = 1;
    int i;

    int array_size = sizeof(array)/sizeof(array[0]);

    for (i = 0; i < 8; i++) {
        final_result *= array[i];
    }
    printf("This is the factorial for %d: %d \n", array_size, final_result);
    
    // While Loops
    int while_array[10] = {1,2,3,4,5,6,7,8,9,10};
    int while_index = 0;

    while (while_index < 9) {
        
        while_index++;
        if (while_index < 4){
            continue;
        }            

        printf("This is the while loop tutorial answer: %d \n", while_array[while_index]);
        
    }


    // Function
    print_big(15);
    print_big(1);

    // Static
    printf("Sum = %d \n", sum(15));
    printf("Sum = %d \n", sum(55));

    // Pointers
    int n = 0;
    int * pointer_to_n = &n;
    int p_i;

    for ( p_i = 0; p_i < 5; p_i++){
        *pointer_to_n+=1;
    }

    printf("%d \n", n);

    // Struc & Function arguments by reference
    person_t p_person;
    p_person.name = "Will_Dev";
    p_person.age = 23;

    birthday(&p_person.age);
    printf("Hello, my name is %s and my age is %d \n", p_person.name, p_person.age);


    // Dynamic Allocation
    person_t * myperson = (person_t *) malloc(sizeof(person_t));

    myperson->age = 40;
    myperson->name = "Christiano Ronaldo";

    printf("Memory Allocation, Name: %p, Age: %p \n", myperson->name, myperson->age);
    printf("Hello, my name is %s and my age is %d \n", myperson->name, myperson->age);
    free(myperson); // To free up the memory that is stored, will not wipe the information inside

    // Array and Pointer
    
    int j;
    /* TODO: define the 2D pointer variable here */
    int **pnumbers;

    /* TODO: complete the following line to allocate memory for holding three rows */
    pnumbers = (int **) malloc(3 *sizeof(int *));

    /* TODO: allocate memory for storing the individual elements in a row */
    pnumbers[0] = (int *) malloc(1 * sizeof(int));
    pnumbers[1] = (int *) malloc(2 * sizeof(int));
    pnumbers[2] = (int *) malloc(3 * sizeof(int));

    pnumbers[0][0] = 1;
    pnumbers[1][0] = 1;
    pnumbers[1][1] = 1;
    pnumbers[2][0] = 1;
    pnumbers[2][1] = 2;
    pnumbers[2][2] = 1;

    for (i = 0; i < 3; i++) {
        for (j = 0; j <= i; j++) {
            printf("%d", pnumbers[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < 3; i++) {
        free(pnumbers[i]);
    }

    /* TODO: free the top-level pointer */
    free(pnumbers);
    

    // Recursion 
    printf("0! = %i\n", factorial(0));
    printf("1! = %i\n", factorial(1));
    printf("3! = %i\n", factorial(3));
    printf("5! = %i\n", factorial(5));

    return 0;
}

