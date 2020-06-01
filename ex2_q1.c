#include "ex2_q1.h"


int main()
{
	int quad_pid, hex_pid = 0;
	int in_fd = 0, out_fd = 0;

	create_input_files();

if ((quad_pid = fork()) == 0) { 
    // printf("in quad helper\n"); 
	// char *argv[] = {QUAD_IN, 0};

		in_fd = open(QUAD_IN, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		out_fd = open(QUAD_OUT , O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

		//redirect input output files
		dup2(in_fd, INPUT_FD);
		dup2(out_fd, OUTPUT_FD);
		close(in_fd);
		close(out_fd);

    	execv(HELPER_PATH, NULL);
		exit(0);
} 
else {
	if((hex_pid = fork()) == 0){ 
		// printf("in hex helper\n"); 
		// char *argv[] = {HEX_IN, 0};

		in_fd = open(HEX_IN, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		out_fd = open(HEX_OUT, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

		//redirect input output files
		dup2(in_fd, INPUT_FD);
		dup2(out_fd, OUTPUT_FD);
		close(in_fd);
		close(out_fd);

       	execv(HELPER_PATH, NULL);
		exit(0);

	}
	else {
		int pid = 0;
		int status = 0;
		// printf("quad_pid: %d\n", quad_pid);
		// printf("hex_pid: %d\n", hex_pid);
		for(int i = 0 ; i < 2 ; i++)
		{
			pid = wait(&status);
			printf("child terminated - created %d ", WEXITSTATUS(status));
			if(pid == quad_pid)
				printf("quadrilaterals\n");
			else
				printf("hexagons\n");			
		}
		
    	printf("all child processes terminated\n");
	}
    
} 

return 0;
}


void create_input_files()
{
	char str_command[MAX_COMMAND_LEN];
	struct command command;

	init_command(&command);

 	FILE *hex_file, *quad_file;
	quad_file = fopen (QUAD_IN,"w");
	hex_file = fopen (HEX_IN,"w");


	while(command.exit == 0)
	{
		// printf("in main while loop\n");
		scanf("%s", str_command);
		command = parse_command(str_command);
		write_to_file(quad_file, hex_file,&command);
	}

	fclose(quad_file);
	fclose(hex_file);

}

struct command parse_command(const char *str_command)
{
	struct command parsed_command;
	enum POLY_TYPE poly_type = NO_POLYGON;
	int isN = 0;
	int str_command_len = strlen(str_command);


	init_command(&parsed_command);


	for (int i = 0; i < str_command_len; i++)
	{
		// printf("letter: %c\n", str_command[i]);
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

			case 'C':
				break;

			case 'L':
				parsed_command.exit = 1;
			case 'A':
			case 'd':
			case 'p':
			case 'a':
				strncat(parsed_command.print_str, &str_command[i],1);
				if (str_command[i] == 'A')
					parsed_command.mode = ALL;
				else if(str_command[i] == 'C')
					parsed_command.mode = CURRENT;
				break;
			
			default:
				printf("Ther is no command for the letter %c\n",str_command[i]);
				break;
		}
	}
	
	if(isN)
	{
		parsed_command.poly_type = poly_type;
		get_vertices(&parsed_command);
	}

	strcpy(parsed_command.str,str_command);

	return parsed_command;

}

void write_to_file(FILE *quad_file, FILE *hex_file , struct command *command)
{
	// printf("in write_to_file\n");
	FILE* primary_file = NULL, *secondary_file = NULL;

	if(command->poly_type == QUADRI) {
		primary_file = quad_file;
		secondary_file = hex_file;
	}
	else {
		if(command->poly_type == HEXAGON) {
			primary_file = hex_file;
			secondary_file = quad_file;
		}
		else {
			if(command->mode == ALL){
				fprintf (quad_file, "%s\n",command->str);
				fprintf (hex_file, "%s\n",command->str);
			}
			return;
		}
	}
	
	fprintf (primary_file, "%s\n",command->str);
	fprintf (primary_file, "%016llx\n",command->vertices[0]);

	if(command->poly_type == HEXAGON)
		fprintf (primary_file, "%016llx\n",command->vertices[1]);

	if(command->mode == ALL)
		fprintf (secondary_file, "%s\n",command->print_str);
	else if(command->exit) // need to print current polygon and exit
		fprintf (secondary_file, "L\n");

}

void init_command(struct command *command)
{
	char ch = '\0';
	command->exit = 0;
	command->poly_type = NO_POLYGON;
	command->mode = NO_MODE;
	strncpy(command->str, &ch, MAX_COMMAND_LEN);
	strncpy(command->print_str, &ch, MAX_COMMAND_LEN);
}

//this function get vertices from user by poly type
void get_vertices(struct command *parsed_command)
{

	lluint points_64bit;

	scanf("%llx", &points_64bit);
	parsed_command->vertices[0] = points_64bit;

	if(parsed_command->poly_type == HEXAGON) {
		scanf("%llx", &points_64bit);
		parsed_command->vertices[1] = points_64bit;
	}
	

}




