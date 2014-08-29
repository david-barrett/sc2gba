// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define ASTEROIDBREAK_WIDTH  (32)
#define ASTEROIDBREAK_HEIGHT (32)


const u16 asteroidbreakData[] =
{
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xC000,
   0x0000, 0x0000, 0x0000, 0xC0C0, 0x0000, 0x0000, 0xB300, 0xB3C0, 0x0000, 0x0000,
   0xC0B3, 0xB3B3, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xB3C0, 0xE7B3,
   0x00E7, 0x0000, 0xB3C0, 0xE7C0, 0x0000, 0x0000, 0xC0B3, 0xE7B3, 0xC000, 0x0000,
   0xB3B3, 0x00C0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xC000,
   0x0000, 0x0000, 0x0000, 0xC000, 0x0000, 0x0000, 0x0000, 0xB3C0, 0x0000, 0x0000,
   0x0000, 0xB3C0, 0x0000, 0x0000, 0x0000, 0xC0C0, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xC0C0, 0xE7B3,
   0x0000, 0x0000, 0xB3C0, 0xB3B3, 0x00E7, 0x0000, 0xB3B3, 0xB3C0, 0x00E7, 0x0000,
   0xB3C0, 0x9BC0, 0x00E7, 0x0000, 0xB3C0, 0xE7C0, 0x0000, 0x0000, 0x0000, 0x0000,
   0xC0C0, 0xB3C0, 0x0000, 0x0000, 0xC000, 0xC0C0, 0x0000, 0x0000, 0x0000, 0xC0C0,
   0x0000, 0x0000, 0x9B00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x9BC0, 0xB39B, 0x00C0, 0x0000, 0x00E7, 0xC000, 0x9BC0, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x9B00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0xC000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0xC000, 0xE7C0, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x009B, 0x0000, 0x00E7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x00C0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x00C0, 0x00E7, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0xE700, 0x0000, 0x0000, 0x0000, 0x9B9B, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xC000,
   0x0000, 0x0000, 0x0000, 0xC000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00C0, 0x0000,
   0x0000, 0x0000, 0x00E7, 0x0000, 0x0000, 0x0000, 0x00C0, 0x00C0, 0x0000, 0x0000,
   0x0000, 0x0000, 0x00C0, 0x00E7, 0x0000, 0x0000, 0xB3C0, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x9B00, 0x0000, 0xB3B3, 0x00C0, 0x0000, 0xC000, 0xB3C0, 0xC0B3,
   0x0000, 0xE700, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x9B00, 0x0000, 0x0000, 0x0000, 0xB300, 0x0000, 0x00C0, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x9BB3, 0x00E7, 0x0000, 0x0000, 0xE7B3, 0x009B, 0x0000, 0x0000, 0xC0B3, 0x0000,
   0x0000, 0x0000, 0x00C0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xC0B3, 0xB3C0, 0x9BB3,
   0xC000, 0xB3B3, 0xB3C0, 0xE7B3, 0xB300, 0xB3C0, 0xB3C0, 0xE7C0, 0x0000, 0xC0B3,
   0x9B9B, 0x00E7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xC000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x009B, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000,
};


