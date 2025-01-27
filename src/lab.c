#include "lab.h"
#include <stdlib.h>

list_t *list_init(void (*destroy_data)(void *),
                  int (*compare_to)(const void *, const void *)) {
    list_t *list = malloc(sizeof(list_t));
    if (!list) {
        return NULL;
    }

    list->destroy_data = destroy_data;
    list->compare_to = compare_to;
    list->size = 0;

    list->head = malloc(sizeof(node_t));
    if (!list->head) {
        free(list);
        return NULL;
    }

    list->head->next = list->head;
    list->head->prev = list->head;
    list->head->data = NULL;

    return list;
}

void list_destroy(list_t **list) {
    if (list == NULL || *list == NULL) {
        return;
    }

    list_t *l = *list;

    node_t *current = l->head->next;
    while (current != l->head) {
        node_t *next_node = current->next;
        l->destroy_data(current->data);
        free(current);
        current = next_node;
    }

    free(l->head);
    free(l);
    *list = NULL;
}

list_t *list_add(list_t *list, void *data) {
    if (list == NULL) {
        return NULL;
    }

    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node) {
        return list;
    }

    new_node->data = data;

    new_node->prev = list->head;
    new_node->next = list->head->next;

    list->head->next->prev = new_node;
    list->head->next = new_node;

    list->size++;

    return list;
}

void *list_remove_index(list_t *list, size_t index) {
    if (list == NULL || index >= list->size) {
        return NULL;
    }

    node_t *current = list->head->next;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;

    void *data = current->data;
    free(current);

    list->size--;

    return data;
}

int list_indexof(list_t *list, void *data) {
    if (list == NULL) {
        return -1;
    }

    int index = 0;
    node_t *current = list->head->next;

    while (current != list->head) {
        if (list->compare_to(current->data, data) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}