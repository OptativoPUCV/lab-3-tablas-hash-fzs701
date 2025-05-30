#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

HashMap * createMap(long capacity) {
    HashMap *newMap = (HashMap*)malloc(sizeof(HashMap));
    newMap->buckets = (Pair **)malloc(sizeof(Pair *) * capacity);
    for(int i = 0; i < capacity; i++){
        newMap->buckets[i] = NULL;
    }
    newMap->size = 0;
    newMap->capacity = capacity;
    newMap->current = -1;
    return newMap;
}

void insertMap(HashMap * map, char * key, void * value) {
    long posi = hash(key,map->capacity);
    long comprobar = posi;

    while(map->buckets[posi] != NULL && map->buckets[posi]->key != NULL){
        if(is_equal(map->buckets[posi]->key,key)){
            return;
        }
        posi = (posi + 1) % map->capacity;

        if(posi == comprobar){
            return;
        }

    }

    Pair * newPair = createPair(key,value);
    map->buckets[posi] = newPair;
    map->current = posi;
    map->size++;
}

Pair * searchMap(HashMap * map,  char * key) {   
    long posi = hash(key,map->capacity);
    long comprobar = posi;

    while(map->buckets[posi] != NULL){
        if(map->buckets[posi]->key != NULL && is_equal(map->buckets[posi]->key,key)){
            map->current = posi;
            return map->buckets[posi];
        }

        posi = (posi + 1) % map->capacity;
        if(posi == comprobar){
            break;
        }
    }
    return NULL;
}

void eraseMap(HashMap * map,  char * key) {    
    long posi = hash(key,map->capacity);
    long comprobar = posi;

    while(map->buckets[posi] != NULL){
        if(map->buckets[posi]->key != NULL && is_equal(map->buckets[posi]->key,key)){
            map->buckets[posi]->key = NULL;
            map->size--;
            return;
        }

        posi = (posi + 1) % map->capacity;
        if(posi == comprobar){
            break;
        }
    }
}

Pair * firstMap(HashMap * map) {
    map->current = 0;
    while((map->current < map->capacity) &&
         (map->buckets[map->current] == NULL || map->buckets[map->current]->key == NULL)){
            map->current++;
    }
    
    if(map->current < map->capacity){
        return map->buckets[map->current];
    } else {
        return NULL;
    }
    
}

Pair * nextMap(HashMap * map) {
    long primer = map->current + 1;

    while(primer < map->capacity){
        if(map->buckets[primer] != NULL && map->buckets[primer]->key != NULL){
            map->current = primer;
            return map->buckets[primer]; 
        }
        primer++;
    }
    
    return NULL;
}



void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)

    Pair ** antiguo = map->buckets;
    long capacidadAnti = map->capacity;
    map->capacity *= 2;

    map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));

    map->size = 0;
    map->current = -1;

    for(int i = 0; i < capacidadAnti; i++){
        if(antiguo[i] != NULL && antiguo[i]->key != NULL){
            insertMap(map, antiguo[i]->key, antiguo[i]->value);
        }
    }
    free(antiguo);

}









/*HashMap * createMap(long capacity) {

    return NULL;
}*/

/*void eraseMap(HashMap * map,  char * key) {    


}*/

/*Pair * searchMap(HashMap * map,  char * key) {   


    return NULL;
}*/

/*Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}*/
