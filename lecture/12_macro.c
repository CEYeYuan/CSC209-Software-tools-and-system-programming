#include <stdio.h>

#define CLASS_SIZE 80
#define TAX_RATE 0.08

#define WITH_TAX(x) ((x) * 1.08)

// in this incorrect version we forgot to parenthesize the parameter
#define WITH_TAX2(x) (x * 1.08)

//macro isn't a function. For a function, we may evaluate it's parameter
//before gets passed in, but macro just do the replcacement

int main() {
    double midterm_grades[CLASS_SIZE];
    midterm_grades[1] = 54.3;

    double purchase = 9.99;
    printf("with tax that is %f\n", WITH_TAX(purchase));


    double purchase2 = 12.49;
    printf("with tax that is %f\n", WITH_TAX(purchase + purchase2));
    //WITH_TAX((purchase + purchase2)*1.08)
    printf("with tax that is %f\n", WITH_TAX2(purchase + purchase2));
    //WITH_TAX2(purchase+purchase2*1.08)


    return 0;
}