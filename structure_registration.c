

#define MAX_STRUCTURE_NAME_SIZE 128
#define MAX_FIELD_NAME_SIZE 128


typedef struct _field_info{
char fname[MAX_FIELD_NAME_SIZE];
unsigned int size;
unsigned int offset
data_type_t dtype;
char nested_str_name[MAX_STRUCTURE_NAME_SIZE];
}field_info_t;


typedef enum{
UINT8,UINT32,INT32,CHAR,OBJ_PTR,FLOAT,DOUBLE,OBJ_STRUCT
}data_type_t;

typedef struct _struct_db_rec_{


struct _struct_db_rec_ *next;
char struct_name[MAX_STRUCTURE_NAME_SIZE];
unsigned int ds_size;
unsigned int n_fields;

field_infor_t *fields;


}db_rec;