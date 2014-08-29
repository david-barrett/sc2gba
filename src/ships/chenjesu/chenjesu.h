// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define CHENJESU_WIDTH  (32)
#define CHENJESU_HEIGHT (32)


const u16 chenjesuData[] =
{
   0x0000, 0x0000, 0x0000, 0xBE00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0xBE00, 0x0000, 0x76BE, 0x00BE, 0x0000, 0x0000, 0xBE00, 0x0000, 0xBE00, 
   0x0000, 0x76BE, 0x00BE, 0xBEBE, 0x0000, 0xBE00, 0xBEE1, 0xE7DC, 0x0000, 0x76BE, 
   0xC0BE, 0xC0E7, 0xBE76, 0x0000, 0x0000, 0x0000, 0x00BE, 0x0000, 0x0000, 0x0000, 
   0xBE76, 0x0000, 0x0000, 0x0000, 0x00BE, 0x0000, 0x0000, 0x0000, 0xBE76, 0x0000, 
   0x0000, 0x0000, 0xBEBE, 0x00BE, 0x0000, 0x0000, 0xE7E9, 0xBEC0, 0xBE00, 0xBEBE, 
   0xC0C0, 0xC0E6, 0xC076, 0xE7E6, 0x0000, 0x0000, 0xBE00, 0xBE76, 0x0000, 0x0000, 
   0x0000, 0x00BE, 0x0000, 0x0000, 0xBE00, 0xBE76, 0x0000, 0x0000, 0x0000, 0x00BE, 
   0x0000, 0x0000, 0xBE00, 0xBE76, 0x0000, 0x0000, 0xBE00, 0xBEBE, 0xBEBE, 0xBE00, 
   0xE6C0, 0xE9E7, 0xC0E9, 0xC076, 0xC0E6, 0xC0C0, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x76BE, 
   0x00BE, 0x0000, 0x0000, 0xBE00, 0x0000, 0x0000, 0x0000, 0x76BE, 0x00BE, 0x0000, 
   0xBEC0, 0xBEE1, 0x0000, 0x0000, 0xC0E7, 0x76BE, 0x00BE, 0x0000, 0x0000, 0xBE00, 
   0xE7E2, 0x74C0, 0x0000, 0x76BE, 0xE7C0, 0xB0C0, 0x0000, 0x76BE, 0xE7C0, 0xC9C0, 
   0xBE00, 0xC076, 0xE6E7, 0x9BE6, 0x7600, 0xE7C0, 0xC0C0, 0xE7C0, 0xC0BE, 0xC0E9, 
   0x76BE, 0x9B51, 0xC0BE, 0xC0E9, 0xC3B0, 0x8698, 0x76BE, 0xC0C0, 0xC9C9, 0x9BE9, 
   0x5176, 0xE79B, 0xE7C0, 0x9B9B, 0x98C3, 0xE686, 0x9BE9, 0xC0C0, 0xE9C9, 0xE99B, 
   0xC09B, 0x76BE, 0x959B, 0x9BE7, 0xBEC0, 0x76BE, 0xE795, 0xC09B, 0xBEBE, 0xC3B0, 
   0xE795, 0xC09B, 0xB0B0, 0xC3C3, 0xE6E6, 0xC09B, 0xC9C9, 0xC3C3, 0xE6E6, 0x9BE9, 
   0xC9C0, 0xC9C9, 0xE79B, 0xE7C0, 0xBEC0, 0x5176, 0x9BC0, 0x95E6, 0xB0C0, 0x98C3, 
   0xC051, 0xE795, 0xC9C0, 0xE9C9, 0x5151, 0x9BC0, 0x9BE7, 0x959B, 0x51CF, 0xC051, 
   0xE79B, 0xE7E6, 0x98C3, 0xB495, 0x9595, 0xC095, 0xE9C9, 0xC0E6, 0xE6E0, 0xC0E7, 
   0x95E6, 0x9BC0, 0xE6E6, 0xC0E6, 0xE79B, 0x74C0, 0x0000, 0x0000, 0xE686, 0x76C0, 
   0x00BE, 0x0000, 0xE99B, 0x76C0, 0x00BE, 0x0000, 0xE9E7, 0xC0E9, 0xBE76, 0x0000, 
   0xC0C0, 0xE7C0, 0xCFC0, 0x0000, 0x7674, 0x9B51, 0xC0E7, 0x00D6, 0xC3B0, 0x8698, 
   0xC0E6, 0x00BE, 0xC9C9, 0x9BE9, 0x76C0, 0x00BE, 0xBEBE, 0xC076, 0x9B9B, 0xE795, 
   0x00BE, 0x76BE, 0xE7C0, 0xC0E7, 0x0000, 0xBEBE, 0xC0CF, 0xC0E6, 0x0000, 0xBE00, 
   0xC076, 0xC0E6, 0x0000, 0xBE00, 0x76BE, 0xE9C0, 0x0000, 0xBE00, 0xBE74, 0xC076, 
   0x0000, 0xBE00, 0xBE00, 0x76BE, 0x0000, 0x0000, 0x0000, 0xBEBE, 0xC0C0, 0xE6C0, 
   0xC09B, 0xC9C9, 0x76BE, 0x9B51, 0x9B95, 0xC0C0, 0xC3B0, 0x9B98, 0xE6E7, 0xE6E6, 
   0xC9C9, 0x9B35, 0xE6E7, 0x9B9B, 0x9B9B, 0xE795, 0x9BE6, 0xC0C0, 0xE6E7, 0xE6E9, 
   0xC09B, 0x7674, 0x9BC0, 0x9BE7, 0xB0C0, 0xC3B0, 0xC076, 0x9BE6, 0xB0C0, 0xC3C3, 
   0xC095, 0xE79B, 0xC0C0, 0xE6C0, 0x9BC0, 0xC0E9, 0x76BE, 0x9B51, 0xE6E7, 0xC0E9, 
   0xC3B0, 0xC898, 0xE99B, 0xC0E6, 0xC9C9, 0x9BE7, 0x95C0, 0x95E6, 0x9B9B, 0xE695, 
   0xC0CF, 0xE69B, 0xE6E6, 0xC0E7, 0x5151, 0x9BC0, 0x9BE6, 0x76C0, 0x98C3, 0x9BE2, 
   0xC0E7, 0xBE76, 0x9B9B, 0xC095, 0xD676, 0x00BE, 0xE7E6, 0x76C0, 0x00BE, 0x00BE, 
   0xC0E6, 0xBE76, 0x00BE, 0x0000, 0xC0E6, 0xBE76, 0x0000, 0x0000, 0x76C0, 0xBEBE, 
   0x0000, 0x0000, 0xBE76, 0xBEBE, 0x0000, 0x0000, 0xBEBE, 0xBE00, 0x0000, 0x0000, 
   0x00BE, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x74BE, 0x0000, 0x0000, 
   0x0000, 0x00BE, 0x0000, 0x0000, 0x0000, 0x00BE, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDCBE, 0x9BE7, 0xC9C0, 0xC3C9, 
   0x76BE, 0xE7C0, 0xC09B, 0xC9C9, 0xBE00, 0xC076, 0x9BE7, 0xC0C0, 0xBE00, 0x76BE, 
   0xE7C0, 0x959B, 0x7400, 0xBEBE, 0xC0BE, 0xC0C0, 0xBE00, 0x0000, 0xBEBE, 0xE174, 
   0x0000, 0x0000, 0xBE00, 0xE1E1, 0x0000, 0x0000, 0xBE00, 0xE1E1, 0x98E6, 0x9543, 
   0xC0E9, 0xBEBE, 0xC0E9, 0xE79B, 0x76C0, 0x00BE, 0x9BC0, 0xC0E6, 0xBE76, 0x0000, 
   0xE99B, 0x76C0, 0xBEBE, 0x0000, 0xC0C0, 0xBEBE, 0xBEBE, 0x0000, 0xBE74, 0x00BE, 
   0xBE00, 0x0000, 0xBEE1, 0x0000, 0x0000, 0x0000, 0xBEE1, 0x0000, 0x0000, 0x0000, 
   0x00BE, 0x0000, 0x0000, 0x0000, 0x00BE, 0x0000, 0x0000, 0x0000, 0x00BE, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 
};

