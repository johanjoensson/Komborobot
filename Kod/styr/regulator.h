#ifndef REGULATOR_H
#define REGULATOR_H

void drive_engines(signed char value);
signed char distance_regulator(unsigned char left_front, unsigned char left_back,
                unsigned char right_front, unsigned char right_back);
signed char line_regulator(signed char new_value);

unsigned char wall;
void drive_engines_line(signed char value);

unsigned char wall;


#endif
