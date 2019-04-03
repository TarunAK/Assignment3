#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <math.h>
#include "msgq.h"
#define SID 5678
#define CID 8765

data *head = NULL;
data *end = NULL;
static int count = 0;


void Insert(char name[12], char department[12], int emp_num, int salary)
{
    data *curr = head;
    while (curr != NULL)
    {
        if (curr->emp_num == emp_num)
            return;
        curr = curr->next;
    }
    free(curr);

    data *new = malloc(sizeof(data));
    if (new == NULL)
    {
        printf("malloc failed\n");
        exit(-1);
    }

    strcpy(new->name, name);
    strcpy(new->department, department);

    if ((floor(log10(abs(emp_num))) + 1) < 13)
        new->emp_num = emp_num;
    else
        new->emp_num = 0;

    if ((floor(log10(abs(emp_num))) + 1) < 13)
        new->salary = salary;
    else
        new->salary = 0;

    new->next = NULL;
    ++count;

    if (head == NULL)
    {
        head = new;
        end = new;
        return;
    }

    end->next = new;
    end = new;
}

char *Check_name(int emp_num)
{
    data *curr = head;
    while (curr != NULL)
    {
        if (curr->emp_num == emp_num)
            return curr->name;
        curr = curr->next;
    }
    free(curr);
    return NULL;
}

char *Check_department(int emp_num)
{
    data *curr = head;
    while (curr != NULL)
    {
        if (curr->emp_num == emp_num)
            return curr->department;
        curr = curr->next;
    }
    free(curr);
    return NULL;
}

int Check_salary(int emp_num)
{
    data *curr = head;
    while (curr != NULL)
    {
        if (curr->emp_num == emp_num)
            return curr->salary;
        curr = curr->next;
    }
    free(curr);
    return -1;
}

int Check_employee_number(char name[12])
{
    data *curr = head;
    while (curr != NULL)
    {
        if (strcmp(curr->name, name) == 0)
            return curr->emp_num;
        curr = curr->next;
    }
    free(curr);
    return -1;
}

int *Check(char department[12])
{
    data *curr = head;
    printf("%d\n", count);
    int *tmp;
    tmp = malloc(count * sizeof(int));
    //tmp = calloc(count, sizeof(int));
    printf("%d\n", count);
    //printf("%d", tmp);
    printf("sizeof of tmp: %lu\n", sizeof(tmp));
    //int *new = NULL;
    int i = 0;
    while (curr != NULL)
    {
        if (strcmp(curr->department, department) == 0)
        {
            tmp[i] = curr->emp_num;
            i++;
        }
        curr = curr->next;
    }
    printf("%d\n", i);
    //tmp = realloc(tmp, i * sizeof(int));
    //printf("%lu\n", sizeof(tmp) / sizeof(tmp[0]));
    return tmp;
}

int Delete(int emp_num)
{

    data *curr = head;
    data *prev = NULL;

    //if list is empty
    if (head == NULL)
        return -1;

    // navigate through list
    while (curr->emp_num != emp_num)
    {
        //if it is last node
        if (curr->next == NULL)
        {
            return -1;
        }
        else
        {
            //store reference to current link
            prev = curr;
            //move to next link
            curr = curr->next;
        }
    }

    --count;
    //found a match, update the link
    if (curr == head)
    {
        //change first to point to next link
        head = head->next;
    }
    else
    {
        //bypass the current link
        prev->next = curr->next;
        //return -1;
    }
    return 0;
}

void print_list(void)
{
    data *curr = head;
    while (curr != NULL)
    {
        printf("%s  %s  %d  %f\n", curr->name, curr->department, curr->emp_num, curr->salary);
        curr = curr->next;
    }
    free(curr);
}

message msgout;
inout msgin;

int main(void)
{
    //data *head = NULL;
    int running = 1;

    // msgout.msg_type = 0;
    // strcpy(msgout.msg.name, "NULL");
    // strcpy(msgout.msg.department, "NULL");
    // msgout.msg.emp_num = 0;
    // msgout.msg.salary = 0;

    // Insert("Joe", "A", 935, 1231);
    // Insert("Abby", "A", 345, 8768);
    // Insert("Dan", "A", 867, 4569);
    // Insert("Joeeeeyyyyy", "A", 455, 1222531);

    // print_list();
    // printf("%d\n", count);
    // printf("\n");
    // //printf("%d\n", Delete(3451));
    // print_list();
    // printf("%d\n", Check_employee_number("Dan"));
    // printf("%d\n", count);
    

    // int *checked = Check("A");
    // //int len = 12;
    // //int len = sizeof(checked) / sizeof(checked[0]);
    // for (int j = 0; j < 5; j++)
    // {
    //     printf("%d ", checked[j]);
    // }
    // printf("\n");
    // free(checked);

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

    while (running)
    {
        char temp[12];

        printf("Waiting for client...\n");
        if (msgrcv(msgid_ser, (void *)&msgout, sizeof(data), 0, 0) == -1)
        {
            perror("msgrcv");
            exit(1);
        }

        msgin.msg_type = msgout.msg_type;

        switch (msgout.msg_type)
        {
            case 1:
                Insert(msgout.msg.name, msgout.msg.department, msgout.msg.emp_num, msgout.msg.salary);
                break;
            case 2:
                //printf("hallo\n");
                
                strcpy(msgin.s, Check_name(msgout.msg.emp_num));
                //printf("\n%s\n", msgin.s);
                break;
            case 3:
                strcpy(msgin.s, Check_department(msgout.msg.emp_num));
                break;
            case 4:
                gcvt(Check_salary(msgout.msg.emp_num), 12, temp);
                strcpy(msgin.s, temp);
                break;
            case 5:
                gcvt(Check_employee_number(msgout.msg.name), 12, temp);
                strcpy(msgin.s, temp);
                break;
            case 6:
                //int *tmp = Check(msgin->msg.department);
                break;
            case 7:
                gcvt(Delete(msgout.msg.emp_num), 12, temp);
                strcpy(msgin.s, temp);
                break;
            default:
                running = 0;
                break;
        }

        if (msgout.msg_type != 1 && msgout.msg_type != 6 && msgout.msg_type != 8 && count != 0)
        {
            printf("Responding to client...\n");
            if (msgsnd(msgid_cli, (void *)&msgin, sizeof(msgin.s), 0) == -1)
            {
                perror("msgsnd");
                exit(1);
            }
        } 
    }

    if (!running)
    {
        printf("Exiting\n");
        exit(EXIT_SUCCESS);
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