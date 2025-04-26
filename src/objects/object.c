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
    break;
  case PFUNC:
    sprint_function(gstr, o->pObject.fObj);
    break;
  case READER: 
    g_append_back_c(gstr, "READER");
    break;
  case TEMP:
    g_append_back_c(gstr, "TEMP");
    break;
  case SYNTAX_RIGHT:
    g_append_back_c(gstr, "->");
    break;
  case NONE:
    g_append_back_c(gstr, "NONE");
    break;
  }
}

void free_object(Object *obj) {
  switch (obj->ty) {
  case VECTOR:
    free_vdliteral(&obj->vLiteral);
    break;
  case CONTEXT:
    free_mfcontext(&obj->mContext);
    break;
  case BOPER:
    free_bopliteral(&obj->bOperation);
    break;
  case UOPER:
    free_uopliteral(&obj->uOperation);
    break;
  case FUNC:
    free_fobject(&obj->fObject);
    break;
  case PFUNC:
    free_pobject(&obj->pObject);
    break;
  case READER:
    free_reader(&obj->reader);
    break;
  case NONE:
    break;
  default:
    break;
  }
  memset(obj, 0, sizeof(Object));
}

__attribute__((always_inline)) inline Object null_object() {
  return (const Object){{{0}},.ty=NONE,.priority=0};
}
