#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include "msgq.h"

message msgout;
inout msgin;

int main(void)
{
    char option[20];
    char name[12];
    char department[12];
    // strcpy(name, NULL);
    // strcpy(department, NULL);
    unsigned int emp_num = 0;
    float salary = 0;
    int running = 1;

    int msgid_cli = msgget((key_t)CID, 0666 | IPC_CREAT);
    if (msgid_cli == -1)
    {
        fprintf(stderr, "msgget_snd failed with error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int msgid_ser = msgget((key_t)SID, 0666 | IPC_CREAT);
    if (msgid_ser == -1)
    {
        fprintf(stderr, "msgget_rec failed with error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    msgout.msg_type = 0;
    strcpy(msgout.msg.name, "NULL");
    strcpy(msgout.msg.department, "NULL");
    msgout.msg.emp_num = 0;
    msgout.msg.salary = 0;

    while (running)
    {
        printf("Option: ");
        fgets(option, 20, stdin);
        if (strcmp(option, "Insert\n") == 0)
        {
            msgout.msg_type = 1;
            strcpy(msgout.msg.name, "name");
            strcpy(msgout.msg.department, "department");
            msgout.msg.emp_num = 123;
            msgout.msg.salary = 999;
        }
        else if (strcmp(option, "Check Name\n") == 0)
        {
            msgout.msg_type = 2;
            msgout.msg.emp_num = 123;
        }
        else if (strcmp(option, "Check Department\n") == 0)
        {
            msgout.msg_type = 3;
            msgout.msg.emp_num = emp_num;
        }
        else if (strcmp(option, "Check Salary\n") == 0)
        {
            msgout.msg_type = 4;
            msgout.msg.emp_num = emp_num;
        }
        else if (strcmp(option, "Check Employee Number\n") == 0)
        {
            msgout.msg_type = 5;
            strcpy(msgout.msg.name, name);
        }
        else if (strcmp(option, "Check\n") == 0)
        {
            msgout.msg_type = 6;
            strcpy(msgout.msg.department, department);
        }
        else if (strcmp(option, "Delete\n") == 0)
        {
            msgout.msg_type = 7;
            msgout.msg.emp_num = emp_num;
        }
        else
        {
            msgout.msg_type = 8;
            running = 0;
        }

        printf("Sending request to server...\n");
        if (msgsnd(msgid_ser, (void *)&msgout, sizeof(data), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }

        printf("Waiting for server...\n");
        if (msgout.msg_type != 1 && msgout.msg_type != 6 && msgout.msg_type != 8)
        {
            if (msgrcv(msgid_cli, (void *)&msgin, sizeof(msgin.s), 0, 0) == -1)
            {
                perror("msgrcv");
                exit(1);
            }
            printf("%s\n", msgin.s);
        }
    }
    
    if (msgctl(msgid_cli, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    
    if (msgctl(msgid_ser, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
