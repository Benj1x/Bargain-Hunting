//
// Created by Benjamin Høj on 31-10-2022.
//
// , ,

typedef struct Params{
    char RequestType[7];
    char URL[250];
    char KeyTypeAndKey[100];
    char ContentLength[50];
    char CheckData[300];
} APIStruct;

typedef struct string {
    char *ptr;
    size_t len;
} string;

char* GetSallingProducts(char* Item);
char* GetCoopProducts(char* Item, char* Stores);
void GetData(char* Items);
char* APICall(APIStruct params);
void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);

#ifndef BARGAIN_HUNTING_SCRAPER_H
#define BARGAIN_HUNTING_SCRAPER_H

#endif //BARGAIN_HUNTING_SCRAPER_H
