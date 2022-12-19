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

    //QFile = fopen("CoopQueryResults.txt", "w");
    //fclose(QFile);
    QFile = NULL;

    FILE* SFile;
    SFile = fopen("./stores.txt", "w");
    fclose(SFile);
    SFile = NULL;

    SDictionary Dictionary = InitDictionary();
    char Product[50] = "\0";

    printf("Hello, and welcome! \nIn this program you can find the best prices on your groceries!\n");
    printf("(Please note, this program does not support scandinavian letters)\n");
    StoreChoice();
    int Runs = 0;

    while (1) {
        int MaxItems = ScanInput(Product);

        if (strcasecmp(Product, "end") == 0) {
            free(Dictionary.entry);
            return 0;
        }

        node* LinkedList = NULL;

        //WriteCoopDataToFile(Product, Dictionary, Runs);
        ReadCoopData(Product, &LinkedList);
        CheckInputForSalling(Product);
        GetNonCoopProducts(Product, Dictionary, &LinkedList);
        Runs++;
        FinalPrint(LinkedList, MaxItems);
        DeleteAllListItems(&LinkedList);
    }
}