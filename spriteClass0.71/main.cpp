//1942 TEST V0.7 ALPHA

#include <stdio.h>
#include "allegro.h"
#include "sprite.h"
#include "spritehandler.h"

#define BLACK makecol(0,0,0)
#define WHITE makecol(255,255,255)
#define RED makecol(255,0,0)
#define WIDTH 800
#define HEIGHT 900
#define MODE GFX_AUTODETECT_WINDOWED
#define BULLETS 100
#define	ENEMIES 50

volatile long speed_counter = 0;
volatile int counter;
volatile int ticks;
volatile int framerate;

//define the sprite handler object
spritehandler *game;

void increment_speed_counter()
{
  speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter);

//calculate framerate every second
void timer1(void)
{
    counter++;
    framerate = ticks;
    ticks=0;
}
END_OF_FUNCTION(timer1);

//check collision function
void checkcollisions(int num)
{
    int cx1,cy1,cx2,cy2;

    for (int n=0; n<201; n++)
    {
		if (n != num && game->get(n)->collided(game->get(num)) && game->get(n)->alive == 1 
			&& n != 151 && n != 152 && n != 153 && n != 154 
			&& n != 155 && n != 156 && n != 157 && n != 158 && n != 159 && n != 160
			&& n != 161 && n != 162 && n != 163 && n != 164 && n != 165 && n != 166
			&& n != 167 && n != 168 && n != 169 && n != 170 && n != 171)
		{
            //calculate center of primary sprite
            cx1 = (int)game->get(n)->x + game->get(n)->width / 2;
            cy1 = (int)game->get(n)->y + game->get(n)->height / 2;
            
            //calculate center of secondary sprite
            cx2 = (int)game->get(num)->x + game->get(num)->width / 2;
            cy2 = (int)game->get(num)->y + game->get(num)->height / 2;
            
            //collided reactions
            if (cx1 <= cx2 && cy1 <= cy2 && (cx2-cx1) < 20 && (cy2-cy1 < 20))
            {
				for(int i=151; i<171; i++)
				{
					if(game->get(i)->alive == 0)
					{
						game->get(i)->alive = 1;
						game->get(i)->x = game->get(num)->x +15;
						game->get(i)->y = game->get(num)->y;
						game->get(i)->xcount = 0;
						break;
					}
				}

					game->get(n)->alive = 0;
					game->get(n)->x = rand() % WIDTH;
					game->get(num)->alive = 0;
					game->get(100)->score += 10;				
			}

            if (cx1 >= cx2 && cy1 >= cy2 && (cx1-cx2) < 20 && (cy1-cy2 < 20))
            {
				for(int i=151; i<171; i++)
				{
					if(game->get(i)->alive == 0)
					{
						game->get(i)->alive = 1;
						game->get(i)->x = game->get(num)->x +15;
						game->get(i)->y = game->get(num)->y;
						game->get(i)->xcount = 0;
						break;
					}
				}

				game->get(n)->alive = 0;
				game->get(n)->x = rand() % WIDTH;
				game->get(num)->alive = 0;
				game->get(100)->score += 10;         
            }			
        }
    }
}

int main(void)
{
	sprite *p38;
	sprite *bullet;
	sprite *enemyplane;
	sprite *explosion;
	sprite *enemybullet;
	BITMAP *buffer;
    
	SAMPLE *snd_bullet;
	SAMPLE *snd_p38;
	SAMPLE *snd_explosion;

	int panning = 128;
	int pitch = 1000;
	int volume = 255;

	int firecount = 0;
	int firedelay = 100;
	int n = 0;
	int number_enemies = 5;
	int enemy_fire = 2;
	int enemy_fire_count = 0;
    
    //initialization
    allegro_init();
    set_color_depth(16);
    set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    install_keyboard();
    install_timer();
    srand(time(NULL));
	install_sound(DIGI_AUTODETECT, MIDI_NONE, ".");

	LOCK_VARIABLE(speed_counter);
	LOCK_FUNCTION(increment_speed_counter);
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(250));

	//identify variables used by interrupt function
    LOCK_VARIABLE(counter);
    LOCK_VARIABLE(framerate);
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(timer1);

    //create new interrupt handler
    install_int(timer1, 1000);

	//load the sound files
	snd_bullet = load_sample("snd_bullet.wav");
	snd_p38 = load_sample("snd_p38.wav");
	snd_explosion = load_sample("snd_explosion.wav");
    
    //create back buffer
    buffer = create_bitmap(SCREEN_W, SCREEN_H);

	//create the sprite handler
	game = new spritehandler();

////////////////////////////////////////////////////////////

	//create the bullet sprites from 0 to 99
    for (n=0; n<BULLETS; n++)
    {
		//create new bullet sprite
		bullet = new sprite();

		//load the bullet image
		if (!bullet->load("bullets.bmp")) {
			allegro_message("Error loading bullet image");
			return 1;
		}

		//set sprite properties
		bullet->x = 500;
		bullet->y = 500;
        bullet->width = 64;
        bullet->height = 32;
        bullet->velx = 0;
        bullet->vely = -2;
		bullet->animdir = 1;
		bullet->alive = 0;
		bullet->ydelay = 1;

		//add this sprite to the handler
		game->add(bullet);
    }

//////////////////////////////////////////////////////////////////
	//create the P38 sprite = 100 

	p38 = new sprite();

	//load the image
	if (!p38->load("p38.bmp")) {
		allegro_message("Error loading P38 image");
		return 1;
	}

	//set sprite properties
    p38->x = WIDTH/2;
    p38->y = HEIGHT*0.9;
    p38->width = 64;
    p38->height = 64;
    p38->velx = 0;
    p38->vely = 0;
	p38->animcolumns = 3;
    p38->curframe = 0;
	p38->totalframes = 3;
	p38->animdir = 1;
	p38->framedelay = 30;
	p38->score = 0;

	//add this sprite to the handler
	game->add(p38);

///////////////////////////////////////////////////////////////////
		
	//create the enemies sprite = 101 to 151 

	for (n=0; n<10; n++)
    {
		enemyplane = new sprite();

		//load the image
		if (!enemyplane->load("enemy4_strip3.bmp")) {
			allegro_message("Error loading enemy image");
			return 1;
		}

		//set sprite properties
		enemyplane->x = 200;
		enemyplane->y = 200;
		enemyplane->width = 32;
		enemyplane->height = 32;
		enemyplane->velx = 0;
		enemyplane->vely = 1;
		enemyplane->animcolumns = 3;
		enemyplane->curframe = 0;
		enemyplane->totalframes = 3;
		enemyplane->animdir = 1;
		enemyplane->framedelay = 20;
		enemyplane->alive = 0;
		enemyplane->xdelay = 8; 
		enemyplane->ydelay = 8;

		//add this sprite to the handler
		game->add(enemyplane);
	}
	
////////////////////////////////////////////////////////////////////

	for (n=10; n<50; n++)
    {
		enemyplane = new sprite();

		//load the image
		if (!enemyplane->load("enemy1_strip3.bmp")) {
			allegro_message("Error loading enemy image");
			return 1;
		}

		//set sprite properties
		enemyplane->x = 200;
		enemyplane->y = 200;
		enemyplane->width = 32;
		enemyplane->height = 32;
		enemyplane->velx = 0;
		enemyplane->vely = 1;
		enemyplane->animcolumns = 3;
		enemyplane->curframe = 0;
		enemyplane->totalframes = 3;
		enemyplane->animdir = 1;
		enemyplane->framedelay = 20;
		enemyplane->alive = 0;
		enemyplane->xdelay = 6; 
		enemyplane->ydelay = 6;

		//add this sprite to the handler
		game->add(enemyplane);
	}

///////////////////////////////////////////////////////////
	
	//create the explosion sprite = 151 to 171
	for (n=0; n<20; n++)
    {
		explosion = new sprite();

		//load the image
		if (!explosion->load("explosion1_strip6.bmp")) {
			allegro_message("Error loading explosion image");
			return 1;
		}

		//set sprite properties
		explosion->x = 0;
		explosion->y = 0;
		explosion->width = 32;
		explosion->height = 32;
		explosion->velx = 0;
		explosion->vely = 0;
		explosion->animcolumns = 6;
		explosion->curframe = 0;
		explosion->totalframes = 6;
		explosion->animdir = 1;
		explosion->framedelay = 30;
		explosion->alive = 0;
		explosion->xcount = 0;
		
		//add this sprite to the handler
		game->add(explosion);
	}
////////////////////////////////////////////////////////////

	//create the enemy bullet sprite = 171 to 201
	for (n=0; n<30; n++)
    {
		enemybullet = new sprite();

		//load the image
		if (!enemybullet->load("enemybullet.bmp")) {
			allegro_message("Error loading enemy bullet image");
			return 1;
		}

		//set sprite properties
		enemybullet->x = 0;
		enemybullet->y = 0;
		enemybullet->width = 32;
		enemybullet->height = 32;
		enemybullet->velx = 0;
		enemybullet->vely = 1;
		enemybullet->animcolumns = 1;
		enemybullet->curframe = 0;
		enemybullet->totalframes = 1;
		enemybullet->animdir = 1;
		enemybullet->framedelay = 0;
		enemybullet->alive = 0;
				
		//add this sprite to the handler
		game->add(enemybullet);
	}

	//Engine sound
	play_sample(snd_p38, volume-30, panning, pitch+500, TRUE);

	//game loop
    while (!key[KEY_ESC])
    {
		//update positions and animations

		//check for keyboard input
		while(speed_counter > 0)
		{
 			if (key[KEY_UP])
			{
				p38->y -= 1;
				if (p38->y < 0)
					p38->y = 0;
			}
 			if (key[KEY_DOWN])
			{
				p38->y += 1;
				if (p38->y > HEIGHT-65)
					p38->y = HEIGHT-65;
			}
			if (key[KEY_LEFT])
			{
				p38->x -= 1;
				if (p38->x < 0)
					p38->x = 0;
			}
			if (key[KEY_RIGHT])
			{
				p38->x += 1;
				if (p38->x > WIDTH-65)
					p38->x = WIDTH-65;
			}

			if (key[KEY_SPACE])
			{
				if (firecount > firedelay)
				{
					firecount = 0;
					for (n=0; n < BULLETS; n++)
					{
						if(game->get(n)->alive == 0)
						{
							play_sample(snd_bullet, volume, panning, pitch, FALSE);
							game->get(n)->alive++;
							game->get(n)->x = p38->x;
							game->get(n)->y = p38->y;
							break;
						}
					}
				}
			}

			if (key[KEY_TAB])
			{
				p38->alive = 1;
				p38->x = WIDTH/2;
				p38->y = HEIGHT*0.9;	
			}
			speed_counter --;
		}

		//update difficulty based on score
//////////////////////////////////////////////	
		if(p38->score == 200)
		{
			number_enemies = 10;
			firedelay = 80;
		}
		if(p38->score == 300)
		{
			number_enemies = 15;
			firedelay = 60;
			for (n=101; n<(151); n++)
			{
				game->get(n)->ydelay = 5;
			}			
		}
		/*if(p38->score == 30){
			number_enemies = 20;
			firedelay = 70;
		}
		if(p38->score == 40){
			number_enemies = 25;
			firedelay = 60;
		}
		if(p38->score == 400){
			number_enemies = 50;
			firedelay = 30;
		}*/
/////////////////////////////////////////////////
		
		//check and set planes to alive
		
		for (n=101; n<(101+number_enemies); n++)
		{
			if(game->get(n)->alive == 0)
			{
				game->get(n)->alive = 1;
				game->get(n)->x = rand() % (WIDTH-50);
				game->get(n)->y = 32;
				if(game->get(n)->x - game->get(n-1)->x <= 60)
					n--;
			}			
		}

		//move planes
		for(n=101; n<(101+number_enemies); n++)
		{
			if(game->get(n)->x < p38->x)
			{
				game->get(n)->velx = 0.5;
			}
			if(game->get(n)->x > p38->x)
			{
				game->get(n)->velx = -0.5;
			}
		}

		//if alive shot
		if(enemy_fire_count > 700)
		{
			n = 101;
			for(int i=171; i<201; i++)
				{
					if(game->get(i)->alive == 0)
					{
						if(game->get(n)->alive == 1)
						{
							game->get(i)->x = game->get(n)->x;
							game->get(i)->y = game->get(n)->y;
							game->get(i)->alive = 1;
							if(game->get(i)->x < p38->x-100)
							{
								game->get(i)->velx = 0.2;
							}
							if(game->get(i)->x > p38->x-100)
							{
								game->get(i)->velx = -0.2;
							}
							if(p38->x > WIDTH/3 && p38->x < (2*WIDTH/3))
							{
								game->get(i)->velx = 0;
							}
							n++;
						}
					}
				}
			enemy_fire_count = 0;
		}

		//make the background blue
		clear_to_color(buffer, makecol(0, 48, 165));

		//update animation frame
		game->get(100)->updateAnimation();
		
		for (n=151; n<171; n++)
        {
			if(game->get(n)->alive == 1)
			{
				game->get(n)->updateAnimation();
			}
		}
		
		for (n=101; n<(101+number_enemies); n++)
        {
		if(game->get(n)->alive == 1)
			game->get(n)->updateAnimation();
		}
///////////////////////////////////////////////////////////////

		//check for collisions
		for (n=0; n<201; n++)
        {
			if(game->get(n)->alive == 1 && n != 151 && n != 152 && n != 153 && n != 154 
				&& n != 155 && n != 156 && n != 157 && n != 158 && n != 159 && n != 160
				&& n != 161 && n != 162 && n != 163 && n != 164 && n != 165 && n != 166
				&& n != 167 && n != 168 && n != 169 && n != 170 && n != 171)
			{
				checkcollisions(n);
			}
			if(game->get(n)->alive == 0)
				game->get(n)->y = -32;
		}
		
		//update position
		for (n=101; n<(101+number_enemies); n++)
        {
			if(game->get(n)->alive == 1)
			{
				if(game->get(n)->y > HEIGHT+32 ||
					game->get(n)->x > WIDTH+32 ||
					game->get(n)->x < -32)
				{
					game->get(n)->alive = 0;
					game->get(n)->velx = 0;
				}
				game->get(n)->updatePosition();
			}
		}

		for (n=171; n<201; n++)
        {
			if(game->get(n)->alive == 1)
			{
				if(game->get(n)->y > HEIGHT+32 ||
					game->get(n)->x > WIDTH+32 ||
					game->get(n)->x < -32)
				{
					game->get(n)->alive = 0;
				}
				game->get(n)->updatePosition();
			}
		}

		for (n=0; n<BULLETS; n++)
        {
			if (game->get(n)->y > -32)
				game->get(n)->updatePosition();
			else
				game->get(n)->alive = 0;
		}
				
        //draw the sprites
		if(game->get(100)->alive == 1)
		{
			game->get(100)->drawframe(buffer);
		}
		
		for(n=151; n<171; n++)
		{
			if(game->get(n)->alive == 1)
			{
				game->get(n)->drawframe(buffer);
				game->get(n)->xcount += 1;
				if(game->get(n)->xcount > 100)
				{
					game->get(n)->alive = 0;
				}
			}
		}
		
		for (n=101; n<(101+number_enemies); n++)
        {
			if(game->get(n)->alive == 1)
			{
				game->get(n)->drawframe(buffer);
			}
		}

		for (n=171; n<201; n++)
        {
			if(game->get(n)->alive == 1)
			{
				game->get(n)->draw(buffer);
			}
		}

		for (n=0; n<BULLETS; n++)
		{
			if(game->get(n)->alive == 1)
			{
				game->get(n)->draw(buffer);
			}
		}
		
		//print the program title
		textout_ex(buffer, font, "Senta a Pua V0.7(alpha) TEST  Program - by Guili - (ESC to quit)", 
			0, 10, RED, -1);
		
		//display some status information
		textprintf_ex(buffer,font,0,20,RED,-1,"POINTS: %d", game->get(100)->score);
		textprintf_ex(buffer,font,0,30,WHITE,-1,"firing delay: %d", firedelay);
		textprintf_ex(buffer,font,0,40,WHITE,-1,"framerate: %d", framerate);
		textprintf_ex(buffer,font,0,50,WHITE,-1,"counter: %ds", counter);
		textprintf_ex(buffer,font,0,60,WHITE,-1,"enemies: %d", number_enemies);


        //update the screen
        acquire_screen();
		blit(buffer,screen,0,0,0,0,WIDTH,HEIGHT);
		clear_bitmap(buffer);
        release_screen();

		firecount++;
		ticks++;
		enemy_fire_count++;
        
    }

	//shutdown
	destroy_bitmap(buffer);
	delete game;
	allegro_exit();
    return 0;

}
END_OF_MAIN();
