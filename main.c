/*  Assignment in the course TIG167
 *  this version was made by: Markus Lidrot
 */

#include <stdio.h>

#include "parser.h"
#include "searcher.h"

int main(int argc, char **argv) {
  sgrep_data sgd;
  int ret;

  /*  parse is called to check and save the user input
   *  if entered ok.
   */
  ret = parse(argc-1, ++argv, &sgd);
  if(ret == PARSE_BAD_INDATA)
  {
    return 2;
  }

  /*  search is called to check the stream and save matches
   *  to the sgd.
   */
  ret = search_file(&sgd);
  if(ret != SEARCHER_OK_MATCHES)
  {
    return 1;
  }

  // Side note:
  // Might have been better to include the two functions below
  // in a new source file if one was to use these outside of
  // the assignment for better consitensy within each source file.

  /*  search is called to check the stream and save matches
   *  to the sgd.
   */
  ret = print_sgrep(&sgd);
  if(ret == SEARCHER_BAD_INDATA)
  {
    return 1;
  }

  /* Frees the memory allocated to a sgrep_data struct*/
  ret = free_data(&sgd);
  if(ret == SEARCHER_BAD_INDATA)
  {
    return 1;
  }

   return 0;
}
