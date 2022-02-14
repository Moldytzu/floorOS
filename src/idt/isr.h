#pragma once

#include "../common/common.h"
#include "../boot/videoDriver.h"

void dummy_isr();


__attribute__ ((interrupt))
void dummy_isr_gp(void* dummy, uint64_t error);


