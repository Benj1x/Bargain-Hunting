#include <stdio.h>
#include "scraper.h"
#include <stdlib.h>
#include <string.h>
#include "include/curl/curl.h"

enum {
    ae = -90, oe = -72, aa = -91, AE = -122, OE = -104, AA = -123
};

product* salling_scan(FILE* file, int* nbHits, product* productArray) {
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
    productArray = realloc(productArray, sizeof(product) * (*nbHits + counter));
    while (fgetc(file) != '[') {
    }
    char c;
    int i = *nbHits;
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
                strcpy(productArray[i].name, desc);
                strcpy(productArray[i].store, "Bilka");
            }
            if ((strcmp(ctgry, "price") == 0)) {
                fscanf(file, "%*c%4lf", &price);
                productArray[i].price = price;
                i += 1;
            }
        }
    }
    *nbHits += counter;
    return productArray;
}

product* rema1000_scan(FILE* file, int* nbHits, product* productArray) {
    int counter = -4;
    while (1) {
        char b = fgetc(file);
        if (feof(file)) {
            break;
        }
        if (b == '}') {
            counter += 1;
        }

    }
    counter /= 2;

    rewind(file);
    productArray = realloc(productArray, sizeof(product) * (*nbHits + counter));
    while (fgetc(file) != '[') {
    }
    char c;
    int i = *nbHits;
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
                strcpy(productArray[i].name, desc);
                strcpy(productArray[i].store, "Rema1000");
            }
            if ((strcmp(ctgry, "price") == 0)) {
                fscanf(file, "%*c%lf", &price);
                productArray[i].price = price;
                i += 1;
            }
        }
    }
    *nbHits += counter - 1;
    return productArray;
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
    if (priceA < priceB)
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
}

char* GetCoopProducts(char* Stores)
{
    char* StoreNumbers;

    SAPIStruct SProducts;
    //1290 gamle kardex
    strcpy(SProducts.URL, "https://api.cl.coop.dk/productapi/v1/product/");
    strcat(SProducts.URL, Stores);
    strcpy(SProducts.RequestType, "GET");
    strcpy(SProducts.CheckData, ""/*"RetailGroup: \"Kvickly\""*/);
    strcpy(SProducts.KeyTypeAndKey, "Ocp-Apim-Subscription-Key: fefba58d42c4456ca7182cc307574653");
    char* r = APICall(SProducts);

    return r;

}

product* GetRemaProducts(char query[])
{
    SAPIStruct SProducts;

    char entireQuery[300] = "{\"requests\":[{\"indexName\":\"aws-prod-products\",\"params\":\"query=";
    char rest[200] = "&hitsPerPage=5000&"
        "page=0&"
        "&attributesToRetrieve=%5B%22name%22%2C%22labelsMain%22%2C%22pricing%22%5D&attributesToHighlight=%5B%5D&attributesToSnippet=%5B%5D\"}]}";
    strcat(entireQuery, query);
    strcat(entireQuery, rest);
    strcpy(SProducts.URL, "https://flwdn2189e-dsn.algolia.net/1/indexes/*/queries?x-algolia-agent=Algolia%20for%20vanilla%20JavaScript%203.21.1&x-algolia-application-id=FLWDN2189E&x-algolia-api-key=fa20981a63df668e871a87a8fbd0caed");
    strcpy(SProducts.RequestType, "POST");
    strcpy(SProducts.PostFields, entireQuery);
    char* response = APICall(SProducts);
    return response;

}

void loaded_check(FILE* file) {
    if (file == NULL) {
        printf("Failed on loading file!");
        exit(EXIT_FAILURE);
    }
}

int count_lines(char* file_name) {

    FILE* match_file = fopen(file_name, "r");
    loaded_check(match_file);
    int lines = 1;
    int ch = 0;
    while ((ch = fgetc(match_file)) != EOF) {
        if (ch == '\n')
            lines++;
    }
    fclose(match_file);
    return lines;
}

int isStringInArray(char** selectedShops, char* shopName, int shopsNumber) {

    for (int i = 0; i < shopsNumber; i++)
    {
        if (strcmp(shopName, selectedShops[i]) == 0)
            return 1;
    }
    return 0;
}

char** getStoresArray(int* storeAmount) {
    FILE* stores = fopen("./stores.txt", "r");
    loaded_check(stores);

    *storeAmount = count_lines("./stores.txt");
    rewind(stores);
    char** storesArray = malloc(sizeof(char*) * (*storeAmount));


    for (int i = 0; i < *storeAmount; i++)
    {
        storesArray[i] = malloc(20 + 1);
        char store[20];
        fscanf(stores, "%s", store);
        strcpy(storesArray[i], store);
        // printf("%s \n", storesArray[i]);
    }

    fclose(stores);
    return storesArray;


}

///Was this deleted ealier?

product* getProductsFromStoreList(char query[]) {

    int storeAmount;
    char** storesArray = getStoresArray(&storeAmount);
    int nbHits;

    // product* rema = GetRemaProducts(query, &nbHitsRema);
    // product* salling = GetRemaProducts(query);
    int current_size = 0;
    product* fullProductArray = malloc(sizeof(product));


    if (isStringInArray(storesArray, "Rema", storeAmount)) {
        int nbHitsRema = 5;
        nbHits += nbHitsRema;
        product rema[5] = { [0] .name = "Eggs",[0].price = 9.0,
        [0 ... 4].store = "Rema",
        [1].name = "Dickus",[1].price = 32.5,
        [2].name = "Balls",[2].price = 51.4,
        [3].name = "Gordon Blue",[3].price = 99.5,
        [4].name = "skrrt",[4].price = 4444.1,
        };
        current_size += nbHitsRema;
        fullProductArray = realloc(fullProductArray, current_size * sizeof(product));
    }

}

//was this deleted earlier?
// printf("elements rema: %d \n", nbHitsRema);
// printf("aa: %d", rema[2].price);


// for (int i = 0; i < storeAmount; i++)
// {
//     char* current[10];
//     fscanf(stores, "%s", current);
//     if (strcmp(current, "Fotex") == 1) {

//     }

//     return

// }

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

void ItemsChoice()
{
    FILE* SLFile;
    char ProductName[50];

    SLFile = fopen("./ShoppingList.txt", "a+");

    //Loop for entering wanted stores
    while (1) {
        printf("Enter product you want to buy, end with ! 'q':");
        scanf("%[^\n]", &ProductName);
        while (getchar() != '\n');

        //Breaks loop
        if (strcmp(ProductName, "q") == 0) {
            break;
        }

        //Check if entered store already is in list
        if (DoesProductExist(ProductName) == 1) {
            printf("\nThe product is already in the list\n");
        }

            //Prints given store to list
        else {
            fprintf(SLFile, "\n%s", ProductName);
            printf("The product has been added to list\n\n");
        }
    }
    fclose(SLFile);

    //Prints stores present in list file
    SLFile = fopen("./ShoppingList.txt", "r");
    int i = 0;

    while (!feof(SLFile)) {
        ++i;
        fgets(ProductName, 50, SLFile);
        printf("Store number %d: %s\n", i, ProductName);
    }
    fclose(SLFile);
}

int DoesProductExist(char curretInput[])
{
    FILE* SLFile;
    int storeExists = 0;
    char searchString[20];
    strcpy(searchString, curretInput);

    SLFile = fopen("./ShoppingList.txt", "r");

    char buffer[20];

    while (fgets(buffer, 20, SLFile)) {
        char* checkForStore = strstr(buffer, searchString);
        if (checkForStore != NULL) {
            storeExists = 1;
            break;
        }
        else {
            storeExists = 0;
        }
    }
    fclose(SLFile);

    //Returns 1 if store is found in list
    //Returns 0 if store is not found in list
    return storeExists;
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
/*________________________________________________________________________________*/
//All code between the these^^ (Line 487-515) is from stackoverflow - this seems like the only way to really do this
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
/*________________________________________________________________________________*/

/*
product* getProductsFromStoreList(char* Items, SDictionary Dictionary, int* length) {

    int storeAmount;
    char** storesArray = getStoresArray(&storeAmount);
    int nbHits = 0;

    // product* rema = GetRemaProducts(query, &nbHitsRema);
    // product* salling = GetRemaProducts(query);
    product* productArray = malloc(sizeof(product) * 999);

    FILE* QFile;
    QFile = fopen("QueryResults.txt", "w+");

    FILE* StoreFile;
    StoreFile = fopen("stores.txt", "r");

    for (int i = 0; i < storeAmount; i++)
    {
        char* Key;
        char IsDigkey[20];

        Key = DictionaryLookup(Dictionary, storesArray[i]);
        if (Key == NULL)
        {
            printf("Store not found (Not supported by API)\n");
        }
        else {
            strcpy(IsDigkey, Key);
            if (isdigit(IsDigkey[0]))
            {
                freopen("QueryResults.txt", "w+", QFile);
                printf("%s (%s) Is a coop store\n", storesArray[i], IsDigkey);
                freopen("QueryResults.txt", "w+", QFile);
                char* c = GetCoopProducts(Key);
                fputs(c, QFile);
            }
            else if (!strcmp(Key, "Rema"))
            {
                freopen("QueryResults.txt", "w+", QFile);
                printf("%s Is Rema store\n", IsDigkey);
                char* c = GetRemaProducts(Items);
                fputs(c, QFile);
                rewind(QFile);
                rema1000_scan(QFile, &nbHits, productArray);

            }
            else {
                freopen("QueryResults.txt", "w+", QFile);
                printf("%s Is a Salling store\n", IsDigkey);
                char* c = GetSallingProducts(Items);
                fputs(c, QFile);
                rewind(QFile);
                salling_scan(QFile, &nbHits, productArray);
            }
        }
    }
    fclose(QFile);
    fclose(StoreFile);
    *length = nbHits;
    qsort(productArray, *length, sizeof(product), cmpfunc);
    return productArray;
}
*/
/*Calls the API's and writes the data to a file*/
void WriteAPIDataToFile(char* Items, SDictionary Dictionary, int Runs)
{
    FILE* QFile;
    QFile = fopen("QueryResults.txt", "w+");

    FILE* StoreFile;
    StoreFile = fopen("stores.txt", "r");

    char buffer[20];

    while (fgets(buffer, 15, StoreFile))
    {
        //Fjerner alle \n fra vores buffer, dette gør det muligt at søge på alle linjer, ellers ville search slå fejl for alle udover den sidste
        buffer[strcspn(buffer, "\n")] = '\0';

        char IsDigkey[20];
        char* Key;
        //char* Test = GetSallingProducts(Items);
        Key = DictionaryLookup(Dictionary, buffer);
        char* Rema = "Rema";
        if (Key == NULL)
        {
            printf("Store not found (Not supported by API)\n");
        }
        else {
            strcpy(IsDigkey, Key);
            if (isdigit(IsDigkey[0]))
            {
                if(Runs == 0){
                    printf("%s (%s) Is a coop store\n", buffer, IsDigkey);
                    char* c = GetCoopProducts(Key);
                    fputs(c, QFile);
                    fputs("????", QFile);
                }
            }
            else if(!strcmp(Key, Rema))
            {
                printf("%s Is Rema store\n", IsDigkey);

                //fputs(c, QFile);
                //fputs("????", QFile);
            }
            else {
                printf("%s Is a Salling store\n", IsDigkey);
                //char* c = GetSallingProducts(Items);
                //fputs(c, QFile);
                //fputs("????", QFile);
            }
        }
    }
    fclose(QFile);
    fclose(StoreFile);
    //Create struct Dict with char* StoreName & char* Kardex

    //init struct for all stores

    //do query
    //for all new lines -> do it again
    //done
}

/*This initializes our dictionary (Gives it all of the entries with keys and values)*/
SDictionary InitDictionary()
{
    //Create the dictionary
    SDictionary Dictionary;
    /*https://stackoverflow.com/a/62004489*/
    //If you pass NULL to realloc(),
    //then realloc tries to allocate a completely new block,
    //but if you pass something different than NULL it
    //consider it represents an already allocated block (which it isn't) and the computer explodes.
    Dictionary.entry = NULL;
    /*Creates our entry for Dagli'Brugsen*/
    SDictEntry EntryError;
    strcpy(EntryError.Key, "Not Found");
    strcpy(EntryError.Value, "000000");

    Dictionary.DictLength = 1;
    Dictionary.DictMaxSize = 10;
    Dictionary.entry = realloc(Dictionary.entry, Dictionary.DictLength * sizeof(EntryError) + 1);
    Dictionary.entry[0] = EntryError;

    SDictEntry EntryDagliBrugs;
    strcpy(EntryDagliBrugs.Key, "Dagli'Brugsen");
    strcpy(EntryDagliBrugs.Value, "2082");

    /*Update the dictionary, first update the length, then update the entry*/
    Dictionary.DictLength = 2;
    Dictionary.DictMaxSize = 10;

    Dictionary.entry = realloc(Dictionary.entry, Dictionary.DictLength * sizeof(EntryDagliBrugs) + 1);
    Dictionary.entry[1] = EntryDagliBrugs;

    /*Creates our entry for Fakta*/
    SDictEntry EntryFakta;
    strcpy(EntryFakta.Key, "Fakta");
    strcpy(EntryFakta.Value, "24080");

    /*Updates our dictonaries length, and adds the new entry to the dictionary*/
    Dictionary.DictLength = 3;
    Dictionary.DictMaxSize = 10;

    Dictionary.entry = realloc(Dictionary.entry, Dictionary.DictLength * sizeof(SDictEntry));
    Dictionary.entry[2] = EntryFakta;

    SDictEntry EntryBilka;
    strcpy(EntryBilka.Key, "Bilka");
    strcpy(EntryBilka.Value, "Bilka");

    Dictionary.DictLength = 4;
    Dictionary.DictMaxSize = 10;
    Dictionary.entry = realloc(Dictionary.entry, Dictionary.DictLength * sizeof(SDictEntry));
    Dictionary.entry[3] = EntryBilka;

    SDictEntry EntryRema;
    strcpy(EntryRema.Key, "Rema");
    strcpy(EntryRema.Value, "Rema");

    Dictionary.DictLength = 5;
    Dictionary.entry = realloc(Dictionary.entry, Dictionary.DictLength * sizeof(SDictEntry));
    Dictionary.entry[4] = EntryRema;

    return Dictionary;
}

/*This is a given, it looks up in our dictionary, this is done with a "Key"*/
char* DictionaryLookup(SDictionary Dictionary, char* Key)
{
    //For the size of our dictionary
    for (int i = 1; i < Dictionary.DictLength; i++)
    {
        //If equal, it returns 0, therefor we want !strcmp (Some might be used to it returning 1)
        /*This checks if our key, is equal to the set key, of the dictionary entry*/
        if (!strcmp(Dictionary.entry[i].Key, Key))
        {
            //If they are equal, return the value
            return Dictionary.entry[i].Value;
        }
        else {

        }
    }

    return NULL;
}

product* coop_scan(FILE* file, int* counter) {
    while (1) {
        char b = fgetc(file);
        if (feof(file)) {
            break;
        }
        if (b == '}') {
            *counter += 1;
        }

    }
    rewind(file);
    product* array = malloc(sizeof(product)* *counter);
    while (fgetc(file) != '[') {
    }
    char c;
    int i = 0;
    while(1) {
        c = fgetc(file);
        if (feof(file)) {
            break;
        }
        if (c == '"') {
            char ctgry[100];
            char desc[100];
            char desc2[100];
            double price;
            fscanf(file, "%[^\"]%*c", ctgry);
            if (strcmp(ctgry, "Navn") == 0) {
                fscanf(file, "%*2s%[^\"]%*c",desc);
                check_DK_char(desc);
                strcpy(array[i].name, desc);
                strcpy(array[i].store, "Coop");
            }
            if (strcmp(ctgry, "Navn2") == 0) {
                fscanf(file, "%*[\"]%*[\"]%s%[^\"]",desc2);
                if (strcmp(desc2, "\"") == 0) {
                }
            }
            if ((strcmp(ctgry, "Pris") == 0)) {
                fscanf(file, "%*c%lf", &price);
                array[i].price = price;
                i += 1;
            }
        }
    }
    return array;
}

void ReadDataFromFile()
{
    FILE* QFile;
    QFile = fopen("QueryResults.txt", "r");

    int ArraySize = 0;

    product* products = coop_scan(QFile, &ArraySize);
     /*for(int i = 0; i < ArraySize; i++){
        printf("%s, %.2lf kr\n", products[i].name, products[i].price);
    }*/

    product* remainingProd = malloc(sizeof(char));
    char query[50] = "APPELSIN";
    int prodIndex = 0;
    for (int i = 0; i < ArraySize; i++){
        char *res = strstr(products[i].name, query);
        if (res != NULL) {
            prodIndex++;
             //Dictionary.entry, Dictionary.DictLength * sizeof(SDictEntry)
            remainingProd = realloc(remainingProd, prodIndex * sizeof(product));
            strcpy(remainingProd[prodIndex-1].name, products[i].name);
            remainingProd[prodIndex-1].price = products[i].price;
        }
    }

    printf("\n\n\n");

    for(int i = 0; i < prodIndex; i++)
    {
        printf("%s, %.2lf kr\n", remainingProd[i].name, remainingProd[i].price);
    }

    //final_print(products, 3);
}

void final_print(product* array, int array_len) {
    printf("|                      Produkt                     |    Price    |    Store    |\n");
    printf("|                                                  |             |             |\n");
    for (int i = 0; i < array_len; ++i) {
        printf("|%50s|%13.2lf|%13s|\n", array[i].name, array[i].price, array[i].store);
    }
}

int main()
{

    SDictionary Dictionary = InitDictionary();
    //

    ReadDataFromFile();

    // product rema[5] = { [0] .name = "Eggs",[0].price = 9.0,
    //     [0 ... 4].store = "Rema",
    //     [1].name = "Dickus",[1].price = 32.5,
    //     [2].name = "Balls",[2].price = 51.4,
    //     [3].name = "Gordon Blue",[3].price = 99.5,
    //     [4].name = "skrrt",[4].price = 4444.1,
    // };
    //int length;
    //product* fullArray = getProductsFromStoreList("Toast", Dictionary, &length);
    //final_print(fullArray, length);
    //printf("%s", GetSallingProducts("for%C3%A5rsl%C3%B8g"));
    /*
    // getProductsFromStoreList("ost");
    int nbhits;
    // product* products = GetRemaProducts("ost", &nbhits);
    GetSallingProducts("ost");
    FILE* aaaa = fopen("salling.txt", "r");
    // printf("%s", products[49].name);
    product* sallingus = salling_scan(aaaa);
    fclose(aaaa);

    printf("%d", (int)"æ");

    //GetData('x');
    //FILE *test = fopen("test.txt", "r");

    /*product *array = salling_scan(test);
    for (int i = 0; i < 3; ++i) {
        printf("%s %lf i %s\n", array[i].name, array[i].price, array[i].store);
    }*/
    //char query[5] = "toast";
    //product* productArray = GetRemaProducts(query);

    // GetRemaProducts(query);

    // char name[30];
    // double max_price;
    // scan_input(name, &max_price);
    // FILE* SFile = fopen("ShoppingList.txt", "w");
    // fputs(name, SFile);
    // putc(max_price, SFile);

    // fclose(SFile);
    // FILE* test = fopen("test.txt", "r");
    // /*product *array = salling_scan(test);
    // for (int i = 0; i < 3; ++i) {
    //     printf("%s %lf i %s\n", array[i].name, array[i].price, array[i].store);
    // }*/
    // //char query[5] = "toast";
    // //product* productArray = GetRemaProducts(query);

    //fclose(test);
   // free(Dictionary.entry);
    return 0;
}
/*Pricerunner scraper, currently not in use, but also does not scrape anything they don't want us to*/
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

/* CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    struct string s;
    if(curl) {
        init_string(&s);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");
        curl_easy_setopt(curl, CURLOPT_CAPATH, "cacert.pem");
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.sallinggroup.com/v1/food-waste/?zip=8000");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        /*Set's a callback function to receive incoming data myfunc);*/
        /*
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        /*-----------------------------------------------------------*/
        /*Callback will take an argument that is set (This is our string)*/
        /*curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        /*-----------------------------------------------------------*/
        /*struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Authorization: Bearer dc6422b7-166d-41e8-94c1-6804da7e17d5");
        headers = curl_slist_append(headers, "Cookie: TiPMix=92.20528390749058; x-ms-routing-name=self");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);

        /* Check for errors */
        /*if (res != CURLE_OK)
        {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));
        }
        }
        curl_easy_cleanup(curl);

        printf("%s", s.ptr);*/
