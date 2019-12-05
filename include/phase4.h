// Add Guard to header file
#ifndef __PHASE_FOUR_H
#define __PHASE_FOUR_H

#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define READ_FLAGS O_RDONLY

int redirect(int mapperCount);
#endif