#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define REF 1000  //given by the problem description

typedef struct page {
    int id;
    int ref;
    int total;
    struct page *next;
} Page;

Page *active = NULL, *inactive = NULL;
Page **pages;
int N, M; // N: total pages, M: checker sleep time in microseconds
int refstring[REF];
int done = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void push_tail(Page **list, Page *p) {
    p->next = NULL;
    if (*list == NULL) { *list = p; return; } // if empty list -> new tail
    Page *cur = *list;
    while (cur->next) cur = cur->next;// go to tail
    cur->next = p;  // append new page at tail
}

void remove_page(Page **list, Page *p) {
    if (*list == NULL) return;
    if (*list == p) { *list = p->next; return; } // if head is to be removed
    Page *cur = *list;
    while (cur->next && cur->next != p) cur = cur->next; // find page before p
    if (cur->next == p) cur->next = p->next; // unlink p from list
}

int size(Page *list) { // return size of linked list
    int c = 0;
    while (list) { c++; list = list->next; }
    return c;
}

void *player_func(void *arg) {
    for (int i = 0; i < REF; i++) { // for each reference
        int id = refstring[i]; // get page id
        Page *p = pages[id]; // get page pointer

        //cs
        pthread_mutex_lock(&lock);
        p->ref = 1;

        remove_page(&active, p); // remove from active if present
        remove_page(&inactive, p); // remove from inactive if present
        push_tail(&active, p); // add to tail of active list

        if (size(active) > 0.7 * N) { // if active list > 70% of N
            for (int k = 0; k < 0.2 * N; k++) { // move 20% of N pages to inactive
                Page *front = active;
                if (!front) break;
                remove_page(&active, front);
                push_tail(&inactive, front);
            }
        }
        //ecs
        pthread_mutex_unlock(&lock);

        usleep(10); // simulate time between references (10 microseconds)
    }
    done = 1;
    return NULL;
}

void *checker_func(void *arg) {
    while (!done) {
        usleep(M); // M microseconds btw checks

        //cs
        pthread_mutex_lock(&lock);
        Page *cur = active;
        while (cur) {
            if (cur->ref) { // go through each page with reff == 1
                cur->total++; // record reference
                cur->ref = 0; // reset ref bit
            }
            cur = cur->next; // iterate to next page
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    pages = malloc(sizeof(Page*) * N);

    // create pages, initially active
    for (int i = 0; i < N; i++) {
        pages[i] = calloc(1, sizeof(Page));
        pages[i]->id = i;
        push_tail(&active, pages[i]);
    }

    // random refstring to simulate random mem access
    for (int i = 0; i < REF; i++) refstring[i] = rand() % N;

    pthread_t player, checker;
    pthread_create(&player, NULL, player_func, NULL);
    pthread_create(&checker, NULL, checker_func, NULL);
    pthread_join(player, NULL);
    pthread_join(checker, NULL);

    printf("Page_Id, Total_Referenced\n");
    for (int i = 0; i < N; i++)
        printf("%d, %d\n", pages[i]->id, pages[i]->total);

    printf("\nActive list: ");
    Page *c = active;
    while (c) { printf("%d ", c->id); c = c->next; }

    printf("\nInactive list: ");
    c = inactive;
    while (c) { printf("%d ", c->id); c = c->next; }
    printf("\n");
}