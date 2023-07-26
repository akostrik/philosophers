#include <sys/time.h>
#include <stdio.h>

int main(void)
{
    struct timeval time;

    if (gettimeofday(&time, NULL) != 0)
        return (printf("some problems\n"), -1);
    printf("Hello world: %ld and %ld\n", time.tv_sec, time.tv_usec);
    printf("from sec: int %d\n", (int)(time.tv_sec * 1000));
    printf("from sec: long %ld\n", time.tv_sec * 1000);
    printf("from usec: int %d\n", (int)(time.tv_usec / 1000));
    printf("from usec: long %ld\n", time.tv_usec / 1000);
    return (0);
}