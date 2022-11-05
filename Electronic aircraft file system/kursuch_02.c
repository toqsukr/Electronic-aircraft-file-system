#define MAXLEN 128
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Airplane
{
    int id;                 /*number on the list*/
    char *name;
    struct Country *manuf;  /*manufacturer*/
    unsigned int year;      /*the first flight*/
    unsigned int range;     /*range of flight in kilometers*/
    float weight;           /*weight empty in tons*/
    float price;            /*in millions of dollars*/
    float options[3];       /*length, height, wingspan in meters*/
    struct Airplane *next;
    struct Airplane *prev;
};

struct Country
{
    int cnt;
    int id;
    char* str;
    struct Country* next;
};

struct Airplane_Head
{
    int cnt;
    int last_id;
    struct Airplane *first;
    struct Airplane *last;
};

typedef struct Airplane    plane;
typedef struct Airplane_Head    headP;
typedef struct Country    country;


char *create_path();
country *create_singly_connected_ring_list(char* path);
headP *create_doubly_linked_list(country *node1, char* path);
headP* make_sort(headP *node0);
void searching_information();
country *add_information(headP *node0, country *node1);
country *adjust_information(headP *node0, country *node1);
country *delete_information(headP *node0, country *node1);
int find_last_id(headP *node0);
void delete_elem_of_doubly_linked_list(headP *node0, plane *p);
country *add_elem_of_ring(country *node1, char* str);
country *delete_elem_of_ring(country *node1, country *p);
headP* make_head();
plane *struct_fill(char **mass, country *node1, int n);
void add_first(headP* my_head, plane* new_node);
void add_last(headP* my_head, plane* new_node);
void save_information(headP *node0);
char **split_string(char *string, int length, char sep);
int catch_exception(int a, int b);
int is_str_in_list(headP *node0, char* str);
int is_in_list(headP *node0,int mdf);
int is_in_ring(country *node1, char* str);
void clear_strings(char **str, int n);
void print_main_menu();
void print_input();
void print_output_menu();
void print_change();
void print_adjust();
void print_searching();
void print_adjust_field();
void print_sort_param();
void print_sort_field();
void print_delete();
void print_chose();
void print_header();
void struct_out(plane *str0);
int check_strings_for_identity(char* string, char* substr);
int comparison_strings(char* string, char* substr);
void clear_list(headP *node0);
void clear_ring(country *node1);
void clear_structure(plane *p);
void output_simply_connected_ring_list(country *node1);
void output_doubly_linked_list(headP *node0);
void output_reverse_doubly_linked_list(headP *node0);
void clear();


int main()
{
    int mdf;
    headP *node0 = NULL;
    country *node1 = NULL;
    char* path = NULL;
    mdf = 0;
    printf("Select modification: ");
    path = "kursuch_02_data.csv";
    clear();
    node1 = create_singly_connected_ring_list(path);
    print_main_menu();
    node0 = create_doubly_linked_list(node1, path);
    printf("Select modification: ");
    mdf = catch_exception(0, 5);
    puts("");
    clear();
    while(mdf != 5)
    {
        if(mdf == 0)            /*reference*/
        {
            print_reference();
        }
        else if(mdf == 1)       /*enter input file*/
        {
            if(node0 != NULL)   clear_list(node0);
            if(node1 != NULL)   clear_ring(node1);
            print_input();
            path = create_path();
            node1 = create_singly_connected_ring_list(path);
            node0 = create_doubly_linked_list(node1, path);
            puts("Information is read successfully");
        }
        else if(mdf == 2)       /*output*/
        {
            if(node0->cnt == 0)     puts("The list is empty!\n\nThe ring is empty!");
            else
            {
                print_output_menu();
                printf("Select modification: ");
                mdf = catch_exception(0, 2);

                if(mdf == 0)    {output_doubly_linked_list(node0);   puts("");   output_simply_connected_ring_list(node1);}
                else if(mdf == 1)   output_simply_connected_ring_list(node1);
                else    searching_information(node0);
            }
        }
        else if(mdf == 3)       /*change information*/
        {
            print_change();
            printf("Select modification: ");
            mdf = catch_exception(0, 3);
            puts("");
            if(mdf == 0)
            {
                node1 = add_information(node0, node1);

            }
            else if(mdf == 1)
            {
                if(node0->cnt == 0)     puts("The list is empty!");
                else
                {
                    node1 = adjust_information(node0, node1);
                }
            }
            else if(mdf == 2)
            {
                if(node0->cnt == 0)     puts("The list is empty!");
                else if(node0->cnt == 1)    output_doubly_linked_list(node0);
                else    node0 = make_sort(node0);
            }
            else
            {
                if(node0->cnt == 0)     puts("The list is empty!");
                else    node1 = delete_information(node0, node1);
            }
        }
        else if(mdf == 4)      /*save information*/
        {
            save_information(node0);
        }
        puts("");
        print_chose();
        mdf = catch_exception(0, 1);
        puts("");
        clear();
        if(mdf == 1)    mdf = 5;
        else
        {
            clear();
            print_main_menu();
            printf("Select modification: ");
            mdf = catch_exception(0, 5);
        }

        clear();
    }
    if(node0 != NULL)   clear_list(node0);
    if(node1 != NULL)   clear_ring(node1);
    puts("The program is closed!");
    return 0;
}

/*
====================
Function create_path
====================
Forming path to datafile
return path - a string storing the path to datafile*/

char *create_path()
{
    char *path = NULL;
    path = (char*)malloc(MAXLEN*sizeof(char));
    if(path)
    {
        printf("Filename: ");
        fgets(path, MAXLEN, stdin);
        path[strlen(path)-1] = '\0';
        puts("");
    }
    else    puts("Error at memory allocation");
    return path;
}

/*
==========================================
Function create_singly_connected_ring_list
==========================================
Create singly connected ring list with manufacturers of planes
path - pointer to the passed string
return node1 - head of the ring of manufacturers
*/

country *create_singly_connected_ring_list(char* path)
{
    int i, n, first, flag;
    country *node1 = NULL, *p = NULL, *country0 = NULL;
    FILE *df = NULL;
    char s1[MAXLEN], **mass = NULL;
    df = fopen(path, "r");
    if(df == NULL)
    {
        if(fclose(df) == EOF)   puts("Error closing data file!");
        df = fopen("kursuch_02_data.csv", "r");
        puts("Incorrect filename!\nThe default settings are set!\n");
    }
    if(df)
    {
        n = 0;
        first = 1;
        while(fgets(s1, MAXLEN, df))    n++;
        rewind(df);
        for(i = 0;i < n;i++)
        {
            flag = 1;
            fgets(s1, MAXLEN, df);
            s1[strlen(s1)-1] = '\0';
            mass = split_string(s1, strlen(s1), ';');
            if(mass)
            {
                country0 = (country*)malloc(sizeof(country));
                if(country0)
                {
                    country0->str = mass[1];
                    if(first)   {node1 = country0;   first = 0;     node1->next = NULL;}
                    else
                    {
                        p = node1;
                        while(p != NULL)
                        {
                            if(comparison_strings(p->str, mass[1]))    flag = 0;
                            p = p->next;
                        }
                        if(flag)
                        {
                            p = node1;
                            while(p->next != NULL)    p = p->next;
                            p->next = country0;
                            country0->next = NULL;
                        }
                    }
                }
                else    puts("Error at memory allocation!");
            }
        }
        if(fclose(df) == EOF)   puts("Error closing data file!");
    }
    else    puts("Error opening data file!");
    node1->cnt = n;
    p = node1;
    while(p->next != NULL)    p = p->next;
    p->next = node1;
    return node1;
}

/*
==================================
Function create_doubly_linked_list
==================================
Create doubly linked list with information about airplanes
node1 - head of the ring of manufacturers
path - pointer to the passed string
return node0 - head of the list of information
*/

headP *create_doubly_linked_list(country *node1, char* path)
{
    headP* node0 = NULL;
    plane* plane0 = NULL;
    int i, n, first;
    char sep, s1[MAXLEN];
    char** mass = NULL;
    FILE *df = NULL;
    df = fopen(path, "r");
    sep = ';';
    if(df == NULL)
    {
        if(fclose(df) == EOF)   puts("Error closing data file!");
        df = fopen("kursuch_02_data.csv", "r");
    }
    if(df)
    {
        n = 0;
        first = 1;
        while(fgets(s1, MAXLEN, df))    n++;
        rewind(df);
        node0 = make_head();
        node0->cnt = n;
        for(i = 0;i < n;i++)
        {

            fgets(s1, MAXLEN, df);
            s1[strlen(s1)-1] = '\0';
            mass = split_string(s1, strlen(s1), sep);
            if(mass)
            {
                plane0 = struct_fill(mass, node1, i+1);
                if(first)
                {
                    add_first(node0, plane0);
                    first = 0;
                }
                else    add_last(node0, plane0);
            }
        }
        node0->last_id = node0->cnt;
        if(fclose(df) == EOF)   puts("Error closing data file!");
    }
    else    puts("Error opening data file!");
    return node0;
}

/*
==================
Function make_sort
==================
Sort the list of airplane for any field(ascending or descending order)
node0 - head of the list of information
return node0 - head of the list of information
*/

headP* make_sort(headP *node0)
{
    int mdf, j, i;
    plane *p = NULL, *p0 = NULL;
    plane *data = NULL;
    print_sort_param();
    printf("Select modification: ");
    mdf = catch_exception(0, 1);
    if(mdf == 0)
    {
        print_sort_field();
        printf("Select modification: ");
        mdf = catch_exception(0, 9);
        puts("");
        if(mdf == 0)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->id > p->next->id)
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 1)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->name[0] > p->next->name[0])
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 2)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->manuf->str[0] > p->next->manuf->str[0])
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 3)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->year > p->next->year)
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 4)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->range > p->next->range)
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 5)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->weight > p->next->weight)
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 6)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->price > p->next->price)
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 7)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->options[0] > p->next->options[0])
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 8)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->options[1] > p->next->options[1])
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->options[2] > p->next->options[2])
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
    }
    else    /*descending*/
    {
        print_sort_field();
        printf("Select modification: ");
        mdf = catch_exception(0, 9);
        puts("");
        if(mdf == 0)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->id < p->next->id)
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 1)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->name[0] < p->next->name[0])
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 2)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->manuf->str[0] < p->next->manuf->str[0])
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 3)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->year < p->next->year)
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 4)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->range < p->next->range)
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 5)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->weight < p->next->weight)
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 6)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->price < p->next->price)
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 7)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->options[0] < p->next->options[0])
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else if(mdf == 8)
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->options[1] < p->next->options[1])
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
        else
        {
            for(i = 0;i < node0->cnt-1;i++)
            {
                p0 = node0->first;
                for(j = 0;j < node0->cnt-1;j++)
                {
                    p = p0;
                    if(p->options[2] < p->next->options[2])
                    {
                        shuffle_elements(node0, p);
                    }
                    else    p0 = p0->next;
                }
            }
        }
    }
    output_doubly_linked_list(node0);
    return node0;
}

/*
=========================
Function shuffle_elements
=========================
Replace two elements of list of airplanes
node0 - head of the list of information
p - pointer to the element of list of information
*/

void shuffle_elements(headP *node0, plane *p)
{
    plane *data = NULL;
    if(p == node0->first)
    {
        data = p->next;
        if(p->next->next != NULL)   p->next = p->next->next;
        else    p->next = NULL;
        node0->first = data;
        data->prev = NULL;
        p->prev = data;
        if(data->next != NULL)  data->next->prev = p;
        data->next = p;

    }
    else if(p->next == node0->last)
    {
        data = p->next;
        p->next = NULL;
        p->prev->next = data;
        data->prev = p->prev;
        p->prev = data;
        node0->last = p;
        data->next = p;
    }
    else
    {
        data = p->next;
        p->next = p->next->next;
        p->prev->next = data;
        data->prev = p->prev;
        p->prev = data;
        data->next->prev = p;
        data->next = p;
    }
}

/*
==============================
Function searching_information
==============================
Searches for elements by the entered substring or numeric value
node0 - head of the list of information
*/

void searching_information(headP *node0)
{
    plane *p = NULL;
    int first, mdf;
    char *sbstr = NULL;
    print_searching();
    printf("Select modification: ");
    mdf = catch_exception(0, 9);
    puts("");
    printf("Enter substring for searching: ");
    sbstr = (char*)malloc(MAXLEN*sizeof(char));
    puts("");
    first = 1;
    if(sbstr)
    {
        fgets(sbstr, MAXLEN, stdin);
        sbstr[strlen(sbstr)-1] = '\0';
        p = node0->first;
        while(p != NULL)
        {
            if(mdf == 0 && atoi(sbstr) == p->id)
            {
                if(first)   print_header();
                struct_out(p);
                puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
                first = 0;
            }
            else if(mdf == 1 && comparison_strings(p->name, sbstr))
            {
                if(first)   print_header();
                struct_out(p);
                puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
                first = 0;
            }
            else if(mdf == 2 && comparison_strings(p->manuf, sbstr))
            {
                if(first)   print_header();
                struct_out(p);
                puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
                first = 0;
            }
            else if(mdf == 3 && atoi(sbstr) == p->year)
            {
                if(first)   print_header();
                struct_out(p);
                puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
                first = 0;
            }
            else if(mdf == 4 && atoi(sbstr) == p->range)
            {
                if(first)   print_header();
                struct_out(p);
                puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
                first = 0;
            }
            else if(mdf == 5 && atof(sbstr) == p->weight)
            {
                if(first)   print_header();
                struct_out(p);
                puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
                first = 0;
            }
            else if(mdf == 6 && atof(sbstr) == p->price)
            {
                if(first)   print_header();
                struct_out(p);
                puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
                first = 0;
            }
            else if(mdf == 7 && atof(sbstr) == p->options[0])
            {
                if(first)   print_header();
                struct_out(p);
                puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
                first = 0;
            }
            else if(mdf == 8 && atof(sbstr) == p->options[1])
            {
                if(first)   print_header();
                struct_out(p);
                puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
                first = 0;
            }
            else if(mdf == 9 && atof(sbstr) == p->options[2])
            {
                if(first)   print_header();
                struct_out(p);
                puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
                first = 0;
            }
            p = p->next;
        }
        free(sbstr);
        sbstr = NULL;
        if(first)   puts("Not found!");
    }
    else    puts("Error at memory allocation!");
}

/*
========================
Function add_information
========================
Creates an element with the  entered values of fields and adds it to the list of information
node0 - head of the list of information
node1 - head of the ring of manufacturers
return node1 - head of the ring of manufacturers
*/

country *add_information(headP *node0, country *node1)
{
    country *k = NULL;
    plane *p = NULL;
    char** mass = NULL;
    int i, flag;
    puts("\nExample input:");
    print_header();
    printf("| %2d | %23s | %12s |%6d  |%7d  |%7.1f  |%7.1f  |%7.1f   |%7.1f  |%8.1f    |\n",1, "Airbus A320","USA",1987,6300,42.2,93.9,37.57,11.0,31.4);
    puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
    mass = (char**)malloc(MAXLEN*sizeof(char*));
    if(mass)
    {
        flag = 1;
        printf("Enter Name: ");
        mass[0] = (char*)malloc(MAXLEN*sizeof(char));
        if(mass[0])     fgets(mass[0], MAXLEN, stdin);
        printf("Enter Manufacturer: ");
        mass[1] = (char*)malloc(MAXLEN*sizeof(char));
        if(mass[1])     fgets(mass[1], MAXLEN, stdin);
        printf("Enter Year: ");
        mass[2] = (char*)malloc(MAXLEN*sizeof(char));
        if(mass[2])     fgets(mass[2], MAXLEN, stdin);
        printf("Enter Range: ");
        mass[3] = (char*)malloc(MAXLEN*sizeof(char));
        if(mass[3])     fgets(mass[3], MAXLEN, stdin);
        printf("Enter Weight: ");
        mass[4] = (char*)malloc(MAXLEN*sizeof(char));
        if(mass[4])     fgets(mass[4], MAXLEN, stdin);
        printf("Enter Price: ");
        mass[5] = (char*)malloc(MAXLEN*sizeof(char));
        if(mass[5])     fgets(mass[5], MAXLEN, stdin);
        printf("Enter Length: ");
        mass[6] = (char*)malloc(MAXLEN*sizeof(char));
        if(mass[6])     fgets(mass[6], MAXLEN, stdin);
        printf("Enter Height: ");
        mass[7] = (char*)malloc(MAXLEN*sizeof(char));
        if(mass[7])     fgets(mass[7], MAXLEN, stdin);
        printf("Enter Wingspan: ");
        mass[8] = (char*)malloc(MAXLEN*sizeof(char));
        if(mass[8])     fgets(mass[8], MAXLEN, stdin);
        for(i = 0;i < 9;i++)    mass[i][strlen(mass[i])-1] = '\0';
        if(node1 == NULL)
        {
            node1 = add_elem_of_ring(node1, mass[1]);
            node1->cnt++;
            p = struct_fill(mass, node1, 1);
            p->manuf = node1->str;
            add_first(node0, p);
            p->id = ++node0->last_id;
            p = node0->first;
            node0->cnt++;
        }
        else        /*if list isn't empty*/
        {
            k = node1;
            do  {if(check_strings_for_identity_plus_reg(mass[1], k->str))    flag = 0;   k = k->next;}   /*check country in list*/
            while(k != node1);
            if(flag)    node1 = add_elem_of_ring(node1, mass[1]);
            node1->cnt++;
            p = struct_fill(mass, node1, 1);
            while(!(check_strings_for_identity_plus_reg(mass[1], k->str)))   k = k->next;
            p->manuf = k->str;
            add_last(node0, p);
            p->id = find_last_id(node0) + 1;
            p = node0->first;
            node0->cnt++;

        }
        puts("\nResult:");
        output_simply_connected_ring_list(node1);
        puts("");
        output_doubly_linked_list(node0);
    }
    return node1;
}

/*
===========================
Function adjust_information
===========================
Edits the selected item in the list of information (in full or a specific field), as well as the selected element of the ring
node0 - head of the list of information
node1 - head of the ring of manufacturers
return node1 - head of the ring of manufacturers
*/

country *adjust_information(headP *node0, country *node1)
{

    int i, mdf, flag, num;
    float chsl;
    plane *p = NULL, *data = NULL, *f = NULL;
    country *k = NULL, *prev = NULL, *infa = NULL;
    char **mass = NULL, *elem = NULL, *str = NULL;
    clear();
    print_adjust();
    printf("Select modification: ");
    mdf = catch_exception(0, 2);
    puts("");
    if(mdf == 0)
    {
        output_simply_connected_ring_list(node1);
        elem = (char*)malloc(MAXLEN*sizeof(char));      /*is being replaced*/
        if(elem)
        {
            printf("Enter Manufacturer to adjust: ");
            fgets(elem, MAXLEN, stdin);
            elem[strlen(elem)-1] = '\0';
            if(is_str_in_list(node0, elem))
            {
                str = (char*)malloc(MAXLEN*sizeof(char));       /*is being replaced with*/
                if(str)
                {
                    printf("\nAdjustment: ");
                    fgets(str, MAXLEN, stdin);
                    str[strlen(str)-1] = '\0';
                    k = node1;
                    while(!(check_strings_for_identity_plus_reg(k->str, elem)))    k = k->next;
                    flag = 0;
                    p = node0->first;
                    while(p != NULL)    {if(check_strings_for_identity_plus_reg(p->manuf, elem))    flag = 1;   p = p->next;}
                    while(flag)
                    {
                        p = node0->first;
                        while(!(check_strings_for_identity_plus_reg(p->manuf, elem)))    p = p->next;
                        printf("%s\n", k->str);
                        p->manuf = str;
                        flag = 0;
                        p = node0->first;
                        while(p != NULL)    {if(check_strings_for_identity_plus_reg(p->manuf, elem))    flag = 1;   p = p->next;}
                    }
                    flag = 0;
                    k = node1;
                    do      {if(check_strings_for_identity_plus_reg(k->str, str))   flag = 1;   k = k->next;}
                    while(k != node1);
                    if(flag)     node1 = delete_elem_of_ring(node1, k);
                    else
                    {
                        while(!check_strings_for_identity_plus_reg(k->str, elem))   k = k->next;
                        k->str = str;
                    }
                    output_doubly_linked_list(node0);
                }
                else    puts("Error at memory allocation!");
            }
            else    puts("Not found!");
            free(elem);
            elem = NULL;
        }
        else    puts("Error at memory allocation!");
    }
    else if(mdf == 1)
    {
        output_doubly_linked_list(node0);
        printf("Select id of element to adjust: ");
        mdf = catch_exception(0, node0->last_id);
        while(!(is_in_list(node0, mdf)))
        {
            printf("Incorrect input! Please try again: ");
            mdf = catch_exception(0, node0->last_id);
        }
        p = node0->first;
        while(p->id != mdf)     p = p->next;
        puts("");
        print_adjust_field();
        printf("Select field to adjust: ");
        mdf = catch_exception(0, 8);
        puts("");
        if(mdf == 0)
        {
            elem = (char*)malloc(MAXLEN*sizeof(char));
            if(elem)
            {
                printf("Enter Name: ");
                fgets(elem, MAXLEN, stdin);
                elem[strlen(elem)-1] = '\0';
                p->name = elem;
            }
            else    puts("Error at memory allocation!");
        }
        else if(mdf == 1)
        {
            elem = (char*)malloc(MAXLEN*sizeof(char));
            if(elem)
            {
                printf("Enter Manufacturer: ");
                fgets(elem, MAXLEN, stdin);
                elem[strlen(elem)-1] = '\0';
                k = node1;
                flag = 1;
                /*проверка на наличие введенного элемента в кольце(вне зависимости от регистра)*/
                do      {if(check_strings_for_identity_plus_reg(k->str, elem))   flag = 0;   k = k->next;}
                while(k != node1);

                /*если элемента нет в кольце - добавляем*/
                if(flag)    node1 = add_elem_of_ring(node1, elem);

                /*находим этот элемент кольца*/
                prev = node1;
                while(!(check_strings_for_identity_plus_reg(prev->str, elem)))  prev = prev->next;

                /*запоминаем элемент кольца на который заменяем и то что заменяем в основном списке, затем заменяем то что должно быть в списке на elem*/

                infa = prev;            /*запоминаем то на что будем заменять*/
                str = p->manuf;         /*запоминаем то что было в списке*/
                p->manuf = prev->str;

                /*проверяем есть ли еще элемент кольца в списке*/
                flag = 1;
                f = node0->first;
                while(f != NULL && flag)    {if(check_strings_for_identity_plus_reg(f->manuf, str))     flag = 0;   f = f->next;}

                /*если нет - удаляем*/
                if(flag)
                {
                    prev = node1;
                    while(!(check_strings_for_identity_plus_reg(str, prev->str)))    prev = prev->next;
                    node1 = delete_elem_of_ring(node1, prev);
                }
            }
            else    puts("Error at memory allocation!");
        }
        else if(mdf == 2)
        {
            printf("Enter Year: ");
            scanf("%d", &num);
            p->year = num;
        }
        else if(mdf == 3)
        {
            printf("Enter Range: ");
            scanf("%d", &num);
            p->range = num;
        }
        else if(mdf == 4)
        {
            printf("Enter Weight: ");
            scanf("%f", &chsl);
            p->weight = chsl;
        }
        else if(mdf == 5)
        {
            printf("Enter Price: ");
            scanf("%f", &chsl);
            p->price = chsl;
        }
        else if(mdf == 6)
        {
            printf("Enter Length: ");
            scanf("%f", &chsl);
            p->options[0] = chsl;
        }
        else if(mdf == 7)
        {
            printf("Enter Height: ");
            scanf("%f", &chsl);
            p->options[1] = chsl;
        }
        else
        {
            printf("Enter Wingspan: ");
            scanf("%f", &chsl);
            p->options[2] = chsl;
        }

    }
    else if(mdf == 2)
    {
        output_doubly_linked_list(node0);
        printf("Select id of element to adjust: ");
        mdf = catch_exception(0, node0->last_id);
        while(!(is_in_list(node0, mdf)))
        {
            printf("Incorrect input! Please try again: ");
            mdf = catch_exception(0, node0->last_id);
        }
        elem = (char*)malloc(MAXLEN*sizeof(char));
        if(elem)
        {
            p = node0->first;
            if(mdf == node0->first->id)    elem = strcpy(elem, p->manuf);
            else if(mdf == node0->last->id)  {p = node0->last;   elem = strcpy(elem, p->manuf);}
            else
            {
                while(p->next->id != mdf)   p = p->next;
                elem = strcpy(elem, p->next->manuf);
            }
            mass = (char**)malloc(MAXLEN*sizeof(char*));
            if(mass)
            {
                flag = 1;
                printf("Enter Name: ");
                mass[0] = (char*)malloc(MAXLEN*sizeof(char));
                if(mass[0])     fgets(mass[0], MAXLEN, stdin);
                printf("Enter Manufacturer: ");
                mass[1] = (char*)malloc(MAXLEN*sizeof(char));
                if(mass[1])     fgets(mass[1], MAXLEN, stdin);
                printf("Enter Year: ");
                mass[2] = (char*)malloc(MAXLEN*sizeof(char));
                if(mass[2])     fgets(mass[2], MAXLEN, stdin);
                printf("Enter Range: ");
                mass[3] = (char*)malloc(MAXLEN*sizeof(char));
                if(mass[3])     fgets(mass[3], MAXLEN, stdin);
                printf("Enter Weight: ");
                mass[4] = (char*)malloc(MAXLEN*sizeof(char));
                if(mass[4])     fgets(mass[4], MAXLEN, stdin);
                printf("Enter Price: ");
                mass[5] = (char*)malloc(MAXLEN*sizeof(char));
                if(mass[5])     fgets(mass[5], MAXLEN, stdin);
                printf("Enter Length: ");
                mass[6] = (char*)malloc(MAXLEN*sizeof(char));
                if(mass[6])     fgets(mass[6], MAXLEN, stdin);
                printf("Enter Height: ");
                mass[7] = (char*)malloc(MAXLEN*sizeof(char));
                if(mass[7])     fgets(mass[7], MAXLEN, stdin);
                printf("Enter Wingspan: ");
                mass[8] = (char*)malloc(MAXLEN*sizeof(char));
                if(mass[8])     fgets(mass[8], MAXLEN, stdin);
                for(i = 0;i < 8;i++)    mass[i][strlen(mass[i])-1] = '\0';
                k = node1;
                do
                {
                    if(check_strings_for_identity_plus_reg(mass[1], k->str))     flag = 0;
                    k = k->next;
                }while(k != node1);
                if(flag)    node1 = add_elem_of_ring(node1, mass[1]);
                data = struct_fill(mass, node1, 1);
                data->id = mdf;
                if(mdf == node0->first->id)
                {
                    node0->first = data;
                    data->next = p;
                    p->prev = data;
                }
                else if(mdf == node0->last->id)
                {
                    p->prev->next = data;
                    data->prev = p->prev;
                    data->next = p;
                    p->prev = data;
                }
                else
                {
                    p->next->prev = data;
                    data->next = p->next;
                    data->prev = p;
                    p->next = data;
                }
                /*function delete 1 elem doubly linked list*/
                p = node0->first;
                while(!(check_strings_for_identity_plus_reg(p, data)))     p = p->next;
                p = p->next;
                struct_out(p);
                if(node0->last_id == p->id)     node0->last_id = find_last_id(node0);
                if(is_in_list(node0, p->id))
                {
                    k = node1;
                    while(!(check_strings_for_identity_plus_reg(k->str, p->manuf)))   k = k->next;
                    node1 = delete_elem_of_ring(node1, k);
                    puts("Result:");
                    output_simply_connected_ring_list(node1);
                    puts("");
                    node1->cnt--;
                }
                delete_elem_of_doubly_linked_list(node0, p);
                node0->cnt++;
                puts("");
                output_doubly_linked_list(node0);
            }
            else    puts("Error at memory allocation!");
        }
        else    puts("Error at memory allocation!");
    }
    puts("\nResult: ");
    output_simply_connected_ring_list(node1);
    puts("");
    output_doubly_linked_list(node0);
    return node1;
}
/*
===========================
Function delete_information
===========================
Delete the selected item in the list of information or delete the selected item from the ring (with the removal of all related list items)
node0 - head of the list of information
node1 - head of the ring of manufacturers
return node1 - head of the ring of manufacturers
*/

country *delete_information(headP *node0, country *node1)
{
    country *k = NULL;
    plane *p = NULL, *next = NULL, *data = NULL;
    char *str = NULL;
    int i, mdf, flag;
    clear();
    print_delete();
    printf("Select modification: ");
    mdf = catch_exception(0, 1);
    puts("");
    if(mdf == 0)
    {
        output_simply_connected_ring_list(node1);
        puts("");
        str = (char*)malloc(MAXLEN*sizeof(char));
        if(str)
        {
            printf("Enter country to delete: ");
            fgets(str, MAXLEN, stdin);
            str[strlen(str)-1] = '\0';
            if(node0->cnt != 1)
            {
                /*delete all fields in the main list with this substring in country*/
                for(i = 0;i < node0->cnt;i++)
                {
                    flag = 0;
                    k = node1;
                    do
                    {
                        if(check_strings_for_identity_plus_reg(k->str, str))     flag = 1;
                        k = k->next;
                    }
                    while(k != node1);      /*check if there is a string in the ring*/
                    if(flag)    /*if there is, we find a line with it in the main list and delete it*/
                    {
                        p = node0->first;
                        while(!(check_strings_for_identity_plus_reg(p->manuf, str)))     p = p->next;
                        if(p->id == node0->last_id)
                        {
                            delete_elem_of_doubly_linked_list(node0, p);
                            node0->last_id = find_last_id(node0);
                        }
                        else    delete_elem_of_doubly_linked_list(node0, p);

                        p = node0->first;
                        /*we check whether the fields with this country are still in the list*/
                        while(p != NULL)
                        {
                            if(check_strings_for_identity_plus_reg(p->manuf, str))   flag = 0;
                            p = p->next;
                        }
                        if(flag)        /*if not, we remove the country from the ring*/
                        {
                            k = node1;
                            while(!(check_strings_for_identity_plus_reg(k->str, str)))     k = k->next;
                            node1 = delete_elem_of_ring(node1, k);
                            node1->cnt--;
                        }
                    }
                    else if(i == 0)     {puts("Not found!");    i = node0->cnt;}
                }
            }
            else
            {
                p = node0->first;
                while(!(check_strings_for_identity_plus_reg(p->manuf, str)))     p = p->next;
                node0->last_id = 0;
                delete_elem_of_doubly_linked_list(node0, p);
                node1 = delete_elem_of_ring(node1, node1);
                node0->first = NULL;
                node0->last = NULL;
            }
            puts("Result:");
            output_simply_connected_ring_list(node1);
            puts("");
            output_doubly_linked_list(node0);
            free(str);
            str = NULL;
        }
        else    puts("Error at memory allocation!");
    }
    else
    {
        output_doubly_linked_list(node0);
        printf("Select id of element to delete: ");
        mdf = catch_exception(1, node0->last_id);
        while(!(is_in_list(node0, mdf)))
        {
            printf("Incorrect input! Please try again: ");
            mdf = catch_exception(1, node0->last_id);
        }
        puts("");
        p = node0->first;
        while(p->id != mdf)     p = p->next;
        if(node0->cnt != 1)
        {
            k = node1;
            while(!(check_strings_for_identity(k->str, p->manuf)))     k = k->next;
            if(node0->last_id == p->id)     node0->last_id = find_last_id(node0);
            delete_elem_of_doubly_linked_list(node0, p);
            flag = 1;
            next = node0->first;
            while(next != NULL)    {if(check_strings_for_identity_plus_reg(next->manuf, k->str))    flag = 0;   next = next->next;}
            if(flag)
            {

                k = node1;
                while(!(check_strings_for_identity(k->str, p->manuf)))   k = k->next;
                node1 = delete_elem_of_ring(node1, k);
                node1->cnt--;
            }
            puts("Result:");
            output_simply_connected_ring_list(node1);
            puts("");
            output_doubly_linked_list(node0);
        }
        else
        {
            node0->last_id = 0;
            delete_elem_of_doubly_linked_list(node0, p);
            node1 = delete_elem_of_ring(node1, node1);
            node0->first = NULL;
            node0->last = NULL;
            puts("The list is empty!");
        }
    }
    return node1;
}

/*
=====================
Function find_last_id
=====================
Find the last id in list of information
node0 - head of the list of information
return lst - the last id
*/

int find_last_id(headP *node0)
{
    int lst;
    plane *p = NULL;
    p = node0->first;
    lst = 0;
    while(p != NULL)    {if(p->id > lst)    lst = p->id;    p = p->next;}
    return lst;
}

/*
==========================================
Function delete_elem_of_doubly_linked_list
==========================================
Delete element the of list of information
node0 - head of the list of information
p - pointer to the element of list of information
*/

void delete_elem_of_doubly_linked_list(headP *node0, plane *p)
{
    plane *k = NULL;
    if(p == node0->first)
    {
        k = p;
        p = p->next;
        node0->first = p;
        free(k);
        k = NULL;
        if(node0->cnt != 1)     p->prev = NULL;
    }
    else if(p == node0->last)
    {
        k = p;
        p = p->prev;
        node0->last = p;
        free(k);
        k = NULL;
        p->next = NULL;
    }
    else
    {
        k = p->prev;
        p =  p->next;
        free(p->prev);
        p->prev = k;
        k->next = p;
    }
    node0->cnt--;
}

/*
=========================
Function add_elem_of_ring
=========================
Add element to the ring
node1 - head of the ring of manufacturers
str - string to add to ring
return node1 - head of the ring of manufacturers
*/

country *add_elem_of_ring(country *node1, char* str)
{
    country *p = NULL, *k = NULL;
    k = node1;
    p = (country*)malloc(sizeof(country));
    if(p)
    {
        if(node1 != NULL)
        {
            do  k = k->next;
            while(k->next != node1);
            p->str = str;
            k->next = p;
            p->next = node1;
        }
        else
        {
            p->str = str;
            p->next = p;
            node1 = p;
        }
    }
    return node1;
}

/*
============================
Function delete_elem_of_ring
============================
Delete element of the ring
node1 - head of the ring of manufacturers
p - pointer to element of the ring
return node1 - head of the ring of manufacturers
*/

country *delete_elem_of_ring(country *node1, country *p)
{
    country *prev = NULL;
    if(p == node1 && p->next == node1)
    {
        free(node1);
        node1 = NULL;
    }
    else
    {
        if(p == node1)  /*delete first elem*/
        {
            prev = p;
            p = p->next;
            node1 = p;
            while(p->next->next != node1)   p = p->next;
            p->next = node1;
            free(prev);
            prev = NULL;

        }
        else if(p->next == node1)    /*delete last elem */
        {
            prev = p;
            while(p->next->next != node1)   p = p->next;
            p->next = node1;
            free(prev);
            prev = NULL;
        }
        else    /*delete else elem*/
        {
            prev = p;
            while(p->next != prev)  p = p->next;
            p->next = prev->next;
            free(prev);
            prev = NULL;
        }
    }
    return node1;
}

/*
==================
Function make_head
==================
Create head of the list of information
return ph - head of the list of information
*/

headP *make_head()
{
    headP *ph = NULL;

    ph = (headP*)malloc(sizeof(headP));
    if(ph)
    {
        ph->cnt = 0;
        ph->last_id = 0;
        ph->first = NULL;
        ph->last = NULL;
    }
    else    puts("Error at memory allocation!");
    return ph;
}

/*
====================
Function struct_fill
====================
Fills in the fields of the element of list of information
mass - pointer to the array of strings to fills fields
node1 - head of the ring of manufacturers
n - id of element
return new_node - filled in list item
*/

plane *struct_fill(char **mass, country *node1, int n)
{
    plane *new_node = NULL;
    new_node = (plane*)malloc(sizeof(plane));
    if(node1->cnt != 1)     while(!(comparison_strings(node1->str, mass[1])))    node1 = node1->next;
    if (new_node)
    {
        new_node->id = n;
        new_node->name = mass[0];
        new_node->manuf = node1->str;
        if(atoi(mass[2]) > 0)  new_node->year = atoi(mass[2]);
        else    new_node->year = 0;
        if(atoi(mass[3]) > 0)     new_node->range = atoi(mass[3]);
        else    new_node->range = 0;
        if(atof(mass[4]) > 0)     new_node->weight = atof(mass[4]);
        else    new_node->weight = 0;
        if(atof(mass[5]) > 0)     new_node->price = atof(mass[5]);
        else    new_node->price = 0;
        if(atof(mass[6]) > 0)     new_node->options[0] = atof(mass[6]);
        else    new_node->options[0] = 0;
        if(atof(mass[7]) > 0)     new_node->options[1] = atof(mass[7]);
        else    new_node->options[1] = 0;
        if(atof(mass[8]) > 0)     new_node->options[2] = atof(mass[8]);
        else    new_node->options[2] = 0;
        new_node->next = NULL;
        new_node->prev = NULL;
    }
    else    puts("Error at memory allocation!");
    return new_node;
}

/*
==================
Function add_first
==================
Adds an element to the beginning of list of information
my_head - head of the list of information
new_node - pointer to element of the list of information
*/

void add_first(headP *my_head, plane *new_node)
{
    if(my_head&&new_node)
    {
        my_head->first = new_node;
        my_head->last = new_node;
    }
}

/*
==================
Function add_last
==================
Adds an element to the end of list of information
my_head - head of the list of information
new_node - pointer to element of the list of information
*/

void add_last(headP *my_head, plane *new_node)
{
    plane *p = NULL;
    if(my_head&&new_node)
    {
        p = my_head->first;
        while(p->next != NULL)    p = p->next;
        p->next = new_node;
        new_node->prev = p;
        my_head->last = new_node;
        new_node->next = NULL;
    }
}

/*
=========================
Function save_information
=========================
Save the list of information to file "result_pakulov_kurs.csv"
node0 - head of the list of information
*/

void save_information(headP *node0)
{
    plane *p = NULL;
    FILE *df = NULL;
    char str[32];
    df = fopen("result_pakulov_kurs.csv", "w");
    if(df)
    {
        p = node0->first;
        while(p != NULL)
        {
            fputs(p->name, df);
            fputs(";", df);
            fputs(p->manuf, df);
            fputs(";", df);
            fprintf(df, "%d", p->year);
            fputs(";", df);
            fprintf(df, "%d", p->range);
            fputs(";", df);
            fprintf(df, "%.1f", p->weight);
            fputs(";", df);
            fprintf(df, "%.1f", p->price);
            fputs(";", df);
            fprintf(df, "%.1f", p->options[0]);
            fputs(";", df);
            fprintf(df, "%.1f", p->options[1]);
            fputs(";", df);
            fprintf(df, "%.1f", p->options[2]);
            fputs("\n", df);
            p = p->next;
        }
        puts("Information is saved to result_pakulov_kurs.csv!");
        if(fclose(df) == EOF)   puts("Error closing result file!");
    }
    else    puts("Error opening result file!");
}

/*
=====================
Function split_string
=====================
Splits the read string and writes it to an array of strings
string - string to be split
length - length of the string
sep - separator between data
return mass - array of strings(fields of element of list of information)
*/

char **split_string(char *string, int length, char sep)
{
    char **mass = NULL;
    int i, j, m, key;
    m = 0;
    j = 0;
    mass = (char**)malloc(9*sizeof(char*));
    if(mass != NULL)
    {
        for(i = 0;i < 9;i++)
        {
            mass[i] = (char*)malloc(length*sizeof(char));
            if (mass[i] != NULL)   key = 1;
            else
            {
                puts("Error at memory allocation!");
                key = 0;
            }
        }
        if(key)
        {
            for(i = 0;i < 9;i++)
            {
                do
                {
                    if(string[j] == sep || string[j] == '\0')   m = j+1;
                    else    mass[i][j-m] = string[j];
                    j++;
                } while(string[j] != sep && j < length);
                mass[i][j-m] = '\0';
            }
        }
        else    clear_strings(mass, 9);
    }
    else    puts("Error at memory allocation!");
    return mass;
}

/*
========================
Function catch_exception
========================
Checks whether the entered value belongs to the specified segment
a - the left border of the segment
b - the right order of the segment
return mdf - the value belonging to the segment
*/

int catch_exception(int a, int b)
{
    int f, mdf;
    do
    {
        do
        {
            f = scanf("%d", &mdf);
            fflush(stdin);
            if(f == 0)  printf("Incorrect input! Please try again: ");
        } while(f != 1);
        if(mdf < a || mdf > b)  printf("Incorrect input! Please try again: ");
    } while(mdf < a || mdf > b);
    return mdf;
}

/*
=======================
Function is_str_in_list
=======================
Checks whether the given string is the manufacturer field of the list items
node0 - head of the list of information
str - string to check
return flag - 0-there is no row in the list, 1-there is
*/


int is_str_in_list(headP *node0, char* str)
{
    int flag;
    plane *p = NULL;
    flag = 0;
    p = node0->first;
    while(p != NULL)
    {
        if(check_strings_for_identity(str, p->manuf))    flag = 1;
        p = p->next;
    }
    return flag;
}

/*
===================
Function is_in_list
===================
Checks whether the given number is the id field of the list items
node0 - head of the list of information
mdf - number to check
return flag - 0-there is no row in the list, 1-there is
*/

int is_in_list(headP *node0,int mdf)
{
    plane *p = NULL;
    int flag;
    flag = 0;
    p = node0->first;
    while(p != NULL && !flag)
    {
        if(p->id == mdf)    flag = 1;
        p = p->next;
    }
    return flag;
}

/*
===================
Function is_in_ring
===================
Checks whether the given string is in the ring of manufacturers
node1 - head of the ring of manufacturers
str - string to check
return flag - 0-there is no row in the list, 1-there is
*/

int is_in_ring(country *node1, char* str)
{
    int flag;
    country *p = NULL;
    p = node1;
    flag = 0;
    do
    {
        if(check_strings_for_identity(p->str, str))     flag = 1;
        p = p->next;
    }
    while(p != node1);
    return flag;
}

/*
======================
Function clear_strings
======================
Clears the memory allocated for an array of strings
str - pointer to the array of strings
n - count of strings
*/

void clear_strings(char **str, int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        free(str[i]);
        str[i]=NULL;
    }
    free(str);
    str=NULL;

}

/*
========================
Function print_main_manu
========================
Draw main menu
*/

void print_main_menu()
{
    puts("++========================================++");
    printf("||%25s               ||\n", "MAIN  MENU");
    puts("++========================================++");
    printf("|| 0 \t----->\t  Info\t\t          ||\n");
    printf("||\t\t\t\t\t  ||\n");
    printf("|| 1 \t----->\t  Chose datafile\t  ||\n");
    printf("||\t\t\t\t\t  ||\n");
    printf("|| 2 \t----->\t  Output the information  ||\n");
    printf("||\t\t\t\t\t  ||\n");
    printf("|| 3 \t----->\t  Change the information  ||\n");
    printf("||\t\t\t\t\t  ||\n");
    printf("|| 4 \t----->\t  Save information\t  ||\n");
    printf("||\t\t\t\t\t  ||\n");
    printf("|| 5 \t----->\t  Close the program\t  ||\n");
    puts("++========================================++");
}

/*
========================
Function print_reference
========================
Draw reference
*/

void print_reference()
{
    puts("++==============================================================================++");
    printf("||%42s                                    ||\n", "INFO");
    puts("++==============================================================================++");
    printf("||\tThis program is a file cabinet that stores information about\t\t||\n");
    printf("||airplanes. Each plane has the following fields:\t\t\t\t||\n");
    printf("||  *  Name - the name of the model\t\t\t\t\t\t||\n");
    printf("||  *  Manuf - aircraft manufacturing country\t\t\t\t\t||\n");
    printf("||  *  Year - the year of the aircraft's commissioning\t\t\t\t||\n");
    printf("||  *  Range - maximum flight distance in kilometers\t\t\t\t||\n");
    printf("||  *  Weight - the weight of an empty aircraft in tons\t\t\t\t||\n");
    printf("||  *  Price - the cost of the aircraft in millions of dollars\t\t\t||\n");
    printf("||  *  Length - the length of the aircraft in meters\t\t\t\t||\n");
    printf("||  *  Height - the height of the aircraft in meters\t\t\t\t||\n");
    printf("||  *  Wingspan - the wingspan of the aircraft in meters\t\t\t||\n");
    printf("||\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||The program is able to read csv files with so many fields (9)\t\t\t||\n");
    printf("||\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||The functions of the file cabinet are diverse:\t\t\t\t||\n");
    printf("||  *  Output of card file elements\t\t\t\t\t\t||\n");
    printf("||  *  Search for card file elements by substring or numeric value\t\t||\n");
    printf("||  *  Adding an item to a file cabinet\t\t\t\t\t\t||\n");
    printf("||  *  Complete editing of the card file element\t\t\t\t|| \n");
    printf("||  *  Editing a specific field of a card file element\t\t\t\t||\n");
    printf("||  *  Sorting of card file items by a specific field(ascending/descending)\t||\n");
    printf("||  *  Deleting an item from a file cabinet\t\t\t\t\t||\n");
    printf("||  *  Saving the card file on disk\t\t\t\t\t\t||\n");
    printf("||And many others!\t\t\t\t\t\t\t\t||\n");
    printf("||\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||It also provides protection against incorrect input\t\t\t\t||\n");
    printf("||\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||Directed by Wastard\t\t\t\t\t\t\t\t||\n");
    puts("++==============================================================================++");
}

/*
====================
Function print_input
====================
Draw input
*/

void print_input()
{
    puts("++==================================++");
    printf("||%22s            ||\n", "INPUT FILE");
    puts("++==================================++");
}

/*
==========================
Function print_output_menu
==========================
Draw output menu
*/

void print_output_menu()
{
    puts("++========================================++");
    printf("||%25s               ||\n", "OUTPUT");
    puts("++========================================++");
    printf("|| 0 \t----->\t  Output all information  ||\n");
    printf("||\t\t\t\t\t  ||\n");
    printf("|| 1 \t----->\t  Output only countries   ||\n");
    printf("||\t\t\t\t\t  ||\n");
    printf("|| 2 \t----->\t  Search the information  ||\n");
    puts("++========================================++");
}

/*
=====================
Function print_change
=====================
Draw change
*/

void print_change()
{
    puts("++========================================++");
    printf("||%29s           ||\n", "CHANGE INFORMATION");
    puts("++========================================++");
    printf("||  0 \t----->\t Add information\t  ||\n");
    printf("||\t\t\t\t\t  ||\n");
    printf("||  1 \t----->\t Adjust information\t  ||\n");
    printf("||\t\t\t\t\t  ||\n");
    printf("||  2 \t----->\t Sorting information\t  ||\n");
    printf("||\t\t\t\t\t  ||\n");
    printf("||  3 \t----->\t Delete information\t  ||\n");
    puts("++========================================++");
}

/*
=====================
Function print_adjust
=====================
Draw adjust
*/

void print_adjust()
{
    puts("++==============================================================++");
    printf("||%40s\t                ||\n", "ADJUST INFORMATION");
    puts("++==============================================================++");
    printf("||  0 \t----->\t Adjust all fields with certain information\t||\n");
    printf("||\t\t\t\t\t\t\t\t||\n");
    printf("||  1 \t----->\t Adjust one certain field of information\t||\n");
    printf("||\t\t\t\t\t\t\t\t||\n");
    printf("||  2 \t----->\t Overwrite information completely\t\t||\n");
    puts("++==============================================================++");
}

/*
========================
Function print_searching
========================
Draw searching
*/

void print_searching()
{
    puts("++=================================++");
    printf("||%26s       ||\n", "SEARCH INFORMATION");
    puts("++=================================++");
    printf("||    0      ----->\tId\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    1      ----->\tName\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    2      ----->\tManuf\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    3      ----->\tYear\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    4      ----->\tRange\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    5      ----->\tWeight     ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    6      ----->\tPrice\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    7      ----->\tLength     ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    8      ----->\tHeight     ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    9      ----->\tWingspan   ||\n");
    puts("++=================================++");
}

/*
===========================
Function print_adjust_field
===========================
Draw adjust field
*/

void print_adjust_field()
{
    puts("++=================================++");
    printf("||%23s          ||\n", "ADJUST FIELD");
    puts("++=================================++");
    printf("||    0      ----->\tName\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    1      ----->\tManuf\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    2      ----->\tYear\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    3      ----->\tRange\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    4      ----->\tWeight     ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    5      ----->\tPrice\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    6      ----->\tLength     ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    7      ----->\tHeight     ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    8      ----->\tWingspan   ||\n");
    puts("++=================================++");
}

/*
=========================
Function print_sort_param
=========================
Draw parameters of sorting
*/

void print_sort_param()
{
    puts("++=================================++");
    printf("||%25s        ||\n", "SORTING PARAMETR");
    puts("++=================================++");
    printf("||    0      ----->\tAscending  ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    1      ----->\tDescending ||\n");
    puts("++=================================++");
}

/*
=========================
Function print_sort_field
=========================
Draw sorting fields
*/

void print_sort_field()
{
    puts("++=================================++");
    printf("||%21s            ||\n", "SORTING FIELD");
    puts("++=================================++");
    printf("||    0      ----->\tId\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    1      ----->\tName\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    2      ----->\tManuf\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    3      ----->\tYear\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    4      ----->\tRange\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    5      ----->\tWeight     ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    6      ----->\tPrice\t   ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    7      ----->\tLength     ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    8      ----->\tHeight     ||\n");
    printf("||\t\t\t\t   ||\n");
    printf("||    9      ----->\tWingspan   ||\n");
    puts("++=================================++");
}

/*
=====================
Function print_delete
=====================
Draw delete
*/

void print_delete()
{
    puts("++========================================++");
    printf("||%29s           ||\n", "DELETE INFORMATION");
    puts("++========================================++");
    printf("||  0 \t----->\t Delete manufacturer\t  ||\n");
    printf("||\t\t\t\t\t  ||\n");
    printf("||  1 \t----->\t Delete element of list\t  ||\n");
    puts("++========================================++");
}

/*
====================
Function print_chose
====================
Draw chose
*/

void print_chose()
{
    puts("++==================================++");
    printf("|| 0 \t----->\t  Back to main menu ||\n");
    puts("||\t\t\t\t    ||");
    printf("|| 1 \t----->\t  Close the program ||\n");
    puts("++==================================++");
    printf("Select modification: ");
}

/*
=====================
Function print_header
=====================
Draw header
*/

void print_header()
{
    puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
    printf("| %2s | %14s          | %7s |%6s  |%7s  |%8s |%7s  |%8s  |%8s |%10s  |\n","Id","Name","Manufacturer","Year","Range","Weight","Price","Length","Height","Wingspan");
    puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
}

/*
===================
Function struct_out
===================
Output element of the list of information
str0 - element of the list
*/

void struct_out(plane *str0)
{
    printf("| %2d | %23s | %12s |%6d  |%7d  |%7.1f  |%7.1f  |%7.1f   |%7.1f  |%8.1f    |\n",
        str0->id, str0->name,str0->manuf,str0->year,str0->range,str0->weight,str0->price,str0->options[0],str0->options[1],str0->options[2]);
}

/*
===================================
Function check_strings_for_identity
===================================
Character - by - character compares strings
string - the first string
substr - the second string
return flag - 0-strings not equal, 1-is equal
*/

int check_strings_for_identity_plus_reg(char* string, char* substr)
{
    int flag;
    unsigned int i, j;
    flag = 0;
    if(strlen(substr) == strlen(string))
    {
        for(i = 0;i < strlen(string)-strlen(substr)+1;i++)
        {
            flag = 1;
            if(string[i] == substr[0] || abs((int)string[i] - (int)substr[0]) == 32)
            {
                for(j = 1;j < strlen(substr);j++)   if(substr[j] != string[i+j] && abs((int)substr[j] - (int)string[i+j]) != 32)    flag = 0;
                if(flag)    i = strlen(string)-strlen(substr)+1;
            }
            else   flag = 0;
        }
    }
    return flag;
}


/*
===================================
Function check_strings_for_identity
===================================
Character - by - character compares strings
string - the first string
substr - the second string
return flag - 0-strings not equal, 1-is equal
*/

int check_strings_for_identity(char* string, char* substr)
{
    int flag;
    unsigned int i, j;
    flag = 0;
    if(strlen(substr) == strlen(string))
    {
        for(i = 0;i < strlen(string)-strlen(substr)+1;i++)
        {
            flag = 1;
            if(string[i] == substr[0])
            {
                for(j = 1;j < strlen(substr);j++)   if(substr[j] != string[i+j])    flag = 0;
                if(flag)    i = strlen(string)-strlen(substr)+1;
            }
            else   flag = 0;
        }
    }
    return flag;
}

/*
===========================
Function comparison_strings
===========================
Checks the substring for occurrence in the string
string - the first string
substr - the substring
return flag - 0-not include, 1-include
*/

int comparison_strings(char* string, char* substr)
{
    int flag;
    unsigned int i, j;
    flag = 0;
    if(strlen(substr) <= strlen(string))
    {
        for(i = 0;i < strlen(string)-strlen(substr)+1;i++)
        {
            flag = 1;
            if(string[i] == substr[0] || abs((int)string[i] - (int)substr[0]) == 32)
            {
                for(j = 1;j < strlen(substr);j++)   if(substr[j] != string[i+j] && abs((int)substr[j] - (int)string[i+j]) != 32)    flag = 0;
                if(flag)    i = strlen(string)-strlen(substr)+1;
            }
            else   flag = 0;
        }
    }
    return flag;
}

/*
===================
Function clear_list
===================
Clears the list of information
node0 - head of the list of information
*/

void clear_list(headP *node0)
{
    plane *p = NULL, *k = NULL;
    p = node0->first;
    while(p != NULL)
    {
        k = p;
        p = p->next;
        free(k);
        k = NULL;
    }
}

/*
===================
Function clear_ring
===================
Clears the ring of manufacturers
node1 - head of the ring
*/

void clear_ring(country *node1)
{
    int i;
    country *p = NULL, *k = NULL;
    p = node1;
    for(i = 0;i < node1->cnt;i++)
    {
        k = p;
        p = p->next;
        free(k);
        k = NULL;
    }
}

/*
===========================
Function clear_structure
===========================
Clears element of the list of information
p - element of the list
*/

void clear_structure(plane *p)
{
    plane *k = NULL;
    while(p != NULL)
    {
        k = p;
        p = p->next;
        if(k != NULL)   free(k);
        k = NULL;
    }
}

/*
=========================================
Function output_simply_connecte_ring_list
=========================================
Output elements of the ring of manufacturers
node1 - head of the ring
*/

void output_simply_connected_ring_list(country *node1)
{
    country *p = NULL;
    p = node1;
    if(node1->cnt == 0)     puts("The ring is empty!");
    else
    {
        puts("+----------------+");
        printf("|%14s  |\n", "Manufacturer");
        puts("+----------------+");
        do  {printf("|%14s  |\n", p->str);     puts("+----------------+");   p = p->next;}while(p != node1);
    }
}

/*
==================================
Function output_doubly_linked_list
==================================
Output elements of the list of information
node0 - head of the list
*/

void output_doubly_linked_list(headP *node0)
{
    plane *p = NULL;
    if(node0->cnt == 0)     puts("The list is empty!");
    else
    {
        print_header();
        p = node0->first;
        while(p != NULL)
        {
            struct_out(p);
            puts("+----+-------------------------+--------------+--------+---------+---------+---------+----------+---------+------------+");
            p = p->next;
        }
    }
}

/*
==============
Function clear
==============
Clears console
*/

void  clear()
{
    #if defined (__linux__) || defined (__unix__) || defined (__APPLE__)
        system("clear");
    #endif

    #if defined (__WIN32) || defined (_WIN64)
        system("cls");
    #endif
}
