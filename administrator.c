#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
//#include "db.h"
#include "msgq.h"

message msgout;

int main(void)
{
    //message *msgout = malloc(sizeof(message));
    //inout *msgin = malloc(sizeof(inout));
    char option[20];
    char name[12];
    char department[12];
    strcpy(name, NULL);
    strcpy(department, NULL);
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
    strcpy(msgout.msg.name, NULL);
    strcpy(msgout.msg.department, NULL);
    msgout.msg.emp_num = 0;
    msgout.msg.salary = 0;

    printf("Option: ");
    fgets(option, 20, stdin);

    while (running)
    {
        if (strcmp(option, "Insert") == 0)
        {
            msgout.msg_type = 1;
            strcpy(msgout.msg.name, "name");
            strcpy(msgout.msg.department, "department");
            msgout.msg.emp_num = 123;
            msgout.msg.salary = 999;
            printf("YES");
        }
        else if (strcmp(option, "Check Name") == 0)
        {
            msgout.msg_type = 2;
            msgout.msg.emp_num = emp_num;
        }
        else if (strcmp(option, "Check Department") == 0)
        {
            msgout.msg_type = 3;
            msgout.msg.emp_num = emp_num;
        }
        else if (strcmp(option, "Check Salary") == 0)
        {
            msgout.msg_type = 4;
            msgout.msg.emp_num = emp_num;
        }
        else if (strcmp(option, "Check") == 0)
        {
            msgout.msg_type = 5;
            strcpy(msgout.msg.department, department);
        }
        else if (strcmp(option, "Delete") == 0)
        {
            msgout.msg_type = 6;
            msgout.msg.emp_num = emp_num;
        }

        if (msgsnd(msgid_ser, &msgout, sizeof(data), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }

        // if (msgrcv(msgid_cli, msgin, 100 * sizeof(char), msgin.msg_type, 0) == -1)
        // {
        //     perror("msgrcv");
        //     exit(1);
        // }
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
