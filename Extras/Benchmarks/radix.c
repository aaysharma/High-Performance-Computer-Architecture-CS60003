#define TOTAL_NUMBERS 1000

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/time.h>

void insertion_sort(int *array, int offset, int end) {
    int x, y, temp;
    for (x=offset; x<end; ++x) {
        for (y=x; y>offset && array[y-1]>array[y]; y--) {
            temp = array[y];
            array[y] = array[y-1];
            array[y-1] = temp;
        }
    }
}

void radix_sort(int *array, int offset, int end, int shift) 
{
    int x, y, value, temp;
    int last[256] = { 0 }, pointer[256];

    for (x=offset; x<end; ++x) {
        ++last[(array[x] >> shift) & 0xFF];
    }

    last[0] += offset;
    pointer[0] = offset;
    for (x=1; x<256; ++x) {
        pointer[x] = last[x-1];
        last[x] += last[x-1];
    }

    for (x=0; x<256; ++x) {
        while (pointer[x] != last[x]) {
            value = array[pointer[x]];
            y = (value >> shift) & 0xFF;
            while (x != y) {
                temp = array[pointer[y]];
                array[pointer[y]++] = value;
                value = temp;
                y = (value >> shift) & 0xFF;
            }
            array[pointer[x]++] = value;
        }
    }

    if (shift > 0) {
        shift -= 8;
        for (x=0; x<256; ++x) {
            temp = x > 0 ? pointer[x] - pointer[x-1] : pointer[0] - offset;
            if (temp > 64) {
                radix_sort(array, pointer[x] - temp, pointer[x], shift);
            } else if (temp > 1) {
                // std::sort(array + (pointer[x] - temp), array + pointer[x]);
                insertion_sort(array, pointer[x] - temp, pointer[x]);
            }
        }
    }
}

int intcmp(const void *aa, const void *bb)
{
    const int *a = (int *)aa, *b = (int *)bb;
    return (*a < *b) ? -1 : (*a > *b);
}

int main() 
{
    if (sizeof(int) != 4) 
    {
        return 1111;
    }

    int N = TOTAL_NUMBERS;

    int *array = (int *)malloc(sizeof(int) * N);

    for (int i=0; i<3; ++i) 
    {
        int n = N;
        srand(1);
        while (n --> 0) {
            array[n] = rand();
        }

        struct timeval start, end;
        time_t mtime, seconds, useconds;   

        gettimeofday(&start, NULL);
        
        radix_sort(array, 0, N, 24);

        gettimeofday(&end, NULL);

        n = N - 1;
        while (n --> 0) 
        {
            if (array[n] > array[n+1]) 
            {
                printf("Not sorted.....\n");
                return -1;
            }
        }

        for(int i =0; i < N;i++)
        {
            printf("%d ",array[i]);
        }

        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = seconds * 1000000 + useconds;
    }

    return 0;
}
