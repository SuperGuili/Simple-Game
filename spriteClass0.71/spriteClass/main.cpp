///////////////////////////////////////////////////////
// Game Programming All In One, Third Edition
// Chapter 10 - SpriteClass
///////////////////////////////////////////////////////

#include <stdio.h>
#include "allegro.h"
#include "sprite.h"
#include "spritehandler.h"

#define BLACK makecol(0,0,0)
#define WHITE makecol(255,255,255)

#define NUM 3
#define WIDTH 800
#define HEIGHT 600
#define MODE GFX_AUTODETECT_WINDOWED

//define the sprite handler object
spritehandler *balls;

void bouncesprite(sprite *spr)
{
    //simple screen bouncing behavior
    if (spr->x < 0)
    {
        spr->x = 0;
        spr->velx = rand() % 2 + 4;
        spr->animdir *= -1;
    }

    else if (spr->x > SCREEN_W - spr->width)
    {
        spr->x = SCREEN_W - spr->width;
        spr->velx = rand() % 2 - 6;
        spr->animdir *= -1;
    }

    if (spr->y < 20)
    {
        spr->y = 20;
        spr->vely = rand() % 2 + 4;
        spr->animdir *= -1;
    }

    else if (spr->y > SCREEN_H - spr->height)
    {
        spr->y = SCREEN_H - spr->height;
        spr->vely = rand() % 2 - 6;
        spr->animdir *= -1;
    }
}

void checkcollisions(int num)
{
    int cx1,cy1,cx2,cy2;

    for (int n=0; n<NUM; n++)
    {
        if (n != num && balls->get(n)->collided(balls->get(num)))
        {
            //calculate center of primary sprite
            cx1 = (int)balls->get(n)->x + balls->get(n)->width / 2;
            cy1 = (int)balls->get(n)->y + balls->get(n)->height / 2;
            
            //calculate center of secondary sprite
            cx2 = (int)balls->get(num)->x + balls->get(num)->width / 2;
            cy2 = (int)balls->get(num)->y + balls->get(num)->height / 2;
            
            //figure out which way the sprites collided
            if (cx1 <= cx2)
            {
                balls->get(n)->velx = -1 * rand() % 6 + 1;
                balls->get(num)->velx = rand() % 6 + 1;
                if (cy1 <= cy2)
                {
                    balls->get(n)->vely = -1 * rand() % 6 + 1;
                    balls->get(num)->vely = rand() % 6 + 1;
                }
                else
                {
                    balls->get(n)->vely = rand() % 6 + 1;
                    balls->get(num)->vely = -1 * rand() % 6 + 1;
                }
            }
            else
            {
                //cx1 is > cx2
                balls->get(n)->velx = rand() % 6 + 1;
                balls->get(num)->velx = -1 * rand() % 6 + 1;
                if (cy1 <= cy2)
                {
                    balls->get(n)->vely = rand() % 6 + 1;
                    balls->get(num)->vely = -1 * rand() % 6 + 1;
                }
                else
                {
                    balls->get(n)->vely = -1 * rand() % 6 + 1;
                    balls->get(num)->vely = rand() % 6 + 1;
                }
            }
        }
    }
}


int main(void)
{
	sprite *ball;
    BITMAP *bg;
    BITMAP *buffer;
    int n;

    //initialization
    allegro_init();
    set_color_depth(16);
    set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    install_keyboard();
    install_timer();
    srand(time(NULL));
    
    //create back buffer
    buffer = create_bitmap(SCREEN_W, SCREEN_H);

	//load background
	bg = load_bitmap("bluespace.bmp", NULL);
	if (!bg) {
		allegro_message("Error loading background");
		return 1;
	}

	//create the sprite handler
	balls = new spritehandler();

    //create the ball sprites
    for (n=0; n<NUM; n++)
    {
		//create new ball sprite
		ball = new sprite();

		//load the ball image
		if (!ball->load("sphere.bmp")) {
			allegro_message("Error loading ball image");
			return 1;
		}

		//set sprite properties
        ball->x = rand() % (SCREEN_W - 64);
        ball->y = rand() % (SCREEN_H - 64);
        ball->width = 64;
        ball->height = 64;
        ball->velx = rand() % 5 + 1;
        ball->vely = rand() % 5 + 1;
		ball->animcolumns = 8;
        ball->curframe = rand() % 32;
		ball->totalframes = 31;
        ball->animdir = 1;

		//add this sprite to the handler
		balls->add(ball);
    }


    //game loop
    while (!key[KEY_ESC])
    {
        //draw the background
		blit(bg, buffer, 0, 0, 0, 0, WIDTH, HEIGHT);

		//print the program title
		textout_ex(buffer, font, "SpriteClass Program (ESC to quit)", 
			0, 0, WHITE, -1);

        //update positions and animations
        for (n=0; n<NUM; n++)
        {
            balls->get(n)->updatePosition();
			balls->get(n)->updateAnimation();
            bouncesprite(balls->get(n));
            checkcollisions(n);
        }

        //draw the sprites
        for (n=0; n<NUM; n++)
			balls->get(n)->drawframe(buffer);

        //update the screen
        acquire_screen();
        blit(buffer,screen,0,0,0,0,WIDTH,HEIGHT);
        release_screen();
        rest(10);
    }

	//shutdown
	destroy_bitmap(buffer);
	destroy_bitmap(bg);
	delete balls;
	allegro_exit();
    return 0;
}
END_OF_MAIN();
