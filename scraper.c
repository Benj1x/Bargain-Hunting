#include <stdio.h>
#include <stdlib.h>
#include "external/curl/curl.h"

#include "scraper.h"

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
/*
int main() {
    printf("Hello, Joe!\n");
    char* aifa = "hehea";
    GetSallingProducts(aifa);
    GetCoopProducts(aifa, aifa);
    return 0;
}*/

void GetSallingProducts(char* Item)
{
    printf("Hello, Salling!\n");
}

void GetCoopProducts(char* Item, char* Stores)
{
    char* StoreNumbers;
    GetKardexNumbers(Stores, StoreNumbers);



    printf("Hello, Coop!\n");
}

void GetKardexNumbers(char* Stores, char* KardexNumbers)
{
    CURL *curl;
    CURLcode res;

    char* store = "Fakta";

    char* Query = "https://api.cl.coop.dk/storeapi/v1/stores?RetailGroupFilter=";

    char *strcat(char *query, const char *store);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, Query);
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    /*
    POST https://api.cl.coop.dk/storeapi/v1/stores?RetailGroupFilter=Fakta HTTP/1.1
    Host: api.cl.coop.dk
    Content-Type: application/json
    Ocp-Apim-Subscription-Key: fefba58d42c4456ca7182cc307574653

    {
        Paging: { Size: 10, Page: 1 },
        Querystring: "",
            Filters:
        [
        {
            Type: "RetailGroup",
                RetailGroups: [ "fakta" ]
        }
        ]
    }

*/
}
int main() {
    printf("Hello, Joe!\n");
    char* aifa = "hehea";
    GetSallingProducts(aifa);
    GetCoopProducts(aifa, aifa);
    return 0;
}


product* getRemaProducts(char* query) {
    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

}
