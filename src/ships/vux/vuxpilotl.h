// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define VUXPILOTL_WIDTH  (32)
#define VUXPILOTL_HEIGHT (32)


const u16 vuxpilotlData[] =
{
   0xECEC, 0xECEC, 0xECEC, 0xECEC, 0xC0EC, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0EC, 0xC0C0,
   0xC0C0, 0xC0C0, 0xC0EC, 0xC0C0, 0xC0C0, 0xC0C0, 0x9BEC, 0x9B9B, 0x9B9B, 0x9B9B,
   0xB0EC, 0xB0B0, 0xB0B0, 0x9B9B, 0xBEEC, 0x9BBE, 0xB09B, 0x9BB0, 0xBEEC, 0x9BBE,
   0xB0B0, 0xB09B, 0xECEC, 0xECEC, 0xECEC, 0xECEC, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0,
   0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC09B, 0xC0C0,
   0xC0C0, 0xC0C0, 0x009B, 0xC0C0, 0xC0C0, 0xC0C0, 0x009B, 0xC0C0, 0xC0C0, 0xC0C0,
   0x009B, 0xC0C0, 0xC0C0, 0xC0C0, 0xECEC, 0xECEC, 0xECEC, 0x0000, 0xC0C0, 0x0000,
   0x0000, 0x0000, 0xC0C0, 0x0000, 0x0000, 0x0000, 0xC0C0, 0x0000, 0x0000, 0x0000,
   0xC0C0, 0x0000, 0x0000, 0x0000, 0xC0C0, 0x0000, 0x0000, 0x0000, 0xC0C0, 0x0000,
   0x0000, 0x0000, 0xC0C0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x9BEC, 0xB09B,
   0x9BB0, 0xB09B, 0xBEEC, 0xB0B0, 0x9BBE, 0xB09B, 0xB0EC, 0x9BB0, 0x9BBE, 0xB09B,
   0xB0EC, 0x9BBE, 0x9BBE, 0xC49B, 0xBEEC, 0xBEBE, 0x9B9B, 0xC4C4, 0x9BEC, 0xBE9B,
   0x9B9B, 0x58C4, 0x9BEC, 0xBE9B, 0x9B9B, 0xC4C4, 0x9BEC, 0xBE9B, 0xB0BE, 0xC4B0,
   0x009B, 0xC0C0, 0xC0C0, 0xC0C0, 0x009B, 0xC0C0, 0xC0C0, 0xC0C0, 0x58C4, 0xC058,
   0xC0C0, 0xC0C0, 0xC4C4, 0x58C4, 0xC058, 0xC0C0, 0x0058, 0xC4C4, 0x58C4, 0xC0C0,
   0x009B, 0xC0C0, 0xC4C4, 0xC058, 0x0058, 0xC0C0, 0xC4C0, 0x58C4, 0x58C4, 0xC058,
   0xC0C0, 0x58C4, 0xC0C0, 0x0000, 0x0000, 0x0000, 0xC0C0, 0x0000, 0x0000, 0x0000,
   0xC0C0, 0x0000, 0x0000, 0x0000, 0xC0C0, 0x0000, 0x0000, 0x0000, 0xC0C0, 0x0000,
   0x0000, 0x0000, 0xC0C0, 0x0000, 0x0000, 0x0000, 0xC0C0, 0x00C0, 0xC0C0, 0x0000,
   0xC0C0, 0xC0C0, 0xC0C0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xB0EC, 0xB0B0, 0xB0B0, 0x9B9B,
   0x9BEC, 0x9B9B, 0x9B9B, 0x9B9B, 0x9BEC, 0x9BD7, 0x9BD7, 0xD79B, 0x9BEC, 0x9B9B,
   0x9BD7, 0x9BD7, 0x9BEC, 0xD7D7, 0x9B9B, 0xD79B, 0x9BEC, 0x9B9B, 0x9B9B, 0x9B9B,
   0xC0EC, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0EC, 0xC0C0, 0xC0C0, 0xC0C0, 0xC4C4, 0x58C4,
   0x5858, 0xC4C4, 0x009B, 0xC4C4, 0xC4C4, 0xC4C4, 0x009B, 0xC0C0, 0xC9C0, 0xC9C9,
   0x009B, 0xC9C9, 0xC9C9, 0xD7C9, 0xB09B, 0xC9B0, 0xD7C9, 0xB0D7, 0xB0C9, 0xB0B0,
   0xC9C9, 0xB0B0, 0xC9C9, 0xB0B0, 0xC9B0, 0xC9C9, 0xC9D7, 0xC9C9, 0xB0B0, 0xB0B0,
   0xC058, 0xC0C0, 0x00C0, 0x0000, 0x5858, 0x5858, 0x3FC4, 0x0000, 0x58C4, 0xC4C4,
   0xC4C4, 0x0000, 0x58C4, 0xC4C4, 0xC4C4, 0x0000, 0xC4B0, 0xC058, 0x3F00, 0x0000,
   0xC4B0, 0xC058, 0x00C0, 0x0000, 0xC4C9, 0xC058, 0xC0C0, 0x0000, 0xC9C9, 0xC958,
   0xC9C9, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0xC0EC, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0EC, 0xC0C0,
   0xC0C0, 0xC0C0, 0xC0EC, 0xC0C0, 0xC0C0, 0xC0C0, 0x00EC, 0x0000, 0x0000, 0x0000,
   0x00EC, 0xC09B, 0x00C0, 0xC09B, 0x9BEC, 0xC0C0, 0x9B00, 0xC0C0, 0xC0EC, 0x00C0,
   0xC09B, 0x00C0, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xD7C0, 0xC9D7, 0xC9C9, 0xC0E7,
   0xC0C0, 0xD7B0, 0xC9D7, 0xC9C9, 0xC0C0, 0xB0C0, 0xD7B0, 0xD7D7, 0x0000, 0x0000,
   0xB0B0, 0xB0B0, 0x00C0, 0xC09B, 0xB0C0, 0xB0B0, 0x9B00, 0xC0C0, 0x9B00, 0xB0B0,
   0xC09B, 0x00C0, 0xC09B, 0xB0C0, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xB0B0, 0xC9C4,
   0xC9C9, 0x0000, 0xC9C9, 0xC0E7, 0xC9C9, 0x0000, 0xC9D7, 0xC9C9, 0xC9C9, 0x0000,
   0xD7B0, 0xD7D7, 0xD7D7, 0x0000, 0xB0B0, 0xB0B0, 0xB0B0, 0x0000, 0xB0B0, 0xB0B0,
   0xB0B0, 0x0000, 0xB0B0, 0xB0B0, 0xB0B0, 0x0000, 0x8C8C, 0x8C8C, 0x8C8C, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000,
};

