// map data structure with various preconditions as a result of purpose to simplify
//    implementation

typedef struct map_elem {
    char key;
    char *value;
    struct map_elem *next;
} map_elem_t;

typedef struct map {
    map_elem_t *head;
    map_elem_t *tail;
} map_t;

// alloc map
map_t *alloc_map();

// Pre: an element with key 'key' does not already exist in the map
void insert(map_t *map, char key, char *value);

// Pre: an element with key 'key' exists in the map. Its value will be returned
char *find(map_t *map, char key);

// free map
void free_map(map_t *map);