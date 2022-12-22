#include "hmap.h"
#include "stdprof.h"

/* Initialize a dictionary with a given capacity */
Dictionary* dictionary_create(size_t capacity) {
    Dictionary* dictionary = malloc_prof(sizeof(Dictionary));
    dictionary->entries = calloc_prof(capacity, sizeof(DictionaryEntry*));
    dictionary->size = 0;
    dictionary->capacity = capacity;
    return dictionary;
}

/* Hash a string to an integer value */
unsigned long hash(char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

/* Insert a key-value pair into the dictionary */
void dictionary_insert(Dictionary* dictionary, char* key, void* value) {
    size_t index;
    DictionaryEntry* entry;

    /* Create a new dictionary entry */
    entry = malloc_prof(sizeof(DictionaryEntry));
    entry->key = strdup(key);
    entry->value = value;
    /* Hash the key to determine the index in the entries array */
    index = hash(key) % dictionary->capacity;
    /* Insert the entry at the beginning of the linked list at the index */
    entry->next = dictionary->entries[index];
    dictionary->entries[index] = entry;
    dictionary->size++;
}

/* Look up a value by its key in the dictionary */
void* dictionary_lookup(Dictionary* dictionary, char* key) {
    DictionaryEntry* entry;
    size_t index;

    /* Hash the key to determine the index in the entries array*/
    index = hash(key) % dictionary->capacity;
    /* Search the linked list at the index for the key*/
    entry = dictionary->entries[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    /* Key not found*/
    return NULL;
}

/*Free the memory used by the dictionary*/
void dictionary_free(Dictionary* dictionary) {
    size_t i;
    for (i = 0; i < dictionary->capacity; i++) {
        DictionaryEntry* entry = dictionary->entries[i];
        while (entry != NULL) {
            DictionaryEntry* next = entry->next;
            free_prof(entry->key);
            free_prof(entry);
            entry = next;
        }
    }
    free_prof(dictionary->entries);
    free_prof(dictionary);
}
