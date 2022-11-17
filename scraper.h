//
// Created by Benjamin Høj on 31-10-2022.
//
// , ,

typedef struct Params {
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

void GetSallingProducts(char* Item);
void GetCoopProducts(char* Item, char* Stores);
void GetKardexNumbers(char* Stores, char* KardexNumbers);
char* APICall(APIStruct params);
void init_string(struct string* s);
size_t writefunc(void* ptr, size_t size, size_t nmemb, struct string* s);

#ifndef BARGAIN_HUNTING_SCRAPER_H
#define BARGAIN_HUNTING_SCRAPER_H

#endif //BARGAIN_HUNTING_SCRAPER_H
