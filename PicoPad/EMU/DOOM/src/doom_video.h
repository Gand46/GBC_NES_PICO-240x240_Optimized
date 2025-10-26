#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DoomVideoLayout
{
        int origin_x;
        int origin_y;
        int width;
        int height;
} DoomVideoLayout;

const DoomVideoLayout *DoomVideoGetLayout(void);

#ifdef __cplusplus
}
#endif

