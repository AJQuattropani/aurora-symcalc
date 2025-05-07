#include "object.h"

void sprint_object(gString *gstr, const Object *o) {
  switch (o->ty) {
  case VECTOR:
    sprint_vector(gstr, &o->vLiteral);
    return;
  case CONTEXT:
    g_append_back_c(gstr, "CONTEXT");
    return;
  case BOPER:
    g_append_back_c(gstr, "BINARYOPR");
    return;
  case UOPER:
    g_append_back_c(gstr, "UNARYOPR");
    return;
  case FUNC:
    sprint_function(gstr, &o->fObject);
    return;
  case PFUNC:
    sprint_function(gstr, o->pObject.fObj);
    return;
  case READER:
    g_append_back_c(gstr, "READER");
    return;
  case TEMP:
    g_append_back_c(gstr, "TEMP");
    return;
  case SYNTAX_EQUALS:
    g_append_back_c(gstr, "=");
    return;
  case NONE:
    g_append_back_c(gstr, "NONE");
    return;
  }
  __UNREACHABLE_BRANCH
}

void free_object(Object *obj) {
  switch (obj->ty) {
  case SYNTAX_EQUALS:
    free_reader(&obj->reader);
    break;
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
    __UNREACHABLE_BRANCH
  }
  memset(obj, 0, sizeof(Object));
}

__attribute__((always_inline)) inline Object null_object() {
  return (const Object){{{0}}, .ty = NONE, .priority = 0};
}
