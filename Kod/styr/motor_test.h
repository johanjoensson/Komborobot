#ifndef MOTOR_TEST_H
#define MOTOR_TEST_H

void init();
void stop(unsigned char speed);
void forward(unsigned char speed);
void forward_left(unsigned char speed);
void forward_right(unsigned char speed);
void rotate_left(unsigned char speed);
void rotate_right(unsigned char speed);
void back(unsigned char speed);

#endif
