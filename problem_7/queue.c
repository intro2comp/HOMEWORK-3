#include <stdlib.h>

#include "queue.h"


/* Initialize a new queue with *max_size* possible items on the heap */
Queue *queue_init(int max_size)
{
    Queue *queue = malloc(sizeof(Queue) + sizeof(int) * max_size);

    queue->head = 0;
    queue->tail = -1;
    queue->size = 0;
    queue->max_size = max_size;

    return queue;
}


/* Free a queue from the heap */
void queue_destroy(Queue *queue)
{
    free(queue);
}


/* Determine if a queue is empty */
int queue_is_empty(Queue *queue)
{
    return queue->size == 0;
}


/* Determine if a queue is full */
int queue_is_full(Queue *queue)
{
    return queue->size == queue->max_size;
}


/* Return the top item of the queue without popping */
int queue_peek(Queue *queue)
{
    return queue->data[queue->head];
}


/* Add an item to the end of the queue */
void queue_push(Queue *queue, int value)
{
    if (queue_is_full(queue))
        return;

    if (queue->tail == queue->max_size - 1)
        queue->tail = -1;

    queue->data[++queue->tail] = value;
    queue->size++;
}


/* Return the top item of the queue and remove item */
int queue_pop(Queue *queue)
{
    int value = queue->data[queue->head++];

    if (queue->head == queue->max_size)
        queue->head = 0;

    queue->size--;

    return value;
}

