#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int cd = channel_create();
    if (cd < 0)
    {
        printf("Failed to create channel\n");
        exit(1);
    }
    if (fork() == 0)
    {
        printf("4");
        if (channel_put(cd, 42) < 0)
        {
            printf("Failed to put data in channel\n");
            exit(1);
        }

        sleep(10);

        printf("5");
        if (channel_put(cd, 43) < 0)
        {
            printf("Failed to put data in channel\n");
            exit(1);
        } // Sleeps until cleared
        // Handle error
        if (channel_destroy(cd) < 0)
        {
            printf("Failed to destroy channel\n");
            exit(1);
        }
        printf("Destroyed\n");

        // Handle error
    }
    else
    {
        int data;
        sleep(10);
        printf("1a");
        if (channel_take(cd, &data) < 0)
        { // 42
            printf("Failed to take data from channel\n");
            exit(1);
        }

        printf("Data:%d\n ", data);

        if (channel_take(cd, &data))
        {
            printf("Failed to take data from channel\n");
            exit(1);
        } // 43
        // Handle error

        printf("Data:%d\n ", data);
        if (channel_take(cd, &data))
        {
            printf("Failed to take data from channel\n");
            exit(1);
        }

        printf("Data:%d\n ", data);
        wait(0); // Sleep until child destroys channel
        // Handle error
    }
    exit(0);
}