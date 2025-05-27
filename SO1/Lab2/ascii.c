/*
Program : ascii.c
Course  : SOL1 SS2024
Author  : Oshini Jayaweera
Date    : 07.05.2024
*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h> //library to check if a charater is printable or not
#include <string.h> // allows to use functions to print string on the screen


// (a) Function to print binary representation of a number
void print_binary(int num) {
    for (int i = 7; i >= 0; i--) {
        putchar((num & (1 << i)) ? '1' : '0'); //computing binary values
    }
    
 
}

int main() {
    printf("ASCII-code\ndec\t\thex\t\tbin\t\t\tchar\n");
    for (int i = 0; i <= 126; i++) {

        printf("%03d\t\t0x%02X\t\t", i, i); //decimal & hex
        print_binary(i); //binary values

        if (isprint(i))                  // charcters; check if the dec is printable or not, if printable will print, if not enters a newline
            printf("\t\t%c\n", i);
        else
            printf("\n");
    }
    //(b)
    char string[20];                    //character array
    printf("\nPlease write a text:");
    scanf("%s", &string);
    printf("\"%s\"\n", string); //printing string inside double quatation marks
    printf("string length:%d\n", strlen(string)); // printing string length

    //(c), (d), (e), (f)
    printf("Decimal Output:\n");
    for (int i = 0; i < strlen(string); i++) {// decimal values of each character of the string
        printf("%03d\t", string[i]);
    }
    printf("\nHexadecimal Output:\n");
    for (int i = 0; i < strlen(string); i++) {// hex values of each character of the string
        printf("0x%02X\t", string[i]);
    }
    printf("\nBinary Output:\n");
    for (int i = 0; i < strlen(string); i++) {// binary values of each character of the string
        print_binary(string[i]);
        printf("\t");
    }
    char ch;
    do{
        printf("\nenter a character (type '.' to exit):");
        ch = getch();  //gets characters from the keyboard;difference over scanf is no need of pressing enter key
        printf("%03d\t0x%02X\t", ch, ch);
        print_binary(ch);
        printf("\t\"%c\"\n ", ch);

    } while (ch != '.'); //exit the program when period is pressed
    
    return 0;
}
