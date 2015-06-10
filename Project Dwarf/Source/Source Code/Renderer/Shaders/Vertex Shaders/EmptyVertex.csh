#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Input
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
//
//
// Runtime generated constant mappings:
//
// Target Reg                               Constant Description
// ---------- --------------------------------------------------
// c0                              Vertex Shader position offset
//
//
// Level9 shader bytecode:
//
    vs_2_0
    def c1, 1, 0, 0, 0
    mov r0.xy, c1
    mad oPos, c0.xyxx, r0.xxyy, r0.yyyx

// approximately 2 instruction slots used
vs_4_0
dcl_output_siv o0.xyzw, position
mov o0.xyzw, l(0,0,0,1.000000)
ret 
// Approximately 2 instruction slots used
#endif

const BYTE EmptyVertex[] =
{
     68,  88,  66,  67,  89,   7, 
    215,  85,  62, 163, 221, 229, 
    182,  12, 187,  66, 152, 172, 
     47, 177,   1,   0,   0,   0, 
      4,   2,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    168,   0,   0,   0, 236,   0, 
      0,   0, 104,   1,   0,   0, 
    192,   1,   0,   0, 208,   1, 
      0,   0,  65, 111, 110,  57, 
    104,   0,   0,   0, 104,   0, 
      0,   0,   0,   2, 254, 255, 
     64,   0,   0,   0,  40,   0, 
      0,   0,   0,   0,  36,   0, 
      0,   0,  36,   0,   0,   0, 
     36,   0,   0,   0,  36,   0, 
      1,   0,  36,   0,   0,   0, 
      0,   0,   0,   2, 254, 255, 
     81,   0,   0,   5,   1,   0, 
     15, 160,   0,   0, 128,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   2,   0,   0, 
      3, 128,   1,   0, 228, 160, 
      4,   0,   0,   4,   0,   0, 
     15, 192,   0,   0,   4, 160, 
      0,   0,  80, 128,   0,   0, 
     21, 128, 255, 255,   0,   0, 
     83,  72,  68,  82,  60,   0, 
      0,   0,  64,   0,   1,   0, 
     15,   0,   0,   0, 103,   0, 
      0,   4, 242,  32,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  54,   0,   0,   8, 
    242,  32,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 128,  63,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    116,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     82,  68,  69,  70,  80,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  28,   0,   0,   0, 
      0,   4, 254, 255,   0,   1, 
      0,   0,  28,   0,   0,   0, 
     77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
     41,  32,  72,  76,  83,  76, 
     32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  54, 
     46,  51,  46,  57,  54,  48, 
     48,  46,  49,  54,  51,  56, 
     52,   0, 171, 171,  73,  83, 
     71,  78,   8,   0,   0,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,  79,  83,  71,  78, 
     44,   0,   0,   0,   1,   0, 
      0,   0,   8,   0,   0,   0, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     83,  86,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0
};