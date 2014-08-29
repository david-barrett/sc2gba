// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define THRADD_WIDTH  (32)
#define THRADD_HEIGHT (32)


const u16 thraddData[] =
{
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDD00, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDD00, 0xD1E4, 0x0000, 0x0000,
   0xBFE4, 0xBFBF, 0x00DD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDD00, 0xD1E4,
   0x00DD, 0x00DD, 0xDD00, 0xBFBF, 0xDDBF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0xDD00, 0x0000, 0x0000, 0x0000, 0xE400, 0x0000, 0x0000, 0x0000, 0xDD00,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x39BF, 0x3986, 0xDDBF, 0x0000, 0x86BC, 0x8686, 0xCBBC, 0x0000, 0x39BF, 0xDC86,
   0xCFE4, 0x0000, 0xBFD1, 0xCFBC, 0xDCD8, 0xDCDC, 0xDD00, 0xDCDD, 0xD8D8, 0xE4E4,
   0x0000, 0xDCE4, 0xDCD8, 0xDCDC, 0x0000, 0xDCDC, 0xD8E4, 0xD8E0, 0x0000, 0xDCE4,
   0xD8DD, 0xE0E0, 0xBFE4, 0x8639, 0xBF39, 0x00E4, 0xBCCB, 0x8686, 0xBC86, 0x00DD,
   0xE4DC, 0x86E2, 0x7139, 0x00DD, 0xDCD8, 0xBCC8, 0xDD71, 0x0000, 0xD8DC, 0xB3C8,
   0x00DD, 0x0000, 0xD8DC, 0xE495, 0x0000, 0x0000, 0xE2D8, 0xE495, 0x0000, 0x0000,
   0xDDC8, 0xE4C8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0xDEA3, 0x0000, 0x0000, 0xDE00, 0xCF9F, 0x0000, 0x0000, 0x9FA3, 0x2FCF,
   0x0000, 0x0000, 0xCFDE, 0x872F, 0x0000, 0x0000, 0xCFA3, 0xE687, 0x0000, 0xDCDC,
   0xDDE4, 0xDDDD, 0xE400, 0xD8D8, 0xD8D8, 0xD8DC, 0xE200, 0xDCD8, 0xD8D8, 0xD8D8,
   0xE2A3, 0xDCD8, 0xD8C8, 0xD8D8, 0xDCCF, 0xDCC8, 0xD8C8, 0xE0D8, 0xDC87, 0xDCE0,
   0xE4C8, 0xD8D8, 0x87E6, 0xDCDC, 0xE0C8, 0xDCDC, 0xE6E6, 0xCF87, 0x6046, 0x6B6B,
   0xDCDD, 0xD8D8, 0x0000, 0x0000, 0xD8E0, 0xE0DC, 0x00E4, 0x0000, 0xD8D8, 0xE0DC,
   0x00DC, 0x0000, 0x86DC, 0xC8DC, 0xA3DC, 0xDEA3, 0x9BDC, 0xC8D8, 0xCFC8, 0x9FCF,
   0xC8DC, 0xC8E0, 0x87E0, 0xCF2F, 0xC8DC, 0xE0C8, 0xE6D8, 0x2F87, 0x469E, 0x87CF,
   0xE6E6, 0x87E6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00DE, 0x0000,
   0x0000, 0x0000, 0xDE9F, 0x0000, 0x0000, 0x0000, 0x74CF, 0x0000, 0x0000, 0x0000,
   0xA3CF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xCFA3, 0x872F, 0x0000, 0x0000,
   0x9FDE, 0x2FCF, 0x0000, 0x0000, 0xDE00, 0xCF9F, 0x0000, 0x0000, 0x0000, 0xDEDE,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x87E6, 0xCF2F, 0x90AD, 0x6046,
   0x2F87, 0x9FCF, 0x00DE, 0x0000, 0xCF2F, 0xDE9F, 0x0000, 0x0000, 0xDEA3, 0x00DE,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xBF90, 0x2FCF,
   0xE687, 0x2F87, 0xDE00, 0xCF9F, 0x872F, 0xCF2F, 0x0000, 0x9FDE, 0xCFCF, 0x9FCF,
   0x0000, 0xDE00, 0xA3DE, 0xDEDE, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0xA3CF, 0x0000, 0x0000, 0x0000, 0x749F, 0x0000, 0x0000, 0x0000, 0x0074, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000,
};



