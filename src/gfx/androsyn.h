// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define ANDROSYN_WIDTH  (32)
#define ANDROSYN_HEIGHT (32)


const u16 androsynData[] =
{
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x00CF, 0x0000, 0x0000, 0xCF00, 0xE062, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x6700, 0x6868, 0x0000, 0x0000, 0x7768, 0xC9C9, 0x0000, 0x0000, 0xC977, 0xC9C9,
   0x0000, 0xD6D6, 0xE2DC, 0xE2E2, 0x0000, 0x0000, 0x9000, 0xC9C3, 0x0000, 0xD6D6,
   0xE2E2, 0xE2E2, 0x0000, 0x0000, 0x00CF, 0x0000, 0x0000, 0xCF00, 0xE0C7, 0x0000,
   0x6867, 0xD668, 0x00CF, 0x0000, 0x7AC9, 0x68D2, 0xEAD6, 0x0000, 0xC9C9, 0xD2E6,
   0xD6D6, 0x0000, 0xE2E2, 0x62CA, 0xE251, 0x0000, 0x70C9, 0x006E, 0x0000, 0x00EA,
   0xCFE2, 0x9851, 0xE2C7, 0x00EA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x6800, 0xC9C3, 0x0000, 0xD6D6, 0xE2E2, 0xE2E2, 0x0000, 0x0000, 0x6800, 0xC9C3,
   0xD600, 0xD6D6, 0xE2DC, 0xCFE2, 0x0000, 0x0000, 0x6800, 0xC9C3, 0xD6D6, 0xE2E2,
   0xE2E2, 0xCFCF, 0x00CF, 0x0000, 0x6800, 0xC9C3, 0x0000, 0xDCD6, 0xD6D6, 0x95CF,
   0x70C9, 0x0090, 0x0000, 0x0000, 0x51CF, 0x9898, 0xE251, 0xEA00, 0x5EC9, 0x0068,
   0x0000, 0x00EA, 0x9851, 0x98E6, 0xCF62, 0x00E2, 0x5EC9, 0x0084, 0x0000, 0x0000,
   0x5151, 0xE698, 0x5198, 0xDCCF, 0x5EC9, 0x0084, 0x0000, 0xE000, 0xE698, 0x6298,
   0xE2CF, 0x00DD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00CF, 0xD6D6, 0xCFD6,
   0x0000, 0x0000, 0xD6D6, 0xD6D6, 0x0000, 0x0000, 0xE0CF, 0xEA00, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x9851, 0xCF51,
   0xE0DD, 0x0000, 0xCFCF, 0xD6CF, 0x0000, 0x0000, 0x0000, 0xE000, 0x0000, 0x0000,
   0xEA00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000,
};

#define ANDROSYN_SIZE (512)



