#include<assert.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "CSS.h"
#define and &&

#include "mld.h"

char *DATA_TYPE[] = {"UINT8", "UINT32", "INT32",
                     "CHAR", "OBJ_PTR", "VOID_PTR", "FLOAT",
                     "DOUBLE", "OBJ_STRUCT"};


int add(struct db_t *head,struct db_rec_t *rec){

if(head->head==NULL)
{
head->head=rec;
head->head->next=NULL;
return 0;
}

db_rec* itr = head->head;

while(itr!=NULL and itr->next!=NULL)
itr=itr->next;

//Add now
itr->next=rec;
itr->next->next=NULL;
return 0;

}




db_rec* lookup_struct_rec(db *db,char *struct_name)
{
   db_rec* itr = db->head;
   while(itr!=NULL)
   {
      
       if(strncmp(itr->struct_name, struct_name, MAX_STRUCTURE_NAME_SIZE) ==0)return itr;
      itr=itr->next;
   }
   return NULL;
   
  
}

void* xcalloc(object_db* object_db,char *struct_name,int units)
{
db_rec* struct_rec = lookup_struct_rec(object_db->db,struct_name);
assert(struct_rec);
void *ptr = calloc(units,struct_rec->ds_size);
add_object_to_object_db(object_db,ptr,units,struct_rec);
return ptr;
}
//

void add_object_to_object_db(object_db* obj_db,void *ptr,int units,db_rec *struct_rec)
{

object_db_rec *obj_rec;
obj_rec=calloc(1,sizeof(object_db_rec));

obj_rec->next=NULL;
obj_rec->ptr=ptr;
obj_rec->units=units;
obj_rec->rec=struct_rec;

object_db_rec *itr = obj_db->head;

if(itr==NULL)
{

obj_db->head=obj_rec;
obj_db->head->next=NULL;
obj_db->count++;
return;

}

while(itr->next!=NULL)
{
itr=itr->next;

}

itr->next=obj_rec;
obj_db++;
return;

}


object_db_rec* object_db_look_up(object_db *obj_db, void *obj_ptr)
{


object_db_rec* rec = obj_db->head;

if(rec==NULL)return NULL;

while(rec!=NULL)
{
if(rec->ptr==obj_ptr)return rec;
rec=rec->next;
}
return NULL;

}




void
mld_set_dynamic_object_as_root(object_db *obj_db, void *obj_ptr){

    object_db_rec *obj_rec = object_db_look_up(obj_db, obj_ptr);
    assert(obj_rec);
    
    obj_rec->is_root = MLD_TRUE;
}



void
init_mld_algorithm(object_db *obj_db){

     object_db_rec *obj_rec = obj_db->head;
     while(obj_rec){
         obj_rec->is_visited = MLD_FALSE;
         obj_rec = obj_rec->next;
     }
     return;
}



void
print_structure_db(struct db_t *struct_db){
    
    if(!struct_db) return;
    printf("printing STRUCURE DATABASE\n");
    int i = 0;
    struct db_rec_t *struct_rec = NULL;
    struct_rec = struct_db->head;
    printf("No of Structures Registered = %d\n", struct_db->count);
    while(struct_rec){
        printf("structure No : %d (%p)\n", i++, struct_rec);
        print_structure_rec(struct_rec);
        struct_rec = struct_rec->next;
    }
}



void
print_structure_rec(struct db_rec_t *struct_rec){
    if(!struct_rec) return;
    int j = 0;
    field_info *field = NULL;
    printf(ANSI_COLOR_CYAN "|------------------------------------------------------|\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "| %-20s | size = %-8d | #flds = %-3d |\n" ANSI_COLOR_RESET, struct_rec->struct_name, struct_rec->ds_size, struct_rec->n_fields);
    printf(ANSI_COLOR_CYAN "|------------------------------------------------------|------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    for(j = 0; j < struct_rec->n_fields; j++){
        field = &struct_rec->fields[j];
        printf("  %-20s |", "");
        printf("%-3d %-20s | dtype = %-15s | size = %-5d | offset = %-6d|  nstructname = %-20s  |\n",
                j, field->fname, DATA_TYPE[field->dtype], field->size, field->offset, field->nested_str_name);
        printf("  %-20s |", "");
        printf(ANSI_COLOR_CYAN "--------------------------------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    }
}






void
mld_init_primitive_data_types_support(struct db_t *struct_db){

    REG_STRUCT(struct_db, int , 0);
    REG_STRUCT(struct_db, float , 0);
    REG_STRUCT(struct_db, double , 0);
}







void 
print_object_rec(struct object_db_rec_t *obj_rec, int i){
    
    if(!obj_rec) return;
    printf(ANSI_COLOR_MAGENTA "-----------------------------------------------------------------------------------------------------|\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "%-3d ptr = %-10p | next = %-10p | units = %-4d | struct_name = %-10s | is_root = %s |\n"ANSI_COLOR_RESET, 
        i, obj_rec->ptr, obj_rec->next, obj_rec->units, obj_rec->rec->struct_name, obj_rec->is_root ? "TRUE " : "FALSE"); 
    printf(ANSI_COLOR_MAGENTA "-----------------------------------------------------------------------------------------------------|\n"ANSI_COLOR_RESET);
}








void
print_object_db(struct object_db_t *object_db){

    struct object_db_rec_t *head = object_db->head;
    unsigned int i = 0;
    printf(ANSI_COLOR_CYAN "Printing OBJECT DATABASE\n");
    for(; head; head = head->next){
        print_object_rec(head, i++);
    }
}





void print_leaked_objects(struct object_db_t* obj)
{

object_db_rec* rec = obj->head;

while(rec)
{
if(rec->is_visited==MLD_FALSE)
print_object_rec(rec,0);
rec=rec->next;
}
return;
}





void dfs(object_db* obj_db,object_db_rec* parent_obj)
{
   assert(parent_obj->is_visited==MLD_TRUE);
   
   char* parent_obj_ptr = parent_obj->ptr;
   char *offset_next_node=NULL;
   void* next_node=NULL;
   field_info* fld_info=NULL;
   
   db_rec* rec = parent_obj->rec;
   
   if(rec->n_fields==0)return; //nothing to explore inside!
   
   for(int i=0;i<parent_obj->units;i++)
   {
   //now lets start taking and exploring parent_arr[]-->
   parent_obj_ptr=(char*)(parent_obj_ptr)+(i*rec->ds_size);
   
   //now lets star exploring the structure;
   
    for(int j=0;j<rec->n_fields;j++)
    {
       fld_info = &rec->fields[j];
       
       if(fld_info->dtype!=OBJ_PTR)continue;
       
       offset_next_node = parent_obj_ptr+fld_info->offset;
       
       //copying data to child node;
       memcpy(&next_node,offset_next_node,sizeof(void*));
       
       if(next_node==NULL)continue;
       
       object_db_rec* child = object_db_look_up(obj_db,next_node);
       
       assert(child);//if found record;
       
       
       if(!child->is_visited)
       {
         child->is_visited=MLD_TRUE;
         dfs(obj_db,child);
       
       }
       else continue;
       
    
    
    
    }
   
   
   
   }

}
















void MLD(struct object_db_t* object_db)
{
 object_db_rec *root_obj = object_db->head;
 while(root_obj){
       if(root_obj->is_root==MLD_TRUE){
       if(root_obj->is_visited==MLD_FALSE){
    //  printf("\nVisiting:%s\n",root_obj->rec->struct_name);
      root_obj->is_visited=MLD_TRUE;
      dfs(object_db,root_obj);
      }
       
       
       }
       root_obj=root_obj->next;
    }
 
return;
}



