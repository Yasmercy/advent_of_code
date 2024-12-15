#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "vector.h"

int getrow(int pos, int cols)
{
    return pos / (cols + 1);
}

int getcol(int pos, int cols)
{
    return pos % (cols + 1);
}

int getpos(int row, int col, int cols)
{
    return row * (cols + 1) + col;
}

void rotate_cw(int *dr, int *dc)
{
    int newdr = *dc;
    int newdc = -*dr;

    *dr = newdr;
    *dc = newdc;
}

int simulate(char *data, int rows, int cols)
{
    int curpos = strchr(data, '^') - data;
    int col = getcol(curpos, cols);
    int row = getrow(curpos, cols);

    int dr = -1;
    int dc = 0;

    do
    {
        char this = data[getpos(row, col, cols)];
        data[getpos(row, col, cols)] = 'X';

        if (data[getpos(row + dr, col + dc, cols)] == '#')
            rotate_cw(&dr, &dc);

        row += dr;
        col += dc;

    } while (0 <= row && row < rows && 0 <= col && col < cols);

    // count the number of 'X'
    int count = 0;
    while (*data != '\0')
        count += *(data++) == 'X';
    return count;
}

int main(void)
{
    int fd = open("data.in", O_RDWR, 0644);

    struct stat stat;
    fstat(fd, &stat);

    char buf[256] = {0};
    read(fd, buf, 255);

    size_t file_size = stat.st_size;
    size_t cols = strchr(buf, '\n') - buf;
    int rows = file_size / cols;

    char *data = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    printf("%d unique pos\n", simulate(data, rows, cols));

    munmap(data, file_size);
    close(fd);
}
