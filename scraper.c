#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scraper.h"


int main() {

    char name[30];
    double max_price;
   // scan_input(name, &max_price);
    FILE* test = fopen("test.txt", "r");
    COOP_API_return(test);
    return 0;
}


product COOP_API_return(FILE* file) {
    char counter_string[100];
    int counter = 0;
    while (fgets(counter_string, 100, file)){
        if (counter_string[0] == '}' )
        counter += 1;
    }

    product* array = malloc(sizeof(product)*counter);
    char string[100];
    char category[15];
    char desc_name[50];
    char desc_name2[50];
    double desc_value;
    product p;
    rewind(file);
    while(fgets(string,100, file)) {
        if (string[2] == '"') {
            sscanf(string, "  %*c%[^\"]", category);
            if (strcmp(category, "Navn") == 0) {
                strcpy(desc_name, category);
                sscanf(string, "%*c%*c %*c%[^\"]", category);
            }

        }
    }
}


void scan_input(char *name, double *max_price) {

    printf("Indskriv produktets navn, saasom 'banan yogurt'>");
    scanf("%[^\n]s", name);
    while ((getchar()) != '\n');
    printf("Indtast maksimal pris i DDK>");
    scanf("%lf", max_price);
}

