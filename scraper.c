#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "scraper.h"

void storeChoice();
int storeCheck(char currentInput[]);

int main() {
    printf("Hello\n");

    storeChoice();

    return 0;
}

void storeChoice() {
    FILE* stores;
    char storeID[5];
    char storeName[25];

    stores = fopen("./stores.txt", "a+");

    //Loop for entering wanted stores
    while (1) {
        printf("Enter store name, break with 'q':");
        scanf("%[^\n]", &storeName);
        while (getchar() != '\n');

        //Breaks loop
        if (strcmp(storeName, "q") == 0) {
            break;
        }

        //Check if entered store already is in list
        if (storeCheck(storeName) == 1) {
            printf("\nSame store is already in list\n");
        }

        //Prints given store to list
        else {
            fprintf(stores, "\n%s", storeName);
            printf("Given store is added to list\n\n");
        }
    }
    fclose(stores);

    //Prints stores present in list file
    stores = fopen("./stores.txt", "r");
    int i = 0;

    while (!feof(stores)) {
        ++i;
        fgets(storeName, 25, stores);
        printf("Store number %d: %s\n", i, storeName);
    }
    fclose(stores);
}

//Function for checking if input store is already in list
int storeCheck(char curretInput[]) {
  FILE *stores;
  int storeExists = 0;
  char searchString[20];
  strcpy(searchString, curretInput);

  stores = fopen("./stores.txt", "r");

  char buffer[20];

  while(fgets(buffer, 20, stores)) { 
    char *checkForStore = strstr(buffer, searchString);
    if(checkForStore != NULL) {
        storeExists = 1;
        break;
    }
    else {
        storeExists = 0;
    }
  }
  fclose(stores);

  //Returns 1 if store is found in list
  //Returns 0 if store is not found in list
  return storeExists;
}
  