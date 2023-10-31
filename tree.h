#ifndef TREE_H
#define TREE_H

struct array{
    int capacity;
    int length;
    char **list;
    
};

struct array * array_list_new(int c);

void array_list_add_to_end(struct array * a, char * s);

void print_size(char *name);

int compare (const void *arg1, const void *arg2);

void print_tree(char *dirname, int space, int hidden, int size);

int is_regular_file(char *path);

void freearrlist(struct array * sorter);

#endif