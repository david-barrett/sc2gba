// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define BLACKURQ_WIDTH  (32)
#define BLACKURQ_HEIGHT (32)


const u16 blackurqData[] =
{
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xE200, 0x0000, 0x0000, 
   0x0000, 0xEBE3, 0x0000, 0x0000, 0xE200, 0xEBEB, 0x0000, 0x0000, 0xEBE3, 0xEBEB, 
   0x0000, 0x0000, 0xEBE3, 0xE3EB, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0xE400, 0xE4E4, 0x0000, 0x0000, 0xE400, 0xEBEB, 0xE3E3, 0xE3E3, 0xE4E4, 0xEBEB, 
   0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xE3EB, 0xEBE3, 0xE3E4, 0xE3E3, 0xE3E3, 0xE3E3, 
   0xE3E3, 0xE4E3, 0xEBE3, 0xEBE3, 0xE3E3, 0xEBEB, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x00E4, 0x0000, 0x0000, 0x0000, 0xE2EB, 0xE4E4, 
   0x0000, 0x0000, 0xEBEB, 0xEBEB, 0x00E2, 0x0000, 0xE3E3, 0xEBEB, 0xD8EB, 0x0000, 
   0xE4E4, 0xEBEB, 0xE2EB, 0x0000, 0xE4EB, 0xEBE3, 0xE2EB, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xE400, 
   0x0000, 0xE400, 0xEBE3, 0xEBE3, 0x0000, 0xE300, 0xEBE3, 0xEBEB, 0x0000, 0x0000, 
   0xE3E3, 0xEBEB, 0x0000, 0xE300, 0xE3E3, 0xEBEB, 0x0000, 0xE300, 0xEBE3, 0xEBEB, 
   0x0000, 0xE3E3, 0xE3E3, 0xEBEB, 0xE3E4, 0xE3E3, 0xE3E3, 0xE3E4, 0xE3EB, 0xEBEB, 
   0xEBEB, 0xE3EB, 0xE3EB, 0xEBE3, 0xEBEB, 0xBF67, 0xEBEB, 0xE4EB, 0xEBEB, 0xBFCB, 
   0xEBEB, 0xEBEB, 0xE3E3, 0xE3E3, 0xE3E4, 0xEBEB, 0xEBEB, 0xE4E3, 0xE3EB, 0xE3EB, 
   0xE2EB, 0xE4DC, 0xE3EB, 0xEBE3, 0xE4E2, 0xDCE4, 0xE2DC, 0xE4EB, 0xE4E2, 0xE4E4, 
   0xE2EB, 0xDCE4, 0xD8E4, 0xE4E4, 0xE300, 0xE3E4, 0xEBEB, 0x00E2, 0x00E3, 0xE3DC, 
   0xEBEB, 0xE2EB, 0xEB00, 0x00E3, 0xEBEB, 0xE2E3, 0x00E3, 0x00E3, 0xE2EB, 0xE2E2, 
   0xE3E4, 0xEB00, 0xE200, 0x0000, 0xE4E4, 0xE3E3, 0xE200, 0x0000, 0xE4DC, 0xE3E3, 
   0xE200, 0x0000, 0xD8E4, 0xE3E4, 0xE2E3, 0x0000, 0x0000, 0x0000, 0x0000, 0xEBE4, 
   0x0000, 0xE400, 0xE3E4, 0xEBEB, 0x0000, 0xE4E2, 0xEBEB, 0xE3EB, 0xE300, 0xEBE4, 
   0xE3EB, 0xE3E3, 0xE400, 0xEBE4, 0xE3EB, 0xEBE3, 0xE2E2, 0xDC00, 0xEBE3, 0xE3EB, 
   0x0000, 0xE200, 0xEBEB, 0xEBEB, 0x0000, 0xE300, 0xE3E3, 0xE3E4, 0xE3EB, 0xEBEB, 
   0xE3E4, 0xE3E4, 0xEBEB, 0xE3E3, 0xE3E3, 0xE3E4, 0xE3E3, 0xEBE3, 0xEBE3, 0xDCE3, 
   0xE3E3, 0xE4E4, 0xEBEB, 0xE4D8, 0xEBEB, 0xE4E4, 0xE4E4, 0xE3E4, 0xE3E3, 0xE0EB, 
   0xE2E3, 0xE3E3, 0xE4E2, 0xEBEB, 0xE4E4, 0xE2E4, 0xD8E3, 0xEBE3, 0xE2E4, 0xE4E4, 
   0xE4EB, 0xE3E4, 0xE3E4, 0xE3EB, 0xE3EB, 0xE3E4, 0xE3E3, 0xEBE3, 0xE0DC, 0xDCE3, 
   0xE4E4, 0xE2DC, 0xD8E3, 0xD8D8, 0x00DC, 0xDCDC, 0xE4E4, 0xD8DC, 0x0000, 0xE000, 
   0xE2E4, 0x00E2, 0x0000, 0x0000, 0xD8E3, 0x00D8, 0x0000, 0x0000, 0xDCE4, 0xD8D8, 
   0x0000, 0x0000, 0xE4E0, 0xE4E4, 0x00DC, 0x0000, 0xE4E3, 0xDCE4, 0x0000, 0x0000, 
   0xE4E3, 0x00DC, 0x0000, 0x0000, 0xE0D8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xE4E4, 0xEBEB, 0xE4E4, 0xE300, 0xE3E4, 
   0xEBEB, 0xE0E3, 0x0000, 0xE4E2, 0xE3E4, 0xEBE3, 0x0000, 0xE0E4, 0xD8D8, 0xE6E2, 
   0x0000, 0xDC00, 0xD8E0, 0xE0E0, 0x0000, 0x0000, 0xDCE3, 0xE2E0, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xEBE3, 0xEBEB, 0xE4DC, 0xE4E4, 
   0xEBE3, 0xE4EB, 0xE4E2, 0xDCE4, 0xE2D8, 0xE0DC, 0xD8E0, 0xE4E4, 0xC8E0, 0xC8C8, 
   0xEBEB, 0xD8E0, 0xC8E0, 0xE2C8, 0xD8EB, 0xD8C8, 0x00E4, 0x0000, 0xC8E0, 0x0000, 
   0x0000, 0x0000, 0x00C8, 0x0000, 0x0000, 0xC800, 0x0000, 0x0000, 0xD8E2, 0x00D8, 
   0x0000, 0x0000, 0xD8D8, 0x0000, 0x0000, 0x0000, 0x00E0, 0x0000, 0x0000, 0x0000, 
   0x00C8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 
};

