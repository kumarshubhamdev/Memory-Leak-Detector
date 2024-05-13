#include "mld.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "CSS.h"

#define MAX_NODES 500




typedef struct emp_ {

    char emp_name[30];
    unsigned int emp_id;
    unsigned int age;
    struct emp_ *mgr;
    float salary;
    int *p;
} emp_t;

typedef struct student_{

    char stud_name[32];
    unsigned int rollno;
    unsigned int age;
    float aggregate;
    struct student_ *best_colleage;
} student_t;

int
main(int argc, char **argv){

   
    struct db_t *struct_db = calloc(1, sizeof(struct db_t));
    mld_init_primitive_data_types_support(struct_db);
        
  
    struct field_info_t emp_fields[] = {
        FIELD_INFO(emp_t, emp_name, CHAR,    0),
        FIELD_INFO(emp_t, emp_id,   UINT32,  0),
        FIELD_INFO(emp_t, age,      UINT32,  0),
        FIELD_INFO(emp_t, mgr,      OBJ_PTR, emp_t),
        FIELD_INFO(emp_t, salary,   FLOAT, 0),
        FIELD_INFO(emp_t, p, OBJ_PTR, 0)
    };
   
    REG_STRUCT(struct_db, emp_t, emp_fields);

    
    struct field_info_t stud_fiels[] = {
        FIELD_INFO(student_t, stud_name, CHAR, 0),
        FIELD_INFO(student_t, rollno,    UINT32, 0),
        FIELD_INFO(student_t, age,       UINT32, 0),
        FIELD_INFO(student_t, aggregate, FLOAT, 0),
        FIELD_INFO(student_t, best_colleage, OBJ_PTR, student_t)
    };
    REG_STRUCT(struct_db, student_t, stud_fiels);
    
    print_structure_db(struct_db);

    
    
    
    



    object_db *object_db = calloc(1, sizeof(object_db));
    object_db->db = struct_db;
    
  
    student_t *abhishek = xcalloc(object_db, "student_t", 1);
    mld_set_dynamic_object_as_root(object_db, abhishek);
   
    student_t *shivani = xcalloc(object_db, "student_t", 1);
    strncpy(shivani->stud_name, "shivani", strlen("shivani"));
    
    abhishek->best_colleage=shivani;

    emp_t *joseph = xcalloc(object_db, "emp_t", 2);
    mld_set_dynamic_object_as_root(object_db, joseph);
    joseph->p = xcalloc(object_db, "int", 1);

    print_object_db(object_db);


     //integrating MLD graph algorithm here , no modular
     //function bullshit

    init_mld_algorithm(object_db);
    
    
    //collecting all root objects;
    
    object_db_rec *root_obj = object_db->head;
    
    
     MLD(object_db); //Run the MLD Algorithm
    
printf(ANSI_COLOR_GREEN"\nPrinting Leaked Objects:\n"ANSI_COLOR_RESET);
print_leaked_objects(object_db);
    

    return 0;
}