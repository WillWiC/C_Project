#include <stdio.h>
#include <string.h>

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

int sum(int num_1) {
    static int sum;
    sum += num_1;

    return sum;
}


int main(){
    
    // Hello world tutorial
    printf("Hello World!\n");

    // Integer Variable tutorial
    short number_8 = 8;        // small integer (>=16 bits)
    int number_16 = 16;        // standard integer (usually 32 bits)
    long number_32 = 32;       // large integer (>=32 bits)
    long long number_64 = 64;  // very large integer (>=64 bits)


    int sum_number = number_8 + number_16 / number_32 * number_64;

    printf("%d\n", sum_number);


    // Array tutorial
    int a_grades[3]; // if is 3 then 0,1,2 inclusively, defined by how many numbers inside the array
    int a_average;

    a_grades[0] = 80;
    a_grades[1] = 85;
    a_grades[2] = 90;

    a_average = (a_grades[0]+ a_grades[1]+ a_grades[2])/3;

    printf("The average of the 3 grades is %d\n", a_average);


    // Multidimensional arrays tutorial
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

    //Condition Tutorial
    guessNumber(500);
    guessNumber(600);
    guessNumber(555);

    
    // String Tutorial
    char first_name[20] = "John";
    char last_name[20] = "Doe";
    strncat(first_name,last_name, 3);
    printf("%s\n", first_name);
    
    // For Loops Tutorial

    int array[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int final_result = 1;
    int i;

    int array_size = sizeof(array)/sizeof(array[0]);

    for (i = 0; i < 8; i++) {
        final_result *= array[i];
    }
    printf("This is the factorial for %d: %d \n", array_size, final_result);
    
    // While Loops Tutorial
    int while_array[10] = {1,2,3,4,5,6,7,8,9,10};
    int while_index = 0;

    while (while_index < 9) {
        
        while_index++;
        if (while_index < 4){
            continue;
        }            

        printf("This is the while loop tutorial answer: %d \n", while_array[while_index]);
        
    }


    // Function Tutorial
    print_big(15);
    print_big(1);

    // Static Tutorial
    printf("Sum = %d \n", sum(15));
    printf("Sum = %d \n", sum(55));

    // Pointers Tutorial
    int n = 0;
    int * pointer_to_n = &n;
    int p_i;

    for ( p_i = 0; p_i < 5; p_i++){
        *pointer_to_n+=1;
    }

    printf("%d", n);

    return 0;
}

