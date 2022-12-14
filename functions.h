#ifndef BARGAIN_HUNTING_SCRAPER_H
#define BARGAIN_HUNTING_SCRAPER_H

typedef struct {
    char ean[13];
    char name[50];
    double price;
    char store[15];
} product;

/*Dictonary entry*/
typedef struct DictionaryEntry {
    char Key[20];
    char Value[20];
} SDictEntry;

/*Our dictonary*/
typedef struct Dictionary {
    int DictLength;
    int DictMaxSize;
    SDictEntry* entry;
} SDictionary;

typedef struct Params {
    char RequestType[7];
    char URL[250];
    char KeyTypeAndKey[100];
    char CheckData[300];
    char PostFields[300];
} SAPIStruct;


typedef struct string {
    char* ptr;
    size_t len;
} string;

typedef struct node {
    product data;
    struct node* next;
} node;

char* GetSallingProducts(char* Item);
void GetNonCoopProducts(char* Items, SDictionary Dictionary, node** LinkedListHead);
char* GetCoopProducts(char* Stores);
void WriteCoopDataToFile(char* Items, SDictionary Dictionary, int Runs);
void storeChoice();
void insertToList(node** head, product data);
int storeCheck(char currentInput[]);
char* APICall(SAPIStruct params);
char* DictionaryLookup(SDictionary Dictionary, char* Key);
SDictionary InitDictionary();
void init_string(struct string* s);

size_t WriteFunc(void* ptr, size_t size, size_t nmemb, struct string* s);
void check_input_for_salling(char* string);

int IsProductInList(node* LinkedList, product data);
void rema1000_scan(FILE* file, node** head);
void salling_scan(FILE* file, node** head);


void insertToList(node** head, product data);
int scan_input(char* name);
void check_output_char(char* string);

void DeleteAllListItems(node** LinkedListHead);
void ReadCoopData(char* Query, node** ProductList);

void final_print(struct node* head, int MaxItems);
void RelevantCoopData(FILE* QFile, char* Store, char* Query, node** LinkedList);



#endif //BARGAIN_HUNTING_SCRAPER_H
