/* File:     analyze.c
 * Purpose:  Perform analysis on US Census data.
 *
 * Input:    CSV File(s) to analyze.
 * Output:   Summary information about the data.
 *
 * Compile:  gcc -g -Wall -o analyze analyze.c
 *           (or run make)
 *
 * Run:      ./analyze data_mt.csv data_ne.csv
 *
 * Opening file: data_mt.csv
 * Opening file: data_ne.csv
 * States found: MT NE
 * -- State: MT --
 * Number of Records: 22807
 * Sample Population: 46114
 * Average Income: 35222.89
 * Average Rent: 520.18
 * Average vehicles per household: 1.80
 * Vehicles per capita: 0.89
 * -- State: NE --
 * Number of Records: 41129
 * Sample Population: 89113
 * Average Income: 41192.08
 * Average Rent: 507.69
 * Average vehicles per household: 1.85
 * Vehicles per capita: 0.85
 *
 * CSV format:
 *
 * NE, 31, 1, 0, 1, 0
 * NE, 31, 02, 00252000, 2, 0
 * NE, 31, 02, 00014000, 1, 00210
 * NE, 31, 04, 00069000, 4, 0
 * NE, 31, 1, 0, 1, 0
 * NE, 31, 04, 00042000, 1, 00450
 * NE, 31, 02, 00067700, 2, 0
 *
 * Fields:
 *      State ID,
 *      state code,
 *      people in household,
 *      income,
 *      vehicles,
 *      rent
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Add elements to the state_info struct as necessary. For instance, you
 * may want to determine the sample population by recording the total number of
 * people found in each household */
struct state_info {
    int state_id;
    char code[3];
    unsigned long total_people;
    unsigned long rent_people;
    unsigned long num_records;
    unsigned long total_income;
    unsigned long total_vehicles;
    unsigned long total_rent;
    float aver_rent;
    float aver_income;
    float aver_vehicles_household;
    float aver_vehicles_captita;
};

void analyze_file(FILE *file, struct state_info *states[], int num_states);
void print_report(struct state_info *states[], int num_states);
struct state_info* create_state(char* data[], int size);
struct state_info* get_state(char* line);
void addSate(struct state_info* states[],struct state_info* state, int state_id);
void addInfor(struct state_info* states[],struct state_info* state, int state_id);
void print_state(struct state_info *state);

int main(int argc, char *argv[]) {

    /* TODO: fix this conditional. You should be able to read multiple files. */
    if (argc < 2) {
        printf("Usage: %s csv_file1 csv_file2 ... csv_fileN \n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Let's create an array to store our state data in. As we know, there are
     * 50 states. However, the US Census also includes unincorporated
     * territories such as Puerto Rico. We'll use 100 to be safe. */
    const int num_states = 100;
    struct state_info *states[num_states] = { NULL };

    for (int i = 1; i < argc; i++) {
        /* TODO: Open the file for reading */
        FILE *file = fopen(argv[i],"r");
        /* TODO: If the file doesn't exist, print an error message and move on
         * to the next file. */
        if (file == NULL)
        {
            printf("Cant open file %s\n", argv[i]);
            perror("fopen");
        }
        else
        {
            analyze_file(file, states, num_states);
        }
         fclose(file);
    /* Now that we have recorded data for each file, we'll summarize them: */

}
  print_report(states, num_states);
    return 0;
}


/* Function:     create_state
 * Purpose:      analyze every data in a line and store them in the ptr_state
 * In/out args:  data[], size
                 ptr_state
 */
struct state_info* create_state(char* data[], int size)
{
  struct state_info * ptr_state = NULL;

  if(size == 6)
  {
      ptr_state = (struct state_info*)calloc(1, sizeof(struct state_info));
      ptr_state->num_records++;
      ptr_state->state_id = atoi (data[0]);
      strcpy(ptr_state->code,data[1]);
      ptr_state->total_people = atoi(data[2]);
      ptr_state->total_income = atol(data[3]);
      ptr_state->total_vehicles = atol(data[4]);
      ptr_state->total_rent = atol(data[5]);
      if(ptr_state->total_rent != 0){
          ptr_state->rent_people++;
      }
    }
    return ptr_state;
}

/* Function:    get_state
 * Purpose:     store a line's data in *state
 * In/out args: line
 *              struct state_info* state;
 */
struct state_info* get_state(char* line){
  char* token = strtok(line, ", ");
  char* tokens[6];
  int tokenIndex = 0;
  while(token != NULL && tokenIndex < 6)
  {
          tokens[tokenIndex++] = token;
          token = strtok(NULL,", ");
  }

  return create_state(tokens, tokenIndex);
}

/* TODO function documentation. Follow the format from previous assignments.*/
/* Function:     analyze_file
 * Purpose:      analyze the data in file and store them in states
 * In/out args:  states[], num of states
 */
void analyze_file(FILE* file, struct state_info* states[], int num_states)
{
    const int line_sz = 100;
    char line[line_sz];
    char *token;
    while (fgets(line, line_sz, file) != NULL)
    {
        struct state_info* state = get_state(line);

        if(state == NULL)
          continue;

        int state_id = state->state_id;

        if(states[state_id] == NULL){
          addSate(states, state, state_id);
        }else{
          addInfor(states, state, state_id);
        }

    }
}

/* Function:     addSate
 * Purpose:      add new state information to states.
 * In/out args:  states[], state, state_id
 */
void addSate(struct state_info* states[],struct state_info* state, int state_id){
  states[state_id] = (struct state_info*)calloc(1, sizeof(struct state_info));
  strcpy(states[state_id]->code,state->code);
  states[state_id]->state_id = state->state_id;
  states[state_id]->total_people = state->total_people;
  states[state_id]->total_income = state->total_income;
  states[state_id]->total_vehicles = state->total_vehicles;
  states[state_id]->total_rent = state->total_rent;
  states[state_id]->num_records = state->num_records;
  states[state_id]->rent_people = state->rent_people;
}

/* Function:     addInfor
 * Purpose:      update the date in state to states
 * In/out args:  states[], state, state_id
 */
void addInfor(struct state_info* states[],struct state_info* state, int state_id){
  states[state_id]->total_people += state->total_people;
  states[state_id]->total_income += state->total_income;
  states[state_id]->total_vehicles += state->total_vehicles;
  states[state_id]->total_rent += state->total_rent;
  states[state_id]->num_records += state->num_records;
  states[state_id]->aver_income = (float)states[state_id]->total_income / (float)states[state_id]->num_records;
  states[state_id]->rent_people += state->rent_people;
  if(states[state_id]->rent_people != 0){
    states[state_id]-> aver_rent = (float)states[state_id]->total_rent / (float)states[state_id]->rent_people;
  }
  states[state_id]->aver_vehicles_captita = (float)states[state_id]->total_vehicles / (float)states[state_id]->total_people;
  states[state_id]->aver_vehicles_household = (float)states[state_id]->total_vehicles / (float)states[state_id]->num_records;
}

/* Function:     print_report
 * Purpose:      print every information in states
 * In/out args:  states[], num_states
 */

void print_report(struct state_info *states[], int num_states) {
    printf("States found: ");
    for (int i = 0; i < num_states; ++i) {
        if (states[i] != NULL) {
            struct state_info *info = states[i];
            printf("%s ", info->code);
        }
    }
    printf("\n");

    /* Print out the summary for each state */
    for(int i = 0; i < num_states; i++){
      if(states[i] != NULL){
        print_state(states[i]);
       }
    }
}
/* Function:     state
 * Purpose:      print the detils in states
 * In/out args:  state
 */
void print_state(struct state_info *state){
  printf("-- State : %s --\n",state->code);
  printf("Number of Records: %lu\n", state->num_records);
  printf("Sample Population: %lu\n", state->total_people);
  printf("Average Income: %.2f\n", state->aver_income);
  printf("Average Rent: %.2f\n",state->aver_rent);
  printf("Average vehicles per household: %.2f\n", state->aver_vehicles_household);
  printf("Vehicles per capita: %.2f\n", state->aver_vehicles_captita);
}
