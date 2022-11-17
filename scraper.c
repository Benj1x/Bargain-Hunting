#include <stdio.h>
#include "scraper.h"
#include <stdlib.h>
#include <string.h>
#include "include/curl/curl.h"

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

void GetSallingProducts(char* Item)
{
    printf("Hello, Salling!\n");
}

void GetCoopProducts(char* Item, char* Stores)
{
    char* StoreNumbers;
    //GetKardexNumbers(Stores, StoreNumbers);

    APIStruct SProducts;
    strcpy(SProducts.URL, "https://api.cl.coop.dk/productapi/v1/product/1290");
    strcpy(SProducts.RequestType, "GET");
    strcpy(SProducts.ContentLength, "Content-Length=20");
    strcpy(SProducts.CheckData, ""/*"RetailGroup: \"Kvickly\""*/);
    strcpy(SProducts.KeyTypeAndKey, "Ocp-Apim-Subscription-Key: fefba58d42c4456ca7182cc307574653");
    char* r = APICall(SProducts);

    printf("\n\n\n\n\n\n\n\n\n________________________________________\n\n\n%s", r);


    free(r);



    printf("Hello, Coop!\n");
}

void GetKardexNumbers(char* Stores, char* KardexNumbers)
{
    APIStruct SKardex;
    strcpy(SKardex.URL, "https://api.cl.coop.dk/storeapi/v1/stores?Content-Length=50");
    strcpy(SKardex.RequestType, "POST");
    strcpy(SKardex.ContentLength, "Content-Length=20");
    strcpy(SKardex.CheckData, "RetailGroup: \"Kvickly\"");
    strcpy(SKardex.KeyTypeAndKey, "Ocp-Apim-Subscription-Key: fefba58d42c4456ca7182cc307574653");
    char* r = APICall(SKardex);
    printf("\n\n\n\n\n\n\n\n\n________________________________________\n\n\n%s", r);


    free(r);
    //printf("\n\n\n\n\n\n\n\n\n________________________________________\n\n\n%s", &c);
}

char* APICall(APIStruct Params)
{
    CURL *curl;
    CURLcode res;
    /*This will init winsock stuff -> Windows only*/
    curl_global_init(CURL_GLOBAL_ALL);
    /* get a curl handle */
    curl = curl_easy_init();
    char* Result[100000];
    struct string s;
    if(curl) {

        init_string(&s);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, Params.RequestType);
        curl_easy_setopt(curl, CURLOPT_URL, Params.URL);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        /*https://stackoverflow.com/a/2329792*/
        /*Set's a callback function to receive incoming data myfunc);*/
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        /*-----------------------------------------------------------*/
        /*Callback will take an argument that is set (This is our string)*/
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        /*-----------------------------------------------------------*/
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, Params.KeyTypeAndKey);
        headers = curl_slist_append(headers, Params.CheckData);
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        const char *data = Params.ContentLength;
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        //printf("\n\n\n\n\n\n\n\n\n________________________________________\n\n\n%s", s.ptr);
        //free(s.ptr);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return s.ptr;
}
//This code is how the documentation said to do so (spørg Henrik om how to declare)

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}
/*
 * @param *ptr
 * @param size
 * @param nmemb
 * @param string *s
 */
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

int main() {
    printf("Hello, Joe!\n");
    char* aifa = "hehea";
    GetSallingProducts(aifa);
    GetCoopProducts(aifa, aifa);
    return 0;
}