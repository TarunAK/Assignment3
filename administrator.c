#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include "msgq.h"

int main(void)
{
    message msgout; // message to send to server
    inout msgin; // message to receive from server
    int option; // options for each operation on array
    int running = 1; // for client to run infinitely

    int msgid_cli = msgget((key_t)CID, 0666 | IPC_CREAT); // creating client message queue here
    if (msgid_cli == -1)
    {
        fprintf(stderr, "msgget_snd failed with error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int msgid_ser = msgget((key_t)SID, 0666 | IPC_CREAT); // creating server message queue here
    if (msgid_ser == -1)
    {
        fprintf(stderr, "msgget_rec failed with error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // initializations
    msgout.msg_type = 0;
    strcpy(msgout.msg.name, "NULL");
    strcpy(msgout.msg.department, "NULL");
    msgout.msg.emp_num = 0;
    msgout.msg.salary = 0;

    // client runs
    while (running)
    {
        printf("(1) - Insert | (2) - Check_name | (3) - Check_department | (4) - Check_salary | (5) Check_employee_number | (6) - Check | (7) - Delete | (8) - Exit\n> ");
        scanf("%d", &option); // get user input
        if (option == 1) // for Insert()
        {
            msgout.msg_type = 1;
            printf("Name: ");
            scanf("%s", msgout.msg.name);
            printf("Department: ");
            scanf("%s", msgout.msg.department);
            printf("Employee Number: ");
            scanf("%d", &msgout.msg.emp_num);
            printf("Salary: ");
            scanf("%f", &msgout.msg.salary);
        }
        else if (option == 2) // for Check_name()
        {
            msgout.msg_type = 2;
            printf("Employee Number: ");
            scanf("%d", &msgout.msg.emp_num);
        }
        else if (option == 3) // for Check_department()
        {
            msgout.msg_type = 3;
            printf("Employee Number: ");
            scanf("%d", &msgout.msg.emp_num);
        }
        else if (option == 4) // for Check_salary()
        {
            msgout.msg_type = 4;
            printf("Employee Number: ");
            scanf("%d", &msgout.msg.emp_num);
        }
        else if (option == 5) // for Check_employee_number()
        {
            msgout.msg_type = 5;
            scanf("%s", msgout.msg.name);
        }
        else if (option == 6) // for Check()
        {
            msgout.msg_type = 6;
            scanf("%s", msgout.msg.department);
        }
        else if (option == 7) // for Delete()
        {
            msgout.msg_type = 7;
            printf("Employee Number: ");
            scanf("%d", &msgout.msg.emp_num);
        }
        else // for ending program
        {
            msgout.msg_type = 8;
            running = 0;
        }

        //printf("Sending request to server...\n");
        if (msgsnd(msgid_ser, (void *)&msgout, sizeof(data), 0) == -1) // sending to server
        {
            perror("msgsnd");
            exit(1);
        }

        //printf("Waiting for server...\n");
        if (msgout.msg_type != 1 && msgout.msg_type != 6 && msgout.msg_type != 8)
        {
            if (msgrcv(msgid_cli, (void *)&msgin, sizeof(msgin.s), 0, 0) == -1) // receiving from server
            {
                perror("msgrcv");
                exit(1);
            }
            printf("%s\n", msgin.s);
        }
    }
    
    if (msgctl(msgid_cli, IPC_RMID, 0) == -1) // deleting client message queue
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    
    if (msgctl(msgid_ser, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n"); // deleting server message queue
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
