#include "user.h"

int main(int argc, char *argv[])
{
    int nCheckers = 3;
    if (argc > 0)
        nCheckers = atoi(argv[0]);

    int checkerCD = channel_create();
    int printerCD = channel_create();

    int generatorPID = fork();

    if (generatorPID == -1)
    {
        printf("Error in generator fork\n");
        exit(1);
    }
    else if (generatorPID == 0)
    { // Generator process
        int status = generatorLogic(checkerCD);
        exit(status);
    }

    for (int i = 0; i < nCheckers; i++)
    {
        int checkerPID = fork();
        if (checkerPID == -1)
        {
            printf("Error in checker %d fork\n", i);
            exit(1);
        }
        else if (checkerPID == 0)
        { // Checker process
            int status = checkerLogic(checkerCD, printerCD);
            exit(status);
        }
    }

    int printerPID = fork();
    if (printerPID == -1)
    {
        printf("Error in printer fork\n");
        exit(1);
    }
    else if (printerPID == 0)
    { // Printer process
        int status = printerLogic(printerCD);
        exit(status);
    }

    while (wait(0) > 0)
    {
    }

    return 0;
}

int checkPrime(int num)
{
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
            return 0;
    }

    return 1;
}

int checkerLogic(int checkerCD, int printerCD)
{
    int num = 0;
    while (1)
    {
        if (channel_take(checkerCD, &num) == -1)
        {
            printf("channel_take on checker chan failed\n");
            return -1;
        }
        if (num != 0 && checkPrime(num))
        {
            if (channel_put(printerCD, num) == -1)
            {
                channel_destroy(checkerCD);
                return 0;
            }
        }
    }
    return 0;
}

int generatorLogic(int checkerCD)
{
    int num = 2;
    while (1)
    {
        if (channel_put(checkerCD, num) == -1) // What if someone opens this channel again before we notice?
            return 0;
        num++;
    }
    return 0;
}

int printerLogic(int printerCD)
{
    int detected = 0;
    int num;
    while (detected < 100)
    {
        if (channel_take(printerCD, &num) == -1)
        {
            printf("channel_take on printer chan failed\n");
            return -1;
        }
        if (num != 0)
        {
            detected++;
            printf("%d\n", num);
        }
    }
    channel_destroy(printerCD);
    return 0;
}