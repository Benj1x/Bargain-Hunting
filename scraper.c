#include <stdio.h>
#include "scraper.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char* name;
    char* description;
    double price;
    char* store;
}product;


int cmpfunc(const void* a, const void* b) {

    double priceA = ((product*)a)->price;
    double priceB = ((product*)b)->price;
    if (priceA > priceB)
        return 1;
    if (priceB < priceB)
        return -1;
    else
        return 0;

}

void sortByPrice(product* productList, int listLen) {
    qsort(productList, listLen, sizeof(product), cmpfunc);
}


int main() {
    printf("Hello, World!\n");
    return 0;
}

