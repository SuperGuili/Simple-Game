#include <allegro.h>
#include "sprite.h"


sprite::sprite()
{
    image = NULL;
    x = 0.0f; 
    y = 0.0f;
}

sprite::~sprite() {
    //remove bitmap from memory
    if (image != NULL)
        destroy_bitmap(image);
}