#ifndef EX2_Q1_HELPER_H_
#define EX2_Q1_HELPER_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <string.h>

#define _GNU_SOURCE
#define MAX_COMMAND_LEN 10 
#define SIZE_OF_COORDINATE 16
#define SIZE_OF_X_Y 8
#define lluint long long unsigned int
#define MAX_STR 50
#define MAX_VERT    6 // maximum number of vertices

#define QUAD_IN "quad_in.tmp"
#define QUAD_OUT "quad_out.log"
#define HEX_IN "hex_in.tmp"
#define HEX_OUT "hex_out.log"

enum POLY_TYPE {NO_POLYGON, QUADRI = 4, HEXAGON = 6};
enum PRINT_MODE {ALL, CURRENT, NO_MODE};
enum PRINT_FLAGS {a = 1, d, p};

struct point { int8_t x, y;};

struct print_flags { char d, p, a; };

struct print_command {
   struct print_flags flags;
   enum PRINT_MODE mode;
};

struct polygon {
     enum POLY_TYPE poly_type;
     struct point vertices[MAX_VERT];
     double area, perimeter;
}; 

struct node {
    struct polygon* poly;
    struct node* next;
};

struct list {
    struct node* head;
    struct node* tail;
    int count;
};

struct command {
    int exit;
    enum POLY_TYPE poly_type;
    lluint coordinates[2];
	struct print_command print_command;
};


double calc_side(struct point pt1, struct point pt2);
double calc_triangle_area(struct point p1, struct point p2, struct point p3);
void calc_area(struct polygon *polygon);
void calc_perimeter(struct polygon *polygon);
double calc_point_distans(struct point pt1, struct point pt2);
void set_polygon_name(enum POLY_TYPE poly, char* poly_name);

void get_polygon_name(enum POLY_TYPE poly, char* poly_name);
void print_point(struct point pt);
void print_polygon(const struct polygon* poly);
struct polygon* create_polygon(struct command* command);


void add_polygon(struct list* ls, struct command* command);
void delete_list(struct list* ls);

void get_vertices(struct command *command, struct polygon *polygon);

void init_command(struct command *command);
void init_list(struct list *list);
void print_request(struct list* poly_list, struct command command);
void fetch_command(struct list *poly_list, struct command command);
struct command parse_command(const char *str_command);
void open_stream_files(FILE **in_file, FILE **out_file, char *str_in_file);




#endif