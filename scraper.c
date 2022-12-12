#include <stdio.h>
#include "scraper.h"
#include <stdlib.h>
#include <string.h>
#include "include/curl/curl.h"
#include <locale.h>
#include <wchar.h>

enum danish_char {
    ae = -90, oe = -72, aa = -91, AE = -122, OE = -104, AA = -123
};

enum misc_char {
    ö = -74, Ö = -106, ô = -76, Ô = -108, é = -87, É = -119, ä = -92, Ä = -124, ü = -68, Ü = -100, TRADEMARK = -82, SPACE = 32
};

product* salling_scan(FILE* file, node** head) {
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
    while (fgetc(file) != '[') {
    }
    char c;
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
                check_output_char(desc);
            }
            if ((strcmp(ctgry, "price") == 0)) {
                fscanf(file, "%*c%4lf", &price);
                product newproduct;
                strcpy(newproduct.name, desc);
                strcpy(newproduct.store, "Bilka");
                newproduct.price = price;
                insertToList(head, newproduct);
            }
        }
    }
}

product* rema1000_scan(FILE* file, node** head) {
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
    while (fgetc(file) != '[') {
    }
    char c;
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
                check_output_char(desc);
            }
            if ((strcmp(ctgry, "price") == 0)) {
                fscanf(file, "%*c%lf", &price);
                product newproduct;
                strcpy(newproduct.name, desc);
                strcpy(newproduct.store, "Rema1000");
                newproduct.price = price;
                insertToList(head, newproduct);
            }
        }
    }
}


void scan_input(char* ProductName, int* maxItems)
{
    printf("Indtast produktets navn, saasom 'banan yoghurt'. Afslut programmet med 'end'>");
    scanf(" %[^\n]s", ProductName);

    //while ((getchar()) != '\n');
    while (getchar() != '\n');
    printf("\nHvor mange resultater vil du se for dit produkt?>");
    while (1)
    {
        char* digit;
        scanf("%s", digit);

        int validChecks = 0;

        for (int i = 0; i < strlen(digit); i++)
        {
            if (isdigit(digit[i]) == 1)
            {
                validChecks++;

                if (validChecks == strlen(digit))
                {
                    int x;
                    //sscanf convers our char to an int
                    sscanf(digit, "%d", &x);
                    printf("%d", x);
                    //Put the number into our maxItems
                    return x;
                }
                else {
                    printf("Dit input er ikke et tal. Prøv igen :).\n");
                }
            }
        }

        while (getchar() != '\n');

    }

}


/**
 *
 * @param string: string scanned from file
 *
 */

void check_output_char(char* string)
{
    int len = strlen(string);
    for (int i = 0; i < len; ++i) {
        char *strB = "Null";
        switch ((int)string[i]) {
            case ae:
                string[i - 1] = 'a';
                string[i] = 'e';
                break;
            case oe:
                string[i - 1] = 'o';
                string[i] = 'e';
                break;
            case aa:
                string[i - 1] = 'a';
                string[i] = 'a';
                break;
            case AE:
                string[i - 1] = 'A';
                string[i] = 'E';
                break;
            case OE:
                string[i - 1] = 'O';
                string[i] = 'E';
                break;
            case AA:
                string[i - 1] = 'A';
                string[i] = 'A';
                break;
            case ö:
                string[i - 1] = 'o';
                string[i] = 'e';
                break;
            case Ö:
                string[i - 1] = 'O';
                string[i] = 'e';
                break;
            case ô:
                strB = "o";
                break;
            case Ô:
                strB = "O";
                break;
            case é:
                strB = "e";
                break;
            case É:
                strB = "E";
                break;
            case ä:
                strB = "a";
                break;
            case Ä:
                strB = "A";
                break;
            case ü:
                strB = "u";
                break;
            case Ü:
                strB = "U";
                break;
            case TRADEMARK:
                strB = "";
                break;
        }
        if (strcmp(strB, "Null") != 0) {
            char *strA = string, strC[50];
            strncpy(strC, strA, i-1);
            strC[i] = '\0';
            strcat(strC, strB);
            strcat(strC, strA + i+1);
            strcpy(string, strC);
        }
    }
}

void insertToList(node** head, product data) {
    node* newnode = malloc(sizeof(node));
    newnode->data = data;
    newnode->next = *head;

    if (*head == NULL || (*head)->data.price >= newnode->data.price) {
        newnode->next = *head;
        *head = newnode;
    }
    else {
        node* current = *head;
        while (current->next != NULL && current->next->data.price < newnode->data.price) {
            current = current->next;
        }
        newnode->next = current->next;
        current->next = newnode;
    }

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

    return r;
}

char* GetCoopProducts(char* Stores)

{
    char* StoreNumbers;

    SAPIStruct SProducts;

    strcpy(SProducts.URL, "https://api.cl.coop.dk/productapi/v1/product/");
    strcat(SProducts.URL, Stores);
    strcpy(SProducts.RequestType, "GET");
    strcpy(SProducts.CheckData, ""/*"RetailGroup: \"Kvickly\""*/);
    strcpy(SProducts.KeyTypeAndKey, "Ocp-Apim-Subscription-Key: 87d818c340b34764beda02cdd0b5865f");
    char* r = APICall(SProducts);

    return r;

}

char* GetRemaProducts(char query[])
{
    SAPIStruct SProducts;

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
    //The last line is always empty, so this detects something it shouldn't, hence -1
    lines = lines - 1;
    fclose(match_file);
    return lines;
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
    }

    fclose(stores);
    return storesArray;


}



void storeChoice() {
    FILE* stores;
    char storeID[5];
    char storeName[25];

    printf("\nProgrammet kan vise dig varer for: Bilka, Fakta, Dagli'Brugsen, Rema1000");
    printf("\nGrundet problemer med Salling Group og Coop, kan programmet "
           "ikke vise varer for andre butikker i disse kaeder :( !\n\n");


    stores = fopen("./stores.txt", "w");

    //Loop for entering wanted stores
    while (1) {
        //Loop for avoiding blank inputs
        while (printf("Skriv et butiksnavn, afslut med 'q'>") && scanf("%[^\n]%*c", storeName) < 1) {
            printf("please give valid input\n");
            while (getchar() != '\n');
        }

        //Breaks loop
        if (strcasecmp(storeName, "q") == 0) {
            break;
        }

        //Check if entered store already is in list
        if (storeCheck(storeName) == 1) {
            printf("\nSame store is already in list\n");
        }

        //Prints given store to list
        else {
            fprintf(stores, "%s\n", storeName);
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

        if (!feof(stores)){
            printf("Store number %d: %s\n", i, storeName);
        }
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
        if (strcasecmp(ProductName, "q") == 0) {
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
        if (Params.KeyTypeAndKey != "") {
            headers = curl_slist_append(headers, Params.KeyTypeAndKey);
        }
        if (Params.CheckData != "") {
            headers = curl_slist_append(headers, Params.CheckData);
        }
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }

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

void GetNonCoopProducts(char* Items, SDictionary Dictionary, node** LinkedList) {

    int storeAmount;
    char** storesArray = getStoresArray(&storeAmount);

    FILE* QFile;
    QFile = fopen("QueryResults.txt", "w+");

    FILE* StoreFile;
    StoreFile = fopen("stores.txt", "r");

    Items[strcspn(Items, "\n")] = '\0';

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
            if (!strcmp(Key, "Rema") || !strcmp(Key, "Rema1000") && !isdigit(IsDigkey[0]))
            {
                freopen("QueryResults.txt", "w+", QFile);
                printf("%s Is Rema store\n", IsDigkey);
                char* c = GetRemaProducts(Items);
                fputs(c, QFile);
                rewind(QFile);
                rema1000_scan(QFile, LinkedList);
                free(c);
            }
            else if (!isdigit(IsDigkey[0])) {
                freopen("QueryResults.txt", "w+", QFile);
                // printf("%s Is a Salling store\n", IsDigkey);
                char* c = GetSallingProducts(Items);
                fputs(c, QFile);
                rewind(QFile);
                salling_scan(QFile, LinkedList);
                free(c);
            }
        }
    }
    fclose(QFile);
    fclose(StoreFile);
}


void DeleteAllListItems(node** LinkedListHead)
{
    node* current = *LinkedListHead;
    while (current != NULL) {
        node* next = current->next;
        free(current);
        current = next;
    }
    *LinkedListHead = NULL;
}

/*Calls the API's and writes the data to a file*/
void WriteCoopDataToFile(char* Items, SDictionary Dictionary, int Runs)
{
    FILE* QFile;
    QFile = fopen("CoopQueryResults.txt", "a");

    FILE* StoreFile;
    StoreFile = fopen("stores.txt", "r");

    char buffer[20];

    Items[strcspn(Items, "\n")] = '\0';

    while (fgets(buffer, 15, StoreFile))
    {
        //Fjerner alle \n fra vores buffer, dette gør det muligt at søge på alle linjer, ellers ville search slå fejl for alle udover den sidste
        buffer[strcspn(buffer, "\n")] = '\0';

        char IsDigkey[20];
        char* Key;

        Key = DictionaryLookup(Dictionary, buffer);

        if (Key == NULL)
        {
            printf("Store not found (Not supported by API)\n");
        } else{
            strcpy(IsDigkey, Key);
        }


        if (isdigit(IsDigkey[0]))
        {
            if (Runs == 0)
            {
                printf("%s (%s) Is a coop store\n", buffer, IsDigkey);
                char* c = GetCoopProducts(Key);
                fputs(c, QFile);
                fputs("??", QFile);
                free(c);

            }
        }
    }
    fclose(QFile);
    fclose(StoreFile);
}

/*This initializes our dictionary (Gives it all of the entries with keys and values)*/
SDictionary InitDictionary()
{
    //Create the dictionary
    SDictionary Dictionary;
    /*https://stackoverflow.com/a/62004489*/
    //If you pass NULL to realloc(),
    //then realloc will try to allocate a completely new block of RAM,
    //but if you pass something different than NULL it, realloc suposses that it already exists (in RAM)
    //and will therefor try to realloc an existing block of RAM (which it isn't) and the computer dies.
    Dictionary.entry = NULL;
    /*Creates our entry for Dagli'Brugsen*/

    SDictEntry EntryDagliBrugs;
    strcpy(EntryDagliBrugs.Key, "Dagli'Brugsen");
    strcpy(EntryDagliBrugs.Value, "2082");

    /*Update the dictionary, first update the length, then update the entry*/
    Dictionary.DictLength = 1;
    Dictionary.DictMaxSize = 10;

    Dictionary.entry = realloc(Dictionary.entry, Dictionary.DictLength * sizeof(SDictEntry));
    Dictionary.entry[0] = EntryDagliBrugs;

    /*Creates our entry for Fakta*/
    SDictEntry EntryFakta;
    strcpy(EntryFakta.Key, "Fakta");
    strcpy(EntryFakta.Value, "24080");

    /*Updates our dictonaries length, and adds the new entry to the dictionary*/
    Dictionary.DictLength = 2;
    Dictionary.DictMaxSize = 10;

    Dictionary.entry = realloc(Dictionary.entry, Dictionary.DictLength * sizeof(SDictEntry));
    Dictionary.entry[1] = EntryFakta;

    SDictEntry EntryBilka;
    strcpy(EntryBilka.Key, "Bilka");
    strcpy(EntryBilka.Value, "Bilka");

    Dictionary.DictLength = 3;
    Dictionary.DictMaxSize = 10;
    Dictionary.entry = realloc(Dictionary.entry, Dictionary.DictLength * sizeof(SDictEntry));
    Dictionary.entry[2] = EntryBilka;

    SDictEntry EntryRema;
    strcpy(EntryRema.Key, "Rema");
    strcpy(EntryRema.Value, "Rema");

    Dictionary.DictLength = 4;
    Dictionary.entry = realloc(Dictionary.entry, Dictionary.DictLength * sizeof(SDictEntry));
    Dictionary.entry[3] = EntryRema;

    SDictEntry EntryRema2;
    strcpy(EntryRema2.Key, "Rema1000");
    strcpy(EntryRema2.Value, "Rema");

    Dictionary.DictLength = 5;
    Dictionary.entry = realloc(Dictionary.entry, Dictionary.DictLength * sizeof(SDictEntry));
    Dictionary.entry[4] = EntryRema2;

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
        if (!strcasecmp(Dictionary.entry[i].Key, Key))
        {
            //If they are equal, return the value
            return Dictionary.entry[i].Value;
        }
        else {

        }
    }

    return NULL;
}

void final_print(struct node* head, int MaxItems)
{
    printf("\n");
    printf("____________________________________________________________________________________\n");
    printf("|                      Produkt                     |     Price     |     Store     |\n");
    printf("|                                                  |               |               |\n");
    node* current = head;

    for (int i = 0; i < MaxItems; i++)
    {
        if (current == NULL){
            break;
        }
        printf("|%49s |%14.2lf |%14s |\n", current->data.name, current->data.price, current->data.store);
        current = current->next;
    }
    /*
    while (current != NULL || i < MaxItems) {
        printf("|%49s |%12.2lf |%12s |\n", current->data.name, current->data.price, current->data.store);
        current = current->next;
        i++;
    }*/
    printf("____________________________________________________________________________________\n\n");
}

product* coop_scan(FILE* file, int* counter, char* Store) {

    FILE* XFile;
    XFile = fopen("CoopQueryResults.txt", "r");

    while (1) {
        char b = fgetc(XFile);
        if (feof(XFile)) {
            break;
        }
        if (b == '}') {
            *counter += 1;
        }

    }
    fclose(XFile);

    product* products = malloc(sizeof(product) * *counter);

    char c;
    int i = 0;
    while (1) {
        c = fgetc(file);
        if (feof(file)) {
            break;
        }

        if (c == '?') {
            char c2;
            c2 = fgetc(file);
            if (c2 == '?')
            {

                return products;
            }
        }
        if (c == '"') {
            char ctgry[100];
            char desc[100];
            char desc2[100];
            double price;
            fscanf(file, "%[^\"]%*c", ctgry);
            if (strcmp(ctgry, "Navn") == 0) {
                fscanf(file, "%*2s%[^\"]%*c", desc);
                check_output_char(desc);
                strcpy(products[i].name, desc);
                strcpy(products[i].store, Store);
            }
            //Add navn2 to navn
            if (strcmp(ctgry, "Navn2") == 0) {
                fscanf(file, "%*[\"]%*[\"]%s%[^\"]", desc2);
                if (strcmp(desc2, "\"") == 0) {
                }
                /*else {
                    fscanf(file, "%*2s%[^\"]%*c", desc);
                    strcat(products[i].name, desc2);
                }*/
            }
            if ((strcmp(ctgry, "Pris") == 0)) {
                fscanf(file, "%*c%lf", &price);
                products[i].price = price;
                i += 1;
            }
        }
    }
    return products;
}

void ReadCoopData(char* Query, node** ProductList)
{
    FILE* QFile;
    QFile = fopen("CoopQueryResults.txt", "r");

    int ArraySize = 0;

    FILE* SFile;
    SFile = fopen("stores.txt", "r");

    //product* products = malloc(sizeof(char));

    char buffer[20];

    for (int i = 0; Query[i] != '\0'; i++)
    {
        Query[i] = toupper(Query[i]);
    }

    while (fgets(buffer, 50, SFile)) {
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcasecmp(buffer, "Fakta") == 0)
        {
            RelevantCoopData(QFile, "Fakta", Query, ProductList);
        }
        if (strcasecmp(buffer, "Dagli'Brugsen") == 0)
        {
            RelevantCoopData(QFile, "Dagli'Brugsen", Query, ProductList);
        }
    }
}

void RelevantCoopData(FILE* QFile, char* Store, char* Query, node** LinkedList)
{
    Query[strcspn(Query, "\n")] = '\0';
    int ArrayIndex = 0;

    product* AllProducts = coop_scan(QFile, &ArrayIndex, Store);

    for (int i = 0; i < ArrayIndex; i++)
    {
        if (strstr(AllProducts[i].name, Query) != NULL)
        {
            insertToList(LinkedList, AllProducts[i]);
        }
    }

    free(AllProducts);
}

void check_input_for_salling(char *string) {
    int stringlen = strlen(string);
    for (int i = 0; i < stringlen; ++i) {
        char *strB = "Null";
        switch ((int) string[i]) {
            case ae:
                strB = "%C3%A6";
                break;
            case oe:
                strB = "%C3%B8";
                break;
            case aa:
                strB = "%C3%A5";
                break;
            case AE:
                strB = "%C3%86";
                break;
            case OE:
                strB = "%C3%98";
                break;
            case AA:
                strB = "%C3%85";
                break;
        }
        if ((int) string[i] == SPACE) {
            char *strA = string, strC[50];
            strncpy(strC, strA, i);
            strC[i] = '\0';
            strcat(strC, "%20");
            strcat(strC, strA + i+1);
            strcpy(string, strC);
        }

        if (strcmp(strB, "Null") != 0) {
            char *strA = string, strC[50];
            strncpy(strC, strA, i);
            strC[i-1] = '\0';
            strcat(strC, strB);
            strcat(strC, strA + i + 1);
            strcpy(string, strC);
        }
    }
}

void delay(int seconds) {
    // Get the current time
    time_t start = time(NULL);

    // Loop until "seconds" seconds have passed
    while (time(NULL) - start < seconds);
}

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

    int MaxItems = scan_input(Product);
    /*Laver et kald for hele shopping listen, hvis du kun vil have et kald, så brug de to funktioner over*/
    FILE* SLFile;
    SLFile = fopen("ShoppingList.txt", "r");
    int Runs = 0;

    node* LinkedList = NULL;

    //WriteCoopDataToFile(Product, Dictionary, Runs);
    ReadCoopData(Product, &LinkedList);
    check_input_for_salling(Product);
    GetNonCoopProducts(Product, Dictionary, &LinkedList);
    Runs++;
    final_print(LinkedList, MaxItems);
    DeleteAllListItems(&LinkedList);
    delay(1);


    free(Dictionary.entry);
    return 0;
}

/*
    while (fgets(buffer, 50, SLFile)) {
        //We use runs, to make sure we only do the coop call once for each store
        //WriteCoopDataToFile(buffer, Dictionary, Runs);
        GetNonCoopProducts(buffer, Dictionary, &LinkedList);
        ReadCoopData(buffer, &LinkedList);
        Runs++;
        final_print(LinkedList);
        DeleteAllListItems(&LinkedList);
        delay(1);
    }*/
