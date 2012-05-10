#ifndef MOTOR_STYRNING_H
#define MOTOR_STYRNING_H
/*
int speed_right;
int speed_left;
*/
void init();
void stop(unsigned char speed);
void forward(unsigned char speed);
void forward_left(unsigned char speed);
void forward_right(unsigned char speed);
void rotate_left(unsigned char speed);
void rotate_right(unsigned char speed);
void back(unsigned char speed);
void trim_left();
void trim_right();
void trim_zero();
void set_speed_left(unsigned char speed);
void set_speed_right(unsigned char speed);
void set_speed(unsigned char speed);
#endif
