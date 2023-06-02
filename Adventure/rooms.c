#include <stdio.h>
#include <stdlib.h>
#include "rooms.h"

struct map {
	size_t size;
	struct room** rooms;
};

struct map m;

/*
int roomInMap(struct room* r) {
	size_t index = 0;
	while (index < m.size) {
		if (r == m.rooms[index]) {
			return 1;
		}
		index++;
	}
	return 0;
}
*/

int addMap(struct room* r) {
	if (r->description != NULL) {
		m.size++;
		m.rooms = (struct room**)realloc(m.rooms, m.size * sizeof(struct room*));
		m.rooms[m.size - 1] = r;
		return 1;
	}
	return 0;
}

void SerializeRooms(struct room* start) {
	if (start != NULL) {
		int worked = addMap(start);
		if (worked) {
			free(start->description);
			start->description = NULL;

			SerializeRooms(start->north);
			SerializeRooms(start->south);
		}
	}
}

size_t freeRooms(struct room* start) {
	
	m.size = 0;
	m.rooms = malloc(1 * sizeof(struct room*));

	SerializeRooms(start);

	for (size_t idx = 0; idx < m.size; idx++) {
		//free(m.rooms[idx]->description);
		free(m.rooms[idx]);
	}

	return m.size;
}