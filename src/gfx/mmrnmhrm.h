// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define MMRNMHRM_WIDTH  (32)
#define MMRNMHRM_HEIGHT (32)


const u16 mmrnmhrmData[] =
{
   0xEBE2, 0xEBEB, 0x00EB, 0x0000, 0xE9E0, 0xE9E9, 0xE9E9, 0x00E0, 0xE2E2, 0xE0E0, 
   0x0000, 0x0000, 0xE2DE, 0xE0E0, 0x00E2, 0x0000, 0xE2DE, 0xE0E0, 0x00E0, 0x0000, 
   0x77DE, 0xABAB, 0x77AB, 0x0000, 0xE2DE, 0xE0E0, 0xE095, 0x0000, 0xE2DE, 0xE0E0, 
   0xE0E0, 0x00E2, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xE2DE, 0xE2E2, 
   0xE2E2, 0xDCE0, 0xDEDE, 0xDEDE, 0xDED6, 0xE2E2, 0xE2DE, 0xE2E2, 0xE2E2, 0xE0E0, 
   0xE0E2, 0xE0E0, 0xE0E0, 0xE0E0, 0xE0E2, 0xE0E0, 0xE095, 0x95E0, 0xE9E0, 0xE9E9, 
   0xE995, 0xE0E0, 0xE2DE, 0xE2E2, 0xE2E2, 0xE9E9, 0xE2DE, 0xE0E0, 0xE0E0, 0xE9E0, 
   0x0000, 0x0000, 0x0000, 0x0000, 0xE2E2, 0xE2DC, 0xE2DC, 0x0000, 0xE2E2, 0xD6D6, 
   0x77E2, 0xE2C9, 0xD6D6, 0xCFD6, 0xE2D6, 0x7777, 0xCFCF, 0x98C7, 0xE0CF, 0xABAB, 
   0xE2E0, 0xCFCF, 0xABE0, 0xABAB, 0xE9E9, 0xE9E9, 0xABAB, 0x00E2, 0x00E2, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xC900, 0x0000, 0x0000, 0x0000, 0x7700, 
   0x0000, 0x0000, 0x0000, 0x77C9, 0xDCAB, 0x0000, 0x0000, 0xE077, 0xE0AB, 0x0000, 
   0xDC00, 0xD677, 0x00E2, 0x0000, 0x7700, 0xCFD6, 0x0000, 0x0000, 0x7700, 0x51D6, 
   0x0000, 0x0000, 0xDC00, 0x98E2, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x00AB, 0x0000, 0x0000, 0x0000, 0x00AB, 0x0000, 0x0000, 0x0000, 
   0xDCAB, 0x0000, 0x0000, 0x0000, 0xAB51, 0x0000, 0x0000, 0x0000, 0xAB98, 0x0000, 
   0x0000, 0x0000, 0xE0E0, 0x0000, 0x0000, 0x0000, 0xE2DE, 0xE0E0, 0xE0E0, 0xE2E9, 
   0xE2DE, 0xE0E0, 0xE9E0, 0x00E2, 0x77DE, 0xABAB, 0x77AB, 0x0000, 0xE2DE, 0xE0E0, 
   0x00E9, 0x0000, 0xE2DE, 0xE9E0, 0x00E2, 0x0000, 0xE0E2, 0xE9E0, 0x0000, 0x0000, 
   0xE9E0, 0xE9E9, 0xE9E9, 0x00E0, 0x00E2, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0xE200, 0xCFE2, 0x0000, 0x0000, 0xDC00, 0xE0E0, 0x0000, 0x0000, 
   0xE200, 0xE0E0, 0x0000, 0x00C9, 0x00DC, 0xE0E2, 0x0000, 0x0068, 0x00E2, 0xE0E2, 
   0x0000, 0xDC68, 0x00E2, 0xE0E2, 0x0000, 0xE067, 0x00E2, 0xE0E2, 0xC900, 0xE09C, 
   0x00E2, 0xE0E2, 0xE0E0, 0x0000, 0x0000, 0x0000, 0xE6E0, 0x0000, 0x0000, 0x0000, 
   0xE6E0, 0x0000, 0x0000, 0x0000, 0xE2E6, 0x00E2, 0x0039, 0x0000, 0xE295, 0x00E0, 
   0x0046, 0x0000, 0xE2E6, 0xE2E0, 0x009C, 0x0000, 0xE2E9, 0xE0E0, 0x00D2, 0x0000, 
   0xE2E6, 0xE0E0, 0xC946, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xE200, 
   0x0000, 0x0000, 0x0000, 0xE000, 0x0000, 0x0000, 0x0000, 0x9500, 0x0000, 0xE0E0, 
   0x00E2, 0xE0E2, 0xDC00, 0xE0E0, 0x00E2, 0xE0E2, 0xD200, 0xE0E0, 0x00E2, 0xE0E2, 
   0xE0E2, 0xE0AB, 0x00E9, 0xE700, 0x95E0, 0xABE0, 0x0000, 0x0000, 0xE0E0, 0x00E9, 
   0x0000, 0x0000, 0xE6E0, 0x0000, 0x0000, 0x0000, 0x00E9, 0x0000, 0x0000, 0x0000, 
   0xE2E9, 0xE0E0, 0x00E0, 0x0000, 0xE2E6, 0xE0E0, 0xE2E0, 0x0000, 0xE2E9, 0x95E0, 
   0xD2E0, 0x0000, 0x0000, 0xE0E2, 0xE0AB, 0x00E2, 0x0000, 0xAB00, 0xE0E0, 0x00E0, 
   0x0000, 0x0000, 0xE0E2, 0xE2E0, 0x0000, 0x0000, 0xDC00, 0xE6E0, 0x0000, 0x0000, 
   0x0000, 0xE0E2, 
};



