#pragma once


int read_count();

int write_count(unsigned short count);

void init_pit();

void timer_callback();