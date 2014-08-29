// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define MYCONPILOTT_WIDTH  (32)
#define MYCONPILOTT_HEIGHT (32)


const u16 myconpilottData[] =
{
   0x0000, 0x0000, 0x0000, 0xB000, 0x0000, 0x0000, 0x0000, 0xB0B0, 0x0000, 0x0000,
   0xB0B0, 0xB0B0, 0x0000, 0xB000, 0xB0B0, 0xB0B0, 0x0000, 0xB000, 0xB0B0, 0xB0B0,
   0x0000, 0xB000, 0xB0B0, 0xB0B0, 0x00B0, 0xB000, 0xB0B0, 0x0000, 0xB0B0, 0xB0B0,
   0x00B0, 0x00BE, 0x00B0, 0x0000, 0x0000, 0x0000, 0xB0B0, 0xB0B0, 0x0000, 0x0000,
   0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0xBEBE, 0xBEBE, 0x0000, 0xBEBE, 0x7676, 0xC076,
   0xBEBE, 0x76BE, 0x5151, 0x7676, 0xB000, 0xB0B0, 0x00B0, 0x0000, 0xB0B0, 0xB0B0,
   0xB0B0, 0x0000, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0,
   0x0000, 0xB000, 0xB0B0, 0xB0B0, 0x00BE, 0x0000, 0xB000, 0xB0B0, 0xBEC0, 0xBEBE,
   0x0000, 0xB000, 0xC0C0, 0xBEC0, 0x00BE, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xB0B0, 0x00B0, 0x0000, 0xB0B0, 0xB0B0,
   0xB0B0, 0x00B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0x0000, 0x0000, 0xB0B0, 0xB0B0,
   0x00B0, 0x0000, 0xB000, 0x0000, 0xB0B0, 0x00B0, 0x0000, 0x0000, 0xB0B0, 0xB0B0,
   0x0000, 0xBE00, 0x0000, 0x00B0, 0x00BE, 0xBEBE, 0xB000, 0x0000, 0x0000, 0xBEBE,
   0xB0B0, 0x0000, 0xBE00, 0xBEBE, 0x00B0, 0xBE00, 0xBE00, 0x76BE, 0x00B0, 0xC0BE,
   0xBE00, 0x76BE, 0x00B0, 0xBE00, 0x0000, 0xBEBE, 0xB000, 0x0000, 0x0000, 0xBEBE,
   0xBEBE, 0x5176, 0x7651, 0x7676, 0x76BE, 0x9851, 0x7651, 0x7651, 0x5176, 0x9898,
   0x7651, 0x7651, 0x5176, 0x5198, 0x5176, 0x5151, 0x5176, 0x5151, 0x5176, 0x5198,
   0x5176, 0x7651, 0x5176, 0x5151, 0x7676, 0x7676, 0x7676, 0x7676, 0xBEBE, 0x7676,
   0x7676, 0xC076, 0xC0C0, 0xC0C0, 0xBEBE, 0x0000, 0xC076, 0xC09B, 0xBEC0, 0x00BE,
   0xC076, 0x9B9B, 0xC0C0, 0xC0BE, 0xC076, 0xE7C0, 0xC09B, 0xBEBE, 0x7676, 0x9BC0,
   0xC09B, 0x76BE, 0x7676, 0xC0C0, 0xC0C0, 0x76BE, 0x7676, 0xC0C0, 0xBEC0, 0xBEBE,
   0xC076, 0xBEC0, 0xBEBE, 0x0000, 0xB000, 0xB0B0, 0x0000, 0x0000, 0x0000, 0xB0B0,
   0xB0B0, 0xB0B0, 0x0000, 0xB0B0, 0xB0B0, 0xB0B0, 0x00C0, 0xB000, 0x00B0, 0x0000,
   0x00C0, 0xB000, 0x00B0, 0x0000, 0xBEC0, 0x0000, 0xB0B0, 0x0000, 0x0000, 0x0000,
   0xB0B0, 0x00B0, 0x0000, 0x0000, 0xB000, 0x00B0, 0xB000, 0x00B0, 0x0000, 0x0000,
   0xB000, 0xB0B0, 0x00B0, 0x0000, 0x0000, 0xB0B0, 0x00B0, 0x0000, 0x0000, 0xB000,
   0xB0B0, 0x0000, 0x0000, 0xB0B0, 0xB0B0, 0x00B0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0,
   0xB000, 0xB0B0, 0xB0B0, 0xB0B0, 0x0000, 0x0000, 0x00B0, 0x0000, 0xBE00, 0xBEBE,
   0xBEBE, 0xBEBE, 0x0000, 0x0000, 0x0000, 0x0000, 0xBEBE, 0x0000, 0x0000, 0x0000,
   0x7600, 0xBE00, 0xBE00, 0xBE00, 0x0000, 0x7600, 0x7600, 0x0000, 0x0000, 0x0000,
   0x0000, 0xBE00, 0xB0B0, 0xBE00, 0x5176, 0xC076, 0x00B0, 0x0000, 0x0000, 0x0000,
   0xBEBE, 0x00BE, 0x0000, 0x0000, 0x0000, 0x0000, 0xBE00, 0x0000, 0x0000, 0xBE00,
   0x0000, 0x00B0, 0xBE00, 0x7600, 0x0000, 0x00B0, 0x7600, 0x0000, 0xB000, 0xB0B0,
   0x0000, 0x0000, 0xB000, 0xB0B0, 0x00BE, 0x0000, 0xB0B0, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xB0B0, 0x0000, 0x0000, 0x0000, 0xB0B0,
   0x0000, 0x0000, 0x0000, 0xB0B0, 0x0000, 0x0000, 0xB000, 0xB0B0, 0x00B0, 0xB000,
   0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0x00B0, 0x0000, 0x0000, 0xB000, 0x00B0, 0x0000,
   0x0000, 0x0000, 0xB0B0, 0x00B0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000,
};

#define MYCONPILOTT_SIZE (512)

