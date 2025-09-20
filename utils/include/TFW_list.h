#ifndef TFW_LIST_H
#define TFW_LIST_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct TFW_ListNode {
    struct TFW_ListNode *prev; /* Current node's pointer to the previous node */
    struct TFW_ListNode *next; /* Current node's pointer to the next node */
} TFW_ListNode;

/* list initialize */
__attribute__((always_inline)) static inline void TFW_ListInit(TFW_ListNode *list)
{
    list->next = list;
    list->prev = list;
}

/* Get list head node */
#define TFW_GET_LIST_HEAD(object) ((object)->next)

/* Get list tail node */
#define TFW_GET_LIST_TAIL(object) ((object)->prev)

/* Insert a new node to list. */
__attribute__((always_inline)) static inline void TFW_ListAdd(TFW_ListNode *list, TFW_ListNode *node)
{
    node->next = list->next;
    node->prev = list;
    list->next->prev = node;
    list->next = node;
}

/* Insert a node to the tail of a list. */
__attribute__((always_inline)) static inline void TFW_ListTailInsert(TFW_ListNode *list, TFW_ListNode *node)
{
    TFW_ListAdd(list->prev, node);
}

/* Insert a new node to list. */
__attribute__((always_inline)) static inline void TFW_ListNodeInsert(TFW_ListNode *list, TFW_ListNode *node)
{
    TFW_ListAdd(list, node);
}

/* Delete a specified node from list. */
__attribute__((always_inline)) static inline void TFW_ListDelete(TFW_ListNode *node)
{
    if (node->next != 0 && node->prev != 0) {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
    node->next = node;
    node->prev = node;
}

__attribute__((always_inline)) static inline bool TFW_IsListEmpty(const TFW_ListNode *node)
{
    return (bool)(node->next == node);
}

/*
 * @brief Obtain the pointer to a list in a structure
 *
 * @param type    [IN] Structure name.
 * @param member  [IN] Member name of the list in the structure.
 */
#define TFW_OFF_SET_OF(type, member) ((size_t)&(((type *)0)->member))

#ifndef TFW_CONTAINER_OF
#define TFW_CONTAINER_OF(ptr, type, member) \
    (type *)((char *)(ptr) - (char *) &((type *)0)->member)
#endif

/*
 * @brief Obtain the pointer to a structure that contains a list.
 * @param item    [IN] Current node's pointer to the next node.
 * @param type    [IN] Structure name.
 * @param member  [IN] Member name of the list in the structure.
 */
#define TFW_LIST_ENTRY(item, type, member) \
    ((type *)(void *)((char *)(item) - TFW_OFF_SET_OF(type, member)))

/* Iterate over a list of given type. */
#define TFW_LIST_FOR_EACH_ENTRY(item, list, type, member) \
    for ((item) = TFW_LIST_ENTRY((list)->next, type, member); \
            &(item)->member != (list); \
            (item) = TFW_LIST_ENTRY((item)->member.next, type, member))

/* Iterate over a list safe against removal of list entry. */
#define TFW_LIST_FOR_EACH_ENTRY_SAFE(item, nextItem, list, type, member) \
    for ((item) = TFW_LIST_ENTRY((list)->next, type, member), \
            (nextItem) = TFW_LIST_ENTRY((item)->member.next, type, member); \
            &((item)->member) != (list); \
            (item) = (nextItem), (nextItem) = TFW_LIST_ENTRY((item)->member.next, type, member))

__attribute__((always_inline)) static inline void TFW_ListDel(TFW_ListNode *prevNode, TFW_ListNode *nextNode)
{
    nextNode->prev = prevNode;
    prevNode->next = nextNode;
}

/* Delete node and initialize list */
__attribute__((always_inline)) static inline void TFW_ListDelInit(TFW_ListNode *list)
{
    TFW_ListDel(list->prev, list->next);
    TFW_ListInit(list);
}

/* Iterate over a list. */
#define TFW_LIST_FOR_EACH(item, list) \
    for ((item) = (list)->next; (item) != (list); (item) = (item)->next)

/* Iterate over a list safe against removal of list entry. */
#define TFW_LIST_FOR_EACH_SAFE(item, nextItem, list) \
    for ((item) = (list)->next, (nextItem) = (item)->next; (item) != (list); \
            (item) = (nextItem), (nextItem) = (item)->next)

/* Initialize a list. */
#define TFW_LIST_HEAD(list) TFW_ListNode list = { &(list), &(list) }

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* TFW_LIST_H */
