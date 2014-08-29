// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define DRUUDGE_WIDTH  (32)
#define DRUUDGE_HEIGHT (32)


const u16 druudgeData[] =
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
   0x0000, 0xE400, 0x0000, 0x0000, 0x0000, 0xE4E2, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDC00, 0x0000, 0x0000, 0xE200, 0xCBD1, 
   0x00E0, 0x0000, 0x0000, 0xADE4, 0xD8CB, 0x0000, 0xE4E4, 0xD8DC, 0x00DD, 0x0000, 
   0xDCE4, 0xD8E0, 0x00E2, 0x0000, 0xD8DC, 0x00DC, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xE300, 
   0x0000, 0x0000, 0x0000, 0xDCE3, 0x0000, 0x0000, 0xE300, 0xE0C8, 0x0000, 0x0000, 
   0xE400, 0xE0D8, 0x0000, 0x0000, 0xE400, 0xDCE4, 0x0000, 0x0000, 0xE4E2, 0xE0DC, 
   0xE300, 0xE400, 0xDCE4, 0xDCD8, 0xDCE3, 0xE4C8, 0xD8DC, 0x00DC, 0xC8DC, 0xDCDC, 
   0xDCD8, 0x00E3, 0xD8C8, 0xDCDC, 0x00E4, 0x0000, 0xE4D8, 0xE4DC, 0xE3E4, 0x0000, 
   0xDCDC, 0xE4E4, 0x00E3, 0x0000, 0xDCD8, 0x0000, 0x0000, 0x0000, 0x00DC, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0xDCE4, 0x0000, 0x0000, 0xDCE2, 0xE0D8, 0x0000, 0xD8E2, 0xD8D8, 0xC8E0, 
   0x0000, 0xCBD8, 0xC867, 0xE0E0, 0x0000, 0xADE4, 0x7390, 0xE0DC, 0x0000, 0x0000, 
   0xE0E2, 0xDCDC, 0x0000, 0xDCE2, 0xD8E0, 0xE2DC, 0xDCE2, 0xE0D8, 0xE4D8, 0xE4E2, 
   0xE0D8, 0xD8E0, 0xE2DC, 0xE4E4, 0xC8D8, 0xE4D8, 0xE4E2, 0x00E4, 0xD8D8, 0xE2DC, 
   0xE4E4, 0x0000, 0xDCD8, 0xE4E2, 0xDCE4, 0x0000, 0xDCD8, 0xE4E4, 0xE3E4, 0x0000, 
   0xE4D8, 0xE4E4, 0xE300, 0xB56F, 0xE4E4, 0xE3E3, 0xE4DC, 0xCDA0, 0xE3E3, 0x0000, 
   0xDCE2, 0xA0E2, 0x00E3, 0xE400, 0xE2DC, 0xE4E4, 0x0000, 0xE400, 0xE4E4, 0xE4E4, 
   0x0000, 0x0000, 0xE3E2, 0xE3E3, 0x0000, 0x0000, 0xE300, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x006F, 0x0000, 0x0000, 0x0000, 0x00B5, 0x0000, 0x0000, 0x0000, 
   0xE36F, 0x0000, 0x0000, 0x0000, 0x00E3, 0x0000, 0x0000, 0x0000, 0x00E3, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xBFCB, 0x7390, 0xDCE4, 0x0000, 0xBFD1, 
   0xDC67, 0xE4E4, 0x0000, 0xCB00, 0xE467, 0x39E4, 0x0000, 0x0000, 0xADCB, 0x7A67, 
   0x0000, 0x0000, 0x8E00, 0x678E, 0x0000, 0x0000, 0x0000, 0xD100, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDCDC, 0xE4E3, 0x00E4, 0x0000, 
   0xE3DC, 0xE4E4, 0x00E3, 0x0000, 0xE439, 0xE3E4, 0x00E3, 0x0000, 0x7A90, 0xE3E4, 
   0x00E3, 0x0000, 0xBFAD, 0xE3E4, 0x0000, 0x0000, 0xE400, 0x00E3, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 
};
