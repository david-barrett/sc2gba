// ************************************
// ***        bmp2spr.exe  v1.0     ***
// ***                              ***
// *** By DMP SOFTWARE [27/06/2002] ***
// ***                              ***
// ***     www.dmpsoftware.co.uk    ***
// ************************************

#define UTWIGPILOT_WIDTH  (64)
#define UTWIGPILOT_HEIGHT (32)


const u16 utwigpilotData[] =
{
   0x0000, 0xEC00, 0xECEC, 0xECEC, 0x0000, 0xEC00, 0x5B5B, 0x5B5B, 0x0000, 0xEC00, 
   0x5B5B, 0x5B5B, 0x0000, 0xEC00, 0x5B5B, 0x5B5B, 0x0000, 0xEC00, 0x5B5B, 0x5B5B, 
   0x0000, 0xEC00, 0x5B5B, 0x5B5B, 0x0000, 0xEC00, 0x5B5B, 0x5B5B, 0x0000, 0xEC00, 
   0x5B5B, 0x5B5B, 0xECEC, 0xECEC, 0xECEC, 0xECEC, 0x5B5B, 0x8CBA, 0x8C8C, 0x8C8C, 
   0x5B5B, 0x8CBA, 0x8C8C, 0x8C8C, 0x5B5B, 0x8CBA, 0x0E8C, 0xBA8C, 0xBA5B, 0x8C8C, 
   0x8C8C, 0x008C, 0xBA5B, 0xC0C0, 0xC0C0, 0x008C, 0xBA5B, 0xC0C0, 0xC0C0, 0x00BA, 
   0xC0BA, 0x8CC0, 0xC0C0, 0x0000, 0xECEC, 0xECEC, 0xECEC, 0xECEC, 0x00BA, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xA600, 0x0000, 0x0000, 
   0x0000, 0x0500, 0x0000, 0x0000, 0x0000, 0x068B, 0xECEC, 0xECEC, 0xECEC, 0xECEC, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xA600, 0xA6A6, 0xA6A6, 0xA600, 0x058B, 
   0x8B8B, 0x8B8B, 0x8BA6, 0x0605, 0x0606, 0x0505, 0x058B, 0xD706, 0xB7B7, 0x0606, 
   0xD706, 0xB7B7, 0xD7D7, 0xD7B7, 0x76D7, 0xD1D7, 0xD19C, 0xB776, 0xECEC, 0xECEC, 
   0xECEC, 0xECEC, 0x0000, 0x0000, 0x0000, 0x0000, 0xA6A6, 0xA6A6, 0x0000, 0x0000, 
   0x8B8B, 0x8B8B, 0xA6A6, 0x0000, 0x0605, 0x0606, 0xA68B, 0x00A6, 0xD706, 0xB7B7, 
   0x06D7, 0xA605, 0xD7B7, 0xD7D7, 0x06D7, 0x0506, 0xD1D7, 0xD19C, 0x76D7, 0x06D7, 
   0xECEC, 0xECEC, 0xECEC, 0xECEC, 0x0000, 0x0000, 0xBA00, 0x8C8C, 0x0000, 0x0000, 
   0x0000, 0x8CBA, 0x0000, 0x0000, 0x0000, 0x8C00, 0x0000, 0x0000, 0x0000, 0xBA00, 
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x008B, 0x0000, 
   0x0000, 0x0000, 0xECEC, 0xECEC, 0xECEC, 0xECEC, 0x8C8C, 0xBA8C, 0x5B5B, 0x5B5B, 
   0x8C8C, 0xBA8C, 0x5BBA, 0x5B5B, 0x0E8C, 0x8C8C, 0x5BBA, 0x5B5B, 0x8C8C, 0x8C8C, 
   0x5B8C, 0x5B5B, 0x8C8C, 0x8C8C, 0xBA8C, 0x5B5B, 0x8C8C, 0x8C8C, 0xBA8C, 0x5B5B, 
   0x8C8C, 0xC08C, 0xBAC0, 0x5B5B, 0xECEC, 0xECEC, 0x0000, 0x0000, 0x5B5B, 0x8C5B, 
   0x0000, 0x0000, 0x5B5B, 0x8C5B, 0x0000, 0x0000, 0x5B5B, 0x8C5B, 0x0000, 0x0000, 
   0x5B5B, 0x8C5B, 0x0000, 0x0000, 0x5B5B, 0x8C5B, 0x0000, 0x0000, 0x5B5B, 0x8C5B, 
   0x0000, 0x0000, 0x5B5B, 0x8C5B, 0x0000, 0x0000, 0x0000, 0xEC00, 0x5B5B, 0x5B5B, 
   0x0000, 0xEC00, 0x5B5B, 0x5B5B, 0x0000, 0xEC00, 0xBA5B, 0x5B5B, 0x0000, 0xEC00, 
   0x5B5B, 0x5B5B, 0x0000, 0xEC00, 0xBA5B, 0x5B5B, 0x0000, 0xEC00, 0x5BBA, 0x5B5B, 
   0x0000, 0xEC00, 0xBA5B, 0x5B5B, 0x0000, 0xEC00, 0x5BBA, 0x5B5B, 0x8CBA, 0x8C8C, 
   0x8C8C, 0x0000, 0xC0BA, 0xC0C0, 0xC0C0, 0x0000, 0x8CBA, 0x8C8C, 0x8C8C, 0x0000, 
   0xBA5B, 0x8C8C, 0x8C8C, 0x0000, 0xBA5B, 0x0E8C, 0x8C8C, 0x00BA, 0xBA5B, 0x8C8C, 
   0x8C8C, 0x008C, 0x5B5B, 0x8CBA, 0x8C8C, 0xBA8C, 0x5B5B, 0x8CBA, 0x8C8C, 0x8C8C, 
   0x0000, 0x0000, 0x0000, 0x068B, 0x0000, 0x0000, 0x0000, 0xD705, 0x0000, 0x0000, 
   0x0000, 0xB706, 0x0000, 0x0000, 0x0000, 0xB706, 0x0000, 0x0000, 0x0000, 0xB7D7, 
   0x0000, 0x0000, 0x0000, 0xD7D7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
   0x0000, 0x0000, 0xB7B7, 0xA6D7, 0xA6D1, 0xD7D7, 0xD776, 0xD7B7, 0xD7D7, 0xB706, 
   0xD776, 0xD7B7, 0x06D7, 0x7676, 0x7676, 0xB7B7, 0x76B7, 0x7676, 0x7376, 0x7676, 
   0x76B7, 0x7673, 0x73B7, 0x7676, 0x7376, 0xB776, 0xB7D7, 0x7373, 0x7673, 0xD1B7, 
   0x0500, 0xB7B7, 0xD7B7, 0xD1D1, 0xA606, 0xA6D1, 0xD7D7, 0x0676, 0xD7D7, 0xD7D7, 
   0xB7B7, 0x76B7, 0xB776, 0xB7B7, 0xB7D7, 0x7676, 0x7676, 0xD776, 0x7376, 0x7376, 
   0x7373, 0x7676, 0x7673, 0x7673, 0x7376, 0x7673, 0x7376, 0x7673, 0xB7D7, 0x7376, 
   0x7373, 0xD776, 0xD79C, 0xB7B7, 0xD7B7, 0x0005, 0x0005, 0x0000, 0x0000, 0x0000, 
   0x0006, 0x0000, 0x0000, 0x0000, 0x0006, 0x0000, 0x0000, 0x0000, 0x0006, 0x0000, 
   0x0000, 0x0000, 0x0006, 0x0000, 0x0000, 0xBA00, 0x0006, 0x0000, 0x0000, 0x8C00, 
   0x008B, 0x0000, 0x0000, 0x8CBA, 0x0000, 0x0000, 0x0000, 0x8C8C, 0x8C8C, 0x8CC0, 
   0xBA8C, 0x5B5B, 0xC0C0, 0xC0C0, 0x5BC0, 0x5B5B, 0x8C8C, 0x8C8C, 0x5B8C, 0x5B5B, 
   0x8C8C, 0x8C8C, 0x5B8C, 0x5B5B, 0x8C8C, 0x8C8C, 0x5B8C, 0xBA5B, 0x0E8C, 0x8C8C, 
   0x5B5B, 0x5BBA, 0x8C8C, 0x8C8C, 0x5B5B, 0xBA5B, 0x8C8C, 0x5B8C, 0x5B5B, 0x5BBA, 
   0x5B5B, 0x8C5B, 0x0000, 0x0000, 0x5B5B, 0x8C5B, 0x0000, 0x0000, 0x5B5B, 0x8C5B, 
   0x0000, 0x0000, 0x5B5B, 0x8C5B, 0x0000, 0x0000, 0xBA5B, 0x8C5B, 0x0000, 0x0000, 
   0x5BBA, 0x8C5B, 0x0000, 0x0000, 0xBA5B, 0x8C5B, 0x0000, 0x0000, 0xBABA, 0x8CBA, 
   0x0000, 0x0000, 0x0000, 0xEC00, 0xBABA, 0x5B5B, 0x0000, 0xEC00, 0x5BBA, 0x5B5B, 
   0x0000, 0xEC00, 0xBA5B, 0x5B5B, 0x0000, 0xEC00, 0x5BBA, 0x5B5B, 0x0000, 0xEC00, 
   0xBA5B, 0x5B5B, 0x0000, 0xEC00, 0x5BBA, 0x5B5B, 0x0000, 0xEC00, 0xBABA, 0xBA5B, 
   0x0000, 0xEC00, 0xBABA, 0x5BBA, 0x5B5B, 0xBA5B, 0x8C8C, 0x8C8C, 0x5B5B, 0xBA5B, 
   0x8CBA, 0x8C0E, 0x5B5B, 0x5B5B, 0x8CBA, 0x8C8C, 0x5B5B, 0x5B5B, 0xBA5B, 0x8C8C, 
   0x5B5B, 0x5B5B, 0x5B5B, 0xC0BA, 0x5B5B, 0x5B5B, 0x5B5B, 0xBA5B, 0x5B5B, 0x5B5B, 
   0x5B5B, 0x5B5B, 0x5B5B, 0x5B5B, 0x5B5B, 0x5B5B, 0x008C, 0x0000, 0x0000, 0x0000, 
   0x8C8C, 0x00BA, 0x0000, 0x0000, 0x8C8C, 0xBA8C, 0x0000, 0x0000, 0xC08C, 0x8CC0, 
   0x8C8C, 0x06BA, 0xC0C0, 0xC0C0, 0x058C, 0x0606, 0xC0BA, 0x058C, 0xD706, 0x06D7, 
   0xBA5B, 0xD705, 0xB7D7, 0xD7D7, 0x055B, 0xD7D7, 0xB7D7, 0xD7B7, 0x0000, 0x0000, 
   0xD100, 0x9CD1, 0x0000, 0x8B8B, 0x9CD1, 0x9CD1, 0x8B8B, 0x0505, 0xD105, 0xD1D1, 
   0x0605, 0x0506, 0x0505, 0x0505, 0x0605, 0x0606, 0x0506, 0x0505, 0x0506, 0x0606, 
   0x0606, 0x0506, 0x0606, 0x0506, 0x0605, 0x1F1F, 0xD7D7, 0xD7D7, 0xD7D7, 0xCA04, 
   0xD1D1, 0x0000, 0x0000, 0x0000, 0x9CD1, 0x8B8B, 0x008B, 0x0000, 0x05D1, 0x0505, 
   0x8B05, 0x008B, 0x0505, 0x0505, 0x0606, 0x0605, 0x0505, 0x0606, 0x0606, 0x0605, 
   0x0606, 0x0606, 0x0506, 0x0606, 0x061F, 0x0505, 0x0606, 0xD706, 0xD704, 0xD7D7, 
   0xD7D7, 0xD7D7, 0x0000, 0x0000, 0xBA00, 0x8C8C, 0x0000, 0x0000, 0x8CBA, 0x8C8C, 
   0x0000, 0xBA00, 0x8C8C, 0x8C8C, 0x5B00, 0x8CBA, 0x8C8C, 0x8C8C, 0x0506, 0x8CBA, 
   0x8CC0, 0x8C8C, 0x06D7, 0x8C05, 0x8C8C, 0x5B8C, 0xB7D7, 0x05D7, 0x5BA6, 0x5B5B, 
   0xB7B7, 0xD7D7, 0x5B05, 0x5B5B, 0x8C8C, 0x5B8C, 0x5B5B, 0xBA5B, 0x8C8C, 0x5BBA, 
   0x5B5B, 0xBABA, 0x8C8C, 0x5BBA, 0x5B5B, 0xBA5B, 0xBA8C, 0x5B5B, 0x5B5B, 0xBABA, 
   0x5B5B, 0x5B5B, 0x5B5B, 0x8C5B, 0x5B5B, 0x5B5B, 0x5B5B, 0xBABA, 0x5B5B, 0x5B5B, 
   0xBA5B, 0x8C5B, 0x5B5B, 0x5B5B, 0x5B5B, 0xBABA, 0xBA5B, 0x8C5B, 0x0000, 0x0000, 
   0xBABA, 0x8CBA, 0x0000, 0x0000, 0xBABA, 0x8C5B, 0x0000, 0x0000, 0xBABA, 0x8CBA, 
   0x0000, 0x0000, 0x8CBA, 0x8CBA, 0x0000, 0x0000, 0xBA8C, 0x8CBA, 0x0000, 0x0000, 
   0x8CBA, 0x8CBA, 0x0000, 0x0000, 0x8C8C, 0x8CBA, 0x0000, 0x0000, 0x0000, 0xEC00, 
   0xBABA, 0xBA5B, 0x0000, 0xEC00, 0xBABA, 0xBABA, 0x0000, 0xEC00, 0x8CBA, 0xBABA, 
   0x0000, 0xEC00, 0xBA8C, 0xBABA, 0x0000, 0xEC00, 0x8CBA, 0xBABA, 0x0000, 0xEC00, 
   0xBA8C, 0xBABA, 0x0000, 0xEC00, 0x8CBA, 0x8CBA, 0x0000, 0x8C00, 0x8C8C, 0x8C8C, 
   0xBA5B, 0x5B5B, 0x5B5B, 0x5B5B, 0x5BBA, 0x5B5B, 0x5B5B, 0x5B5B, 0xBA5B, 0xBA5B, 
   0x5B5B, 0x5B5B, 0xBABA, 0x5BBA, 0x5B5B, 0x5B5B, 0xBA5B, 0xBA5B, 0x5B5B, 0x8B5B, 
   0xBABA, 0x5BBA, 0x5B5B, 0x8B5B, 0xBABA, 0xBA5B, 0x5B5B, 0x8B5B, 0x8C8C, 0x8C8C, 
   0x8C8C, 0x8C8C, 0x065B, 0xD7D7, 0xD7D7, 0xB7B7, 0xB78B, 0xB7B7, 0xD7D7, 0xB7D7, 
   0xD78B, 0x7376, 0xB7B7, 0xB7B7, 0x0605, 0x0505, 0x7605, 0x7373, 0x0605, 0x06D7, 
   0x05D7, 0x0505, 0x0605, 0x06D7, 0xB7D7, 0x06D7, 0x0605, 0xD706, 0xB7B7, 0x06D7, 
   0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xD7B7, 0xD7D7, 0xD7D7, 0x1FD7, 0xB7B7, 0xB7B7, 
   0xD7D7, 0x06D7, 0xB7D7, 0xB7B7, 0xB7B7, 0x06B7, 0x7673, 0xD776, 0xB7B7, 0x06B7, 
   0x0505, 0x7605, 0x7376, 0x7673, 0xD706, 0x05D7, 0x0505, 0x8B05, 0xD7D7, 0xD7D7, 
   0xD7D7, 0x8B05, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xD7D7, 0xD7D7, 0xD7D7, 0xB7B7, 
   0xD7D7, 0xB7D7, 0xB7B7, 0xB7B7, 0xB7B7, 0xB7B7, 0xB7B7, 0xB7D7, 0xB7B7, 0xD7B7, 
   0x76B7, 0x7373, 0x7373, 0x7373, 0x0506, 0x0505, 0x0505, 0x0505, 0xD7B7, 0x0506, 
   0xD705, 0xD7D7, 0xD7B7, 0x0506, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 0xD7B7, 0xD7D7, 
   0x05D7, 0x5B5B, 0xD7D7, 0xB7D7, 0xB7D7, 0x5B8B, 0xB7B7, 0x7376, 0xD773, 0x5B8B, 
   0x7673, 0x0505, 0x0605, 0x5B05, 0x0505, 0xD706, 0x06D7, 0x8B05, 0xB776, 0xD706, 
   0x06D7, 0x8B05, 0xB776, 0x06D7, 0x06D7, 0x8B05, 0x8C8C, 0x8C8C, 0x8C8C, 0x8C8C, 
   0x5B5B, 0x5B5B, 0xBA5B, 0x8CBA, 0x5B5B, 0x5B5B, 0xBABA, 0xBABA, 0xBA5B, 0xBA5B, 
   0xBA5B, 0x8CBA, 0x5B5B, 0x5BBA, 0xBABA, 0xBABA, 0x5B5B, 0xBA5B, 0x8C5B, 0x8CBA, 
   0x5B5B, 0xBABA, 0xBABA, 0x8C8C, 0xBA5B, 0xBA5B, 0x8CBA, 0x8CBA, 0x8C8C, 0x8C8C, 
   0x8C8C, 0x8C8C, 0x8CBA, 0x8CBA, 0x0000, 0x0000, 0x8C8C, 0x8CBA, 0x0000, 0x0000, 
   0x8C8C, 0x8C5B, 0x0000, 0x0000, 0x8C8C, 0x8C5B, 0x0000, 0x0000, 0x8C8C, 0x8C5B, 
   0x0000, 0x0000, 0x8C8C, 0x8C5B, 0x0000, 0x0000, 0x8C8C, 0x8C5B, 0x0000, 0x0000, 
   0x8C8C, 0x8C8C, 0x0000, 0x0000, 
};

#define UTWIGPILOT_SIZE (1024)

