// A map consists of malloc'd struct rooms, each of which
// contains a pointer to a distinct non-null malloc'd string,
// and also contains pointers to a room to the north
// (or null if there is no such room)
// and a room to the south
// (or null if there is no such room).
struct room {
    char *description;
    struct room *north;
    struct room *south;
};

// Free every struct room that is either start,
// or can be reached from start 
// by following any sequence of north
// and south pointers.
// Also free descriptions.
// Returns number of rooms freed.
size_t freeRooms(struct room *start);
