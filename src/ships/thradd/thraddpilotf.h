// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define THRADDPILOTF_WIDTH  (32)
#define THRADDPILOTF_HEIGHT (32)


const u16 thraddpilotfData[] =
{
   0xECEC, 0xECEC, 0xECEC, 0xECEC, 0xD6EC, 0xD6D6, 0xD6D6, 0xD6D6, 0xBEEC, 0xBEBE, 
   0xBEBE, 0xBEBE, 0x74EC, 0x7474, 0x7474, 0x7474, 0x74EC, 0x7474, 0x7474, 0x1074, 
   0x74EC, 0x7474, 0x7474, 0x1074, 0x74EC, 0x7474, 0x1010, 0x1010, 0x74EC, 0x1010, 
   0x1010, 0x1010, 0xECEC, 0xECEC, 0xECEC, 0xECEC, 0xD6D6, 0xD6D6, 0xD6D6, 0xD6D6, 
   0xBEBE, 0xBEBE, 0xBEBE, 0xBEBE, 0x7474, 0x7474, 0x1074, 0x7474, 0x1010, 0x1010, 
   0x1010, 0x1010, 0x10FB, 0x1010, 0x1010, 0xE110, 0x9310, 0xE1E1, 0xE1E1, 0xD7D7, 
   0xE1E1, 0xE117, 0xE1E1, 0x73E1, 0xECEC, 0xECEC, 0xECEC, 0x0000, 0xD6D6, 0xD6D6, 
   0x74D6, 0x0000, 0xBEBE, 0xBEBE, 0xBEBE, 0x0000, 0x7474, 0x7474, 0x7474, 0x0000, 
   0x1010, 0x1010, 0x1010, 0x0000, 0xE1E1, 0xE1E1, 0xE1E1, 0x0000, 0xE1E1, 0xE1E1, 
   0xD7E1, 0x0000, 0xD7D7, 0xE1E1, 0x73E1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x74EC, 0x1010, 
   0xE110, 0xBDBD, 0x74EC, 0x1010, 0xE1E1, 0xBDBD, 0x74EC, 0x1010, 0xE1E1, 0xBDBD, 
   0x74EC, 0x1010, 0xE1E1, 0xBDBD, 0x10EC, 0xFB10, 0xE193, 0xBDBD, 0xBDEC, 0xBDBD, 
   0x178C, 0xBD27, 0xBDEC, 0xBDBD, 0xBDBD, 0x9CBD, 0xBDEC, 0xBDBD, 0xBDBD, 0xBDBD, 
   0xE1BD, 0x27E1, 0xE1E1, 0xE1E1, 0xBDBD, 0xC6E1, 0xBD9C, 0xBDBD, 0xBDBD, 0xBDBD, 
   0xDBBD, 0xBDBD, 0xBDBD, 0xBDBD, 0xBDBD, 0xBD5A, 0xBDBD, 0xBDBD, 0xBDBD, 0x5ABD, 
   0xBDBD, 0xBDBD, 0xBDBD, 0xBDBD, 0xBDDB, 0xBDBD, 0xBDBD, 0x7373, 0x5ABD, 0xBD5A, 
   0x73BD, 0x7676, 0x7673, 0xD7D7, 0xC6C6, 0x0000, 0x73C6, 0xD776, 0xC6D7, 0x0000, 
   0x73BD, 0x7676, 0xC6D7, 0x0000, 0x06BD, 0x76B7, 0x0676, 0x0000, 0x05BD, 0xB7D7, 
   0x0676, 0x0000, 0x7373, 0xB7D7, 0xD7D7, 0x0000, 0x7676, 0xD7D7, 0x05D7, 0x0000, 
   0xD7B7, 0x0505, 0xD705, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xBDEC, 0xBDBD, 0xBDBD, 0xBDBD, 
   0xBDEC, 0xBDBD, 0xBDBD, 0xBDBD, 0xBDEC, 0xBDBD, 0xBDBD, 0xBDBD, 0xBDEC, 0xBDBD, 
   0xBDBD, 0xBDBD, 0xBDEC, 0xBDBD, 0xBDBD, 0xBDBD, 0xBDEC, 0xBDBD, 0xBDBD, 0xBDBD, 
   0xBDEC, 0xBDBD, 0xBDBD, 0x73BD, 0xBDEC, 0xBDBD, 0xBDBD, 0xB776, 0xBDBD, 0x8BBD, 
   0x7605, 0xB776, 0xBDBD, 0xB78B, 0x058B, 0x06B7, 0xBDBD, 0x7673, 0x8BB7, 0x0606, 
   0xBDBD, 0x7373, 0x0676, 0x068B, 0x76BD, 0x7673, 0xD7D7, 0x8B8B, 0xD705, 0x7676, 
   0xD7D7, 0xBD06, 0x7673, 0x7676, 0xD7D7, 0x76D7, 0x73D7, 0x7673, 0xD7D7, 0xD7D7, 
   0x0506, 0x8BBD, 0xB706, 0x0000, 0x8B06, 0x8BBD, 0xD7D7, 0x0000, 0x8B06, 0x8BBD, 
   0xD706, 0x0000, 0xBD8B, 0xBDBD, 0x068B, 0x0000, 0xBDBD, 0xBDBD, 0x8BBD, 0x0000, 
   0xBDBD, 0xBDBD, 0xBDBD, 0x0000, 0xBDBD, 0xBDBD, 0xBDBD, 0x0000, 0xBD76, 0xBDBD, 
   0xBDBD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0xC0EC, 0xBD8C, 0xBDBD, 0xD773, 0xECEC, 0xC00E, 
   0x738C, 0xBDD7, 0xE0EC, 0xECE0, 0xD70E, 0xBD06, 0xECEC, 0xE0EC, 0xD7EC, 0xBD06, 
   0x0EEC, 0xEC0E, 0xD7E0, 0xBD06, 0xC0EC, 0x0EC0, 0xE1EC, 0xD7E1, 0x8CEC, 0x0EC0, 
   0xE0EC, 0xE1E1, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0x0606, 0x7373, 0xD7D7, 0x0606, 
   0xBDBD, 0x7676, 0xD7D7, 0xBD06, 0x3908, 0xB7B7, 0xD7D7, 0xB039, 0xEE39, 0xB7B7, 
   0xD7D7, 0xCC39, 0x39CC, 0xD7D7, 0xD7D7, 0xCC39, 0x39BC, 0x0606, 0x0606, 0x08BC, 
   0xBC06, 0xBCBC, 0xBCBC, 0x0508, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xD7D7, 0xBDBD, 
   0xBDBD, 0x0000, 0x06BD, 0xBD76, 0x0505, 0x0000, 0xBDBD, 0x0606, 0x0505, 0x0000, 
   0x05BD, 0x0606, 0x0505, 0x0000, 0x0605, 0x0506, 0xEC05, 0x0000, 0x0606, 0x058B, 
   0x0EEC, 0x0000, 0x8B05, 0x8B8B, 0x0EEC, 0x0000, 0x8C8C, 0x8C8C, 0x8C8C, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 
};

#define THRADDPILOTF_SIZE (512)

