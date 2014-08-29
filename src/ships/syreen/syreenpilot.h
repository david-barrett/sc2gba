// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define SYREENPILOT_WIDTH  (64)
#define SYREENPILOT_HEIGHT (32)


const u16 syreenpilotData[] =
{
   0x0000, 0x4A00, 0x4A4A, 0x4A4A, 0x0000, 0xEC00, 0x8C8C, 0x298C, 0x0000, 0xEC00,
   0x0000, 0x1D00, 0x0000, 0xEC00, 0x0000, 0x0029, 0x0000, 0xEC00, 0x0000, 0x0029,
   0x0000, 0xEC00, 0x0000, 0x0000, 0x0000, 0xEC00, 0x0000, 0x0000, 0x0000, 0xEC00,
   0x0000, 0x0000, 0x4A4A, 0x4A4A, 0x4A4A, 0x4A4A, 0x4A4A, 0x4A4A, 0x294A, 0x8C8C,
   0x3ABA, 0x3AE0, 0x1DBA, 0x0000, 0xA300, 0xA3E7, 0x0000, 0x2900, 0xBE00, 0xBEE7,
   0x0000, 0x2900, 0xBE00, 0xBEE7, 0x0000, 0x0000, 0xBE00, 0xBEE7, 0x0000, 0x0000,
   0xBE00, 0xBE87, 0x0000, 0x0000, 0x4A4A, 0x4A4A, 0x4A4A, 0x4A4A, 0x298C, 0x8C8C,
   0x8C8C, 0x8C8C, 0x1D00, 0x0000, 0x0000, 0x001D, 0x0000, 0x0000, 0x0000, 0x001D,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4A4A, 0x4A4A, 0x4A4A, 0x4A4A,
   0x8C8C, 0x0144, 0x4A44, 0x8C4A, 0x0000, 0x47A2, 0xC047, 0xBAC0, 0x0000, 0xA200,
   0x9647, 0x9616, 0x0000, 0x0000, 0xA2A2, 0x9647, 0x2900, 0x0000, 0x0000, 0xC096,
   0x2900, 0x0000, 0x0000, 0xC0C0, 0x0000, 0x0000, 0x0000, 0xC0BA, 0x4A4A, 0x4A4A,
   0x4A4A, 0x4A4A, 0x0144, 0x8C44, 0x8C8C, 0x8C8C, 0x4747, 0x00A2, 0x0000, 0xD9D9,
   0xA247, 0x1D00, 0x0000, 0xCFD9, 0x00A2, 0x1D00, 0x0000, 0xC129, 0x0000, 0x0000,
   0xD400, 0xC1DC, 0x0000, 0x0000, 0x5100, 0x51E2, 0x00BA, 0x0000, 0x5100, 0xCF51,
   0x4A4A, 0x4A4A, 0x4A4A, 0x4A4A, 0x2323, 0x8C8C, 0x8C8C, 0x8C8C, 0xC3D7, 0xD920,
   0x0000, 0xD900, 0xC3D7, 0xC3C3, 0xD9D9, 0xC3D9, 0xD7CF, 0xC3D7, 0xC3C3, 0xC3D7,
   0xD7CA, 0xD7D7, 0xC3C3, 0xD7D7, 0xD73A, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xC3CF,
   0xD7C3, 0xD7D7, 0x4A4A, 0x4A4A, 0x4A4A, 0x4A4A, 0x8C8C, 0x2323, 0x8C8C, 0x8C8C,
   0xD9D9, 0xD7C3, 0x00D9, 0x0000, 0xD7C3, 0xD7C3, 0xD9D9, 0x0000, 0xD7C3, 0xD7D7,
   0xD9D9, 0x0000, 0xC3D7, 0xD7C3, 0xD9D7, 0x0000, 0xC3D7, 0xC3C3, 0xD9D9, 0x0000,
   0xD9D9, 0xC3D7, 0xD9D9, 0x0000, 0x4A4A, 0x4A4A, 0x0000, 0x0000, 0x8C8C, 0xC08C,
   0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000,
   0x0000, 0x8C00, 0x0000, 0x0000, 0x001D, 0x8C00, 0x0000, 0x0000, 0x001D, 0x8C00,
   0x0000, 0x0000, 0x0000, 0x8C29, 0x0000, 0x0000, 0x0000, 0xEC00, 0x001D, 0x0000,
   0x0000, 0xEC00, 0x001D, 0x001D, 0x0000, 0xEC00, 0x0000, 0x001D, 0x0000, 0xEC00,
   0x0000, 0x0000, 0x0000, 0xEC00, 0x0000, 0x0000, 0x0000, 0xEC00, 0x0000, 0x0000,
   0x0000, 0xEC00, 0x0000, 0x0000, 0x0000, 0xEC00, 0x0000, 0x0000, 0xBE00, 0xBE87,
   0x0000, 0x0000, 0xBE00, 0xBE87, 0x0000, 0x001D, 0xBE00, 0xBE98, 0x0000, 0x001D,
   0xBE00, 0xBE98, 0x0000, 0x0000, 0xBE00, 0xBE98, 0x0000, 0x0000, 0xBE00, 0xBE98,
   0x0000, 0x0000, 0xBE00, 0xBE98, 0x0000, 0x0000, 0xBE00, 0xBE98, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0029, 0x0000, 0x0000, 0x0029, 0x0029, 0x0000, 0x0000, 0x0029,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xD400, 0x0000, 0x0000,
   0x0000, 0xCAD4, 0x0000, 0x0000, 0x001D, 0xC000, 0x0000, 0x0000, 0x001D, 0xBA00,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x001D, 0x0000, 0x0000,
   0xBABA, 0x001D, 0x0000, 0xBA00, 0xC0C0, 0x00BA, 0x0029, 0xC000, 0xC0C0, 0x00C0,
   0x29CA, 0xC0D4, 0xC0C0, 0xBAC0, 0xBAC0, 0x0000, 0xCA00, 0xD7C1, 0xC0C0, 0x29BA,
   0xCAD4, 0x3AC1, 0xC0BA, 0xCAB4, 0xCA51, 0xCA51, 0x3A00, 0x5151, 0x7651, 0x5151,
   0x7600, 0x51CA, 0xCA51, 0x51CA, 0xA300, 0xCA76, 0x51CA, 0xCAA3, 0xA300, 0xCACA,
   0x51CA, 0xA3C1, 0xCA00, 0x5151, 0x5151, 0x1DCA, 0xD73A, 0xC3CF, 0xC3C3, 0xD9D7,
   0xD929, 0xD7D7, 0xC3D7, 0x20C3, 0x001D, 0xD7D9, 0xD9D7, 0xC3C3, 0x1DC1, 0xD900,
   0x00D9, 0xD9D9, 0xA351, 0x0000, 0x0000, 0x0000, 0xCA51, 0x0000, 0x0000, 0x0000,
   0x5151, 0x0029, 0x0000, 0x0000, 0x51D4, 0x1DCA, 0x0000, 0x0029, 0xD900, 0xD9D9,
   0xD7D9, 0x00D9, 0xD7D9, 0x0000, 0xD900, 0x00D9, 0xD9D7, 0x0000, 0x0000, 0x0000,
   0x00D9, 0x0000, 0x0000, 0x1D00, 0x0000, 0x0000, 0x0000, 0x1D00, 0x1D00, 0x0000,
   0x0000, 0x0000, 0x1D00, 0x0000, 0x0000, 0x1D00, 0x0000, 0x0000, 0x0000, 0x1D00,
   0x0000, 0x8C29, 0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0x8C00,
   0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000,
   0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0x8C00,
   0x0000, 0x0000, 0x0000, 0xEC00, 0x0000, 0x0000, 0x0000, 0xEC00, 0x0000, 0x0029,
   0x0000, 0xEC00, 0x0000, 0x0029, 0x0000, 0xEC00, 0x1D00, 0x0000, 0x0000, 0xEC00,
   0x1D00, 0x0000, 0x0000, 0xEC00, 0x0000, 0x0000, 0x0000, 0xEC00, 0x0000, 0x0000,
   0x0000, 0xEC00, 0x0000, 0x0000, 0xBE00, 0xBE98, 0x0000, 0x0000, 0xBE00, 0xBE98,
   0x0000, 0x0000, 0xBE00, 0xBE98, 0x0000, 0x0000, 0xBE00, 0xBE98, 0x0000, 0x1D00,
   0xBE00, 0xBE98, 0x0000, 0x1D00, 0xBE00, 0xBE98, 0x0000, 0x0000, 0xBE00, 0xBEEA,
   0x0000, 0x0000, 0xBE00, 0xBEEA, 0x0000, 0x2900, 0x0000, 0x0000, 0x0000, 0xCACA,
   0x0000, 0x0000, 0x2900, 0xC1CA, 0x0000, 0x0000, 0xCA00, 0xC1CA, 0x0000, 0x0000,
   0xCA00, 0xCACA, 0x0000, 0x0000, 0xCA29, 0xCACA, 0x0000, 0x001D, 0xCACA, 0xCACA,
   0x0000, 0x001D, 0xCACA, 0xA3CA, 0x0000, 0x0000, 0xCACA, 0x1DA3, 0xCACA, 0xB4CA,
   0xC03A, 0xC0C0, 0xCACA, 0xCACA, 0x3ACA, 0xC0C0, 0xCAC1, 0xCACA, 0xCACA, 0x2929,
   0xCA1D, 0xCACA, 0xC1CA, 0xC1A3, 0x2900, 0xCACA, 0xBECA, 0x51CA, 0x0000, 0xCA3A,
   0xA3A3, 0x5151, 0x0000, 0x3AC0, 0x76BE, 0x5151, 0x0000, 0xC0C0, 0x291D, 0xCACA,
   0x5100, 0x5151, 0xCA51, 0x00A3, 0x51BA, 0x5151, 0xA351, 0x001D, 0x51C0, 0x5151,
   0x1D51, 0x0000, 0xCA3A, 0x5151, 0x1D51, 0x0000, 0x76CA, 0xCACA, 0xA351, 0x0000,
   0xCA51, 0x76CA, 0x76CA, 0x0000, 0x5151, 0xCA51, 0x7676, 0x0000, 0x5151, 0x5151,
   0x7676, 0x0000, 0xCA00, 0xA351, 0x0000, 0x0029, 0x2900, 0xCA51, 0x001D, 0x0000,
   0x0000, 0x51D4, 0x00A3, 0x0000, 0x0000, 0x5100, 0x00CA, 0x0000, 0x2900, 0xD400,
   0xD451, 0x0000, 0x2900, 0x0000, 0xCFCF, 0x0000, 0x0000, 0x0000, 0xCFD9, 0x00D4,
   0x001D, 0x0000, 0x51D4, 0xD4EA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x1D00, 0x0000, 0x0000, 0x0000, 0x1D00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x2900, 0x0000, 0x0000, 0x0000, 0x2900, 0x0000, 0x8C00, 0x0000, 0x0000,
   0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0x8C00,
   0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000,
   0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0xEC00,
   0x0000, 0x0000, 0x0000, 0xEC00, 0x0000, 0x001D, 0x0000, 0xEC00, 0x0000, 0x001D,
   0x0000, 0xEC00, 0x0000, 0x0000, 0x0000, 0xEC00, 0x0000, 0x0000, 0x0000, 0xEC00,
   0x0000, 0x0000, 0x0000, 0xEC00, 0x0000, 0x0000, 0x0000, 0xC000, 0xBABA, 0xBABA,
   0xBE00, 0xBEEA, 0x0000, 0x2900, 0xBE00, 0xBE51, 0x0000, 0x0000, 0xBE00, 0xBEEA,
   0x0000, 0x1D00, 0xBE00, 0xBEEA, 0x0000, 0x1D00, 0xBE00, 0xBE51, 0x0000, 0x0000,
   0xBE1D, 0xBE51, 0x0000, 0x0000, 0xBE1D, 0xBECA, 0x0000, 0xD400, 0x8BBA, 0x8B29,
   0xBABA, 0xB4BA, 0x0000, 0xD400, 0xCACA, 0x001D, 0x0000, 0xCA00, 0xD4CA, 0x0000,
   0x0000, 0xCA29, 0x00CA, 0x001D, 0x0000, 0xCACA, 0x00CA, 0x001D, 0xD400, 0xCACA,
   0x00CA, 0x0000, 0xCA00, 0xCACA, 0x0029, 0x0000, 0xCA29, 0x29CA, 0x0000, 0x0000,
   0xB429, 0xBA29, 0xBABA, 0xBABA, 0x0000, 0xC0C0, 0x00BA, 0xA3A3, 0x0000, 0xC0C0,
   0x1DC0, 0xBEBE, 0x0000, 0xC0C0, 0x29BA, 0xBEBE, 0xBA00, 0xC0C0, 0xC000, 0x1D1D,
   0x291D, 0x2929, 0x291D, 0x001D, 0x1D1D, 0xBE1D, 0x1D1D, 0x001D, 0x0000, 0xBE00,
   0x0000, 0x0000, 0xBABA, 0x8BBA, 0xBABA, 0xBABA, 0xCACA, 0xCACA, 0xA376, 0x001D,
   0xA3A3, 0xA3A3, 0xBEA3, 0x1DBE, 0xBEBE, 0xBEBE, 0xBEBE, 0x1DBE, 0x1D1D, 0x1D1D,
   0x1D1D, 0x001D, 0x0000, 0x1D1D, 0x1D00, 0x0000, 0x0000, 0xBEBE, 0x1D00, 0x0000,
   0x0000, 0xBEBE, 0x0000, 0x1D00, 0xBABA, 0x8B8B, 0xBABA, 0x8BBA, 0x001D, 0x0000,
   0xD4D4, 0xD426, 0x0000, 0x291D, 0xD429, 0x2929, 0x1D00, 0xCAA3, 0xA7C7, 0xCAC7,
   0xA300, 0xEACA, 0xA798, 0xEA98, 0xA300, 0xEACA, 0xCAEA, 0xEAEA, 0x1D00, 0xCAA3,
   0xA3CA, 0xCACA, 0x0000, 0xA3BE, 0xBEA3, 0xA3A3, 0xBABA, 0x8B8B, 0x8B8B, 0x8B8B,
   0x0000, 0x0000, 0x1D00, 0x0000, 0x001D, 0x0000, 0x1D00, 0x0000, 0x1DA3, 0x0000,
   0x0000, 0x0000, 0xA3CA, 0x0000, 0x0000, 0x0000, 0xA3CA, 0x0000, 0x0000, 0x2900,
   0x1DA3, 0x0000, 0x0000, 0x2900, 0x00BE, 0x0000, 0x0000, 0x001D, 0xBA8B, 0xBABA,
   0xBABA, 0xBA8B, 0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000,
   0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0x8C00,
   0x0000, 0x0000, 0x0000, 0x8C1D, 0x0000, 0x0000, 0x0000, 0x8C1D, 0x0000, 0x0000,
   0xBABA, 0x8CBA, 0x0000, 0x0000,
};

#define SYREENPILOT_SIZE (1024)



