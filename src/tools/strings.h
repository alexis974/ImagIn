#ifndef STRINGS_H
#define STRINGS_H

const char *get_filename_ext(const char *filename);

const char *get_filename_from_path(const char *filename);

void format_datetime(char* input, char* output_date, char* output_time);

#endif /* ! STRINGS_H */
