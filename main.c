#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/curl/curl.h"
#include "functions.h"

int main()
{
    //reset files
    FILE* QFile;
    QFile = fopen("QueryResults.txt", "w");
    fclose(QFile);

    FILE* SFile;
    SFile = fopen("./stores.txt", "w");
    fclose(SFile);
    //QFile = fopen("CoopQueryResults.txt", "w");

    //fclose(QFile);

    SDictionary Dictionary = InitDictionary();
    char Product[50] = "\0";

    printf("Hej, og velkommen til! \nI dette program kan du finde de bedste priser paa dine dagligvare!\n");

    storeChoice();

    while (1) {
        int MaxItems = scan_input(Product);

        if (strcasecmp(Product, "end") == 0) {
            free(Dictionary.entry);
            return 0;
        }

        int Runs = 0;

        node* LinkedList = NULL;

        //WriteCoopDataToFile(Product, Dictionary, Runs);
        ReadCoopData(Product, &LinkedList);
        check_input_for_salling(Product);
        GetNonCoopProducts(Product, Dictionary, &LinkedList);
        Runs++;
        final_print(LinkedList, MaxItems);
        DeleteAllListItems(&LinkedList);
    }
}