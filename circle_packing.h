//
// Created by bsliy on 2022/10/10.
//

#ifndef MAIN_C_CIRCLE_PACKING_H
#define MAIN_C_CIRCLE_PACKING_H

typedef struct node {
    struct circle* c;
    /* link chain */
    struct node* next;
    struct node* prev;
} node;

typedef struct circle {
    double r;
    double x;
    double y;
} circle;

#endif //MAIN_C_CIRCLE_PACKING_H
