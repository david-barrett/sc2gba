#include "gba.h"
#include "sc.h"

extern pOAMEntry sprites;
extern pRotData rotData;

void RestoreGFXFury(pPlayer p)
{
	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].object.life>0)
		{
			if(p->weapon[i].type==SIMPLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 =SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+64 | PRIORITY(1);
			}
		}
	}
}



void RestoreGFX(pPlayer pl)
{

switch(pl->ship)
{
	case DREADNAUGHT:
		RestoreGFXDreadnaught(pl);
		break;
	case FURY:
		RestoreGFXFury(pl);
		break;
	default:
		break;
	 }

}
