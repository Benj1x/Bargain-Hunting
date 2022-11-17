//
// Created by Benjamin Høj on 31-10-2022.
//

#ifndef BARGAIN_HUNTING_SCRAPER_H
#define BARGAIN_HUNTING_SCRAPER_H

typedef struct {
    char name[20];
    char description[50];
    double price;
    char store[];
} product;


product COOP_API_return(FILE* file);

void scan_input(char* name,double* max_price);




#endif //BARGAIN_HUNTING_SCRAPER_H
