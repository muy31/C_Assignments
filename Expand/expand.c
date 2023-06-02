#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct dict* Dict;

/* create a new empty dictionary */
Dict DictCreate(void);

/* destroy a dictionary */
void DictDestroy(Dict);

/* insert a new key-value pair into an existing dictionary */
void DictInsert(Dict, const char* key, const char* value);

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
const char* DictSearch(Dict, const char* key);

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void DictDelete(Dict, const char* key);

struct elt {
    struct elt* next;
    char* key;
    char* value;
};

struct dict {
    int size;           /* size of the pointer table */
    int n;              /* number of elements stored */
    struct elt** table;
};

#define INITIAL_SIZE (1024)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)

/* dictionary initialization code used in both DictCreate and grow */
Dict
internalDictCreate(int size)
{
    Dict d;
    int i;

    d = malloc(sizeof(*d));

    assert(d != 0);

    d->size = size;
    d->n = 0;
    d->table = malloc(sizeof(struct elt*) * d->size);

    assert(d->table != 0);

    for (i = 0; i < d->size; i++) d->table[i] = 0;

    return d;
}

Dict
DictCreate(void)
{
    return internalDictCreate(INITIAL_SIZE);
}

void
DictDestroy(Dict d)
{
    int i;
    struct elt* e;
    struct elt* next;

    for (i = 0; i < d->size; i++) {
        for (e = d->table[i]; e != 0; e = next) {
            next = e->next;

            free(e->key);
            free(e->value);
            free(e);
        }
    }

    free(d->table);
    free(d);
}

#define FNV_PRIME_64 ((1ULL<<40)+(1<<8)+0xb3)
#define FNV_OFFSET_BASIS_64 (14695981039346656037ULL)

uint64_t
hash_function(const char* bytes)
{
    uint64_t h = FNV_OFFSET_BASIS_64;

    for (size_t i = 0; bytes[i] != '\0'; i++) {
        h = h ^ bytes[i];
        h *= FNV_PRIME_64;
    }

    return h;
}

static void
grow(Dict d)
{
    Dict d2;            /* new dictionary we'll create */
    struct dict swap;   /* temporary structure for brain transplant */
    int i;
    struct elt* e;

    d2 = internalDictCreate(d->size * GROWTH_FACTOR);

    for (i = 0; i < d->size; i++) {
        for (e = d->table[i]; e != 0; e = e->next) {
            /* note: this recopies everything */
            /* a more efficient implementation would
             * patch out the strdups inside DictInsert
             * to avoid this problem */
            DictInsert(d2, e->key, e->value);
        }
    }

    /* the hideous part */
    /* We'll swap the guts of d and d2 */
    /* then call DictDestroy on d2 */
    swap = *d;
    *d = *d2;
    *d2 = swap;

    DictDestroy(d2);
}

/* insert a new key-value pair into an existing dictionary */
void
DictInsert(Dict d, const char* key, const char* value)
{
    struct elt* e;
    unsigned long h;

    assert(key);
    assert(value);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = strdup(key);
    e->value = strdup(value);

    h = hash_function(key) % d->size;

    e->next = d->table[h];
    d->table[h] = e;

    d->n++;

    /* grow table if there is not enough room */
    if (d->n >= d->size * MAX_LOAD_FACTOR) {
        grow(d);
    }
}

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
const char*
DictSearch(Dict d, const char* key)
{
    struct elt* e;

    for (e = d->table[hash_function(key) % d->size]; e != 0; e = e->next) {
        if (!strcmp(e->key, key)) {
            /* got it */
            return e->value;
        }
    }

    return 0;
}

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void
DictDelete(Dict d, const char* key)
{
    struct elt** prev;          /* what to change when elt is deleted */
    struct elt* e;              /* what to delete */

    for (prev = &(d->table[hash_function(key) % d->size]);
        *prev != 0;
        prev = &((*prev)->next)) {
        if (!strcmp((*prev)->key, key)) {
            /* got it */
            e = *prev;
            *prev = e->next;

            free(e->key);
            free(e->value);
            free(e);

            return;
        }
    }
}

size_t bSize;
char* buffer;

struct codeWord {
    char* word;
    char* definition;
};

struct codeWord** codeWords;

Dict d;

struct codeWord createWord(char* wrd, char* def) {
    struct codeWord cW;
    cW.word = malloc(strlen(wrd) + 1);
    cW.definition = malloc(strlen(def) + 1);

    strcpy(cW.word, wrd);
    strcpy(cW.definition, def);

    return cW;
}

void emptyBuffer() {
    memset(buffer, 0, strlen(buffer));
    buffer[0] = '\0';
    bSize = 0;
}

void appendToBuffer(char c) {
    buffer = (char*) realloc(buffer, bSize + 2);
    buffer[bSize] = c;
    buffer[bSize + 1] = '\0';
    bSize++;
}

void DefineWord() {
    char c;
    emptyBuffer();

    //struct codeWord* pcW = calloc(3, sizeof(char*));

    char* word = malloc(10);
    char* definition = malloc(10);

    int createdWord = 0;
    int createdDefinition = 0;

    while (!createdWord) {
        c = getchar();
        //printf("getting word\n");
        if (c == 32) {
            //see space
            if (strlen(buffer) != 0) {

                /*
                pcW = realloc(pcW, 2*sizeof(char*) + 1);

                //have word
                pcW->word = malloc(strlen(buffer));
                strcpy(pcW->word, buffer); //Set word to buffer word
                createdWord = 1; // we defined the word

                //printf("%s\n", cW.word);
                */

                word = realloc(word, strlen(buffer) + 1);
                word = strcpy(word, buffer);

                createdWord = 1; // we defined the word
            }
            else {
                continue; //Extra space
            }
            emptyBuffer();
        }
        else {
            //making word
            appendToBuffer(c);
        }
    }

    size_t bufferLen = 0;
    emptyBuffer();

    //appendToBuffer(32);

    while (!createdDefinition) {
        c = getchar();
        //printf("getting def %c\n", c);
        appendToBuffer(c);
        bufferLen++;

        if (bufferLen >= 4){

            if (buffer[bufferLen - 1] == 'P'
                && buffer[bufferLen - 2] == 'O'
                && buffer[bufferLen - 3] == 'T'
                && buffer[bufferLen - 4] == 'S'
                ) {

                if (bufferLen < 5) {
                    c = getchar();
                    //printf("Got here%d\n", c);
                    if (c == 32 || c == EOF || c == '\0') {
                        /*
                        //Then we have definition in buffer
                        buffer[bufferLen - 5] = '\0';

                        pcW->definition = calloc((bufferLen - 4), sizeof(char));
                        strcpy(pcW->definition, buffer); //Set word to buffer word

                        //printf("%s%d\n", cW.definition, cW.definition[strlen(cW.definition)]);
                        */

                        //Then we have definition in buffer
                        bufferLen -= 5;
                        buffer[bufferLen] = '\0';

                        definition = (char *) realloc(definition, bufferLen + 1);
                        definition = strcpy(definition, buffer);

                        createdDefinition = 1; // we defined the word
                    }
                }
                else if (buffer[bufferLen - 5] == 32 || buffer[bufferLen - 5] == '\n') {
                    c = getchar();
                    //printf("Got here%d\n", c);
                    if (c == 32 || c == EOF || c == '\0') {
                        /*
                        //Then we have definition in buffer
                        buffer[bufferLen - 5] = '\0';

                        pcW->definition = calloc((bufferLen - 4), sizeof(char));
                        strcpy(pcW->definition, buffer); //Set word to buffer word
                        
                        //printf("%s%d\n", cW.definition, cW.definition[strlen(cW.definition)]);
                        */

                        //Then we have definition in buffer
                        bufferLen -= 5;
                        buffer[bufferLen] = '\0';

                        definition = (char*) realloc(definition, bufferLen + 1);
                        definition = strcpy(definition, buffer);

                        createdDefinition = 1; // we defined the word
                    }
                }
            }
        }

    }
    /*
    codeWords = (struct codeWord**)realloc(codeWords, (wordLen + 1) * sizeof(struct codeWord*));
    codeWords[wordLen] = pcW;
    wordLen++;
    */

    DictInsert(d, word, definition);
}

void replaceDefinedWord() {
    //Replaces buffer word with its definition
    const char * found = DictSearch(d, buffer);

    /*
    if (wordLen > 0) {
        for (size_t index = wordLen; index > 0; index--) {
            if (strcmp(buffer, codeWords[index - 1]->word) == 0) {
                found = 1;
                //Found the word! Print the definition
                //printf("(%s %s ", buffer, codeWords[index]->word);
                printf("%s", codeWords[index - 1]->definition);
                break;
            }
        }
    }
    

    //Prints out the word if not found
    //printf("%d\n",found);
    */

    if (found == 0) {
        printf("%s", buffer);
    }
    else {
        printf("%s", found);
    }
    //Empty buffer afterward
    //Function should put its own space after
}

void LevelOne() {
    //Surface level stuff
    char c;
    emptyBuffer();

    while (1) {
        c = getchar();
        //printf("%d\n", c);
        if (c == 32 || c == '\n') {
            //Sees space

            if (strlen(buffer) != 0) {
                //buffer has a word in it
                //printf("%s", buffer);
                //Check if it is DEFINE
                if (strcmp(buffer, "DEFINE") == 0) {
                    DefineWord();
                }
                else {
                    //Check if word is defined
                    replaceDefinedWord();
                    printf("%c", c);
                }

            }
            else {
                //Print the space because it's just an extra space
                printf("%c", c);
            }

            emptyBuffer();

        }
        else if (c == EOF) {
            //Sees EOF

            //assume it is not DEFINE
            //match Word to Definition
            replaceDefinedWord();

            //which prints word or definition
            break;
        }
        else {
            appendToBuffer(c);
        }


    }
}

int main(int args, char** argv)
{
    d = DictCreate();
    buffer = (char*)calloc(2, sizeof(char));

    bSize = 0;

    LevelOne();
    
    DictDestroy(d);

    return 0;
}