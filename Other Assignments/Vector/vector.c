#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

struct vectorPart {
    size_t spaceAlloc;
    size_t size;

    size_t lSize;
    size_t rSize;

    int* leftList;
    int* rightList;
};

struct vector {
    struct vectorPart* lower;
    struct vectorPart* upper;
};

struct vectorPart* vPCreate(size_t n, int value) {
    struct vectorPart* v = malloc(sizeof(struct vectorPart));
    
    v->size = n;

    size_t l = n / 2;

    if (l < 500) {
        v->spaceAlloc = 500;
    }
    else {
        v->spaceAlloc = l * 2;
    }

    v->lSize = l;
    v->leftList = calloc(v->spaceAlloc, sizeof(int));

    v->rSize = n - l;
    v->rightList = calloc(v->spaceAlloc, sizeof(int));

    for (size_t i = 0; i < v->lSize; i++) {
        v->leftList[i] = value;
    }

    for (size_t i = 0; i < v->rSize; i++) {
        v->rightList[i] = value;
    }

    return v;
}

Vector* vectorCreate(size_t n, int value) {
    Vector* v = malloc(sizeof(Vector));
    v->lower = vPCreate(n / 2, value);
    v->upper = vPCreate(n - n / 2, value);

    return v;
}

void vPDestroy(struct vectorPart* root) {
    free(root->leftList);
    free(root->rightList);
    free(root);
}

void vectorDestroy(Vector* v) {
    vPDestroy(v->lower);
    vPDestroy(v->upper);
    free(v);
}

size_t vectorSize(const Vector* v) {
    return v->upper->size + v->lower->size;
}

void vPInsert(struct vectorPart* v, size_t i, int value) {
    //Idea: Insert small index at back of lList
    //Insert large index at back of rList

    if (i > v->size) {
        return;
    }

    v->size++;

    //Reallocating memory for lists
    if ((v->lSize + 1) > v->spaceAlloc || (v->rSize + 1) > v->spaceAlloc) {
        v->leftList = realloc(v->leftList , (v->rSize + v->lSize + 500) * sizeof(int));
        v->rightList = realloc(v->rightList, (v->rSize + v->lSize + 500) * sizeof(int));
        v->spaceAlloc = (v->rSize + v->lSize + 1);
    }

    if (i >= v->lSize) {
        //Place in rList
        size_t index = i - v->lSize;
        
        for (size_t c = v->rSize; c >= index + 1; c--) {
            v->rightList[c] = v->rightList[c - 1];
        }
        v->rightList[index] = value;
        v->rSize++;
    }
    else {
        //Place in lList
        size_t index = v->lSize - i;

        for (size_t c = v->lSize; c >= index + 1; c--) {
            v->leftList[c] = v->leftList[c - 1];
        }
        v->leftList[index] = value;
        v->lSize++;
    }
}

void vectorInsert(Vector* v, size_t i, int value) {
    if (i <= v->lower->size) {
        vPInsert(v->lower, i, value);
    }
    else {
        if ((i - v->lower->size) > v->upper->size) {
            return; //out of bounds
        }
        vPInsert(v->upper, i - v->lower->size, value);
    }
}


//lList is in decreasing order, means i[0] is the last element
//rList is in increasing order, means i[0] is the first element
int * vPRef(struct vectorPart* v, size_t i) {
    if (i < v->lSize) {
        if (i >= 0) {
            return &(v->leftList[v->lSize - 1 - i]);
        }
        return 0;
    }
    else {
        if (i < v->size) {
            return &(v->rightList[i - v->lSize]);
        }
        return 0;
    }
}

int* vectorRef(Vector* v, size_t i) {
    if (i < v->lower->size) {
        if (i >= 0) {
            return vPRef(v->lower, i);
        }
        return 0;
    }
    else {
        if ((i - v->lower->size) < v->upper->size) {
            return vPRef(v->upper, i - v->lower->size);
        }
        return 0;
    }
}

/*
void printVP(struct vectorPart* v) {
    //left
    for (size_t i = v->lSize; i > 0; i--) {
        printf("%d, ", v->leftList[i - 1]);
    }
    printf("|");
    //right
    for (size_t i = 0; i < v->rSize; i++) {
        printf("%d, ", v->rightList[i]);
    }
    printf(" End\n");
}

void printVector(Vector * v) {
    printf("Vector: %p:\n", v);
    printf("Sizes T: %zu L: %zu %zu U: %zu %zu\n", vectorSize(v), v->lower->lSize, v->lower->rSize, v->upper->lSize, v->upper->rSize);

    printVP(v->lower);
    printVP(v->upper);
}



int main() {
    Vector* v = vectorCreate(1000, 2);
    
    printf("%p\n", vectorRef(v, 3));

    *vectorRef(v, 999) = 4;
    printVector(v);
}
*/
