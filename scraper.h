typedef struct {
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

typedef enum Store { Fakta = 24080, DagliBrugsen = 2082 } EStore;
const char* KardexValue[] = { [Fakta] = "24080",[DagliBrugsen] = "2082" };

char* GetSallingProducts(char* Item);

char* GetCoopProducts(char* Stores);
void WriteAPIDataToFile(char* Items, SDictionary Dictionary);
void storeChoice();
int storeCheck(char currentInput[]);
char* APICall(SAPIStruct params);
char* DictionaryLookup(SDictionary Dictionary, char* Key);
SDictionary InitDictionary();
void init_string(struct string* s);
size_t writefunc(void* ptr, size_t size, size_t nmemb, struct string* s);
product* salling_scan(FILE* file, int* nbHits, product* productArray);
void scan_input(char* name, double* max_price);
void check_DK_char(char* string);
void correct_DK_char(char* string, int i, int str_len, int type);
void ReadDataFromFile();
void final_print(product* array, int array_len);
product* rema1000_scan(FILE* file, int* nbHits, product* productArray);

#ifndef BARGAIN_HUNTING_SCRAPER_H
#define BARGAIN_HUNTING_SCRAPER_H

#endif //BARGAIN_HUNTING_SCRAPER_H
