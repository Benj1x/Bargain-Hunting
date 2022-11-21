#include <stdio.h>
#include "scraper.h"
#include <stdlib.h>
#include <string.h>
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

char* GetSallingProducts(char* Item)
{
    APIStruct SProducts;
    strcpy(SProducts.URL, "https://api.sallinggroup.com/v1-beta/product-suggestions/relevant-products?query=");
    strcat(SProducts.URL, Item);
    strcpy(SProducts.RequestType, "GET");
    strcpy(SProducts.ContentLength, "Content-Length=20");
    strcpy(SProducts.CheckData, ""/*"RetailGroup: \"Kvickly\""*/);
    strcpy(SProducts.KeyTypeAndKey, "Authorization: Bearer dc6422b7-166d-41e8-94c1-6804da7e17d5");
    char* r = APICall(SProducts);

    return r;
    //printf("Hello, Salling!\n");
    free(r);
}

char* GetCoopProducts(char* Item, char* Stores)
{
    char* StoreNumbers;
    
    APIStruct SProducts;
    strcpy(SProducts.URL, "https://api.cl.coop.dk/productapi/v1/product/1290");
    strcpy(SProducts.RequestType, "GET");
    strcpy(SProducts.ContentLength, "Content-Length=20");
    strcpy(SProducts.CheckData, ""/*"RetailGroup: \"Kvickly\""*/);
    strcpy(SProducts.KeyTypeAndKey, "Ocp-Apim-Subscription-Key: fefba58d42c4456ca7182cc307574653");
    char* r = APICall(SProducts);
  
    printf("%s", r);

    return r;
    free(r);

    //printf("Hello, Coop!\n");
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


char* APICall(APIStruct Params)
{
    CURL *curl;
    CURLcode res;
    /*This will init winsock stuff -> Windows only*/
    curl_global_init(CURL_GLOBAL_ALL);
    /* get a curl handle */
    curl = curl_easy_init();

    struct string s;
    if(curl)
    {
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

//This code is from stackoverflow - this seems like the only way to really do this
//https://stackoverflow.com/a/2329792
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

void GetData(char* Items)
{
    FILE *QFile;
    QFile = fopen("QueryResults.txt", "w+");

    char* c = GetSallingProducts("m%C3%A6lk");
    fputs(c, QFile);
    fputs("????", QFile);
    c = GetCoopProducts("aifa", "aifa");
    fputs(c, QFile);

    fclose(QFile);
}
int main()
{
    //printf("Hello, %c!\n", (char) 0x86);
    //char* aifa = "hehea";

    //printf("\nThis was salling \n\n\n\n");
    //GetCoopProducts(aifa, aifa);
    //printf("\nThis was coop \n\n\n\n");

    GetData('x');

    //char query[5] = "toast";
    //product* productArray = GetRemaProducts(query);


    return 0;
}

char* PRScraper()
{
    CURL *curl;
    CURLcode res;

    struct string s;
    if(curl)
    {
        init_string(&s);
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 102400L);
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.pricerunner.dk/pl/35-3202851516/Motherboard/ASUS-PRIME-Z790-P-WIFI-Sammenlign-Priser");
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Joe");
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(curl, CURLOPT_FTP_SKIP_PASV_IP, 1L);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        /*-----------------------------------------------------------*/
        /*Callback will take an argument that is set (This is our string)*/
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        /* Here is a list of options the curl code used that cannot get generated
           as source easily. You may choose to either not use them or implement
           them yourself.

        CURLOPT_WRITEDATA set to a objectpointer
        CURLOPT_WRITEFUNCTION set to a functionpointer
        CURLOPT_READDATA set to a objectpointer
        CURLOPT_READFUNCTION set to a functionpointer
        CURLOPT_SEEKDATA set to a objectpointer
        CURLOPT_SEEKFUNCTION set to a functionpointer
        CURLOPT_ERRORBUFFER set to a objectpointer
        CURLOPT_STDERR set to a objectpointer
        CURLOPT_HEADERFUNCTION set to a functionpointer
        CURLOPT_HEADERDATA set to a objectpointer

        */

        res = curl_easy_perform(curl);

        curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 1002400L);

        curl_easy_setopt(curl, CURLOPT_URL, "<div class=\"EUXXvl3ByR pr-cv9jbm\">.^"/*(?<div class=\"products\">).*?(?</div>)"*/);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "joe");
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(curl, CURLOPT_FTP_SKIP_PASV_IP, 1L);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    }

    return s.ptr;
}