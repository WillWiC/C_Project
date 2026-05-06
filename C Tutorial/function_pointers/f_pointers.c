    #include <stdio.h>

    void f1(int var)
    {
            printf("this is f1 and var is: %d\n", var);
    }

    void f2(int var)
    {
            printf("this is f2 and var is: %d\n", var);
    }

    void f3(int var)
    {
            printf("this is f3 and var is: %d\n", var);
    }

    int main()
    {
        /* define an array full of function pointers 
        to the above functions, that take an `int` as 
        their only argument */

        void (*pf_1)(int);
        pf_1 = &f1;

        void (*pf_2)(int);
        pf_2 = &f2;


        void (*pf_3)(int);
        pf_3 = &f3;

        void (*function_array[3])(int) = {pf_1,pf_2,pf_3};


		int c = 0;
		while(c < 3)
		{
            /* call the functions using the function pointers
            of the array at index `c` with `c` as an argument */
            function_array[c](c);
    
			c++;
		}

	  return 0;
    }