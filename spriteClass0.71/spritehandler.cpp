#include "spritehandler.h"

spritehandler::spritehandler(void)
{
	count = 0;
}

spritehandler::~spritehandler(void)
{
    //delete the sprites
	for (int n = 0; n < count; n++)
		delete sprites[n];
}

void spritehandler::add(sprite *spr) 
{
	if (spr != NULL) {
		sprites[count] = spr;
		count++;
	}
}

void spritehandler::create() 
{
	sprites[count] = new sprite();
	count++;
}

sprite *spritehandler::get(int index)
{
	return sprites[index];
}