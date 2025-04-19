#include "object.h"

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




