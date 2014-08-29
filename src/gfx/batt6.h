// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define BATT6_WIDTH  (8)
#define BATT6_HEIGHT (8)


u16 batt6Data[] =
{
   0x0000, 0x0000, 0x0000, 0x0000, 0x00D0, 0x00D0, 0x00D0, 0x0000, 0x00D0, 0x00D0, 
   0x00D0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00D0, 0xB9D0, 0x00D0, 0x0000, 
   0x00D0, 0x00D0, 0x00D0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 
};

#define BATT6_SIZE (32)


u16 batt6Palette[] =
{
   0x0000, 0x03E0, 0x293F, 0x6402, 0x0935, 0x05FB, 0x021F, 0x025D, 0x02FF, 0x0091, 
   0x0073, 0x1A40, 0x4E32, 0x013A, 0x063C, 0x005F, 0x437C, 0x01FB, 0x01BA, 0x0002, 
   0x0ABD, 0x061B, 0x0E3A, 0x2D3F, 0x0DD8, 0x01FC, 0x0015, 0x044A, 0x09FA, 0x0426, 
   0x0997, 0x4C26, 0x004C, 0x0425, 0x0159, 0x0048, 0x02DD, 0x18B9, 0x0956, 0x197F, 
   0x0240, 0x00F0, 0x0706, 0x0A46, 0x1962, 0x04F3, 0x017F, 0x20A0, 0x0555, 0x5814, 
   0x00B4, 0x601C, 0x0DB7, 0x0136, 0x4014, 0x627C, 0x6D5D, 0x048F, 0x04B1, 0x0577, 
   0x6064, 0x00B2, 0x0004, 0x0018, 0x0535, 0x04D4, 0x01DA, 0x61F0, 0x0576, 0x0003, 
   0x1B19, 0x046D, 0x1B00, 0x0027, 0x6F5B, 0x1C0C, 0x0514, 0x41FC, 0x5CC9, 0x05D9, 
   0x0319, 0x0447, 0x0090, 0x0115, 0x01B9, 0x0533, 0x0178, 0x059A, 0x125A, 0x1F1F, 
   0x0407, 0x023B, 0x025C, 0x035F, 0x0137, 0x00A0, 0x00D7, 0x0575, 0x779D, 0x1EBD, 
   0x00F5, 0x09D8, 0x0406, 0x6260, 0x006F, 0x0005, 0x0095, 0x236C, 0x1979, 0x04F2, 
   0x004B, 0x019F, 0x5680, 0x0401, 0x01D9, 0x017C, 0x031F, 0x00B8, 0x0028, 0x0534, 
   0x006E, 0x3006, 0x0138, 0x207C, 0x031E, 0x00B0, 0x1E5F, 0x07DF, 0x0072, 0x0259, 
   0x02FC, 0x05B8, 0x0094, 0x0074, 0x00E0, 0x0096, 0x61FC, 0x4C00, 0x7D0A, 0x00D1, 
   0x0174, 0x18AC, 0x0199, 0x067C, 0x0049, 0x18B3, 0x0029, 0x0596, 0x0093, 0x006D, 
   0x7B14, 0x0360, 0x7C1F, 0x60E4, 0x4410, 0x021A, 0x0070, 0x000F, 0x004E, 0x00FC, 
   0x004A, 0x027B, 0x0253, 0x0071, 0x6358, 0x1973, 0x7FC1, 0x008F, 0x5695, 0x7C01, 
   0x02BC, 0x03C1, 0x00B9, 0x237C, 0x025F, 0x0413, 0x007C, 0x002D, 0x1806, 0x62F0, 
   0x1A59, 0x6018, 0x0179, 0x0300, 0x00D3, 0x0020, 0x0278, 0x0598, 0x04A6, 0x41E4, 
   0x001F, 0x0160, 0x1BC0, 0x217C, 0x1800, 0x5400, 0x0260, 0x292A, 0x0078, 0x4160, 
   0x02E0, 0x01E0, 0x40E0, 0x2000, 0x6268, 0x62F8, 0x61E4, 0x0010, 0x00BF, 0x0400, 
   0x616C, 0x000C, 0x0173, 0x20E0, 0x006C, 0x0006, 0x6000, 0x41F0, 0x2060, 0x416C, 
   0x00AC, 0x7FFF, 0x00B3, 0x0068, 0x4000, 0x5274, 0x3000, 0x216C, 0x2064, 0x20E8, 
   0x0001, 0x7BDF, 0x0000, 0x7FDF, 0x0000, 0x0000, 0x4210, 0x4A52, 0x6739, 0x5AD6, 
   0x6B5A, 0x6F7B, 0x739C, 0x77BD, 0x7BDE, 0x7FFF, 0x7BFF, 0x5294, 0x4210, 0x001F, 
   0x03E0, 0x03FF, 0x7C00, 0x7C1F, 0x7FE0, 0x7FFF, 
};

