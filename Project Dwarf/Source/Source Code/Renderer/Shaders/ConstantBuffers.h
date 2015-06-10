#include "BufferDefines.h"


CONSTANT_BUFFER_BEGIN(cb_WorldView, b0)
float4x4 view_matrix;
float4x4 proj_matrix;
CONSTANT_BUFFER_END

CONSTANT_BUFFER_BEGIN(cb_WorldPos, b1)
float4x4 world_matrix;
CONSTANT_BUFFER_END

CONSTANT_BUFFER_BEGIN(cb_BatchPos, b2)
float4x4 batch_matricies[1024];
CONSTANT_BUFFER_END
