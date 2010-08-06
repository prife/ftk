#ifndef FTK_PATH_H
#define FTK_PATH_H

#include "ftk_typedef.h"

FTK_BEGIN_DECLS
struct _FtkPath;
typedef struct _FtkPath FtkPath;

FtkPath* ftk_path_create(const char* path);
size_t ftk_path_get_levels(FtkPath* thiz);
Ret ftk_path_set_path(FtkPath* thiz, const char* path);
Ret ftk_path_up(FtkPath* thiz);
Ret ftk_path_down(FtkPath* thiz);
Ret ftk_path_root(FtkPath* thiz);
int ftk_path_is_leaf(FtkPath* thiz);
int ftk_path_current_level(FtkPath* thiz);
const char* ftk_path_full(FtkPath* thiz);
const char* ftk_path_current(FtkPath* thiz);
const char* ftk_path_get_sub(FtkPath* thiz, size_t level);
void ftk_path_destroy(FtkPath* thiz);

FTK_END_DECLS

#endif/*FTK_PATH_H*/
