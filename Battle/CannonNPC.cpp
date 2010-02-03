#include "SDL/SDL.h"

#include "Gameplay.h"

#include "Projectile.h"

#include "CannonNPC.h"

#define CANNON_W	0
#define CANNON_NW	1
#define CANNON_N	2
#define CANNON_NE	3
#define CANNON_E	4
#define CANNON_W_D	5
#define CANNON_NW_D	6
#define CANNON_N_D	7
#define CANNON_NE_D	8
#define CANNON_E_D	9

CannonNPC::CannonNPC() : NPC() {
	is_stationary = true;

	frame_w = 52;
	frame_h = 36;

	frames = 5;

	frame_idle = CANNON_N;

	frame_dead = 0;

	sprites = Main::graphics->npc_cannon;
	
	position->w = frame_w;
	position->h = frame_h;

	last_position->w = frame_w;
	last_position->h = frame_h;

	hitpoints = 50;

	frame_first = CANNON_W;
	frame_last = CANNON_E;
}

void CannonNPC::process() {
	if(!is_dead && hitpoints <= 0) {
		is_dead = true;
		dead_start = Gameplay::frame;
	}
	if(is_dead && (Gameplay::frame - dead_start >= 30)) {
		done = true;
	}

	if(hitpoints <= 25 && current_sprite <= CANNON_E) {
		current_sprite += frames;
		frame_first = CANNON_W_D;
		frame_last = CANNON_E_D;
	}

	if(Gameplay::frame % 60 == 0) {
		cycle_sprite_updown(frame_first, frame_last);
	}

	if(Gameplay::frame % 20 == 0) {
		shoot();
	}
}

void CannonNPC::shoot() {
	Projectile * pr;
	SDL_Rect * clip_weapon;

	clip_weapon = new SDL_Rect();
	clip_weapon->x = 0;
	clip_weapon->y = 0;
	clip_weapon->w = 8;
	clip_weapon->h = 8;

	pr = new Projectile(Main::graphics->weapons, clip_weapon);
	pr->owner = NULL;

	pr->damage = 10;
	
	pr->speedx = 0;
	pr->speedy = 0;

	switch(current_sprite) {
		case CANNON_W:
		case CANNON_W_D:
			pr->speedx = -10;
			pr->position->x = position->x - 8;
			pr->position->y = position->y + 22;
			pr->max_distance = 500;
			break;
		case CANNON_NW:
		case CANNON_NW_D:
			pr->speedx = -10;
			pr->speedy = -10;
			pr->position->x = position->x - 8;
			pr->position->y = position->y + 2;
			pr->max_distance = 500;
			break;
		case CANNON_N:
		case CANNON_N_D:
			pr->speedy = -10;
			pr->position->x = position->x + 24;
			pr->position->y = position->y - 8;
			pr->max_distance = 500;
			break;
		case CANNON_NE:
		case CANNON_NE_D:
			pr->speedx = 10;
			pr->speedy = -10;
			pr->position->x = position->x + position->w;
			pr->position->y = position->y + 2;
			pr->max_distance = 500;
			break;
		case CANNON_E:
		case CANNON_E_D:
			pr->speedx = 10;
			pr->position->x = position->x + position->w;
			pr->position->y = position->y + 22;
			pr->max_distance = 500;
			break;
	}
	
	Gameplay::instance->add_object(pr);
}

void CannonNPC::hit_player_side(Player * p) {
	if(!p->is_dead && !p->is_hit) {
		Main::audio->play(SND_HIT, position->x);
		p->is_hit = true;
		p->hit_start = Gameplay::instance->frame;
		p->hitpoints -= 10;
	}
}

void CannonNPC::hit_player_top_bottom(Player * p) {
	if(!p->is_dead && !p->is_hit) {
		Main::audio->play(SND_HIT, position->x);
		p->is_hit = true;
		p->hit_start = Gameplay::instance->frame;
		p->hitpoints -= 10;
	}
}
