// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define UMGAHPILOTS_WIDTH  (32)
#define UMGAHPILOTS_HEIGHT (16)


u16 umgahpilotsData[] =
{
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0xECEC, 0xECEC, 0xECEC, 0xECEC, 0xABAB, 0xABAB, 0xABAB, 0xC9C9, 
   0xABAB, 0xABAB, 0xABAB, 0xABAB, 0xABAB, 0xABAB, 0xE7AB, 0xABAB, 0xABAB, 0xABAB, 
   0xABAB, 0xABAB, 0xABE7, 0xABAB, 0xABAB, 0xD773, 0xE7AB, 0xABAB, 0x73AB, 0xB0D7, 
   0xABE7, 0xABE7, 0x73E7, 0xB0D7, 0xECEC, 0xECEC, 0xECEC, 0xECEC, 0xB0B0, 0xB0B0, 
   0xB0B0, 0xB0B0, 0xB0C9, 0xB0B0, 0xB0B0, 0xABB0, 0xABAB, 0xB0C9, 0xABB0, 0xABAB, 
   0x7373, 0xD773, 0xB0D7, 0xABAB, 0xABD7, 0xB0B0, 0xABB0, 0xABAB, 0xC9AB, 0xB0B0, 
   0xB0B0, 0xABAB, 0xABAB, 0xB0C9, 0xABB0, 0xABAB, 0xECEC, 0x0000, 0x0000, 0x0000, 
   0xC9C9, 0x0000, 0x0000, 0x0000, 0xABAB, 0x0000, 0x0000, 0x0000, 0xABAB, 0x0000, 
   0x0000, 0x0000, 0xABAB, 0x0000, 0x0000, 0x0000, 0xABAB, 0x0000, 0x0000, 0x0000, 
   0xABAB, 0x0000, 0x0000, 0x0000, 0xABAB, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0xE7AB, 0xE7AB, 0x73AB, 0xB0D7, 0xE7E7, 0xABE7, 0xABE7, 0xD773, 0xE7E7, 0xE7E7, 
   0xE7E7, 0x73AB, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0xE7AB, 0xABE7, 0xABAB, 0xABAB, 0xB0B0, 0xABAB, 0xABE7, 0xE7AB, 
   0xD773, 0xABB0, 0xABAB, 0xABAB, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0xABAB, 0x0000, 0x0000, 0x0000, 0xABAB, 0x0000, 
   0x0000, 0x0000, 0xABAB, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
};

#define UMGAHPILOTS_SIZE (256)


u16 umgahpilotsPalette[] =
{
   0x0000, 0x3F37, 0x281B, 0x0160, 0x3D60, 0x3C07, 0x3C0B, 0x3EF3, 0x0077, 0x00FB, 
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
   0x7BDF, 0x7FFF, 0x0000, 0x7FDF, 0x5AE7, 0x0000, 0x4210, 0x4A52, 0x6739, 0x5AD6, 
   0x6B5A, 0x6F7B, 0x739C, 0x77BD, 0x7BDE, 0x7FFF, 0x7BFF, 0x5294, 0x4210, 0x001F, 
   0x03E0, 0x03FF, 0x7C00, 0x7C1F, 0x7FE0, 0x7FFF, 
};

