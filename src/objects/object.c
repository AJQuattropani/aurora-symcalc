#include "object.h"

void sprint_object(gString *gstr, const Object *o) {
  switch (o->ty) {
  case VECTOR:
    sprint_vector(gstr, &o->vLiteral);
    break;
  case CONTEXT:
    g_append_back_c(gstr, "CONTEXT");
    break;
  case BOPER:
    g_append_back_c(gstr, "BINARYOPR");
    break;
  case UOPER:
    g_append_back_c(gstr, "UNARYOPR");
    break;
  case FUNC:
    sprint_function(gstr, &o->fObject);
    g_append_back_c(gstr, "todofunc");
    break;
  case READER: 
    g_append_back_c(gstr, "READER");
    break;
  case NONE:
    g_append_back_c(gstr, "NONE");
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




