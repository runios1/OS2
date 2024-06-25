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
        sleep(20);
        printf("cd: %d\n", cd);
        if (channel_put(cd, 42) < 0)
        {
            printf("Failed to put data in channel\n");
        }

        // sleep(20);

        if (channel_put(cd, 43) < 0)
        {
            printf("Failed to put data in channel\n");
        } // Sleeps until cleared
        // Handle error

        // sleep(20);

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
        // sleep(20);
        if (channel_take(cd, &data) < 0)
        { // 42
            printf("Failed to take data from channel\n");
        }

        printf("Data:%d\n ", data);

        if (channel_take(cd, &data) < 0)
        {
            printf("Failed to take data from channel\n");
        } // 43
        // Handle error

        // sleep(10);

        printf("Data:%d\n ", data);
        if (channel_take(cd, &data) < 0)
        {
            printf("Failed to take data from channel\n");
        }

        printf("Data:%d\n ", data);
        wait(0); // Sleep until child destroys channel
        // Handle error
    }
    exit(0);
}

// Tested: WRWDRR, WRWRDR
// Testing:

// The problem happens when the channel is destroyed before the 2nd process takes