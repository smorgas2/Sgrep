#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "sgrep_data.h"

/*  Assigns the variables in the struct sgd based on user input.
 *  The control flow is rather straight forward except for when
 *  there is one loop left after the current one. When the text
 *  is not an option the current loops value is set as the
 *  search string and the last value is set as a file that
 *  should be read. The loop is then ended by icrementing the
 *  control variable before the next loop is started.
 */
int parse(int argc, char **argv, sgrep_data *data) {

  data->reg_exp = NULL;

  if ( (argc<1) || (argc>5) || (data==NULL) )
  {
    return PARSE_BAD_INDATA;
  }

  int i;
  for(i = 0; i < argc; i++)
  {

    if (strncmp(argv[i], "-i", strlen("-i"))==0)
    {
      data->case_sense = 1;
    }
    else if(strncmp(argv[i], "-c", strlen("-c"))==0)
    {
      data->line_count = 1;
    }
    else if(strncmp(argv[i], "-n", strlen("-n"))==0)
    {
      data->mode = 1;
    }
    else if(i == (argc-2))
    {
      data->reg_exp = argv[i];
      data->in = fopen(argv[i+1], "r");
      if(data->in == NULL)
      {
        return PARSE_BAD_INDATA;
      }
      ++i;
    }
    else if(i == (argc-1))
    {
      data->reg_exp = argv[i];
      data->in = stdin;
    }
    else
    {
      return PARSE_BAD_INDATA;
    }
  }

  if(data->reg_exp == NULL)
  {
    return PARSE_BAD_INDATA;
  }

  return PARSE_OK;
}
