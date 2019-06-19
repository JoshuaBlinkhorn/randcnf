// Rand.c -- a tool for generating random formulas
// Joshua Blinkhorn 22.04.2019

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define FILENAME_LEN 128

#define MAX_FORMULAS 1000
#define MAX_VARS 100000000000
#define MAX_CLAUSES 100000000000
#define MAX_WIDTH 10000

typedef signed long literal_t;

int main(int argc, char** argv){
  // setup
  FILE* output;
  char filename[FILENAME_LEN];
  time_t t;
  unsigned int which_formula, which_clause, which_literal, which_literal_in_clause;
  literal_t current_literal; 
  literal_t* clause;
  int good_literal;

  // check the user input
  if (argc != 5)
    {
      // report correct usage when use input not correct
      fprintf(stderr, "FATAL error: Bad input.\n");
      fprintf(stderr, "usage: randcnf <num-formulas> <num-vars> <num-clauses> <clause-width>\n");      
      exit(1);
    }

  // read user input values
  const unsigned long int num_formulas = atoi(argv[1]);
  const unsigned long int num_vars = atoi(argv[2]);
  const unsigned long int num_clauses = atoi(argv[3]);
  const unsigned long int clause_width = atoi(argv[4]);

  // terminate if input too large
  if (num_formulas > MAX_FORMULAS ||
      num_vars > MAX_VARS ||
      num_clauses > MAX_CLAUSES ||
      clause_width > MAX_WIDTH)
    {
      fprintf(stderr, "Fatal error: Input out of bounds.\n");
      exit(1);
    }
  
  // terminate if width exceeds number of variables
  if (clause_width > num_vars)     {
      fprintf(stderr, "Fatal error: width exceeds number of variables.\n");
      exit(1);
    }
  
  //seed rand() with system timer
  srand((unsigned)time(&t));

  // allocate memory for clause
  if ((clause = (literal_t *)malloc(sizeof(literal_t) * clause_width)) == NULL)
    {
      fprintf(stderr, "Cannot allocate memory for clause. Aborting.\n");
      exit(1);
    }

  // outer loop is per formula
  for(which_formula = 0; which_formula < num_formulas; which_formula++)
    {
      sprintf(filename, "randomcnf-%u", which_formula + 1);
      
      //open file
      if ((output = (FILE *)fopen(filename,"w")) == NULL)
	{
	  fprintf(stderr, "Cannot open file %s. Aborting.\n", filename);
	  exit(1);
	}
      
      //write the header
      fprintf(output, "c random formula\n"); 
      fprintf(output, "p cnf %lu %lu\n", num_vars, num_clauses);
      
      // write the clauses
      // middle loop is per clause
      for(which_clause = 0; which_clause < num_clauses; which_clause++)
	{     
	  // inner loop is per literal
	  for(which_literal = 0; which_literal < clause_width; which_literal++)
	    {
	      // choose random literal
	      good_literal = 0;
	      while(good_literal == 0)
		{
		  // avoid repeated and complementary literals
		  current_literal = (current_literal = rand() % (2 * num_vars)) > num_vars - 1 ? num_vars - 1 - current_literal : 1 + current_literal;
		  good_literal = 1;
		  for (which_literal_in_clause = 0; which_literal_in_clause < which_literal; which_literal_in_clause++)
		    {
		    if (current_literal == clause[which_literal_in_clause] || current_literal == -(clause[which_literal_in_clause]))
		      {
			good_literal = 0;
		      }
		    }
		}
	      // write literal to clause
	      clause[which_literal] = current_literal;

	      fprintf(output, "%ld ", current_literal);
	    }
	  // line end
	  fprintf(output,"0\n");      
	}

      //close file
      fclose(output);
    }
  //free memory for clause
  clause = NULL;
  free(clause);

  return 0;
}
