// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define YEHATPILOT_WIDTH  (64)
#define YEHATPILOT_HEIGHT (32)


const u16 yehatpilotData[] =
{
   0x0000, 0xEC00, 0xECEC, 0xECEC, 0x0000, 0xEC00, 0xC0D7, 0xB076, 0x0000, 0xEC00, 
   0xD7D7, 0x3FB0, 0x0000, 0xEC00, 0xB0D7, 0xB03F, 0x0000, 0xEC00, 0x3FD7, 0xD7B0, 
   0x0000, 0xEC00, 0x3FB0, 0x3FB0, 0x0000, 0xEC00, 0xB0B0, 0xB03F, 0x0000, 0xEC00, 
   0x3FB0, 0xD7B0, 0xECEC, 0xECEC, 0xECEC, 0xECEC, 0xD7B0, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xB0B0, 0xD7D7, 0xD7D7, 0xD7D7, 0xB076, 0xB0B0, 0xD7D7, 0xC0D7, 0x3F3F, 0xB0B0, 
   0xD7D7, 0xC0C0, 0xB0B0, 0xB076, 0xC0C0, 0xD7C0, 0xD7B0, 0x76C0, 0xC0C0, 0xD7D7, 
   0xD7D7, 0xC0D7, 0x7676, 0xD7D7, 0xECEC, 0xECEC, 0xECEC, 0xECEC, 0xC0D7, 0xD7C0, 
   0xD7D7, 0xD7D7, 0xC0C0, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7C0, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xECEC, 0xECEC, 0xECEC, 0xECEC, 
   0xD7D7, 0xD7D7, 0xC0D7, 0xD7C0, 0xD7D7, 0xD7D7, 0xD7D7, 0xC0C0, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xC0C0, 0xD7D7, 0xD7D7, 0xD7D7, 0xC0D7, 0xD7D7, 0xD7D7, 0x76D7, 0x7676, 
   0xD7D7, 0xD7D7, 0x7676, 0x7676, 0xD7D7, 0xD7D7, 0x7676, 0x7676, 0xECEC, 0xECEC, 
   0xECEC, 0xECEC, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xD7C0, 0xD7D7, 0xD7D7, 0xD7D7, 0xC0C0, 0xD7D7, 0xD7D7, 0xD7D7, 0x7676, 0xD7C0, 
   0xD7D7, 0xD7D7, 0x7676, 0xC076, 0xD7D7, 0xD7D7, 0x7676, 0xC076, 0xD7C0, 0xD7D7, 
   0xECEC, 0xECEC, 0xECEC, 0xECEC, 0xD7D7, 0xD7D7, 0xD7D7, 0xC0D7, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xECEC, 0xECEC, 0xECEC, 0xECEC, 0xC076, 0xC0C0, 0xD7D7, 0xD7D7, 
   0x76C0, 0xC0C0, 0xD7D7, 0x3FD7, 0xC0D7, 0xC076, 0x3FD7, 0xB03F, 0xD7D7, 0x76C0, 
   0xB03F, 0xD7D7, 0xD7D7, 0xC0C0, 0xC0B0, 0xB0D7, 0xD7D7, 0xC0C0, 0x76C0, 0x3FB0, 
   0xD7D7, 0xC0C0, 0x3FD7, 0xB0B0, 0xECEC, 0xECEC, 0x0000, 0x0000, 0xD7D7, 0x8CD7, 
   0x0000, 0x0000, 0xD73F, 0x8CD7, 0x0000, 0x0000, 0x3FB0, 0x8CD7, 0x0000, 0x0000, 
   0xB0B0, 0x8CB0, 0x0000, 0x0000, 0x3FB0, 0x8CB0, 0x0000, 0x0000, 0xB03F, 0x8CB0, 
   0x0000, 0x0000, 0xB0B0, 0x8CB0, 0x0000, 0x0000, 0x0000, 0xEC00, 0xB03F, 0xD7D7, 
   0x0000, 0xEC00, 0xB03F, 0xD7D7, 0x0000, 0xEC00, 0xC0B0, 0xD7C0, 0x0000, 0xEC00, 
   0xD7B0, 0xC0C0, 0x0000, 0xEC00, 0xD7B0, 0xC0D7, 0x0000, 0xEC00, 0xD7B0, 0xD7D7, 
   0x0000, 0xEC00, 0x3FB0, 0xD7D7, 0x0000, 0xEC00, 0xB0B0, 0xD7B0, 0xD7D7, 0xD7D7, 
   0x7676, 0xD776, 0xD7D7, 0xD7D7, 0x76C0, 0xD776, 0xD7D7, 0xD7D7, 0xC0C0, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xC0C0, 0xD7D7, 0xD7C0, 0xD7D7, 0xC0C0, 0xD7D7, 0xC0C0, 0xD7D7, 
   0xC0C0, 0xD7D7, 0xC0D7, 0xD7C0, 0xC0C0, 0xD7D7, 0xD7D7, 0xC0C0, 0xC0C0, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0x76D7, 0x7676, 0xD7D7, 0xD7D7, 0xD7D7, 0xB0B0, 
   0xD7D7, 0xD7D7, 0x76D7, 0x7676, 0xD7D7, 0xC4D7, 0xE7AB, 0xE7E7, 0xD7D7, 0x58C4, 
   0x3FC4, 0xE7AB, 0xC4D7, 0xAB58, 0xABC4, 0xE7E7, 0xC4D7, 0xAB58, 0xABC4, 0xABE7, 
   0xC4D7, 0x5858, 0xABC4, 0xABE7, 0x7676, 0xC0D7, 0xC0C0, 0xD7D7, 0xC0B0, 0xC0C0, 
   0xC0C0, 0xD7C0, 0x7676, 0xD7D7, 0xC0D7, 0xC0C0, 0x3FAB, 0xC03F, 0xD7D7, 0xC0D7, 
   0x3F3F, 0xC4C0, 0xD7C0, 0xD7D7, 0x3FAB, 0x58C0, 0xC0C4, 0xD7D7, 0x3FAB, 0x58C0, 
   0xC0C4, 0xD7D7, 0x3FAB, 0x58C0, 0xC0C4, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7C0, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xC0C0, 0xD7D7, 0xD7D7, 0xC0D7, 0xC0D7, 0xD7C0, 0xD7D7, 0xC0C0, 
   0xD7D7, 0xC0C0, 0xC0D7, 0xD7C0, 0xD7D7, 0xC0D7, 0xC0C0, 0xD7D7, 0xD7D7, 0xC0C0, 
   0xB03F, 0xC0B0, 0xD7D7, 0xC0C0, 0xC0B0, 0x76C0, 0xC0D7, 0xD7C0, 0xB0B0, 0x76D7, 
   0xC0C0, 0xD7D7, 0xB0D7, 0xD7B0, 0xD7C0, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xB0C0, 0x8CB0, 0x0000, 0x0000, 0xB076, 0x8CB0, 0x0000, 0x0000, 0xD776, 0x8CB0, 
   0x0000, 0x0000, 0xD7D7, 0x8CB0, 0x0000, 0x0000, 0xD7D7, 0x8CB0, 0x0000, 0x0000, 
   0xD7D7, 0x8CB0, 0x0000, 0x0000, 0x3FD7, 0x8C3F, 0x0000, 0x0000, 0x3FD7, 0x8CB0, 
   0x0000, 0x0000, 0x0000, 0xEC00, 0xB0B0, 0xD7B0, 0x0000, 0xEC00, 0xB0B0, 0x3FB0, 
   0x0000, 0xEC00, 0xB0B0, 0xB0B0, 0x0000, 0xEC00, 0xB0B0, 0xB0B0, 0x0000, 0xEC00, 
   0xB03F, 0xB0B0, 0x0000, 0xEC00, 0xB0B0, 0xB0B0, 0x0000, 0xEC00, 0xB0B0, 0xB03F, 
   0x0000, 0xEC00, 0xB0B0, 0xB0B0, 0xD7D7, 0xC0D7, 0xC0C0, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xC0C0, 0xD7D7, 0xD7B0, 0xD7D7, 0xC0C0, 0xC0C0, 0xB0B0, 0xD7D7, 0xC0C0, 0xC0C0, 
   0xB0B0, 0xD73F, 0xC0C0, 0xC0C0, 0xB0B0, 0xB0B0, 0xC0C0, 0xC0C0, 0xB0B0, 0xB0B0, 
   0xD7B0, 0xC0C0, 0xB03F, 0xB0B0, 0x3FB0, 0xC0D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xD7D7, 0xD7D7, 0xC0D7, 0xD7D7, 0xD7D7, 0xC0D7, 0xC0C0, 0xC0C0, 0xC0C0, 
   0xC0C0, 0xD7C0, 0xD7C0, 0xC0D7, 0xC0C0, 0xD7D7, 0xD7D7, 0xC0D7, 0xD7C0, 0xD7D7, 
   0xD7D7, 0xC0C0, 0xD7D7, 0xD7D7, 0xC0C0, 0xD7C0, 0xD7D7, 0xD7D7, 0xC0C0, 0x58C4, 
   0xABC4, 0xABE7, 0xD7C0, 0xC4AB, 0xE7AB, 0xABAB, 0xD7D7, 0xABD7, 0xE7AB, 0xABAB, 
   0xD7D7, 0xABD7, 0x3F3F, 0xABE7, 0xD7D7, 0xD7D7, 0xC03F, 0xABE7, 0xD7D7, 0xD7D7, 
   0x3FAB, 0xAB3F, 0xD7D7, 0xD7D7, 0xABD7, 0xABAB, 0xD7D7, 0xD7D7, 0x3FD7, 0xABAB, 
   0x3FAB, 0xC4C0, 0xD7C0, 0xD7D7, 0x3FAB, 0xC03F, 0xD73F, 0xD7D7, 0x3F3F, 0x3F3F, 
   0xD73F, 0xD7D7, 0xC03F, 0x3FC0, 0xD7D7, 0xD7D7, 0xC03F, 0x3FC0, 0xD7D7, 0xC0D7, 
   0xC0C0, 0xD73F, 0xD7D7, 0xC0C0, 0x3F3F, 0xD7C0, 0xC0D7, 0xD7C0, 0xC03F, 0xD73F, 
   0xC0C0, 0xD7D7, 0xD7D7, 0xC0D7, 0xC0C0, 0xD7D7, 0xD7D7, 0xC0C0, 0xC0D7, 0xC0C0, 
   0xC0D7, 0xD7C0, 0xD7D7, 0xC0C0, 0xC0C0, 0xD7D7, 0xD7D7, 0xC0C0, 0xD7C0, 0xD7D7, 
   0xD7D7, 0xC0D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xC0D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xC0D7, 
   0xD7D7, 0xD7D7, 0xD7D7, 0xC0D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 0xD7D7, 
   0xD7D7, 0x3FD7, 0xC0C0, 0xC0C0, 0xC0C0, 0xB03F, 0xD7C0, 0xC0D7, 0x3FC0, 0xB0B0, 
   0xD7D7, 0xC0D7, 0x3FC0, 0xB0B0, 0xD7D7, 0xC0C0, 0xB03F, 0xB0B0, 0xC0C0, 0x3FC0, 
   0xB0B0, 0xB0B0, 0xC0C0, 0xB03F, 0xB03F, 0xB03F, 0xB03F, 0x8CB0, 0x0000, 0x0000, 
   0xB0B0, 0x8CB0, 0x0000, 0x0000, 0xB0B0, 0x8CB0, 0x0000, 0x0000, 0xB0B0, 0x8CB0, 
   0x0000, 0x0000, 0xB0B0, 0x8CB0, 0x0000, 0x0000, 0xB0B0, 0x8CB0, 0x0000, 0x0000, 
   0x3FB0, 0x8CB0, 0x0000, 0x0000, 0xB0B0, 0x8CB0, 0x0000, 0x0000, 0x0000, 0xEC00, 
   0xB03F, 0xB0B0, 0x0000, 0xEC00, 0xB0B0, 0xB03F, 0x0000, 0xEC00, 0xB0B0, 0xB0B0, 
   0x0000, 0xEC00, 0xB0B0, 0xB0B0, 0x0000, 0xEC00, 0xB03F, 0xB03F, 0x0000, 0xEC00, 
   0x3F3F, 0x3F3F, 0x0000, 0xEC00, 0x3F3F, 0x3F3F, 0x0000, 0x8C00, 0x8C8C, 0x8C8C, 
   0xB0B0, 0x3FB0, 0xB0B0, 0xB0B0, 0xB0B0, 0xB0B0, 0x3FB0, 0x3FB0, 0xB03F, 0xB03F, 
   0xB03F, 0xB03F, 0xB0B0, 0x3FB0, 0x3FB0, 0x3F3F, 0xB03F, 0xB03F, 0xB03F, 0x3F3F, 
   0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x8C8C, 0x8C8C, 
   0x8C8C, 0x8C8C, 0xC0C0, 0xD7D7, 0xD7D7, 0xD7D7, 0xC0B0, 0xD7C0, 0xD7D7, 0xD7D7, 
   0x3F3F, 0xC03F, 0xD7D7, 0xD7D7, 0x3F3F, 0x3F3F, 0x3F3F, 0xD7D7, 0x3F3F, 0xC93F, 
   0x3F3F, 0x3FC9, 0x3F3F, 0x3F3F, 0x3F3F, 0xC93F, 0x3F3F, 0x3F3F, 0x3FC9, 0x3FC9, 
   0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xD7D7, 0xD7D7, 0x3FD7, 0xAB3F, 0xD7D7, 0xD7D7, 
   0xABAB, 0xAB3F, 0xD7D7, 0xABD7, 0xE7AB, 0x3FAB, 0xABD7, 0xABAB, 0xE7E7, 0xABAB, 
   0xC9C9, 0xE7C9, 0xABE7, 0xABAB, 0xC93F, 0xABAB, 0xC9C9, 0xABAB, 0x3FC9, 0xC9C9, 
   0xC9C9, 0xC9C9, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xC03F, 0xC03F, 0xD7C0, 0xD7D7, 
   0x3FC0, 0xC03F, 0xD7D7, 0xD7D7, 0x3F3F, 0x3F3F, 0xD7D7, 0xD7D7, 0x3F3F, 0xB03F, 
   0xD73F, 0xD7D7, 0x3FC9, 0x3FB0, 0xB0B0, 0xB03F, 0xC9AB, 0xB0C9, 0x3FB0, 0x3FB0, 
   0xC9C9, 0xB0B0, 0xB03F, 0xB03F, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xD7D7, 0xD7D7, 
   0xD7D7, 0xC0D7, 0xD7D7, 0xD7D7, 0xD7D7, 0x3F3F, 0xD7D7, 0xD7D7, 0x3F3F, 0x3F3F, 
   0x3FD7, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 
   0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 
   0x3FC0, 0x3FB0, 0xB0B0, 0xB0B0, 0xB03F, 0xB03F, 0xB03F, 0xB03F, 0x3FB0, 0x3FB0, 
   0x3FB0, 0x3FB0, 0x3F3F, 0xB03F, 0xB03F, 0xB0B0, 0x3F3F, 0x3FB0, 0x3FB0, 0x3FB0, 
   0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x3F3F, 0x8C8C, 0x8C8C, 
   0x8C8C, 0x8C8C, 0xB0B0, 0x8CB0, 0x0000, 0x0000, 0x3FB0, 0x8CB0, 0x0000, 0x0000, 
   0xB0B0, 0x8CB0, 0x0000, 0x0000, 0xB0B0, 0x8CB0, 0x0000, 0x0000, 0x3FB0, 0x8CB0, 
   0x0000, 0x0000, 0x3F3F, 0x8C3F, 0x0000, 0x0000, 0x3F3F, 0x8C3F, 0x0000, 0x0000, 
   0x8C8C, 0x8C8C, 0x0000, 0x0000, 
};

