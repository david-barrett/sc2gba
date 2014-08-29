// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define PAUSE1_WIDTH  (32)
#define PAUSE1_HEIGHT (16)


const u16 pause1Data[] =
{
   0x00EB, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8C8C, 0x8C8C, 0x0000, 0x0000,
   0x8C8C, 0x8C8C, 0x0000, 0x0000, 0x8C8C, 0xF8F8, 0x0000, 0x0000, 0x8C8C, 0xF8F8,
   0x0000, 0x0000, 0x8C8C, 0xF8F8, 0x0000, 0x0000, 0x8C8C, 0xF8F8, 0x0000, 0x0000,
   0x8C8C, 0xF8F8, 0x0000, 0x0000, 0x0000, 0x0000, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C,
   0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8,
   0xF8F8, 0xF8F8, 0x8CF8, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xF8F8, 0xF8F8, 0xF8F8,
   0x8C8C, 0xF8F8, 0xF8F8, 0xF8F8, 0x0000, 0x0000, 0x0000, 0x0000, 0x8C8C, 0x8C8C,
   0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8,
   0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0x8CF8, 0x8C8C, 0x8C8C, 0xF8F8, 0x8C8C,
   0xF8F8, 0xF8F8, 0xF8F8, 0x8C8C, 0xF8F8, 0xF8F8, 0x0000, 0x0000, 0x0000, 0x0000,
   0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xF8F8, 0xF8F8,
   0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF88C, 0xF8F8, 0x8C8C, 0xF8F8,
   0x8C8C, 0xF8F8, 0x8C8C, 0xF88C, 0x8C8C, 0xF8F8, 0x8C8C, 0x8C8C, 0x0000, 0x0000,
   0x8C8C, 0xF8F8, 0x0000, 0x0000, 0x8C8C, 0xF8F8, 0x0000, 0x0000, 0x8C8C, 0xF8F8,
   0x0000, 0x0000, 0x8C8C, 0xF8F8, 0x0000, 0x0000, 0x8C8C, 0xF8F8, 0x0000, 0x0000,
   0x8C8C, 0xF8F8, 0x0000, 0x0000, 0xF88C, 0xF7F7, 0x0000, 0x0000, 0xF7F8, 0xF7F7,
   0x8C8C, 0x8CF8, 0x8C8C, 0x8C8C, 0x8C8C, 0xF8F8, 0xF8F8, 0x8C8C, 0x8C8C, 0xF8F8,
   0xF8F8, 0x8C8C, 0x8CF8, 0x8C8C, 0x8C8C, 0x8C8C, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8,
   0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF7F7, 0xF7F7, 0xF7F7, 0xF7F7, 0xF7F7, 0xF7F7,
   0xF7F7, 0xF7F7, 0xF8F8, 0x8C8C, 0x8C8C, 0x8C8C, 0xF8F8, 0x8C8C, 0xF8F8, 0xF8F8,
   0xF8F8, 0x8C8C, 0xF8F8, 0xF8F8, 0xF8F8, 0x8C8C, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8,
   0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF7F7, 0xF7F7, 0xF7F7, 0xF7F7,
   0xF7F7, 0xF7F7, 0xF7F7, 0xF7F7, 0x8C8C, 0xF8F8, 0x8C8C, 0x8CF8, 0x8C8C, 0xF8F8,
   0x8C8C, 0xF8F8, 0x8C8C, 0xF8F8, 0x8C8C, 0xF8F8, 0x8C8C, 0xF8F8, 0x8C8C, 0xF8F8,
   0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF8F8, 0xF7F7, 0xF7F7,
   0xF7F7, 0xF7F7, 0xF7F7, 0xF7F7, 0xF7F7, 0xF7F7,
};



