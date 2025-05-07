#pragma once

#if NDEBUG
#define __UNREACHABLE_BRANCH __builtin_unreachable();
#else
#define __UNREACHABLE_BRANCH fprintf(stderr, "[FATAL] Reached dangerous exception in %s at %d.\n", __func__, __LINE__); exit(1);
#endif
