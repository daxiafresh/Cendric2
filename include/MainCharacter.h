#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "Level.h"
#include "InputController.h"

// Cendric in a level
class MainCharacter : public MovableGameObject
{
public:
	MainCharacter(Level* level);
	~MainCharacter();

	void load() override;
	void update(sf::Time& frameTime) override;
	void checkCollisions(sf::Vector2f nextPosition) override;
	void calculateUnboundedVelocity(sf::Time& frameTime, sf::Vector2f& nextVel) override;

	const float getConfiguredMaxVelocityY() override;
	const float getConfiguredMaxVelocityX() override;

	Level* getLevel();

private:
	const float WALK_ACCELERATION = 1500.0f;
	const float GRAVITY_ACCELERATION = 1000.0f;
	// choose a value between 0.9 for really slow halting and 1.0f for aprupt halting.
	const float DAMPING_GROUND_PER_S = 0.999f;
	const float DAMPING_AIR_PER_S = 0.9f;

	// handle input and calculate the next position
	void handleInput();
	// update animation based on the current velocity + grounded
	void updateAnimation();
	bool m_isFacingRight;
	bool m_nextIsFacingRight;
	GameObjectState m_state;
	bool m_isGrounded;
	sf::Vector2f m_nextPosition;
	Level* m_level;
};