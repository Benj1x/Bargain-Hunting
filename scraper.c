#include <stdio.h>
#include "scraper.h"
#include <stdlib.h>
#include <stdio.h>
#include "include/curl/curl.h"

typedef struct {
    char name[20];
    char description[50];
    double price;
    char store[10];
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


product* GetRemaProducts(char query[]) {
    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://flwdn2189e-dsn.algolia.net/1/indexes/*/queries?x-algolia-agent=Algolia%%20for%%20vanilla%%20JavaScript%%203.21.1&x-algolia-application-id=FLWDN2189E&x-algolia-api-key=fa20981a63df668e871a87a8fbd0caed");

        char entireQuery[200] = "{\"requests\":[{\"indexName\":\"aws-prod-products\",\"params\":\"query=";
        char rest[200] = "&hitsPerPage=5000&"
            "page=0&"
            "attributesToRetrieve=%%5B%%22name%%22%%2C%%22labels%%22%%2C%%22pricing%%22%%5D&attributesToSnippet=%%5B%%5D\"}]}";
        strcat(entireQuery, query);
        strcat(entireQuery, rest);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, entireQuery);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}

int main() {
    printf("Hello, Joe!\n");
    char query[6] = "toast";
    product* productArray = GetRemaProducts(query);
    return 0;
}

