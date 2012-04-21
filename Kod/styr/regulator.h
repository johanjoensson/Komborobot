#ifndef REGULATOR2_H
#define REGULATOR2_H

void drive_engines(signed char value);
signed char distance_regulator(unsigned char left, unsigned char right);
signed char line_regulator(unsigned char new_value);

#endif
