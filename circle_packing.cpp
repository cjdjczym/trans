//
// Created by bsliy on 2022/10/10.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <vector>

#include "circle_packing.h"

using namespace std;

double width = 10000, height = 10000;
int n = 5000;
double r = sqrt(0.2628 * width * height / n);
double dr = r * 0.2;
int judge_count = 15;

static void place(circle *a, circle *b, circle *c) {
    double dx = b->x - a->x, _x, a2, dy = b->y - a->y, _y, b2;
    double d2 = dx * dx + dy * dy;
    if (d2 > 0) {
        a2 = a->r + c->r;
        a2 *= a2;
        b2 = b->r + c->r;
        b2 *= b2;
        if (a2 > b2) {
            _x = (d2 + b2 - a2) / (2 * d2);
            _y = sqrt(max(.0, b2 / d2 - _x * _x));
            c->x = b->x - _x * dx - _y * dy;
            c->y = b->y - _x * dy + _y * dx;
        } else {
            _x = (d2 + a2 - b2) / (2 * d2);
            _y = sqrt(max(.0, a2 / d2 - _x * _x));
            c->x = a->x + _x * dx - _y * dy;
            c->y = a->y + _x * dy + _y * dx;
        }
    } else {
        c->x = a->x + c->r;
        c->y = a->y;
    }
}

bool intersects(circle *a, circle *b) {
    double eps = 1e-6;
    double _dr = a->r + b->r - eps;
    double dx = b->x - a->x;
    double dy = b->y - a->y;
    return _dr * _dr > dx * dx + dy * dy;
}

double random_r() {
    double d = rand() / double(RAND_MAX);
    return (d * (2 * dr + 1)) + r - dr;
}

static circle *alloc_circle() {
    auto *c = (circle *) malloc(sizeof(circle));
    c->r = random_r();
    return c;
}

static node *alloc_node(circle *c) {
    node *_n = (node *) malloc(sizeof(node));
    _n->c = c;
    _n->next = nullptr;
    _n->prev = nullptr;
    return _n;
}

void write_txt(const std::string &output_file, vector<circle> *circles) {
    ofstream f;
    f.open(output_file);
    f << circles->size() << endl;
    for (auto &i: *circles) {
        f << i.x / double(width) << " " << i.y / double(height) << endl;
    }
    f.close();
}

int main(int argc, char **argv) {
    if (argc > 1) width = atof(argv[1]);
    if (argc > 2) height = atof(argv[2]);
    if (argc > 3) n = atoi(argv[3]);
    r = sqrt(0.2628 * width * height / n);
    dr = r * 0.2;
    cout << "generating " << n << " points in [" << width << ", " << height << "]" << endl;
    double x = width / sqrt(2), y = height / sqrt(2);

    clock_t start, end;
    start = clock();

    srand(time(nullptr));
    vector<circle *> points;

    circle *c0 = alloc_circle(), *c1 = alloc_circle(), *c2 = alloc_circle();
    c0->x = x - c0->r;
    c1->x = x + c1->r;
    c0->y = y;
    c1->y = c0->y;

    place(c1, c0, c2);

    points.push_back(c0);
    points.push_back(c1);
    points.push_back(c2);

    node *a = alloc_node(c0), *b = alloc_node(c1), *c = alloc_node(c2);

    a->next = c->prev = b;
    b->next = a->prev = c;
    c->next = b->prev = a;

    bool flag[4] = {false, false, false, false};

    pack:
    while (!(flag[0] && flag[1] && flag[2] && flag[3])) {
        circle *tmp = alloc_circle();

        place(a->c, b->c, tmp);

        node *j = b->next, *k = a->prev;
        int iter = 0;

        while (j != k->next && iter++ < judge_count) {
            if (intersects(j->c, tmp)) {
                b = j;
                goto pack;
            }
            if (intersects(k->c, tmp)) {
                a = k;
                goto pack;
            }
            j = j->next, k = k->prev;
        }

        c = alloc_node(tmp);

        c->prev = a;
        c->next = b;
        b->prev = c;
        a->next = b->prev;

        points.push_back(c->c);

        if (c->c->x < x - x / sqrt(2) && c->c->y < y - y / sqrt(2))
            flag[0] = true;
        if (c->c->x > x + x / sqrt(2) && c->c->y > y + y / sqrt(2))
            flag[1] = true;
        if (c->c->x > x + x / sqrt(2) && c->c->y < y - y / sqrt(2))
            flag[2] = true;
        if (c->c->x < x - x / sqrt(2) && c->c->y > y + y / sqrt(2))
            flag[3] = true;

        a = c;
    }

    vector<circle> filteredPoints;

    for (auto &iter: points) {
        if (iter->x > x - x / sqrt(2) && iter->x < x + x / sqrt(2) && iter->y > y - y / sqrt(2) &&
            iter->y < y + y / sqrt(2)) {
            circle item = *iter;
            item.x -= width / 2 * (sqrt(2) - 1);
            item.y -= height / 2 * (sqrt(2) - 1);
            filteredPoints.push_back(item);
        }
    }

    end = clock();

    cout << "The run time is: " << double(end - start) * 1000 / CLOCKS_PER_SEC << "ms" << endl;
    cout << "output " << points.size() << " points" << endl;
    cout << "output " << filteredPoints.size() << " points" << endl;

    write_txt("sample.txt", &filteredPoints);
    return 0;
}
