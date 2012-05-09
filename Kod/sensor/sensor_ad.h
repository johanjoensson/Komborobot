#ifndef SENSOR_AD_H
#define SENSOR_AD_H

unsigned char count;
unsigned char line_array_1, line_array_2;
unsigned char level;
unsigned char dist_left_front, dist_left_back, dist_right_front;
unsigned char dist_right_back, dist_front, dist_left_short, dist_right_short;
void decide_header();

#endif

