#ifndef HMAP_H
#define HMAP_H

#include <stdlib.h>
#include <string.h>

extern char* strdup(const char*);

typedef struct DictionaryEntry {
    char* key;
    void* value;
    struct DictionaryEntry* next;
} DictionaryEntry;

typedef struct {
    DictionaryEntry** entries;
    size_t size;
    size_t capacity;
} Dictionary;

/* Initialize a dictionary with a given capacity */
Dictionary* createDictionary(size_t capacity);

/* Hash a string to an integer value */
unsigned long hash(char* str);

/* Insert a key-value pair into the dictionary */
void dictionarySet(Dictionary* dictionary, char* key, void* value);

/* Look up a value by its key in the dictionary */
void* dictionaryGet(Dictionary* dictionary, char* key);

/* Reset a dictionary to its initial state */
void clearDictionary(Dictionary* dictionary);

/*Free the memory used by the dictionary*/
void freeDictionary(Dictionary* dictionary);

#endif /* HMAP_H */
