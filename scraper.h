typedef struct Params {
typedef struct {
    char name[50];
    double price;
    char store[20];
} product;

typedef struct Params{
    char RequestType[7];
    char URL[250];
    char KeyTypeAndKey[100];
    char CheckData[300];
    char PostFields[300];
} APIStruct;

typedef struct string {
    char* ptr;
    size_t len;
} string;

char* GetSallingProducts(char* Item);
char* GetCoopProducts(char* Item, char* Stores);
void GetData(char* Items);
char* APICall(APIStruct params);
void init_string(struct string* s);
size_t writefunc(void* ptr, size_t size, size_t nmemb, struct string* s);

product* salling_scan(FILE* file);
void scan_input(char* name,double* max_price);
void check_DK_char (char* string);
void correct_DK_char (char* string, int i, int str_len, int type);
product* rema100_scan(FILE* file);

#ifndef BARGAIN_HUNTING_SCRAPER_H
#define BARGAIN_HUNTING_SCRAPER_H

#endif //BARGAIN_HUNTING_SCRAPER_H
