#pragma once
#include "GameObject.h"

class Item : public GameObject
{
protected:
	
	int score;
	int lineIndex;
	bool itemType; // 0 : negative 1 : positive
	float moveSpeed = 5.f;
	float xBoundVolume = 0.5f;
	float yBoundVolume = 0.5f;
	float zBoundVolume = 0.5f;

public:

	int GetLineIndex() { return lineIndex; }

	bool getItemType() { return itemType; }

	void setItemType(bool itemType) {
		this->itemType = itemType;
	}

	void SetCollisionBound(float xVolume, float yVolume, float zVolume)
	{
		xBoundVolume = xVolume;
		yBoundVolume = yVolume;
		zBoundVolume = zVolume;
	}

	void SetInitialPositionByIndex(int index)
	{
		lineIndex = index;
		SetInitialPosition(-2.f + 2.f * index, -10);
	}

	// Set Spawn Position, initialPosX must in {-2, 0, 2}
	void SetInitialPosition(float initialPosX, float initialPosZ)
	{
		_transform->setLocalPosition(glm::vec3(
			initialPosX,
			0.f,
			initialPosZ)
		);


	}

	virtual void CollisionEvent()
	{

	}

	// Collision Check

	bool IsCollideWithPlayer(const Player& player)
	{
		// Get Item and Player Position
		glm::vec3 curPosition = _transform->getLocalPosition();
		glm::vec3 playerPosition = player._transform->getLocalPosition();

		// Check Collision
		bool isCollideX = (playerPosition.x >= curPosition.x - xBoundVolume / 2.0f) && (playerPosition.x <= curPosition.x + xBoundVolume / 2.0f);
		bool isCollideY = (playerPosition.y >= curPosition.y - yBoundVolume / 2.0f) && (playerPosition.y <= curPosition.y + yBoundVolume / 2.0f);
		bool isCollideZ = (playerPosition.z >= curPosition.z - zBoundVolume / 2.0f) && (playerPosition.z <= curPosition.z + zBoundVolume / 2.0f);

		return isCollideX && isCollideY && isCollideZ;
	}

	void SetScore(int score)
	{
		
		this->score = score;
	}

	int GetScore() const { return score; }


	virtual void Update(float deltaTime)
	{
		if (!bActivated) return;

		_transform->setLocalPosition(glm::vec3(_transform->getLocalPosition().x,
			_transform->getLocalPosition().y,
			_transform->getLocalPosition().z + moveSpeed * deltaTime));
	}

};