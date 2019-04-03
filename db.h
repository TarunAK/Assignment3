#ifndef DB_H
#define DB_H
typedef struct record {
    char *name;
    char *department;
    unsigned int emp_num;
    float salary;
    struct record *next;
} data;
#endif