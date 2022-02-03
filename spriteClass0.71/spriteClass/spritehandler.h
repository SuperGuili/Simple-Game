#pragma once
#include "sprite.h"

class spritehandler
{
private:
	int count;
	sprite *sprites[10];

public:
	spritehandler(void);
	~spritehandler(void);
	void add(sprite *spr);
	void create();
	sprite *get(int index);
	int size() { return count; }
};