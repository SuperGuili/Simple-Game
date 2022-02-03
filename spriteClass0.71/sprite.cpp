#include <allegro.h>
#include "sprite.h"


sprite::sprite() {
    image = NULL;
    alive = 1;
    direction = 0;
    animcolumns = 0;
    animstartx = 0; 
    animstarty = 0;
    x = 0.0f; 
    y = 0.0f;
    width = 0; 
    height = 0;
    xdelay = 0; 
    ydelay = 0;
    xcount = 0; 
    ycount = 0;
    velx = 0.0; 
    vely = 0.0;
    speed = 0.0;
    curframe = 0; 
    totalframes = 1;
    framecount = 0; 
    framedelay = 4;
    animdir = 1;
    faceAngle = 0; 
    moveAngle = 0;
	score = 0;
}

sprite::~sprite() {
    //remove bitmap from memory
    if (image != NULL)
        destroy_bitmap(image);
}

int sprite::load(char *filename) {
	image = load_bitmap(filename, NULL);
	if (image == NULL) return 0;
	width = image->w;
	height = image->h;
    return 1;
}

void sprite::draw(BITMAP *dest) 
{
	draw_sprite(dest, image, (int)x, (int)y);
}

void sprite::drawframe(BITMAP *dest)
{
    int fx = animstartx + (curframe % animcolumns) * width;
    int fy = animstarty + (curframe / animcolumns) * height;
    masked_blit(image,dest,fx,fy,(int)x,(int)y,width,height);
}

void sprite::updatePosition()
{
    //update x position
    if (++xcount > xdelay)
    {
        xcount = 0;
        x += velx;
    }

    //update y position
    if (++ycount > ydelay)
    {
        ycount = 0;
        y += vely;
    }
}

void sprite::updateAnimation() 
{
    //update frame based on animdir
    if (++framecount > framedelay)
    {
        framecount = 0;
		curframe += animdir;

		if (curframe < 0) {
            curframe = totalframes-1;
		}
		if (curframe > totalframes-1) {
            curframe = 0;
        }
    }
}

int sprite::inside(int x,int y,int left,int top,int right,int bottom)
{
    if (x > left && x < right && y > top && y < bottom)
        return 1;
    else
        return 0;
}

int sprite::pointInside(int px,int py)
{
	return inside(px, py, (int)x, (int)y, (int)x+width, (int)y+height);
}

int sprite::collided(sprite *other, int shrink)
{
	int wa = (int)x + width;
    int ha = (int)y + height;
    int wb = (int)other->x + other->width;
    int hb = (int)other->y + other->height;

    if (inside((int)x, (int)y, (int)other->x+shrink, (int)other->y+shrink, wb-shrink, hb-shrink) ||
        inside((int)x, ha, (int)other->x+shrink, (int)other->y+shrink, wb-shrink, hb-shrink) ||
        inside(wa, (int)y, (int)other->x+shrink, (int)other->y+shrink, wb-shrink, hb-shrink) ||
        inside(wa, ha, (int)other->x+shrink, (int)other->y+shrink, wb-shrink, hb-shrink))
        return 1;
    else
        return 0;
}