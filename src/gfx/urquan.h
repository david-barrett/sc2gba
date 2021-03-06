// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define URQUAN_WIDTH  (32)
#define URQUAN_HEIGHT (32)


const u16 urquanData[] =
{
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xE400, 0x0000, 0x0000,
   0x0000, 0xC500, 0x0000, 0x0000, 0x6F00, 0x60AD, 0x0000, 0x0000, 0xC5B5, 0xCDCD,
   0x0000, 0xB500, 0xC4CD, 0xC4C4, 0x0000, 0xCDB5, 0x9391, 0xABAB, 0xB500, 0xB2CD,
   0xAB93, 0xABAB, 0xCDB5, 0xB291, 0xC5C5, 0xC4C4, 0x91C5, 0xCDB2, 0xC4CD, 0xA0C4,
   0xCDCD, 0xCDC5, 0xC488, 0xC488, 0x6075, 0xC5AD, 0x0000, 0x0000, 0xC4C4, 0xC4C4,
   0x00C5, 0x0000, 0xC4C4, 0xC4C5, 0xC5C4, 0x0000, 0xABAB, 0xABAB, 0xC4AB, 0x00C5,
   0xABAB, 0xABAB, 0xABAB, 0xC588, 0xC4C4, 0xC4C4, 0xABC4, 0xA0AB, 0xC4C4, 0xC4CD,
   0xC4C4, 0xABAB, 0x58C4, 0xC4C4, 0xC4C4, 0x8858, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00C5, 0x0000, 0x0000, 0x0000,
   0xC5A0, 0x0000, 0x0000, 0x0000, 0xCDA0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0xCD00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xBCCB, 0xBC60, 0x00CB,
   0x0000, 0x7590, 0x75AB, 0x0090, 0x0000, 0xE2E2, 0x95D8, 0x00C8, 0x0000, 0xE2E4,
   0xC8D8, 0x00C8, 0x0000, 0xC5B5, 0x88C4, 0x00CD, 0x0000, 0xC5B5, 0xC4C4, 0x00CD,
   0xCDCD, 0xCDC4, 0xC4C4, 0xC4C4, 0xCDC5, 0xCDCD, 0xC4CD, 0xC458, 0xC500, 0xCDC5,
   0xC5CD, 0x88C4, 0x0000, 0xCDE4, 0xCDCD, 0x88C4, 0x0000, 0xCD00, 0xCDCD, 0xB9C4,
   0x0000, 0xCD00, 0xCDCD, 0x88C4, 0x0000, 0xC400, 0xC5CD, 0xB9C4, 0x0000, 0xCD00,
   0xCDCD, 0xC458, 0xC4C4, 0xC4B9, 0xA0B9, 0xA088, 0xC4C4, 0xC4C4, 0xA088, 0xA0A0,
   0xC4B9, 0xA0B9, 0xA088, 0xC5B9, 0xC4C4, 0xC4C4, 0xA0A0, 0x00C5, 0xB9C4, 0x88C4,
   0xA0A0, 0x0000, 0xC4C4, 0x8858, 0xA0A0, 0x0000, 0xB9C4, 0x88C4, 0xA088, 0x0000,
   0xC4C4, 0x88C4, 0xA088, 0x0000, 0xCDA0, 0x0000, 0x0000, 0x0000, 0x00C5, 0x0000,
   0x0000, 0x0000, 0x0000, 0xBCCB, 0xBC46, 0x00CB, 0x0000, 0x9E90, 0x9EAB, 0x0090,
   0x0000, 0xE2DC, 0xC8D8, 0x00C8, 0x0000, 0xE2E4, 0xC8D8, 0x00C8, 0x0000, 0xC5B5,
   0x88B9, 0x00CD, 0x0000, 0xC5E4, 0x88C4, 0x00CD, 0x0000, 0xC5B5, 0x88C4, 0x00CD,
   0x0000, 0xC5B5, 0x58C4, 0x00CD, 0x0000, 0xC5B5, 0x88C4, 0x00CD, 0x0000, 0xC5B5,
   0x88C4, 0xD8E2, 0x0000, 0xC5B5, 0xC4C4, 0xE2E2, 0x0000, 0xC5B5, 0x58C4, 0xD8D8,
   0x0000, 0xC500, 0x88C4, 0x00CD, 0x0000, 0xC500, 0x88C4, 0x00CD, 0x0000, 0xCD00,
   0xC5CD, 0xB9C4, 0x0000, 0xCD00, 0xCDCD, 0xC4C4, 0x0000, 0xCD00, 0xCDCD, 0xC4B9,
   0xE2E2, 0xC5E4, 0xCDCD, 0x90C4, 0xE4E2, 0xC5E4, 0xE0CF, 0x7C91, 0xE2D8, 0xCDDC,
   0xB9CD, 0x90C4, 0x0000, 0xCD00, 0xCDCD, 0xB9C4, 0x0000, 0xCD00, 0xCDCD, 0xC4B9,
   0xC5C8, 0x88B9, 0xA0A0, 0x0000, 0xB9D8, 0x88C4, 0xA0A0, 0x0000, 0xC4B2, 0x88C4,
   0xA0A0, 0x0000, 0x4691, 0x88B9, 0xA058, 0xE2DC, 0xB2AB, 0xD87C, 0xA0C8, 0xE2E2,
   0x4691, 0x88B9, 0xA088, 0xD8D8, 0xB991, 0x88C4, 0xA0A0, 0x0000, 0xB9E0, 0x88C4,
   0x8888, 0x0000, 0x0000, 0xC5C5, 0x88B9, 0x00CD, 0x0000, 0xC5B5, 0xC4C4, 0x00CD,
   0x0000, 0xC5E4, 0x88B9, 0x00CD, 0xE4DC, 0xC5E4, 0xC4C4, 0x00CD, 0xE4E2, 0xC5E4,
   0x88B9, 0x00CD, 0xE2D8, 0xC5E2, 0x8888, 0x00CD, 0x0000, 0xC5B5, 0x88B9, 0x00C5,
   0x0000, 0xC5B5, 0x88B9, 0x0000, 0x0000, 0xC500, 0x88C4, 0x00CD, 0x0000, 0x0000,
   0x58C4, 0x00CD, 0x0000, 0x0000, 0xA0C5, 0x00CD, 0x0000, 0x0000, 0xC500, 0x00CD,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xCD00, 0xCDCD, 0xC4C4,
   0x0000, 0xC500, 0xC5C5, 0x88CD, 0x0000, 0x0000, 0xC5C5, 0xC5C5, 0x0000, 0xC500,
   0xCDCD, 0xB958, 0x0000, 0x58C5, 0xCDCD, 0xC4B9, 0xCD00, 0xC5CD, 0xCDCD, 0xC4C4,
   0x0000, 0x468E, 0x9375, 0x6B6B, 0x0000, 0xCB00, 0xADCB, 0x6090, 0xC4E0, 0x88C4,
   0xA0A0, 0x0000, 0xC4B9, 0xA088, 0x5858, 0x0000, 0xC5C5, 0xC5C5, 0xE4C5, 0x0000,
   0xB9C4, 0xB958, 0xCD88, 0x0000, 0xC4C4, 0x88C4, 0x5888, 0x00B9, 0xC4B9, 0x88B9,
   0x8888, 0xB958, 0x6B6B, 0x936B, 0x469E, 0x00CB, 0x6075, 0xAD90, 0xCBCB, 0x0000,
   0x0000, 0xC5C5, 0xC5B9, 0x0000, 0x0000, 0xCDE4, 0x00B9, 0x0000, 0x0000, 0xC5C5,
   0x00B5, 0x0000, 0x0000, 0xE4B5, 0x0000, 0x0000, 0x0000, 0x006F, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000,
};


