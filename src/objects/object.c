#include "object.h"

void sprint_object(gString *gstr, const Object *o) {
  switch (o->ty) {
  case VECTOR:
    sprint_vector(gstr, o->vLiteral);
    break;
  case CONTEXT:
    g_append_back(gstr, "CONTEXT", 9);
    break;
  case BOPER:
    g_append_back(gstr, "BINARYOPR", 9);
    break;
  case UOPER:
    g_append_back(gstr, "UNARYOPR", 8);
    break;
  case FUNC:
    g_append_back(gstr, "todofunc", 8);
    break;
    //sprint_function(gstr, o->fObject);
  case READER: 
    g_append_back(gstr, "READER", 6);
    break;
  case NONE:
    g_append_back(gstr, "NONE", 4);
    break;
  }
}

void free_object(Object *o) {
  switch (o->ty) {
  case VECTOR:
    free_vdliteral(&o->vLiteral);
    break;
  case CONTEXT:
    free_mfcontext(&o->mContext);
    break;
  case BOPER:
    free_bopliteral(&o->bOperation);
    break;
  case UOPER:
    free_uopliteral(&o->uOperation);
    break;
  case FUNC:
    free_fobject(&o->fObject);
    break;
  case READER:
    free_reader(&o->reader);
    break;
  case NONE:
    break;
  }
  memset(o, 0, sizeof(Object));
}




