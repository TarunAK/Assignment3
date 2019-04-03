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
static int count = 0; // number of employees
static int length = 0; // number of employees in given department (for Check())


/**
 * Adds employee to linked list
 */
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

/*
 * Returns name of employee given employee number
 */
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

/*
 * Returns department of employee given employee number
 */
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

/*
 * Returns salary of employee given employee number
 */
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

/*
 * Returns name of employee number given name
 */
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

/*
 * Returns array of employee numbers given department
 */
int *Check(char department[12])
{
    data *curr = head;
    int *tmp = malloc(count * sizeof(int));
    printf("sizeof of tmp: %lu\n", sizeof(tmp));
    while (curr != NULL)
    {
        if (strcmp(curr->department, department) == 0)
        {
            tmp[length] = curr->emp_num;
            length++;
        }
        curr = curr->next;
    }
    //tmp = realloc(tmp, i * sizeof(int));
    //printf("%lu\n", sizeof(tmp) / sizeof(tmp[0]));
    return tmp;
}

/**
 * Deletes employee from linked list
 */
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

/**
 * Prints out current linked list
 */
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

int main(void)
{
    message msgout; // message to receive from client
    inout msgin; // message to send to client
    int running = 1; // for server to run infinitely
    int *checked = malloc(length * sizeof(int)); // to store array returned from Check()
    char *str[12]; 

    // Insert("Joe", "A", 935, 1231);
    // Insert("Abby", "A", 345, 8768);
    // Insert("Dan", "A", 867, 4569);
    // Insert("Joeeeeyyyyy", "C", 455, 1222531);

    // print_list();
    // printf("%d\n", count);
    // printf("\n");
    // //printf("%d\n", Delete(3451));
    // print_list();
    // printf("%d\n", Check_employee_number("Dan"));
    // printf("%d\n", count);

    // int *checked = Check("A");
    // for (int j = 0; j < length; j++)
    // {
    //     printf("%d ", checked[j]);
    // }
    // printf("\n");
    // free(checked);

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

    // server runs
    while (running)
    {
        char temp[12];

        //printf("Waiting for client...\n");
        if (msgrcv(msgid_ser, (void *)&msgout, sizeof(data), 0, 0) == -1) // receiving from client
        {
            perror("msgrcv");
            exit(1);
        }

        msgin.msg_type = msgout.msg_type; // ensuring message reaches client when sent

        // calls corresponding function based on message type from message queue element
        // see administrator.c for what each case represents
        switch (msgout.msg_type)
        {
            case 1:
                Insert(msgout.msg.name, msgout.msg.department, msgout.msg.emp_num, msgout.msg.salary);
                break;
            case 2:
                strcpy(msgin.s, Check_name(msgout.msg.emp_num));
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
                checked = Check(msgout.msg.department);
                for (int i = 0; i < 12; ++i)
                {
                    char c[sizeof(int)];
                    snprintf(c, sizeof(int), "%d", checked[i]);
                    str[i] = malloc(sizeof(c));
                    strcpy(str[i], c);
                }
                strcpy(msgin.s, (const char*)str);
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

        // clears console and shows all employees after every update
        system("clear");
        print_list();

        if (msgout.msg_type != 1 && msgout.msg_type != 6 && msgout.msg_type != 8)
        {
            //printf("Responding to client...\n");
            if (msgsnd(msgid_cli, (void *)&msgin, sizeof(msgin.s), 0) == -1) // sending to client
            {
                perror("msgsnd");
                exit(1);
            }
        } 
    }

    if (!running) // exits program if not running
    {
        printf("Exiting\n");
        exit(EXIT_SUCCESS);
    }

    if (msgctl(msgid_cli, IPC_RMID, 0) == -1) // deleting client message queue
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    if (msgctl(msgid_ser, IPC_RMID, 0) == -1) // deleting server message queue
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}