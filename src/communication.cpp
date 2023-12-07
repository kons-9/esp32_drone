#include "util.hpp"

void communication_task(void *arg)
{
    while (true)
    {
        char command[128];
        scanf("%s", command);
        printf("Command: %s\n", command);
    }
}