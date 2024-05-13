#ifndef __MLD__
#include<assert.h>
#include<string.h>
#include<stdlib.h>


#define MAX_STRUCTURE_NAME_SIZE 128
#define MAX_FIELD_NAME_SIZE 128

typedef enum{

UINT8,
UINT32,
INT32,
CHAR,
OBJ_PTR,
FLOAT,
DOUBLE,
OBJ_STRUCT

}data_type_t;


#define OFFSETOF(struct_name, fld_name)     \
    (unsigned long)&(((struct_name *)0)->fld_name)
    
#define FIELD_SIZE(struct_name, fld_name)   \
    sizeof(((struct_name *)0)->fld_name)
    
#define FIELD_INFO(struct_name, fld_name, dtype, nested_struct_name)    \
   {#fld_name, dtype, FIELD_SIZE(struct_name, fld_name),                \
        OFFSETOF(struct_name, fld_name), #nested_struct_name} 




typedef enum{
MLD_FALSE,MLD_TRUE

}mld_boolean_t;

typedef struct field_info_t{

char fname[MAX_FIELD_NAME_SIZE];
data_type_t dtype;
unsigned int size;
unsigned int offset;

char nested_str_name[MAX_STRUCTURE_NAME_SIZE];
}field_info;




typedef struct db_rec_t{

struct db_rec_t *next;
char struct_name[MAX_STRUCTURE_NAME_SIZE];
unsigned int ds_size;

unsigned int n_fields;
field_info *fields;



}db_rec;



typedef struct db_t
{

struct db_rec_t *head;
unsigned int count;

}db;



//OBJECT DATABASE[][]

typedef struct object_db_rec_t{

struct object_db_rec_t *next;
void *ptr;//Key
unsigned int units;
db_rec* rec;


mld_boolean_t is_visited;
mld_boolean_t is_root;

}object_db_rec;


typedef struct object_db_t{

db* db;
object_db_rec* head;
unsigned int count;

}object_db;






int add(struct db_t *head,struct db_rec_t *rec);
void mld_init_primitive_data_types_support(struct db_t *struct_db);


void* xcalloc(object_db* object_db,char *struct_name,int units);
void add_object_to_object_db(object_db* obj_db,void *ptr,int units,db_rec *struct_rec);


object_db_rec* object_db_look_up(object_db *obj_db, void *obj_ptr);


void
mld_set_dynamic_object_as_root(object_db *obj_db, void *obj_ptr);

void
init_mld_algorithm(object_db *obj_db);


void
print_structure_db(struct db_t *struct_db);


void
print_structure_rec(struct db_rec_t *struct_rec);




void
mld_init_primitive_data_types_support(struct db_t *struct_db);

#define REG_STRUCT(struct_db, st_name, fields_arr)                    \
    do{                                                               \
        struct db_rec_t *rec = calloc(1, sizeof(struct db_rec_t));    \
        strncpy(rec->struct_name, #st_name, MAX_STRUCTURE_NAME_SIZE); \
        rec->ds_size = sizeof(st_name);                              \
        rec->n_fields = sizeof(fields_arr)/sizeof(struct field_info_t);     \
        rec->fields = fields_arr;                                    \
        if(add(struct_db, rec)){              \
            assert(0);                                               \
        }                                                            \
    }while(0);
   






#endif


void 
print_object_rec(struct object_db_rec_t *obj_rec, int i);


void
print_object_db(struct object_db_t *object_db);

void print_leaked_objects(struct object_db_t* obj);


void MLD(struct object_db_t* object_db);
