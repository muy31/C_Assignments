#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

struct item {
    uint64_t weight;
    uint64_t price;
    char* name;
};

typedef struct item* Item;

struct heap {
    int weight;
    size_t len;
    Item* items;
};

Item makeItem(int w, int p, char* s) {
    Item im = malloc(sizeof(struct item));
    im->weight = w;
    im->price = p;
    im->name = malloc(strlen(s) + 1);
    strcpy(im->name, s);

    return im;
}

struct heap makeHeap() {
    struct heap h;
    h.len = 0;
    h.items = calloc(100000, sizeof(Item));
    h.weight = 0;
    return h;
}

typedef struct heap* Heap;

void printHeap(Heap h) {
    for (size_t i = 0; i < (h->len); i++) {
        printf("%s| ", h->items[i]->name);
    }
    printf("%d\n", h->weight);
}

void addToHeap(Heap h, Item im) {
    
    h->items = realloc(h->items, sizeof(Item) * (2 + h->len));
    
    h->items[h->len] = im;
    h->weight += im->weight;

    //Backtracing
    size_t current = h->len;
    size_t root = (current - 1) / 2;

    int cont = current == 0;

    //if root is larger than leaf swap root and leaf

    while (!cont && 
        (h->items[current]->price * h->items[root]->weight) < (h->items[root]->price) * (h->items[current]->weight) ) {
        //Swap current and leaf
        Item tmp = h->items[root];
        h->items[root] = h->items[current];
        h->items[current] = tmp;

        //Switch current and leaf indices
        current = root;
        root = (current - 1) / 2;

        cont = current == 0;
    }

    h->len++;
    //printHeap(h);
}

Item removeFromHeap(Heap h) {
    //Save item
    Item toR = h->items[0];

    //Decrease array by 1
    h->len--;
    h->weight -= toR->weight;
    h->items[0] = h->items[h->len];
    h->items[h->len] = NULL;

    size_t current = 0;

    //printf("Item traverse: %s\n", h->items[0]->name);

    while (1) {
        size_t R = 2 * current + 1;
        size_t L = 2 * current + 2;

        if (L >= h->len) {
            //exclude L from calculation
            if (R >= h->len) {
                //no calculation
                break;
            }
            else {

                uint64_t pwC = h->items[current]->price * h->items[R]->weight;
                uint64_t pwR = h->items[R]->price * h->items[current]->weight;
           
                if (pwC > pwR) {
                    //swap current and R
                    Item tmp = h->items[R];
                    h->items[R] = h->items[current];
                    h->items[current] = tmp;

                    current = R; //tbh might as well break after this
                }
                else {
                    //do nothing
                    break;
                }
            }
        }
        else {
            uint64_t pC = h->items[current]->price;
            uint64_t wC = h->items[current]->weight;
            uint64_t pR = h->items[R]->price;
            uint64_t wR = h->items[R]->weight;
            uint64_t pL = h->items[L]->price;
            uint64_t wL = h->items[L]->weight;

            if (pC * wR > pR * wC || pC * wL > pL * wC) {
                //do something

                //printf("PC: %zu, %zu LR calc: %zu %zu vs %zu %zu\n",pC, wC, pR, wR, pL, wL);

                size_t index = R;
                //Compare the children
                if (pR * wL > pL * wR) {
                    index = L;
                }
                
                //swap current and L
                Item tmp = h->items[index];
                h->items[index] = h->items[current];
                h->items[current] = tmp;

                current = index;
            }
            else {
                //nothing happens since current = current
                break;
            }

        }

    }

    //printHeap(h);

    return toR;
}

void freeHeap(Heap h) {
    for (size_t i = 0; i < h->len; i++) {
        free(h->items[i]->name);
        free(h->items[i]);
    }
    free(h->items);
}

int main() {

	//Logistics
	struct heap hp = makeHeap();

	//addToHeap using &hp

	//First line
	uint64_t weightLimit;

	if (scanf("%lu", &weightLimit) < 1) {
		//ERROR
	}
	else {
		//value here of weightlimit will be
		//set to input
		uint64_t w, p;
		char* s = NULL;

		while (1) {
			if (scanf("%lu %lu %m[^\n]", &p, &w, &s) < 3) {
				//ERROR or end of input
                //printf("Some error?");
				break;
			}
			else {
				//p and w set to price and weight
				//s allocated and set to name
                Item it = makeItem(w, p, s);
                free(s);

                addToHeap(&hp, it);

                while (hp.weight > weightLimit) {
                    Item toPrint = removeFromHeap(&hp);
                    printf("%lu %lu %s\n", toPrint->price, toPrint->weight, toPrint->name);
                    free(toPrint->name);
                    free(toPrint);
                }

			}
		}

        freeHeap(&hp);

	}

	


}