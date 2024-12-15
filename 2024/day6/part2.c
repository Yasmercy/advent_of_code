#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "vector.h"
static int mask[] = {1, 16, 64, 128};

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

void rotate_cw(int *dr, int *dc, int *dir)
{
    int newdr = *dc;
    int newdc = -*dr;

    *dr = newdr;
    *dc = newdc;
    *dir = (*dir + 1) & 0b11;
}

int detect_cycle(char *data, int rows, int cols, int row, int col, int dr, int dc, int dir)
{
    do
    {
        if (data[getpos(row, col, cols)] & mask[dir])
            return 1;

        data[getpos(row, col, cols)] |= mask[dir];
        if (data[getpos(row + dr, col + dc, cols)] == '#')
            rotate_cw(&dr, &dc, &dir);

        row += dr;
        col += dc;

    } while (0 <= row && row < rows && 0 <= col && col < cols);

    return 0;
}

int simulate(char *data, int rows, int cols)
{
    int curpos = strchr(data, '^') - data;
    int col = getcol(curpos, cols);
    int row = getrow(curpos, cols);

    int dir = 0;
    int dr = -1;
    int dc = 0;

    data[curpos] = '.';
    int count = 0;
    char *datacpy = malloc(rows * (cols + 1));

    do
    {
        char this = data[getpos(row, col, cols)];

        assert(!(this & mask[dir]));
        data[getpos(row, col, cols)] |= mask[dir];

        if (data[getpos(row + dr, col + dc, cols)] == '#')
            rotate_cw(&dr, &dc, &dir);
        else if (data[getpos(row + dr, col + dc, cols)] == '.')
        {
            // check if adding an obstacle leads in front to a cycle
            int newdr = dr;
            int newdc = dc;
            int newdir = dir;
            rotate_cw(&newdr, &newdc, &newdir);

            int newrow = row + newdr;
            int newcol = col + newdc;

            memcpy(datacpy, data, rows * (cols + 1));
            if (detect_cycle(datacpy, rows, cols, newrow, newcol, newdr, newdc, newdir))
            {
                printf("(%d,%d)\n", row + dr, col + dc);
                ++count;
            }
        }

        row += dr;
        col += dc;

    } while (0 <= row && row < rows && 0 <= col && col < cols);

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

    simulate(data, rows, cols);
    // printf("%d unique pos\n", simulate(data, rows, cols));

    munmap(data, file_size);
    close(fd);
}
