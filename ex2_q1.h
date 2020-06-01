#ifndef EX1_Q2_H_
#define EX1_Q2_H_

#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>

#define INPUT_FD 0
#define OUTPUT_FD 1

#define QUAD_IN "quad_in.tmp"
#define QUAD_OUT "quad_out.log"
#define HEX_IN "hex_in.tmp"
#define HEX_OUT "hex_out.log"
#define HELPER_PATH "./ex2_q1_helper"

#define _GNU_SOURCE
#define MAX_COMMAND_LEN 10 

#define lluint long long unsigned int


enum POLY_TYPE {NO_POLYGON, QUADRI = 4, HEXAGON = 6 };
enum PRINT_MODE {ALL, CURRENT, NO_MODE};


struct command {
    int exit;
    enum POLY_TYPE poly_type;
	enum PRINT_MODE mode;
    char str[MAX_COMMAND_LEN];
    char print_str[MAX_COMMAND_LEN];
    lluint vertices[2];
};


void get_vertices(struct command *parsed_command);
void init_command(struct command *command);
void write_to_file(FILE *quad_file, FILE *hex_file , struct command *command);
struct command parse_command(const char *str_command);
void create_input_files();




#endif