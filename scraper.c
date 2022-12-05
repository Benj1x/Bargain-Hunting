#include <stdio.h>
#include <stdlib.h>
#include "scraper.h"
#include <strings.h>
#include "include/curl/curl.h"

enum {
    ae = -90, oe = -72, aa = -91, AE = -122, OE = -104, AA = -123
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

product* rema1000_scan(FILE* file, int* nbHits) {
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

    *nbHits = counter;
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

void scan_input(char* name, double* max_price)
{
    printf("Indtast produktets navn, saasom 'banan yogurt'>");
    scanf("%[^\n]s", name);
    while ((getchar()) != '\n');
    printf("Indtast maksimal pris i DKK>");
    scanf("%lf", max_price);

    FILE* SFile = fopen("ShoppingList.txt", "w");
    fputs(name, SFile);
    fprintf(SFile, "%s_%.2lf", name, max_price);

    fclose(SFile);
}

void check_DK_char(char* string)
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

void correct_DK_char(char* string, int position, int str_len, int type)
{
    if (type == ae) {
        string[position - 1] = 'a';
        string[position] = 'e';
    }
    else if (type == oe) {
        string[position - 1] = 'o';
        string[position] = 'e';
    }
    else if (type == aa) {
        string[position - 1] = 'a';
        string[position] = 'a';
    }
    else if (type == AE) {
        string[position - 1] = 'A';
        string[position] = 'E';
    }
    else if (type == OE) {
        string[position - 1] = 'O';
        string[position] = 'E';
    }
    else if (type == AA) {
        string[position - 1] = 'A';
        string[position] = 'A';
    }
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
    SAPIStruct SProducts;
    strcpy(SProducts.URL, "https://api.sallinggroup.com/v1-beta/product-suggestions/relevant-products?query=");
    strcat(SProducts.URL, Item);
    strcpy(SProducts.RequestType, "GET");
    strcpy(SProducts.PostFields, "Content-Length=20");
    strcpy(SProducts.CheckData, ""/*"RetailGroup: \"Kvickly\""*/);
    strcpy(SProducts.KeyTypeAndKey, "Authorization: Bearer dc6422b7-166d-41e8-94c1-6804da7e17d5");
    char* r = APICall(SProducts);
    FILE* aaaa = fopen("salling.txt", "w");
    fputs(r, aaaa);
    fclose(aaaa);
    return r;
    free(r);
}

char* GetCoopProducts(char* Item, char* Stores)
{
    char* StoreNumbers;

    SAPIStruct SProducts;
    strcpy(SProducts.URL, "https://api.cl.coop.dk/productapi/v1/product/1290");
    strcpy(SProducts.RequestType, "GET");
    strcpy(SProducts.CheckData, ""/*"RetailGroup: \"Kvickly\""*/);
    strcpy(SProducts.KeyTypeAndKey, "Ocp-Apim-Subscription-Key: fefba58d42c4456ca7182cc307574653");
    char* r = APICall(SProducts);

    return r;
    free(r);

}

void GetKardexNumbers(char* Stores, char* KardexNumbers)
{
    SAPIStruct SKardex;
    strcpy(SKardex.URL, "https://api.cl.coop.dk/storeapi/v1/stores?Content-Length=50");
    strcpy(SKardex.RequestType, "POST");
    strcpy(SKardex.CheckData, "RetailGroup: \"Kvickly\"");
    strcpy(SKardex.KeyTypeAndKey, "Ocp-Apim-Subscription-Key: fefba58d42c4456ca7182cc307574653");
    char* r = APICall(SKardex);
    printf("\n\n\n\n\n\n\n\n\n________________________________________\n\n\n%s", r);

    free(r);
    //printf("Hello, Coop!\n");
}

product* GetRemaProducts(char query[], int* nbHits) {
    SAPIStruct SProducts;

    char entireQuery[300] = "{\"requests\":[{\"indexName\":\"aws-prod-products\",\"params\":\"query=";
    char rest[200] = "&hitsPerPage=5000&"
                     "page=0&"
                     "&attributesToRetrieve=%5B%22name%22%2C%22labels%22%2C%22pricing%22%5D&attributesToHighlight=%5B%5D&attributesToSnippet=%5B%5D\"}]}";
    strcat(entireQuery, query);
    strcat(entireQuery, rest);
    strcpy(SProducts.URL,
           "https://flwdn2189e-dsn.algolia.net/1/indexes/*/queries?x-algolia-agent=Algolia%20for%20vanilla%20JavaScript%203.21.1&x-algolia-application-id=FLWDN2189E&x-algolia-api-key=fa20981a63df668e871a87a8fbd0caed");
    strcpy(SProducts.RequestType, "POST");
    strcpy(SProducts.PostFields, entireQuery);
    char *response = APICall(SProducts);


    FILE *remaproducts = fopen("remaproducts.txt", "w+");
    fputs(response, remaproducts);

    rewind(remaproducts);

    product *product_array = rema1000_scan(remaproducts, nbHits);
    fclose(remaproducts);
    // printf("%s", r);
    return product_array;
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
int storeCheck(char curretInput[])
{
    FILE* stores;
    int storeExists = 0;
    char searchString[20];
    strcpy(searchString, curretInput);

    stores = fopen("./stores.txt", "r");

    char buffer[20];

    while (fgets(buffer, 20, stores)) {
        char* checkForStore = strstr(buffer, searchString);
        if (checkForStore != NULL) {
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

char* APICall(SAPIStruct Params)
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

void WriteAPIDataToFile(char* Items)
{
    FILE* QFile;
    QFile = fopen("QueryResults.txt", "w+");

    //Create struct Dict with char* StoreName & char* Kardex

    //init struct for all stores
    //Loop through structs and get the store that matches the one we are reading from the file
    //do query
    //for all new lines -> do it again
    //done

    char* c = GetSallingProducts("m%C3%A6lk");
    fputs(c, QFile);
    fputs("????", QFile);
    c = GetCoopProducts("aifa", "aifa");
    fputs(c, QFile);

    fclose(QFile);
}

/*This initializes our dictionary (Gives it all of the entries with keys and values)*/
SDictionary InitDictionary() {
    //Create the dictionary
    SDictionary Dictionary;
    Dictionary.entry = NULL;
    /*Creates our entry for Dagli'Brugsen*/
    SDictEntry EntryDagliBrugs;
    strcpy(EntryDagliBrugs.Key, "Dagli'Brugsen");
    strcpy(EntryDagliBrugs.Value, "2082");

    /*Update the dictionary, first update the length, then update the entry*/
    Dictionary.DictLength = 1;
    Dictionary.DictMaxSize = 10;
    Dictionary.entry = realloc(Dictionary.entry, Dictionary.DictMaxSize + 1 * sizeof(EntryDagliBrugs));
    Dictionary.entry[0] = EntryDagliBrugs;

    /*Creates our entry for Fakta*/
    SDictEntry EntryFakta;
    strcpy(EntryFakta.Key, "Fakta");
    strcpy(EntryFakta.Value, "24080");

    /*Updates our dictonaries length, and adds the new entry to the dictionary*/
    Dictionary.DictLength = 2;
    Dictionary.DictMaxSize = 10;
    Dictionary.entry = realloc(Dictionary.entry, Dictionary.DictMaxSize + 1 * sizeof(EntryFakta));
    Dictionary.entry[1] = EntryFakta;

    return Dictionary;
}

/*This is a given, it looks up in our dictionary, this is done with a "Key"*/
char* DictionaryLookup(SDictionary Dictionary, char Key[]) {
    //For the size of our dictionary
    for (int i = 0; i < Dictionary.DictLength; i++)
    {
        //If equal, it returns 0, therefor we want !strcmp (Some might be used to it returning 1)
        /*This checks if our key, is equal to the set key, of the dictionary entry*/
        if (!strcmp(Dictionary.entry[i].Key, Key))
        {
            //If they are equal, return the value
            return Dictionary.entry[i].Value;
        }
    }
}

int main()
{
    SDictionary Dictionary = InitDictionary();
    WriteAPIDataToFile("Mel", Dictionary);

    GetSallingProducts("forarslog");
    FILE* aaaa = fopen("salling.txt", "r");
    product* sallingProducts = salling_scan(aaaa);
    fclose(aaaa);
    printf("%s", sallingProducts[0].name);

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
  }