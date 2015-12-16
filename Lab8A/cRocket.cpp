/*
=================
cRocket.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cRocket.h"

void cRocket::render()
{
	glPushMatrix();

	glTranslatef(spritePos2D.x, spritePos2D.y, 0.0f);
	glRotatef(spriteRotation, 0.0f, 0.0f, 1.0f);
	glScalef(spriteScaling.x, spriteScaling.y, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GLTextureID); // Binding of GLtexture name 

	glBegin(GL_QUADS);
	glColor3f(255.0f, 255.0f, 255.0f);
	glTexCoord2f(spriteTexCoordData[0].x, spriteTexCoordData[0].y);
	glVertex2f(-(textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[1].x, spriteTexCoordData[1].y);
	glVertex2f((textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[2].x, spriteTexCoordData[2].y);
	glVertex2f((textureWidth / 2), (textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[3].x, spriteTexCoordData[3].y);
	glVertex2f(-(textureWidth / 2), (textureHeight / 2));

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cRocket::update(float deltaTime)
{
	if (m_InputMgr->isKeyDown(VK_RIGHT))
	{
		spriteRotation += 5.0f;
		spriteTranslation = (glm::vec2(2.0f, 5.0f));
	}
	if (m_InputMgr->isKeyDown(VK_LEFT))
	{
		spriteRotation += -5.0f;
		spriteTranslation = (glm::vec2(2.0f, 5.0f));
	}
	if (m_InputMgr->isKeyDown(VK_SPACE))
	{
		spriteTranslation = (glm::vec2(10.0f, 10.0f));
	}
	if (m_InputMgr->isKeyDown(VK_UP))
	{
		spriteTranslation = (glm::vec2(4.0f, 4.0f));
	}
	if (m_InputMgr->isKeyDown(VK_DOWN))
	{
		spriteTranslation = -(glm::vec2(2.0f, 2.0f));
	}

	//Barriers for the sides and top of the screen
	if (spritePos2D.y + (textureHeight / 2) > 1024)
	{
		spritePos2D.y -= (textureHeight / 2) + 10.0f;
	}
	if (spritePos2D.y - (textureHeight / 2) < 0)
	{
		spritePos2D.y += (textureHeight / 2) + 10.0f;
	}
	if (spritePos2D.x + (textureWidth / 2) > 768)
	{
		spritePos2D.x -= (textureWidth / 2) + 10.0f;
	}
	if (spritePos2D.x - (textureWidth / 2) < 0)
	{
		spritePos2D.x += (textureWidth / 2) + 10.0f;
	}


	if (spriteRotation > 360)
	{
		spriteRotation -= 360.0f;
	}

	glm::vec2 spriteVelocityAdd = glm::vec2(0.0f, 0.0f);
	spriteVelocityAdd.x = (glm::sin(glm::radians(spriteRotation)));
	spriteVelocityAdd.y = -(glm::cos(glm::radians(spriteRotation)));

	spriteVelocityAdd *= spriteTranslation;

	rocketVelocity += spriteVelocityAdd;

	spritePos2D += rocketVelocity * deltaTime;

	rocketVelocity *= 0.95;

}

	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
//	for (vector<cBullet*>::iterator rocketIterartor = theRocketBullets.begin(); rocketIterartor != theRocketBullets.end(); ++rocketIterartor)
//	{
//		(*rocketIterartor)->update(deltaTime);
//		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
//		{
//			if ((*asteroidIterator)->collidedWith((*asteroidIterator)->getBoundingRect(), (*rocketIterartor)->getBoundingRect()))
//			{
//				// if a collision set the bullet and asteroid to false
//				(*asteroidIterator)->setActive(false);
//				(*rocketIterartor)->setActive(false);
//			}
//		}
//	}
//
//	vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin();
//	while (bulletIterartor != theRocketBullets.end())
//	{
//		if ((*bulletIterartor)->isActive() == false)
//		{
//			bulletIterartor = theRocketBullets.erase(bulletIterartor);
//			// play the explosion sound.
//			m_SoundMgr->getSnd("Explosion")->playAudio(AL_TRUE);
//		}
//		else
//		{
//			//(*bulletIterartor)->update(deltaTime);
//			(*bulletIterartor)->render();
//			++bulletIterartor;
//		}
//	}
//}
/*
=================================================================
  Sets the velocity for the rocket
=================================================================
*/
//void cRocket::setRocketVelocity(glm::vec2 rocketVel)
//{
//	rocketVelocity = rocketVel;
//}
/*
=================================================================
  Gets the rocket velocity
=================================================================
*/
//glm::vec2 cRocket::getRocketVelocity()
//{
//	return rocketVelocity;
//}
