// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define PLANET8_WIDTH  (64)
#define PLANET8_HEIGHT (64)


const u16 planet8Data[] =
{
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x2A30, 0x0000, 0x0000, 0x2A00, 0x2A2A, 0x0000, 0x0000, 0x2A2A, 0xD5D5,
   0x0000, 0x2A00, 0xD52A, 0xD5D5, 0x0000, 0x2A2A, 0xD5D5, 0xD5D5, 0x2A00, 0xD52A,
   0xD5D5, 0xD5D5, 0x0000, 0x0000, 0x0000, 0x0000, 0x2A30, 0xD52A, 0xD5D5, 0xD5D5,
   0x2A2A, 0xD5D5, 0xD5D5, 0xD5D5, 0xD5D5, 0xD5D5, 0xD5D5, 0xD564, 0xD5D5, 0xD5D5,
   0x64D5, 0x6464, 0xD5D5, 0x64D5, 0x6464, 0x7F7F, 0xD5D5, 0x6464, 0x7F7F, 0xD07F,
   0x64D5, 0x7F64, 0xD07F, 0xD0D0, 0x0000, 0x0000, 0x0000, 0x0000, 0xD5D5, 0xD5D5,
   0x64D5, 0x647F, 0xD5D5, 0xD5D5, 0x6464, 0x7F7F, 0xD5D5, 0x7F64, 0xD064, 0xD0D0,
   0x6464, 0x7F7F, 0x7FD0, 0xD0D0, 0xD07F, 0xD07F, 0xD0D0, 0xD0D0, 0xD0D0, 0xD0D0,
   0xD0D0, 0xD0D0, 0xD0D0, 0xD0D0, 0xD0D0, 0x1E1E, 0x0000, 0x0000, 0x0000, 0x0000,
   0x7F7F, 0x6464, 0x6464, 0x7F7F, 0x7F7F, 0xD07F, 0x7F7F, 0xD07F, 0xD07F, 0xD0D0,
   0xD0D0, 0xD0D0, 0xD0D0, 0xD0D0, 0xD0D0, 0xD0D0, 0xD0D0, 0xA4D0, 0xA4A4, 0xA4A4,
   0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xA41E, 0xA4A4, 0xA4A4, 0xA4A4, 0x0000, 0x0000,
   0x0000, 0x0000, 0x7F7F, 0x7F7F, 0x7F7F, 0x7F7F, 0xD0D0, 0xD0D0, 0xD0D0, 0xD0D0,
   0xD0D0, 0xD0D0, 0xD0D0, 0xD0D0, 0xD0D0, 0xA4A4, 0xA4A4, 0xD0A4, 0xA4A4, 0xA4A4,
   0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4,
   0x0000, 0x0000, 0x0000, 0x0000, 0x7F7F, 0x6464, 0x6464, 0xD564, 0xD0D0, 0x7F7F,
   0x7F7F, 0x647F, 0xD0D0, 0xD0D0, 0xD0D0, 0x7FD0, 0xD0D0, 0xD0D0, 0xD0D0, 0xD0D0,
   0xA4A4, 0xA4A4, 0xD0A4, 0xD0D0, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4,
   0xA4A4, 0xA4A4, 0x0000, 0x0000, 0x0000, 0x0000, 0xD5D5, 0x2AD5, 0x0000, 0x0000,
   0x6464, 0xD5D5, 0x00D5, 0x0000, 0x7F7F, 0xD564, 0xD5D5, 0x0000, 0xD0D0, 0x7F7F,
   0xD564, 0x00D5, 0xD0D0, 0xD0D0, 0x7F7F, 0xD564, 0xD0D0, 0xD0D0, 0xD0D0, 0x647F,
   0xA4A4, 0xD0A4, 0xD0D0, 0xD0D0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00D5, 0x0000,
   0x0000, 0x0000, 0xD564, 0x0000, 0x0000, 0x0000, 0x2A2A, 0xD5D5, 0xD5D5, 0xD5D5,
   0xD52A, 0xD5D5, 0xD5D5, 0x64D5, 0xD52A, 0xD5D5, 0xD5D5, 0x6464, 0xD5D5, 0xD5D5,
   0x64D5, 0x7F64, 0xD5D5, 0xD5D5, 0x6464, 0x7F7F, 0xD5D5, 0xD5D5, 0x7F64, 0xD07F,
   0xD5D5, 0x6464, 0x7F64, 0xD0D0, 0xD5D5, 0x6464, 0xD07F, 0xD0D0, 0x6464, 0x7F7F,
   0xD0D0, 0xD0D0, 0x7F64, 0xD07F, 0xD0D0, 0xD0D0, 0x7F7F, 0xD0D0, 0xD0D0, 0xD0D0,
   0xD07F, 0xD0D0, 0xD0D0, 0x1ED0, 0xD0D0, 0xD0D0, 0xD0D0, 0xA41E, 0xD0D0, 0xD0D0,
   0x1ED0, 0x1E1E, 0xD0D0, 0xD0D0, 0x1E1E, 0x1E1E, 0xD0D0, 0xA4D0, 0x1E1E, 0x1E1E,
   0xD0D0, 0xD0D0, 0x1E1E, 0xA41E, 0xD0D0, 0x1E1E, 0x1E1E, 0x1E1E, 0x1EA4, 0x1E1E,
   0x1E1E, 0x1E1E, 0x1E1E, 0x1E1E, 0x1E1E, 0xCB1E, 0x1E1E, 0x1E1E, 0xCB1E, 0xCBCB,
   0x1E1E, 0x1E1E, 0xCB1E, 0xB0CB, 0x1E1E, 0xCB1E, 0xCB1E, 0xB0B0, 0xCB1E, 0x1ECB,
   0xB0B0, 0xB0B0, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0x1E1E,
   0xA4A4, 0x1E1E, 0xCB1E, 0x8F1E, 0xCBCB, 0xB01E, 0x8F8F, 0x8FB0, 0xB01E, 0xB0B0,
   0xB0B0, 0x9494, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0x6CB0,
   0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xA4A4, 0x1E1E, 0x1E1E, 0x1E1E, 0x1E1E, 0x1E1E,
   0x8F8F, 0x8F8F, 0x8F8F, 0x8F8F, 0x8F8F, 0x8F8F, 0x948F, 0x9494, 0x9494, 0x9494,
   0xB094, 0x94B0, 0xB0B0, 0x6C6C, 0xB0B0, 0x6C6C, 0xB0B0, 0x6C6C, 0x6C6C, 0x6CB0,
   0x6C6C, 0x6CB0, 0xB06C, 0xB0B0, 0xB0B0, 0x0F0F, 0x1E1E, 0xA41E, 0xA4A4, 0xA4A4,
   0x1E1E, 0x1E1E, 0x1E1E, 0xA41E, 0x8F8F, 0x8F8F, 0x1E8F, 0x1E1E, 0x9494, 0x9494,
   0x8F94, 0x8F8F, 0x6C6C, 0x9494, 0x9494, 0x9494, 0x6C6C, 0x6C6C, 0x6C6C, 0x946C,
   0x6C6C, 0x6C6C, 0x6C6C, 0x6C6C, 0x0F0F, 0x0F0F, 0x6C6C, 0x6C6C, 0xA4A4, 0xA4A4,
   0xA4A4, 0xD0D0, 0xA4A4, 0xA4A4, 0xA4A4, 0xD0A4, 0x1E1E, 0xA4A4, 0xA4A4, 0xA4A4,
   0x8F8F, 0x1E1E, 0xA4A4, 0xA4A4, 0x8F8F, 0x1E8F, 0x1E1E, 0xA4A4, 0x9494, 0x8F94,
   0x8F8F, 0xA41E, 0x6C6C, 0x9494, 0x8F94, 0x1E8F, 0x6C6C, 0x6C6C, 0x946C, 0x8F94,
   0x64D0, 0x00D5, 0x0000, 0x0000, 0xD0D0, 0xD5D5, 0x0000, 0x0000, 0xD0D0, 0xD57F,
   0x00D5, 0x0000, 0xA4A4, 0x7FD0, 0x00D5, 0x0000, 0xA4A4, 0xD0D0, 0xD564, 0x0000,
   0xA4A4, 0xA4A4, 0xD5D0, 0x00D5, 0xA41E, 0xA4A4, 0xD0D0, 0x00D5, 0x1E1E, 0xA4A4,
   0xA4A4, 0xD57F, 0xD5D5, 0x7FD5, 0xD07F, 0xD0D0, 0xD5D5, 0x7F7F, 0xD07F, 0xD0D0,
   0x6464, 0xD07F, 0xD07F, 0xD0D0, 0x7F64, 0xD07F, 0xD0D0, 0xD0D0, 0x7FD5, 0xD0D0,
   0xD0D0, 0xA4D0, 0x7F7F, 0xD0D0, 0xD0D0, 0x1ED0, 0x7F7F, 0xD0D0, 0xD0D0, 0xA4A4,
   0xD07F, 0xD0D0, 0xD0D0, 0x1EA4, 0xD0D0, 0x1E1E, 0x1EA4, 0x1E1E, 0xD0D0, 0x1E1E,
   0xA41E, 0x1E1E, 0x1ED0, 0x1E1E, 0xA41E, 0xCB1E, 0x1EA4, 0x1E1E, 0x1EA4, 0x1ECB,
   0xA4A4, 0x1EA4, 0x1E1E, 0xB0CB, 0xA4A4, 0xA4A4, 0x1E1E, 0x8F8F, 0xA41E, 0xA4A4,
   0x1E1E, 0x8F8F, 0x1E1E, 0xCBA4, 0xB0CB, 0x948F, 0xCBCB, 0x8FCB, 0xB0B0, 0xB0B0,
   0xCBCB, 0x8FB0, 0xB0B0, 0xB0B0, 0xB0CB, 0x94B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB094,
   0xB0B0, 0xB0B0, 0x94B0, 0x6CB0, 0x6CB0, 0xB0B0, 0xB08F, 0x6CB0, 0xB06C, 0xB0B0,
   0x94B0, 0xB06C, 0xB06C, 0xB0B0, 0x6C94, 0x6C6C, 0xB06C, 0xB00F, 0xB0B0, 0xB0B0,
   0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0,
   0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0x0FB0, 0xB0B0, 0xB0B0,
   0xB0B0, 0x68B0, 0xB0B0, 0xB0B0, 0xB0B0, 0x6868, 0xB0B0, 0xB0B0, 0xB0B0, 0x6868,
   0x0FB0, 0xB00F, 0xB0B0, 0x0FB0, 0xB0B0, 0xB0B0, 0xB0B0, 0x0FB0, 0xB0B0, 0xB0B0,
   0xB0B0, 0x0FB0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB068,
   0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0x0808, 0xB0B0, 0x08B0,
   0x0808, 0x0808, 0x0F0F, 0x0F0F, 0x0F0F, 0x0F0F, 0x0F0F, 0x0F0F, 0x0F0F, 0x0F0F,
   0x6868, 0x68B0, 0x0F68, 0x0F0F, 0xB0B0, 0x68B0, 0x68B0, 0x6868, 0xB0B0, 0xB0B0,
   0x6868, 0x6868, 0xB0B0, 0xB0B0, 0xB0B0, 0x68B0, 0x0808, 0x0808, 0x0808, 0x0808,
   0x0808, 0x0808, 0x0808, 0x0808, 0x6C6C, 0x6C6C, 0x6C6C, 0x9494, 0x0F0F, 0x6C0F,
   0x6C6C, 0x946C, 0x0F0F, 0x0F0F, 0x6C0F, 0x6C6C, 0x0F0F, 0x0F0F, 0x0F0F, 0x6C6C,
   0x6868, 0x0F68, 0x0F0F, 0x0F0F, 0xB068, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0,
   0xB0B0, 0xB0B0, 0x0808, 0xB008, 0xB0B0, 0xB0B0, 0x1E8F, 0xA41E, 0xA4A4, 0xD5D0,
   0x8F94, 0xA41E, 0xA4A4, 0x7FA4, 0x9494, 0x1E8F, 0xA41E, 0xD0A4, 0x946C, 0x8F94,
   0xA41E, 0xA4A4, 0x6C6C, 0x9494, 0x1E8F, 0xA4A4, 0x6C6C, 0x946C, 0x8F8F, 0xA4A4,
   0x6CB0, 0x6C6C, 0x8F94, 0xA41E, 0xB0B0, 0x6C0F, 0x946C, 0xA48F, 0xD07F, 0xD0D0,
   0xD0D0, 0x1E1E, 0xD07F, 0xD0D0, 0x1ED0, 0xA41E, 0xD0D0, 0xD0D0, 0x1ED0, 0x1E1E,
   0xD0D0, 0xD0D0, 0x1ED0, 0x1E1E, 0xD0D0, 0xD0D0, 0x1ED0, 0x1E1E, 0xD0D0, 0xD0D0,
   0x1ED0, 0x1E1E, 0xD0D0, 0xD0D0, 0x1ED0, 0x1E1E, 0x7FD0, 0xD0D0, 0x1E1E, 0x1E1E,
   0x1E1E, 0xCB1E, 0xB0B0, 0x9494, 0x1E1E, 0xCBCB, 0x8F8F, 0xB0B0, 0x1E1E, 0x1ECB,
   0xB0B0, 0x6C94, 0x1E1E, 0xB0CB, 0xB0B0, 0x6CB0, 0xA41E, 0xB0CB, 0xB0B0, 0xB094,
   0x1E1E, 0xB0CB, 0xB0B0, 0xB0B0, 0xCB1E, 0xB0CB, 0xB0B0, 0xB0B0, 0xCB1E, 0xB0CB,
   0xB0B0, 0x6CB0, 0x6C6C, 0x6C6C, 0x0F6C, 0x0F0F, 0x6C6C, 0x6C6C, 0x0F0F, 0x0F0F,
   0xB0B0, 0xB06C, 0x0FB0, 0x0F0F, 0xB06C, 0x0FB0, 0xB0B0, 0x68B0, 0x6CB0, 0xB00F,
   0x0F0F, 0x680F, 0x6C6C, 0x0F0F, 0x0F0F, 0x680F, 0x6C6C, 0x0F0F, 0x0F0F, 0x6868,
   0x6C6C, 0x0F0F, 0x0F0F, 0x6868, 0x0FB0, 0x6868, 0xB068, 0x68B0, 0x680F, 0x6868,
   0x6868, 0x08B0, 0x6868, 0x6868, 0x6868, 0x0808, 0x6868, 0x6868, 0x08B0, 0x0808,
   0x6868, 0x6868, 0x0868, 0x0808, 0x68B0, 0x6868, 0x08B0, 0x0808, 0x6868, 0x0868,
   0x0808, 0x0808, 0xB068, 0x0808, 0x0808, 0x0808, 0x0868, 0x0808, 0x0808, 0x0808,
   0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808,
   0x0808, 0x0808, 0x0808, 0x0808, 0xBF08, 0x08BF, 0x0808, 0x0808, 0x0808, 0x0808,
   0xBF08, 0x0808, 0x0808, 0x0808, 0xBF08, 0x0808, 0x0808, 0xBF08, 0x0808, 0x0808,
   0x0808, 0x0808, 0x0808, 0xBF08, 0xBFBF, 0x08BF, 0xBFBF, 0x08BF, 0xBFBF, 0xBFBF,
   0xBF08, 0xBF08, 0x0808, 0xBFBF, 0x0808, 0xBFBF, 0x0808, 0xBF08, 0x08BF, 0xBFBF,
   0xBFBF, 0xBFBF, 0xBFBF, 0xBFBF, 0xBF08, 0xBFBF, 0x0808, 0xBF08, 0x08BF, 0xBFBF,
   0x0808, 0x0808, 0xB008, 0xB0B0, 0x0808, 0x0808, 0x0808, 0xB0B0, 0xBFBF, 0x0808,
   0x0808, 0x0808, 0xBFBF, 0xBFBF, 0x0808, 0x0808, 0xBFBF, 0xBFBF, 0x08BF, 0x0808,
   0xBFBF, 0xBFBF, 0x08BF, 0x0808, 0xBFBF, 0xBFBF, 0xBFBF, 0x0808, 0xBFBF, 0xBFBF,
   0xBFBF, 0x08BF, 0xB0B0, 0x6C0F, 0x946C, 0x1E8F, 0xB0B0, 0x0FB0, 0x6CB0, 0x1E8F,
   0x0808, 0x0FB0, 0x6C6C, 0x8F94, 0x0808, 0x0FB0, 0x6C0F, 0x946C, 0xB008, 0x0FB0,
   0x6CB0, 0x946C, 0x0808, 0xB0B0, 0x6C0F, 0x946C, 0x0808, 0xB0B0, 0x0FB0, 0x94B0,
   0x0808, 0xB0B0, 0xB0B0, 0x6CB0, 0xD0D0, 0xD0D0, 0x1ED0, 0x1E1E, 0xD07F, 0xD0D0,
   0x1E1E, 0x1E1E, 0xD0D0, 0xD0D0, 0x1E1E, 0x1E1E, 0xD0D0, 0xD0D0, 0x1ED0, 0x1E1E,
   0xD0D0, 0xD0D0, 0x1ED0, 0x1E1E, 0xD07F, 0xD0D0, 0x1ED0, 0x1E1E, 0xD07F, 0xD0D0,
   0x1ED0, 0x1E1E, 0xD07F, 0xD0D0, 0x1ED0, 0x1E1E, 0xCB1E, 0xB0CB, 0xB0B0, 0xB06C,
   0xCB1E, 0xB0CB, 0xB0B0, 0xB0B0, 0xCB1E, 0xB0CB, 0xB0B0, 0xB0B0, 0xCB1E, 0xB0CB,
   0xB0B0, 0xB0B0, 0xCB1E, 0xB0CB, 0xB0B0, 0xB0B0, 0xCB1E, 0xB0CB, 0xB0B0, 0xB0B0,
   0xCB1E, 0xB0CB, 0xB0B0, 0xB0B0, 0x1E1E, 0xB0CB, 0xB0B0, 0xB0B0, 0x6C6C, 0x0F0F,
   0x680F, 0x6868, 0x0F6C, 0x0F0F, 0xB00F, 0x6868, 0x0FB0, 0x0F0F, 0x680F, 0xB068,
   0xB0B0, 0x0F0F, 0xB00F, 0xB068, 0xB0B0, 0xB00F, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0,
   0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0x08B0, 0x0808, 0xB0B0, 0xB0B0, 0x08B0, 0x0808,
   0xB068, 0x0808, 0x0808, 0x0808, 0x08B0, 0x0808, 0x0808, 0x0808, 0x08B0, 0x0808,
   0x0808, 0x0808, 0x08B0, 0x0808, 0x0808, 0xBF08, 0x08B0, 0x0808, 0x0808, 0xBF08,
   0x08B0, 0x0808, 0x0808, 0x0808, 0x08B0, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808,
   0x0808, 0x0808, 0x0808, 0xBF08, 0xBFBF, 0xBFBF, 0x0808, 0xBFBF, 0xBFBF, 0xBFBF,
   0xBF08, 0xBFBF, 0xBFBF, 0xBFBF, 0x08BF, 0xBFBF, 0xBFBF, 0xAD08, 0x08BF, 0x08BF,
   0x08BF, 0x08AD, 0x0808, 0x08BF, 0x0808, 0x0808, 0x0808, 0x0808, 0x08BF, 0x0808,
   0x0808, 0x0808, 0x0808, 0x0808, 0x08BF, 0x0808, 0x0808, 0xAD08, 0x08BF, 0x0808,
   0x0808, 0x0808, 0x08AD, 0x0808, 0x0808, 0xBFBF, 0xADAD, 0xAD08, 0xBFBF, 0xBFBF,
   0x0808, 0xBFBF, 0xBFBF, 0xADBF, 0xBF08, 0xBFBF, 0xBFBF, 0xBFBF, 0xBFBF, 0xBFBF,
   0xBFBF, 0xBFBF, 0xBFBF, 0xBFBF, 0xBFBF, 0xBFBF, 0xBFAD, 0xBFBF, 0xBFBF, 0xBFBF,
   0xADAD, 0xBFAD, 0xBF08, 0xBFBF, 0x0808, 0x08AD, 0x08BF, 0x0808, 0xBFBF, 0x08BF,
   0x08AD, 0x0808, 0xBFBF, 0xBFAD, 0x0808, 0x0808, 0xADBF, 0xBFBF, 0x08BF, 0x0808,
   0xADAD, 0xBFBF, 0x08BF, 0x0808, 0xBFBF, 0xBFBF, 0x08BF, 0x0808, 0x0808, 0xB0B0,
   0xB0B0, 0xB0B0, 0x0808, 0xB008, 0xB0B0, 0xB0B0, 0x0808, 0xB008, 0xB008, 0xB0B0,
   0x0808, 0xB008, 0xB008, 0xB0B0, 0x0808, 0x0808, 0xB008, 0xB0B0, 0x0808, 0x0808,
   0xB008, 0xB0B0, 0x0808, 0x0808, 0xB0B0, 0xB0B0, 0x0808, 0xB008, 0xB0B0, 0x1EB0,
   0xD07F, 0xD0D0, 0x1ED0, 0x1E1E, 0x7F7F, 0xD0D0, 0xD0D0, 0x1E1E, 0x647F, 0xD0D0,
   0xD0D0, 0x1E1E, 0x6464, 0xD0D0, 0xD0D0, 0x1E1E, 0x7F64, 0xD064, 0xD0D0, 0x1ED0,
   0xD564, 0xD07F, 0xD0D0, 0x1ED0, 0xD5D5, 0x6464, 0xD0D0, 0xD0D0, 0xD5D5, 0x64D5,
   0xD0D0, 0xD0D0, 0x1E1E, 0xB0CB, 0xB0B0, 0xB0B0, 0x1E1E, 0xCBCB, 0xB0B0, 0xB0B0,
   0x1E1E, 0xCBCB, 0xB0B0, 0xB0B0, 0x1E1E, 0xCB1E, 0xB0B0, 0xB0B0, 0x1E1E, 0xCB1E,
   0xB0CB, 0xB0B0, 0x1E1E, 0x1E1E, 0xB0CB, 0xB0B0, 0x1E1E, 0x1E1E, 0xCBCB, 0xB0B0,
   0x1E1E, 0x1E1E, 0xCB1E, 0xB0B0, 0xB0B0, 0xB0B0, 0x08B0, 0x0808, 0xB0B0, 0xB0B0,
   0xB0B0, 0x0808, 0xB0B0, 0xB0B0, 0xB0B0, 0x0808, 0xB0B0, 0xB0B0, 0xB0B0, 0xB008,
   0xB0B0, 0xB0B0, 0xB0B0, 0x08B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0,
   0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0x08B0, 0x0808, 0x0808, 0x0808,
   0x0808, 0x0808, 0x0808, 0x0808, 0x08B0, 0x0808, 0x0808, 0xBF08, 0xB0B0, 0x0808,
   0x0808, 0x0808, 0xB0B0, 0x08B0, 0x0808, 0x0808, 0xB008, 0x08B0, 0x0808, 0x0808,
   0xB0B0, 0xB0B0, 0x0868, 0x0808, 0x68B0, 0xB0B0, 0x68B0, 0x0808, 0x0808, 0x0808,
   0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808,
   0x0808, 0x0808, 0x0808, 0x0808, 0xBF08, 0x0808, 0x0808, 0xBF08, 0x0808, 0x0808,
   0xBF08, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0xBF08, 0xBFBF, 0xBF08,
   0xBFBF, 0xBFBF, 0xBFBF, 0xBFBF, 0xBFBF, 0xADBF, 0xBFAD, 0xBFBF, 0xBFBF, 0xADAD,
   0xBFBF, 0xADBF, 0xADBF, 0xBFAD, 0xBFBF, 0xADBF, 0xAD08, 0xBFBF, 0xBFBF, 0xADAD,
   0x0808, 0xBFAD, 0xBFBF, 0xADAD, 0xBF08, 0xBFBF, 0xBFBF, 0xADAD, 0x08BF, 0x0808,
   0xBF08, 0xBFBF, 0xBFBF, 0xBFBF, 0xBFBF, 0x08BF, 0xBFBF, 0xADBF, 0x08BF, 0xBF08,
   0xBFBF, 0xBFAD, 0x08BF, 0x0808, 0xADAD, 0xBFBF, 0x08BF, 0x0808, 0xADAD, 0xBFAD,
   0x0808, 0x0808, 0xADAD, 0x08BF, 0x0808, 0x0808, 0xBFAD, 0xBFBF, 0x0808, 0x0808,
   0xBFBF, 0x08BF, 0xBFBF, 0x0808, 0x08BF, 0xB008, 0xB0B0, 0x1EB0, 0x0808, 0xB008,
   0xB0B0, 0xD0B0, 0x0808, 0x0808, 0xB0B0, 0xD51E, 0x0808, 0xB008, 0xB0B0, 0xD5D0,
   0xB008, 0xB0B0, 0xA4B0, 0x00D5, 0xB008, 0xB0B0, 0xD0CB, 0x00D5, 0xB008, 0xB0B0,
   0xD51E, 0x0000, 0xB0B0, 0x1EB0, 0x0064, 0x0000, 0xD5D5, 0x7FD5, 0xD07F, 0xD0D0,
   0xD5D5, 0xD5D5, 0xD07F, 0xD0D0, 0xD5D5, 0x64D5, 0x7F64, 0xD0D0, 0xD52A, 0xD5D5,
   0x64D5, 0xD07F, 0x2A00, 0xD5D5, 0xD5D5, 0x6464, 0x0000, 0xD52A, 0xD5D5, 0x64D5,
   0x0000, 0x2A00, 0xD5D5, 0xD5D5, 0x0000, 0x0000, 0xD52A, 0xD5D5, 0x1ED0, 0x1E1E,
   0xCB1E, 0xB0CB, 0x1ED0, 0x1E1E, 0x1E1E, 0xB0CB, 0xD0D0, 0x1E1E, 0x1E1E, 0xCB1E,
   0xD0D0, 0x1ED0, 0x1E1E, 0xCB1E, 0xD07F, 0xD0D0, 0x1E1E, 0x1E1E, 0xD07F, 0xD0D0,
   0xA4D0, 0x1E1E, 0x6464, 0xD07F, 0xD0D0, 0xA4A4, 0x64D5, 0x7F7F, 0xD0D0, 0xA4D0,
   0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0CB, 0xB0B0,
   0xB0B0, 0xB0B0, 0xB0CB, 0xB0B0, 0xB0B0, 0xB0B0, 0xCB1E, 0xB0B0, 0xB0B0, 0xB0B0,
   0xA41E, 0x1E1E, 0xB0B0, 0xB0B0, 0x1EA4, 0x1EA4, 0xB01E, 0xB0B0, 0xA4A4, 0xA4A4,
   0x1E1E, 0xB0CB, 0xB0B0, 0xB0B0, 0xB0B0, 0x08B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0,
   0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0,
   0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0x0F6C, 0x0F0F, 0xB0B0, 0x6CB0, 0xB06C, 0x6C6C,
   0x8FB0, 0xB094, 0x6C6C, 0x6CB0, 0x0808, 0x0808, 0xBFBF, 0xBFBF, 0x0808, 0x0808,
   0xBF08, 0xBFBF, 0xB0B0, 0x0808, 0x0808, 0xBF08, 0xB0B0, 0x6868, 0x0808, 0x0808,
   0x68B0, 0x6868, 0xB068, 0x0868, 0xB00F, 0xB068, 0x68B0, 0x6868, 0x0F0F, 0x0F0F,
   0x0F0F, 0x68B0, 0x0F6C, 0x0F0F, 0xB00F, 0xB0B0, 0xBFBF, 0xBFBF, 0xBF08, 0xBFBF,
   0xBFBF, 0x0808, 0xBF08, 0xBFBF, 0xBFBF, 0xBFBF, 0xBFBF, 0xBFBF, 0x0808, 0x0808,
   0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x6868, 0x6868, 0x6868, 0x6868, 0x6868,
   0x6868, 0x6868, 0x0F0F, 0x0F0F, 0x0F0F, 0x0F0F, 0x0F0F, 0x6C6C, 0xBFBF, 0x08BF,
   0x0808, 0x6808, 0xBFBF, 0x0808, 0xB008, 0x68B0, 0x0808, 0xB008, 0xB0B0, 0xB0B0,
   0x6808, 0xB0B0, 0x0F68, 0xB00F, 0x6868, 0x0F68, 0x0F0F, 0x1EB0, 0x0F68, 0x6C0F,
   0x946C, 0xD51E, 0x6C0F, 0x946C, 0xD0A4, 0x00D5, 0x946C, 0xA48F, 0xD5D5, 0x0000,
   0xB0B0, 0xD0B0, 0x00D5, 0x0000, 0xB0B0, 0xD51E, 0x0000, 0x0000, 0x1EB0, 0x00D5,
   0x0000, 0x0000, 0xD51E, 0x0000, 0x0000, 0x0000, 0x00D5, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x3000, 0xD5D5, 0x0000, 0x0000, 0x0000, 0x2A30,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0xD5D5, 0x6464, 0xD07F, 0xD0D0, 0xD5D5, 0x64D5,
   0xD064, 0xD07F, 0x2A00, 0xD5D5, 0x64D5, 0x7F7F, 0x0000, 0xD500, 0xD5D5, 0x64D5,
   0x0000, 0x0000, 0x2A00, 0xD5D5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xA4D0, 0x1EA4, 0x1EA4, 0x1E1E,
   0xD0D0, 0xA4D0, 0xA4A4, 0x1EA4, 0xD07F, 0xD0D0, 0x1ED0, 0xA4A4, 0x7F7F, 0xD07F,
   0xD0D0, 0xA4D0, 0x64D5, 0x7F7F, 0xD07F, 0xD0D0, 0xD5D5, 0xD5D5, 0x7F64, 0xD0D0,
   0x0000, 0x3000, 0xD5D5, 0xD5D5, 0x0000, 0x0000, 0x0000, 0x0000, 0x8FCB, 0xB08F,
   0x948F, 0x6C6C, 0xA4CB, 0x8F1E, 0x8F8F, 0x948F, 0x1EA4, 0x1EA4, 0x1E1E, 0x1E1E,
   0xA4A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xD0A4, 0xA4A4, 0xA4A4, 0xA4A4, 0xD0D0, 0xD0D0,
   0xD0D0, 0xA4D0, 0xD5D5, 0xD5D5, 0xD5D5, 0x64D5, 0x0000, 0x0000, 0x0000, 0x0000,
   0x6C6C, 0x6C6C, 0xB06C, 0x6CB0, 0x9494, 0x6C94, 0x6C6C, 0x6C6C, 0x8F8F, 0x8F94,
   0x948F, 0x8F94, 0x1EA4, 0x1E1E, 0x1E8F, 0x1E8F, 0xA4A4, 0xA4A4, 0x1EA4, 0xA41E,
   0xA4A4, 0xA4A4, 0xD0D0, 0xD57F, 0x6464, 0xD57F, 0x2AD5, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x6C0F, 0x6C0F, 0x6C6C, 0x6C94, 0x6C6C, 0x6C94, 0x946C, 0x1E8F,
   0x948F, 0x8FB0, 0xA41E, 0x7FA4, 0xA4CB, 0xA4A4, 0x64D0, 0x00D5, 0xD0A4, 0xD564,
   0x0000, 0x0000, 0x00D5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0xA48F, 0xD57F, 0x0000, 0x0000, 0xD5D0, 0x00D5,
   0x0000, 0x0000, 0x00D5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};