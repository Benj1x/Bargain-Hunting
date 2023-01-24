#ifndef BARGAIN_HUNTING_SCRAPER_H
#define BARGAIN_HUNTING_SCRAPER_H

typedef struct {
    char ean[13];
    char name[50];
    char desc[30];
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
void GetNonCoopProducts(char* Item, SDictionary Dictionary, node** LinkedListHead);
char* GetCoopProducts(char* Store);
void WriteCoopDataToFile(char* Item, SDictionary Dictionary, int Runs);
void StoreChoice();
void InsertToList(node** head, product data);
int StoreCheck(char CurrentInput[]);
char* APICall(SAPIStruct params);
char* DictionaryLookup(SDictionary Dictionary, char* Key);
SDictionary InitDictionary();
void InitString(struct string* s);

size_t WriteFunc(void* ptr, size_t size, size_t nmemb, struct string* s);
void CheckInputForSalling(char* string);

int IsProductInList(node* LinkedList, product data);
void Rema1000Scan(FILE* file, node** head);
void SallingScan(FILE* file, node** head);


void InsertToList(node** head, product data);
int ScanInput(char* name);
void CheckOutputChar(char* string);

void DeleteAllListItems(node** LinkedListHead);
void ReadCoopData(char* Query, node** ProductList);

void FinalPrint(struct node* head, int MaxItems);
void RelevantCoopData(FILE* QFile, char* Store, char* Query, node** LinkedList);



#endif //BARGAIN_HUNTING_SCRAPER_H
