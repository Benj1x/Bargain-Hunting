#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h> //access()
#include "scraper.h"

void store_choice();


int main() {
    printf("Hello, I will sell your personal information\n");

    store_choice();

    return 0;
}


void store_choice() {
    FILE *stores;
    char store_id [5];
    char store_name [25];

    /*if (access(stores, F_OK) != 0) {
        stores = fopen("stores.txt", "w+");
    } 
    else {
        stores = fopen("stores.txt", "r+");
    }*/


    stores = fopen("C:\\Users\\rasmu\\Documents\\GitHub\\Bargain-Hunting\\stores.txt", "a+");
    
    if (stores == NULL) {
        printf("File couldn't be opened. Creating new file\n");
        fclose(stores);
        stores = fopen("C:\\Users\\rasmu\\Documents\\GitHub\\Bargain-Hunting\\stores.txt", "a+");        
    }


    while (1) {
        printf("Enter store name, break with 'q':");
        scanf("%[^\n]", &store_name);
        while (getchar() != '\n');
        if (store_name[0] == 'q') {
            break;
        }
        else fprintf(stores, "%s\n", store_name);

    }  
    fclose(stores);

    stores = fopen("C:\\Users\\rasmu\\Documents\\GitHub\\Bargain-Hunting\\stores.txt", "r");
    int i = 0;
    while (!feof(stores)) {
        ++i;
        fscanf(stores, "%[^\n]", store_name);
        printf("Store number %d: %s\n", i, store_name);
    }
    fclose(stores);

}