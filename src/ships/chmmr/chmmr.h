// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define CHMMR_WIDTH  (32)
#define CHMMR_HEIGHT (32)


const u16 chmmrData[] =
{
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDC00, 0x0000, 
   0x0000, 0x0000, 0xDC00, 0x0000, 0x0000, 0x0000, 0xD800, 0x0000, 0x0000, 0x0000, 
   0xDC00, 0xE400, 0x0000, 0x0000, 0xDC00, 0xD6E4, 0x0000, 0x0000, 0xDC00, 0xD6D6, 
   0x0000, 0x0000, 0xDC00, 0xCF9F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00C8, 
   0x0000, 0x0000, 0x0000, 0x00C8, 0x0000, 0x0000, 0x0000, 0x00C8, 0x0000, 0x0000, 
   0x00E2, 0x00C8, 0x0000, 0x0000, 0xE4DE, 0x00C8, 0x0000, 0x0000, 0xC6D6, 0x00C8, 
   0x0000, 0x0000, 0xD6D6, 0x00C8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0xD800, 0xCFD6, 0x0000, 0x0000, 0xDCE2, 0xA3D6, 0x0000, 0x0000, 
   0xDEDC, 0xD6D6, 0x0000, 0x0000, 0xE3DC, 0xDEDE, 0x0000, 0x0000, 0xE4D8, 0xE3E4, 
   0x0000, 0x0000, 0xE4DC, 0xE2D8, 0x0000, 0xE400, 0xE4DC, 0xD8D8, 0x0000, 0xE200, 
   0xDCDC, 0xE6C8, 0x9FD6, 0x00C8, 0x0000, 0x0000, 0xD6DE, 0xDCD8, 0x0000, 0x0000, 
   0xDEDE, 0xC8C6, 0x0000, 0x0000, 0xDEDE, 0xC8E3, 0x0000, 0x0000, 0xE4EB, 0xC8C8, 
   0x0000, 0x0000, 0xE0E2, 0xC8DC, 0x00E4, 0x0000, 0xD8E0, 0xC8DC, 0x00E4, 0x0000, 
   0xE0C8, 0xC8DC, 0x00E4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDC00, 0x0000, 
   0x0000, 0x0000, 0xDC00, 0x0000, 0x0000, 0x0000, 0xE400, 0x0000, 0x0000, 0xCBE2, 
   0xE2E4, 0x0000, 0xE2E2, 0xCBE4, 0xDCE4, 0xC800, 0xDCE0, 0xCBE4, 0x0000, 0xE400, 
   0x95CF, 0xEBE4, 0x0000, 0xE400, 0xD8E0, 0xC6EB, 0x0000, 0xE4E2, 0xEBE0, 0xDEC6, 
   0x0000, 0xE4E4, 0xEBC8, 0xDEDE, 0xCBE2, 0xE4E4, 0xEBE0, 0x9FDE, 0xCBE4, 0xE4E4, 
   0xEBE0, 0xD6DE, 0xCBE4, 0xE4DC, 0xEBE0, 0xE3DE, 0xCBDC, 0xE4E4, 0xE3E0, 0xBDC6, 
   0xDCEB, 0xC8C8, 0x00E4, 0x0000, 0xEBBD, 0xC8D8, 0x00E4, 0x0000, 0xC6BD, 0xC8C6, 
   0xE4E4, 0x0000, 0xDEE3, 0xC8C6, 0xE2E4, 0x008E, 0xC6BD, 0xE0C6, 0xDCE4, 0xDCBC, 
   0xE3BD, 0xC8C6, 0xE0E4, 0xDCBC, 0xD6DE, 0xC8C6, 0xD8E4, 0xD8BC, 0x87D6, 0xC8DE, 
   0xE0E4, 0xD8BC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00DC, 0x00CB, 0x0000, 
   0x0000, 0x00CF, 0xDCBC, 0x00E2, 0x0000, 0x00E4, 0xD8BC, 0xCFD8, 0x00E4, 0xDCE2, 
   0xD8BC, 0xD8D8, 0xDCDC, 0xDCDC, 0xE6E4, 0xE0E2, 0xE0E0, 0xCBCF, 0xE0DC, 0xE0E2, 
   0xE0E6, 0xBCE0, 0xE6E0, 0x0000, 0xE000, 0xBCE6, 0xC800, 0x0000, 0x0000, 0x9000, 
   0xE600, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xCBE4, 0xE4E4, 0xEBE0, 0xC6EB, 
   0xCBDC, 0xE4E4, 0xE2E0, 0xEBE3, 0xCBE0, 0xE4E4, 0xD8D8, 0xE4E4, 0xBCE9, 0xE2E4, 
   0xD8D8, 0xC8C8, 0x9000, 0xE4DC, 0xE6C8, 0x00E4, 0x0000, 0xE4C8, 0xDCE6, 0x0000, 
   0x0000, 0xE400, 0xE4E0, 0x0000, 0x0000, 0x0000, 0x00D8, 0x0000, 0xDEBD, 0xC8EB, 
   0xE0E4, 0xE0BC, 0xE4EB, 0xC8E2, 0xE0E2, 0xE0BC, 0xE4E4, 0xC8E0, 0xE0E4, 0xC8BC, 
   0xDCE2, 0xD8D8, 0xE0E4, 0xE6BC, 0xE400, 0xE0D8, 0xC8E4, 0xC87A, 0x0000, 0xE0CF, 
   0xC8E4, 0x0000, 0x0000, 0xD8DC, 0x00E4, 0x0000, 0x0000, 0xD800, 0x0000, 0x0000, 
   0xE0BC, 0xE0C8, 0xC8E0, 0xE0E6, 0xE0BC, 0xE6C8, 0xE0E0, 0xE0E0, 0xE6BC, 0x00E0, 
   0x0000, 0xE0E6, 0xE07A, 0x0000, 0x0000, 0x00C8, 0x0000, 0x0000, 0x0000, 0x00E6, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 
};
