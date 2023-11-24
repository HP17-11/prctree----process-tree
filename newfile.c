#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    // Root process
    pid_t root_pid = getpid();

    printf("PID = %d (Root Process)\n", root_pid);

    // First level of child processes
    pid_t child_pid1 = fork();
    if (child_pid1 == 0)
    {
        // Child process 1
        pid_t child_pid2 = fork();
        if (child_pid2 == 0)
        {

            // Infinite loop in child process 2
            while (1)
            {
                // Child process 2
                printf("Grand Child process PID = %d => PPID = %d\n", getpid(), getppid());
                sleep(2);
                // Your code here
            }
        }
        else
        {
            pid_t child_pid2 = fork();
            if (child_pid2 == 0)
            {
                // Infinite loop in child process 2
                while (1)
                {
                    // Child process 2
                    printf("Grand Child 2 process PID = %d => PPID = %d\n", getpid(), getppid());
                    sleep(2);
                    // Your code here
                }
            }
            else
            {
                while (1)
                {
                    // Child process 1
                    printf("Child process 1 PID = %d => PPID = %d\n", getpid(), getppid());
                    sleep(2);
                    // Your code here
                }
            }
        }
    }
    else
    {
        // Parent process of child process 1
        pid_t child_pid3 = fork();
        if (child_pid3 == 0)
        {
            pid_t child_pid4 = fork();
            if (child_pid4 == 0)
            {

                // Infinite loop in child process 3
                while (1)
                {
                    // Child process 3
                    printf("Child process 4 PID = %d => PPID = %d\n", getpid(), getppid());
                    sleep(2);
                    // Your code here
                }
            }
            else
            {
                // Infinite loop in child process 3
                // while (1)
                // {
                //     // Child process 3
                //     printf("Child process 2 PID = %d => PPID = %d\n", getpid(), getppid());
                //     sleep(2);
                //     // Your code here
                // }
                exit(0);
            }
        }
        else
        {
            while (1)
            {
                // Child process 1
                printf("Parent process PID = %d => PPID = %d\n", getpid(), getppid());
                sleep(2);
                // Your code here
            }
        }

        wait(NULL); // Wait for child_pid1 to complete
    }

    return 0;
}