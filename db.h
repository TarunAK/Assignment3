#ifndef DB_H
#define DB_H
typedef struct record {
    char name[12];
    char department[12];
    unsigned int emp_num;
    float salary;
    struct record *next;
} data;
#endif