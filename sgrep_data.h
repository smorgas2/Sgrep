#ifndef SGREP_DATA_H
#define SGREP_DATA_H

//linecount and text stuct
typedef struct line_count_text_ {
  char *text;
  int   line;
} line_count_text;

typedef struct sgrep_data_ {
  char *reg_exp;   //text to search for
  FILE *in;        //stream to read from
  int matches;     //number of matches found
  int case_sense;  //specifies if case sensitive or not
  int mode;        //specifies if we shall print the number of hits or not
  int line_count;  //specifies if we shall print the row count
  line_count_text **lctext; //‘array’ of lctexts matching the pattern.
} sgrep_data;

#endif /* SGREP_DATA_H */
