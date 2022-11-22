#include <stdio.h>
#include "scraper.h"
#include <stdlib.h>
#include <string.h>
#include "include/curl/curl.h"

enum {
    ae = -86, oe = -87, aa = -111, AE = -91, OE = -65, AA = -96
};

product* salling_scan(FILE* file) {
    int counter = -1;
    while (1) {
        char b = fgetc(file);
        if (feof(file)) {
            break;
        }
        if (b == '}') {
            counter += 1;
        }

    }
    rewind(file);
    product* array = malloc(sizeof(product) * counter);
    while (fgetc(file) != '[') {
    }
    char c;
    int i = 0;
    while (1) {
        c = fgetc(file);
        if (feof(file)) {
            break;
        }
        if (c == '"') {
            char ctgry[100];
            char desc[100];
            double price;
            fscanf(file, "%[^\"]%*c", ctgry);
            if (strcmp(ctgry, "title") == 0) {
                fscanf(file, "%*2s%[^\"]%*c", desc);
                check_DK_char(desc);
                strcpy(array[i].name, desc);
                strcpy(array[i].store, "Bilka");
            }
            if ((strcmp(ctgry, "price") == 0)) {
                fscanf(file, "%*c%lf", &price);
                array[i].price = price;
                i += 1;
            }
        }
    }
    return array;
}

product* rema100_scan(FILE* file) {
    int counter = -1;
    while (1) {
        char b = fgetc(file);
        if (feof(file)) {
            break;
        }
        if (b == '}') {
            counter += 1;
        }

    }
    rewind(file);
    product* array = malloc(sizeof(product) * counter);
    while (fgetc(file) != '[') {
    }
    char c;
    int i = 0;
    while (1) {
        c = fgetc(file);
        if (feof(file)) {
            break;
        }
        if (c == '"') {
            char ctgry[100];
            char desc[100];
            double price;
            fscanf(file, "%[^\"]%*c", ctgry);
            if (strcmp(ctgry, "name") == 0) {
                fscanf(file, "%*2s%[^\"]%*c", desc);
                check_DK_char(desc);
                strcpy(array[i].name, desc);
                strcpy(array[i].store, "Rema1000");
            }
            if ((strcmp(ctgry, "price") == 0)) {
                fscanf(file, "%*c%lf", &price);
                array[i].price = price;
                i += 1;
            }
        }
    }
    return array;
}

void scan_input(char *name, double *max_price)
{
    printf("Indtast produktets navn, saasom 'banan yogurt'>");
    scanf("%[^\n]s", name);
    while ((getchar()) != '\n');
    printf("Indtast maksimal pris i DKK>");
    scanf("%lf", max_price);
}

void check_DK_char(char *string)
{
    int len = strlen(string);
    for (int i = 0; i < len; ++i) {
        switch ((int)string[i]) {
            case ae:
                correct_DK_char(string, i, len, ae);
                break;
            case oe:
                correct_DK_char(string, i, len, oe);
                break;
            case aa:
                correct_DK_char(string, i, len, aa);
                break;
            case AE:
                correct_DK_char(string, i, len, AE);
                break;
            case OE:
                correct_DK_char(string, i, len, OE);
                break;
            case AA:
                correct_DK_char(string, i, len, AA);
                break;
        }
    }
}

void correct_DK_char(char *string, int position, int str_len, int type)
{
    if (type == ae) {
        string[position - 4] = 'a';
        string[position - 3] = 'e';
    }
    else if (type == oe) {
        string[position - 4] = 'o';
        string[position - 3] = 'e';
    }
    else if (type == aa) {
        string[position - 4] = 'a';
        string[position - 3] = 'a';
    }
    else if (type == AE) {
        string[position - 4] = 'A';
        string[position - 3] = 'E';
    }
    else if (type == OE) {
        string[position - 4] = 'O';
        string[position - 3] = 'E';
    }
    else if (type == AA) {
        string[position - 4] = 'A';
        string[position - 3] = 'A';
    }

    int i = 1;
    for (int j = position - 2; j < str_len; ++j) {
        string[j] = string[position + i];
        ++i;
    }
    string[str_len] = '\000';
}

int cmpfunc(const void* a, const void* b)
{
    double priceA = ((product*)a)->price;
    double priceB = ((product*)b)->price;
    if (priceA > priceB)
        return 1;
    if (priceB < priceB)
        return -1;
    else
        return 0;
}

void sortByPrice(product* productList, int listLen)
{
    qsort(productList, listLen, sizeof(product), cmpfunc);
}

char* GetSallingProducts(char* Item)
{
    APIStruct SProducts;
    strcpy(SProducts.URL, "https://api.sallinggroup.com/v1-beta/product-suggestions/relevant-products?query=");
    strcat(SProducts.URL, Item);
    strcpy(SProducts.RequestType, "GET");
    strcpy(SProducts.PostFields, "Content-Length=20");
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
    strcpy(SProducts.CheckData, ""/*"RetailGroup: \"Kvickly\""*/);
    strcpy(SProducts.KeyTypeAndKey, "Ocp-Apim-Subscription-Key: fefba58d42c4456ca7182cc307574653");
    char* r = APICall(SProducts);

    return r;
    free(r);

    printf("Hello, Coop!\n");
}

void GetKardexNumbers(char* Stores, char* KardexNumbers)
{
    APIStruct SKardex;
    strcpy(SKardex.URL, "https://api.cl.coop.dk/storeapi/v1/stores?Content-Length=50");
    strcpy(SKardex.RequestType, "POST");
    strcpy(SKardex.CheckData, "RetailGroup: \"Kvickly\"");
    strcpy(SKardex.KeyTypeAndKey, "Ocp-Apim-Subscription-Key: fefba58d42c4456ca7182cc307574653");
    char* r = APICall(SKardex);
    printf("\n\n\n\n\n\n\n\n\n________________________________________\n\n\n%s", r);

    free(r);
    //printf("Hello, Coop!\n");
}

product* GetRemaProducts(char query[])
{
    APIStruct SProducts;

    char entireQuery[300] = "{\"requests\":[{\"indexName\":\"aws-prod-products\",\"params\":\"query=";
    char rest[200] = "&hitsPerPage=5000&"
        "page=0&"
        "&attributesToRetrieve=%5B%22name%22%2C%22labels%22%2C%22pricing%22%5D&attributesToHighlight=%5B%5D&attributesToSnippet=%5B%5D\"}]}";
    strcat(entireQuery, query);
    strcat(entireQuery, rest);
    strcpy(SProducts.URL, "https://flwdn2189e-dsn.algolia.net/1/indexes/*/queries?x-algolia-agent=Algolia%20for%20vanilla%20JavaScript%203.21.1&x-algolia-application-id=FLWDN2189E&x-algolia-api-key=fa20981a63df668e871a87a8fbd0caed");
    strcpy(SProducts.RequestType, "POST");
    strcpy(SProducts.PostFields, entireQuery);
    char* response = APICall(SProducts);

    FILE* remaproducts = fopen("remaproducts.txt", "w");
    fputs(response, remaproducts);

    fclose(remaproducts);
    // printf("%s", r);

}


char* APICall(APIStruct Params)
{
    CURL* curl;
    CURLcode res;
    /*This will init winsock stuff -> Windows only*/
    curl_global_init(CURL_GLOBAL_ALL);
    /* get a curl handle */
    curl = curl_easy_init();

    struct string s;
    if (curl)
    {
        init_string(&s);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");
        curl_easy_setopt(curl, CURLOPT_CAPATH, "cacert.pem");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, Params.RequestType);
        curl_easy_setopt(curl, CURLOPT_URL, Params.URL);
        if (Params.PostFields != "") curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Params.PostFields);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        /*Set's a callback function to receive incoming data myfunc);*/
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        /*-----------------------------------------------------------*/
        /*Callback will take an argument that is set (This is our string)*/
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        /*-----------------------------------------------------------*/
        struct curl_slist* headers = NULL;
        if (Params.KeyTypeAndKey != "") headers = curl_slist_append(headers, Params.KeyTypeAndKey);
        if (Params.CheckData != "") headers = curl_slist_append(headers, Params.CheckData);
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }
        //free(s.ptr);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return s.ptr;
}

//This code is how the documentation said to do so (spørg Henrik om how to declare)

//This code is from stackoverflow - this seems like the only way to really do this
//https://stackoverflow.com/a/2329792
void init_string(struct string* s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
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
size_t writefunc(void* ptr, size_t size, size_t nmemb, struct string* s)
{
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return size * nmemb;
}


void GetData(char* Items)
{
    FILE* QFile;
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
    printf("Hello, %c!\n", (char) 0x86);
    //char* aifa = "hehea";

    //printf("\nThis was salling \n\n\n\n");
    //GetCoopProducts(aifa, aifa);
    //printf("\nThis was coop \n\n\n\n");
    //GetData('x');

    char query[6] = "kål";

    GetRemaProducts(query);

    char name[30];
    double max_price;
    scan_input(name, &max_price);
    FILE *SFile = fopen("ShoppingList.txt", "w");
    fputs(name, SFile);
    putc(max_price, SFile);

    fclose(SFile);
    FILE *test = fopen("test.txt", "r");
    /*product *array = salling_scan(test);
    for (int i = 0; i < 3; ++i) {
        printf("%s %lf i %s\n", array[i].name, array[i].price, array[i].store);
    }*/
    //char query[5] = "toast";
    //product* productArray = GetRemaProducts(query);

    fclose(test);
    return 0;
}

char* PRScraper()
{
    CURL* curl;
    CURLcode res;

    struct string s;
    if (curl)
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