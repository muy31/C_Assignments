#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "rooms.h"

#define SIZE_INITIAL (16)
#define SIZE_MULTIPLIER (2)

#define FIELDS (4)

// convert room number to pointer
// pointer will be null for bad room number
static struct room *
pointerize(size_t size, struct room **rooms, int number)
{
    if(number < 0 || number >= size) {
        return 0;
    } else {
        return rooms[number];
    }
}

// parses a file with format
// <room> <north> <south> <description>
//
// returns pointer to room 0
//
// It is the file-writer's responsibility to make sure
// that there are no duplicate or unreachable rooms.
//
// Exits to rooms that don't exist will become null pointers.
static struct room *
loadFile(char *filename)
{
    FILE *f;

    // updated 2022-03-16 to give better error output
    if((f = fopen(filename, "r")) == 0) {
        perror(filename);
        exit(1);
    }

    size_t size = SIZE_INITIAL; 

    struct line {
        char *description;
        int north;
        int south;
    } *lines = calloc(size, sizeof(struct line));

    int room;
    int north;
    int south;
    char *description;
    int count;

    // process all the lines
    while((count = fscanf(f, "%d %d %d %m[^\n]", &room, &north, &south, &description)) != EOF) {
        assert(count == FIELDS);
        assert(room >= 0);

        while(room >= size) {
            size_t oldSize = size;

            size *= SIZE_MULTIPLIER;
            lines = realloc(lines, size * sizeof(struct line));
            memset(&lines[oldSize], 0, (size - oldSize) * sizeof(struct line));
        }

        lines[room].north = north;
        lines[room].south = south;
        lines[room].description = description;
    }

    // now construct rooms by replacing room numbers with pointers,
    // turning a reasonable data structure into a bad one ☹
    struct room **rooms = calloc(size, sizeof(struct room *));

    // allocate structs for all the rooms
    for(size_t i = 0; i < size; i++) {
        if(lines[i].description != 0) {
            rooms[i] = malloc(sizeof(struct room));
            rooms[i]->description = lines[i].description;
        }
    }

    // translate all the indices to pointers
    for(size_t i = 0; i < size; i++) {
        if(lines[i].description != 0) {
            // pointerize will translates bogus room numbers
            // into null pointers
            rooms[i]->north = pointerize(size, rooms, lines[i].north);
            rooms[i]->south = pointerize(size, rooms, lines[i].south);
        }
    }

    struct room *start = rooms[0];

    free(lines);
    free(rooms);
    fclose(f);

    return start;
}

int
main(int argc, char **argv)
{

    if(argc != 2) {
        fprintf(stderr, "Usage: %s mapFile\n", argv[0]);
        return 1;
    }

    struct room *start = loadFile(argv[1]);

    int c;

    for(;;) {
        puts(start->description);

noPrint:
        switch((c = getchar())) {

            case 'n':
                if(start->north) {
                    start = start->north;
                } else {
                    puts("Ye can't go north.");
                }
                break;

            case 's':
                if(start->south) {
                    start = start->south;
                } else {
                    puts("Ye can't go south.");
                }
                break;

            case '\n':
            case '\t':
            case ' ':
                goto noPrint;

            case 'q':
            case EOF:
                goto done;

            default:
                printf("Unrecognized command %c\n", c);
                goto noPrint;
        }
    }

done:
    fprintf(stderr, "%zu rooms freed\n", freeRooms(start));

    return 0;
}
