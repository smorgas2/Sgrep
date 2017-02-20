#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "searcher.h"
#include "sgrep_data.h"

/*  A string (indata) is compared to search string saved in the sgd struct.
 *  Case sensitive determined by the user dictates if it should be
 *  taken into account or not.
 */
int search_string(char *indata, sgrep_data* sgd) {
  char *ptr;

  if(indata == NULL || sgd == NULL) {
    return SEARCHER_BAD_INDATA;
  }

  if(sgd->reg_exp == NULL) {
    return SEARCHER_BAD_INDATA;
  }

  if(sgd->case_sense == 1)
  {
    ptr = strcasestr(indata, sgd->reg_exp);
  }
  else
  {
    ptr = strstr(indata, sgd->reg_exp);
  }

  if(ptr != NULL)
  {
    return SEARCHER_OK_MATCHES;
  }

  return SEARCHER_OK_NO_MATCHES;
}

/*  Saves the all the text that match to the sgd struct.
 *  memory is allocated and reallocated dynamically for
 *  each match found.
 */
int search_file(sgrep_data *sgd)  {

    if(sgd == NULL || sgd->reg_exp ==  NULL)
    {
      return SEARCHER_BAD_INDATA;
    }

    sgd->lctext    = NULL;
    sgd->matches   = 0;
    int line       = 0;
    char *buf      = NULL;
    size_t bufsize = 0;

    /*  The while-loop reads the stream assigned to the struct sgd
     *  and saves the results to the struct line_count_text (lctext)
     *  existing inside the struct sgd.
     */
    while(getline(&buf, &bufsize, sgd->in) != -1)
    {
      line++;
      if(search_string(buf, sgd) == SEARCHER_OK_MATCHES)
      {
        /*  The struct lctext is realloc to one size bigger than
         *  before when a string match is detecded.
         */
        line_count_text **tmp = realloc(sgd->lctext,
           (sgd->matches +1) * sizeof(line_count_text*));

        if(tmp == NULL) {
          return 2;
        }

        sgd->lctext = tmp;

        /*  If the realloc is successful the lctext, space is allocated
         *  for the the text and the text is saved to the char pointer
         *  in the lctext struct. The line is also saved to the struct
         *  in case the line should be printed with the text.
         */
        sgd->lctext[sgd->matches] = malloc(sizeof(line_count_text));
        sgd->lctext[sgd->matches]->text = malloc(sizeof(char) *
         (strlen(buf) +1));
        strcpy(sgd->lctext[sgd->matches]->text, buf);

        sgd->lctext[sgd->matches]->line = line;

        sgd->matches++; //Increased by one for each match of string
      }
    }

    free(buf);

  // If no matches were found an errorcode is returned.
  if(sgd->matches == 0)
  {
    return SEARCHER_OK_NO_MATCHES;
  }

  return SEARCHER_OK_MATCHES;
}

/*  Checks what options the user has chosen and prints
 *  to stdout based on it. If the user has chosen to print the
 *  number of matches the loop is broken since it overides other
 *  options and only prints the number of hits (as in grep).
 */
int print_sgrep(sgrep_data *sgd) {

  if(sgd == NULL)
  {
    return SEARCHER_BAD_INDATA;
  }

  int i;

  for(i = 0; i < sgd->matches; i++) {
    if(sgd->line_count == 1)
    {
      printf("%d\n", sgd->matches);
      break;
    }
    else if(sgd->mode == 1)
    {
      printf ("%d:%s", sgd->lctext[i]->line, sgd->lctext[i]->text);
    }
    else
    {
      printf ("%s", sgd->lctext[i]->text);
    }
  }
  return 0;
}

/*  If a file is used it is closed.
 *  All int variables are set to zero.
 *  The struct lctext is deallocated.
 */
int free_data(sgrep_data *sgd)
{
  if(sgd == NULL)
  {
    return SEARCHER_BAD_INDATA;
  }

  int i;

  if(sgd->in != stdin)
  {
    fclose(sgd->in);
  }

  for(i = 0; i < sgd->matches; i++)
  {
    free(sgd->lctext[i]->text);
    free(sgd->lctext[i]);
  }
  free(sgd->lctext);

  sgd->reg_exp    = 0;
  sgd->in         = 0;
  sgd->matches    = 0;
  sgd->case_sense = 0;
  sgd->mode       = 0;

  return 0;
}
