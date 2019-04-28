typedef struct {
    int head;
    int tail;
    int size;
    int max_size;
    int data[];
} Queue;

Queue *queue_init(int size);
void queue_destroy(Queue *queue);

int queue_is_empty(Queue *queue);
int queue_is_full(Queue *queue);

int queue_peek(Queue *queue);
int queue_pop(Queue *queue);
void queue_push(Queue *queue, int value);