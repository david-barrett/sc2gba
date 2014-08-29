// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define STARS_WIDTH  (32)
#define STARS_HEIGHT (32)


u16 starsData[] =
{
   0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 
   0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 
   0xEBEB, 0xEBEB, 0xB56F, 0xB5B5, 0xEBEB, 0x6FEB, 0xC56F, 0xC4C4, 0xEBEB, 0xB5EB, 
   0x88B5, 0x5858, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 
   0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 
   0xEBEB, 0xEBEB, 0xDDB5, 0xEBEB, 0xDDEB, 0xEBEB, 0xC5C4, 0xEBEA, 0xEBEA, 0xEBEA, 
   0xC488, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 
   0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 
   0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xDDEB, 0xEBEB, 0xDDEB, 0xEBEB, 0xEBEA, 0xEBEA, 
   0xEBEA, 0xEBEA, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 
   0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 
   0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xDDEB, 0xDED6, 0xEBEB, 0xEBDD, 
   0xDEEA, 0x9FD6, 0x9FCF, 0xEBEA, 0xEBEB, 0xCF9F, 0xA3CF, 0xEBE8, 0xEBEB, 0xB5EB, 
   0x88C5, 0x9558, 0xEBEB, 0xE4EB, 0x88C5, 0x5858, 0xEBEB, 0xEBEB, 0xC5B5, 0xB9C4, 
   0xEBEB, 0xEBEB, 0xEAEA, 0xEAEB, 0xEBEB, 0xEBEB, 0xEBEA, 0xEAEB, 0xEBEB, 0xEBEB, 
   0xEBEA, 0xEADD, 0xEBEB, 0xEBEB, 0xEADD, 0xEAEB, 0xEBEB, 0xEBEB, 0xEBEA, 0xEBEA, 
   0xB958, 0xE8EA, 0xE8EA, 0xE8EA, 0xC5A0, 0xEAE8, 0xEBE4, 0xEBEB, 0x86C5, 0xE8E4, 
   0xEBEA, 0xEAE8, 0xE4EB, 0xDCE0, 0xDDE8, 0xDDE4, 0xEAEB, 0xE0E4, 0xCBE4, 0xCBCB, 
   0xEAEB, 0xE4EB, 0xE4E0, 0xCBCB, 0xEAEB, 0xE4EB, 0xE0E4, 0xBFA4, 0xEBEA, 0xD1EA, 
   0xCBCB, 0xBFAD, 0xE8EA, 0xE8EA, 0xE8EA, 0xE8EA, 0xEBE4, 0xEBEB, 0xEBE4, 0xDCEB, 
   0xEAE8, 0xEAE8, 0xEAE8, 0xD8E4, 0xD1DD, 0xDDE4, 0xE4EB, 0xE0D8, 0xCBCB, 0xCBCB, 
   0xE0E4, 0xE8E0, 0x71CB, 0xCBBF, 0xE0CB, 0xEAE8, 0xBC71, 0xBCBF, 0xCB71, 0xEBEB, 
   0x73C9, 0xC939, 0xBFBC, 0xEAE8, 0xD8DC, 0x3AD6, 0xE8A3, 0xEBCE, 0xE0D8, 0xEBE8, 
   0xEBE8, 0xEBEB, 0xE8E0, 0xEBEA, 0xEBEA, 0xEBEB, 0xEAE8, 0xEAEB, 0xEAEB, 0xEAEB, 
   0xEBEA, 0xEBEA, 0xEADD, 0xEAEB, 0xEAEB, 0xEAEB, 0xEBEA, 0xEBEA, 0xEBEB, 0xEBEB, 
   0xEBEB, 0xEBEB, 0xEAE8, 0xEAE8, 0xEAE8, 0xEAE8, 0xEBEB, 0xEBEB, 0xEBEA, 0xEBEA, 
   0xEBEB, 0xEBEB, 0xEBEA, 0xEBEA, 0xEBEB, 0xEBEB, 0xEBEA, 0xEAE4, 0xEBEB, 0xDDEB, 
   0xEBEA, 0xEBEA, 0xEBEB, 0xEBEB, 0xEBEA, 0xEBEA, 0xEBEB, 0xEBEB, 0xEBEA, 0xD8DC, 
   0xEBEB, 0x9CEB, 0xDCC9, 0xE4E0, 0xEBEB, 0xB277, 0x6BB2, 0xEAE8, 0xE4EA, 0xD1EA, 
   0xCBCB, 0xBC71, 0xEBEA, 0xDDEA, 0xCBCB, 0x6767, 0xEAEB, 0xE4EB, 0xCBCB, 0xBC71, 
   0xEBEA, 0xD8DC, 0xCBE4, 0xBFBF, 0xD8DC, 0xE4E0, 0xD1EA, 0xCBCB, 0xE4E0, 0xEAEA, 
   0xEAEB, 0xE4EB, 0xEBEA, 0xEBEB, 0xEBEB, 0xDCEB, 0xEBE8, 0xEAE8, 0xEAE8, 0x8EE8, 
   0x8639, 0x3986, 0x67AE, 0xEAEB, 0x8639, 0x7386, 0xCBBC, 0xEBEB, 0x39C9, 0x2E73, 
   0xDD71, 0xEAE8, 0x67BC, 0x71BC, 0xE0E0, 0xEBE4, 0xAD71, 0x85A4, 0xE2CE, 0xE0E0, 
   0xE8E0, 0xEBEB, 0xE8EB, 0xE2EB, 0xEBE0, 0xEBEB, 0xEBEB, 0xE8EB, 0xE8CB, 0xE8EA, 
   0xE8EA, 0xEBEA, 0xEAEB, 0xEAEB, 0xE4EB, 0xEBEB, 0xEBEB, 0xEBE4, 0xEAEB, 0xEAE8, 
   0xEAE8, 0xEAE8, 0xEBE8, 0xEBEB, 0xEBEB, 0xEBEB, 0xEAEB, 0xEAE8, 0xE8E4, 0xE8EA, 
   0xE8EA, 0xEBEA, 0xE4E0, 0xA5A5, 0xEBEB, 0xEBEB, 0xA5EA, 0x73C3, 0xE889, 0xE8EA, 
   0xC3EB, 0x7389, 0xEA73, 0xEBE8, 0xEBEB, 0xB277, 0xABAB, 0xE8EA, 0xEBEB, 0x6BEB, 
   0xE8AB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEAEB, 0xEAEB, 0xEBEB, 0xEBEB, 0xEAEB, 0xEAEB, 
   0xEBEB, 0xEBEB, 0xEAEB, 0xEAEB, 0xEBEB, 0xEBEB, 0xEAEB, 0xEAEB, 0xEBEB, 0xEBEB, 
   0xEAEB, 0xE4EB, 0xEBEB, 0xDDEB, 0xEBEB, 0xEAEB, 0xEBEA, 0xEBEA, 0xEBEA, 0xCB8E, 
   0xEBEB, 0xEBEB, 0xEBEB, 0x67CB, 0xEAE8, 0xEAE8, 0xEAE8, 0xBCE8, 0xEAEB, 0xEAEB, 
   0xEAEB, 0xEAEB, 0xEAEB, 0xE4EB, 0xEBEA, 0xEBEA, 0xE4EB, 0xEAEA, 0xEAEB, 0xE4EB, 
   0xEAEA, 0xEAEB, 0xEAEB, 0xEAEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xBCBF, 0xEBEB, 
   0xEBEB, 0xE8EB, 0x3986, 0xEAE8, 0xEAE8, 0xEAE4, 0xE839, 0xE8EA, 0xEBEA, 0xE8EB, 
   0xE8EB, 0xEBEA, 0xEBEA, 0xEBEA, 0xEBEA, 0xEBEA, 0xEAE4, 0xEAEB, 0xEBEA, 0xEBEA, 
   0xEBEA, 0xEBEA, 0xEAEB, 0xEAEB, 0xEAEB, 0xEAEB, 0xEBEB, 0xEBEB, 0xEBEB, 0xEBEB, 
   0xE8EA, 0x7389, 0xEBE8, 0xEBEB, 0xEAE8, 0xEAEB, 0xEAEB, 0xEAE8, 0xEBEA, 0xEBEA, 
   0xE8EB, 0xEBEA, 0xEBEA, 0xEBEA, 0xEBEA, 0xEAE4, 0xEAEB, 0xEAEB, 0xEAEB, 0xEBEA, 
   0xEBEA, 0xE4EA, 0xEBEA, 0xEBEA, 0xE4EB, 0xEBEA, 0xEBEA, 0xEBEA, 0xEAEB, 0xEBEB, 
   0xEBEB, 0xEBEB, 
};

#define STARS_SIZE (512)


u16 starsPalette[] =
{
   0x5AE7, 0x3F37, 0x281B, 0x0160, 0x3D60, 0x3C07, 0x3C0B, 0x3EF3, 0x0077, 0x00FB, 
   0x0167, 0x1C0B, 0x5EF7, 0x1E78, 0x3DEF, 0x0073, 0x3C00, 0x3E73, 0x5273, 0x0292, 
   0x1D67, 0x3D67, 0x1E73, 0x0277, 0x3E78, 0x3A8B, 0x016B, 0x3CF8, 0x3D78, 0x2800, 
   0x000F, 0x3CE0, 0x3CEB, 0x1D77, 0x00F7, 0x3CF3, 0x3D73, 0x1CF3, 0x1D65, 0x0173, 
   0x1C07, 0x3CE7, 0x0004, 0x1CF8, 0x1018, 0x04D4, 0x01DA, 0x61F0, 0x0003, 0x1AF8, 
   0x046D, 0x1CEB, 0x1B00, 0x6F5B, 0x200B, 0x0514, 0x0535, 0x41FB, 0x5CC9, 0x1D6B, 
   0x05D9, 0x0319, 0x0447, 0x0115, 0x00EB, 0x00F3, 0x1C00, 0x0533, 0x1D73, 0x0178, 
   0x059A, 0x125A, 0x1F1F, 0x0407, 0x3D6B, 0x035F, 0x00D7, 0x0575, 0x0177, 0x00E7, 
   0x779D, 0x5280, 0x00E0, 0x00F5, 0x0090, 0x006F, 0x0005, 0x0095, 0x236C, 0x1979, 
   0x0067, 0x0401, 0x01D9, 0x025C, 0x01B9, 0x1D00, 0x017C, 0x0027, 0x6260, 0x00B8, 
   0x0028, 0x0576, 0x006E, 0x207B, 0x0074, 0x031E, 0x1E5F, 0x07DF, 0x0072, 0x0259, 
   0x0094, 0x00E0, 0x09D8, 0x0096, 0x031F, 0x65FC, 0x4C00, 0x01FB, 0x7D0A, 0x0174, 
   0x0406, 0x18AC, 0x0138, 0x0199, 0x067C, 0x00A0, 0x18B3, 0x0029, 0x0596, 0x0137, 
   0x004B, 0x023B, 0x0093, 0x006D, 0x1EBD, 0x7B14, 0x0360, 0x7C1F, 0x04F2, 0x3806, 
   0x1CE7, 0x0070, 0x000F, 0x004E, 0x00FC, 0x027B, 0x0253, 0x02FC, 0x0071, 0x6358, 
   0x1973, 0x0534, 0x7FC1, 0x00B0, 0x09B8, 0x5695, 0x00D0, 0x0049, 0x021A, 0x7C01, 
   0x03C1, 0x019F, 0x004A, 0x60C4, 0x002D, 0x4411, 0x1806, 0x62F0, 0x025F, 0x02BC, 
   0x1A59, 0x2F7C, 0x008F, 0x007C, 0x04B9, 0x00D3, 0x0813, 0x0020, 0x0278, 0x04A6, 
   0x41E4, 0x0160, 0x0179, 0x5C17, 0x1BC0, 0x0301, 0x1C63, 0x0598, 0x001F, 0x1800, 
   0x5400, 0x0078, 0x292A, 0x4160, 0x40E0, 0x5C18, 0x02E0, 0x01E0, 0x2000, 0x6268, 
   0x62F8, 0x217D, 0x61E4, 0x0010, 0x00BF, 0x0661, 0x0400, 0x616C, 0x000C, 0x006C, 
   0x0173, 0x77FF, 0x20E0, 0x0006, 0x6000, 0x4C13, 0x41F0, 0x280A, 0x2060, 0x00AC, 
   0x416C, 0x0068, 0x4000, 0x00B3, 0x5274, 0x3000, 0x216C, 0x2064, 0x20E8, 0x0001, 
   0x7BDF, 0x7FFF, 0x0000, 0x7FDF, 0x0000, 0x0000, 0x4210, 0x4A52, 0x6739, 0x5AD6, 
   0x6B5A, 0x6F7B, 0x739C, 0x77BD, 0x7BDE, 0x7FFF, 0x7BFF, 0x5294, 0x4210, 0x001F, 
   0x03E0, 0x03FF, 0x7C00, 0x7C1F, 0x7FE0, 0x7FFF, 
};

