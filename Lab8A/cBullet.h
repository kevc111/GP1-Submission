/*
=================
cBullet.h
- Header file for class definition - SPECIFICATION
- Header file for the Bullet class which is a child of cSprite class
=================
*/
#ifndef _CBULLET_H
#define _CBULLET_H
#include "cSprite.h"


class cBullet : public cSprite
{
private:
	glm::vec2 bulletVelocity = glm::vec2(0.0f, 0.0f);

public:
	// Default render function
	void render();		
	// Bullet update method
	void update(float deltaTime);	
	// Sets the velocity for the Bullet
	void setBulletVelocity(glm::vec2 bulletVel);  
	// Gets the Bullet velocity 
	glm::vec2 getBulletVelocity();		
	// Use this function to show the collision box	 
	void renderCollisionBox();				
};
#endif