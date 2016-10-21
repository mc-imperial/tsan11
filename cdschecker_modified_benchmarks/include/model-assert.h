#ifndef __MODEL_ASSERT_H__
#define __MODEL_ASSERT_H__

#if __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#include <cassert>

static void model_assert(bool expr, const char *file, int line) {
  assert(expr && file && line);
}
#define MODEL_ASSERT(expr) model_assert((expr), __FILE__, __LINE__)

#if __cplusplus
}
#endif

#endif /* __MODEL_ASSERT_H__ */
