#include "ex2_q1_helper.h"



int main(int argc, char *argv[])
{
	int list_len;
	struct command command;
	struct list poly_list;
	// printf("argc: %d\n", argc);

	init_command(&command);
	init_list(&poly_list);

	
	char str_command[MAX_COMMAND_LEN];

	while(command.exit == 0)
	{
		// printf("in main while loop\n");
		scanf("%s", str_command);
		command = parse_command(str_command);

		fetch_command(&poly_list, command);
	}

	list_len = poly_list.count;
	delete_list(&poly_list);
	return list_len;
}

void open_stream_files(FILE **in_file, FILE **out_file, char *str_in_file)
{
	*in_file = fopen(str_in_file, "r");

	if(strcmp(str_in_file, QUAD_IN) == 0)
		*out_file = fopen(QUAD_OUT, "w");
	else
		*out_file = fopen(HEX_OUT, "w");
}



struct command parse_command(const char *str_command)
{
	struct command parsed_command;
	enum POLY_TYPE poly_type = NO_POLYGON;
	int str_command_len = strlen(str_command);
	int isN = 0;

	init_command(&parsed_command);


	for (int i = 0; i < str_command_len; i++)
	{
		switch (str_command[i])
		{
			case 'N':
				isN = 1;
				break;
			case 'Q':
				poly_type = QUADRI;
				break;

			case 'H':
				poly_type = HEXAGON;
				break;

			case 'L':
				parsed_command.exit = 1;
				break;

			case 'C':
				parsed_command.print_command.mode = CURRENT;
				break;

			case 'A':
				parsed_command.print_command.mode = ALL;
				break;

			case 'd':
				parsed_command.print_command.flags.d = 1;
				break;
			case 'p':
				parsed_command.print_command.flags.p = 1;
				break;
			case 'a':
				parsed_command.print_command.flags.a = 1;
				break;
			
			default:
				printf("Ther is no command for the letter %c\n",str_command[i]);
				break;
		}
	}
	
	if(isN)
	{
		parsed_command.poly_type = poly_type;
		scanf("%llx", &(parsed_command.coordinates[0]));
		if(parsed_command.poly_type == HEXAGON)
			scanf("%llx", &(parsed_command.coordinates[1]));
		
	}

	return parsed_command;

}

void fetch_command(struct list *poly_list, struct command command)
{
	if(command.poly_type != NO_POLYGON)
		add_polygon(poly_list,&command);
	

	print_request(poly_list, command);

}

void init_command(struct command *command)
{
	command->exit = 0;

	command->poly_type = NO_POLYGON;

	command->print_command.mode = NO_MODE;
	command->print_command.flags.a = 0;
	command->print_command.flags.d = 0;
	command->print_command.flags.p = 0;
}

void init_list(struct list *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->count = 0;
}

//this function get vertices from user by poly type
void get_vertices(struct command *command, struct polygon *polygon)
{

	long long coordinate;
	int coordinate_mask = 0xffff;
	int x_y_mask = 0xff;

	lluint points_64bit = command->coordinates[0];


	for (int i = 0 ; i < command->poly_type; i++)
	{
		if(i == 4)
			points_64bit = command->coordinates[1];

		coordinate = (points_64bit >> (SIZE_OF_COORDINATE * (i % 4))) & coordinate_mask;
	
		polygon->vertices[i].x = coordinate & x_y_mask;
		polygon->vertices[i].y = (coordinate >> SIZE_OF_X_Y) & x_y_mask;

	}
}

void my_exception(int line, const char* function, char* msg)
{
	fprintf(stderr, "ERROR (line %d function: %s) !!!%s\n", line, function, msg);
    exit(1);
}

double calc_triangle_area(struct point p1, struct point p2, struct point p3)
{
	double area, 
	      x1 = p1.x, 
	      x2 = p2.x, 
	      x3 = p3.x, 
	      y1 = p1.y, 
	      y2 = p2.y, 
	      y3 = p3.y;		  
	area = 0.5*(x1*y2 + x2*y3 + x3*y1 - x2*y1 - x3*y2 -x1*y3);
	area = (area > 0) ? area : -area;
	return area;
}

void calc_area(struct polygon *polygon)
{
    double area = 0.0;
    enum POLY_TYPE type = polygon->poly_type;
	for(int i = 1 ; i < type - 1 ; i++)
        area += calc_triangle_area(polygon->vertices[0], polygon->vertices[i], polygon->vertices[i+1]);
    

    polygon->area = area;
}

void calc_perimeter(struct polygon *polygon)
{
	double perimeter = 0.0;
	enum POLY_TYPE type = polygon->poly_type;
	for(int i = 0 ; i < type ; i++)
		perimeter += calc_point_distans(polygon->vertices[i], polygon->vertices[(i + 1) % type]);

	polygon->perimeter =  perimeter;
}

double calc_point_distans(struct point pt1, struct point pt2)
{
	double x = pt1.x - pt2.x;
	double y = pt1.y - pt2.y;
	return sqrt(x*x + y*y);	
}

void print_request(struct list* poly_list, const struct command command)
{
	struct node* node = poly_list->head;
	if(node && command.print_command.mode != NO_MODE)
	{
		if(command.print_command.mode == CURRENT)
		{
			if(command.poly_type != NO_POLYGON)
				node = poly_list->tail;
			else
				return;
		}
		
		while(node){
			if(command.print_command.flags.d == 1)
				print_polygon(node->poly);

			if(command.print_command.flags.p == 1)
				printf("perimeter = %.1f\n", node->poly->perimeter);

			if(command.print_command.flags.a == 1)
				printf("area = %.1f\n", node->poly->area);

			node = node->next;
		}
	}
}


void print_polygon(const struct polygon* poly)
{
	char poly_name[MAX_STR] = {'\0'};
	get_polygon_name(poly->poly_type, poly_name);
	printf("%s ", poly_name);

	for(int i = 0 ; i < poly->poly_type ; i++)
	{
		print_point(poly->vertices[i]);
	}

	printf("\n");

}

//this function set polygon type into string
void get_polygon_name(enum POLY_TYPE poly, char* poly_name)
{
  switch (poly)
  {
    case QUADRI:  sprintf(poly_name, "quadrilateral"); break;
    case HEXAGON: sprintf(poly_name, "hexagon"); break;
    default: my_exception(__LINE__, __func__, "bad-polygon_type");
  }
}


void print_point(struct point pt)
{
	printf("{%d, %d} ", pt.x, pt.y);
}

struct polygon* create_polygon(struct command* command)
{
	struct polygon* new_poly = malloc(sizeof(struct polygon));
	new_poly->poly_type = command->poly_type;
	get_vertices(command, new_poly);
	calc_area(new_poly);
	calc_perimeter(new_poly);
	return new_poly;
}

void add_polygon(struct list* ls, struct command* command)
{
	struct node *new_node = malloc(sizeof(struct node));
	
	new_node->poly = create_polygon(command);
	new_node->next = NULL;



	if(ls->head == NULL)
		ls->head = ls->tail = new_node;
	else{
		ls->tail->next = new_node;
		ls->tail = new_node;
	}
	ls->count++;
}

void delete_list(struct list* ls)
{
	struct node *node = ls->head;
	
	while (node)
	{
		ls->head = node->next;

		free(node->poly);
		free(node);
		node = ls->head;
	}
	ls->head = ls->tail = NULL;
	
}


