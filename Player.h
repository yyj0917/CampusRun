#pragma once
#include "GameObject.h"
#include "CustomMath.h"

using namespace std;

class Player : public GameObject
{
private:
	bool bCanJump = true;
	float PlayerPosition = 0.f;
	float PlayerToMove = 0.f;
	float HowMuchMove = 2.0f;
	bool bCanMove = true;
	float JumpForce = 5.5f;
	float CurrentVerticalSpeed = 0;
	float Gravity = 9.8f;
	float PlayerY = 0.f;

public: 
	Animation* _anim;
	Animator* _animator;

	Player()
	{

	}

	~Player()
	{
		if (_anim != nullptr) delete _anim;
		if (_animator != nullptr) delete _animator;
	}

	virtual void SetModel(const string& modelPath) override
	{
		__super::SetModel(modelPath);

		_anim = new Animation(modelPath, _model);
		_animator = new Animator(_anim);
	}

	virtual void RunKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) override
	{
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			if (!bCanJump) return;
			if (PlayerToMove == -HowMuchMove) return;
			if (!bCanMove) return;

			// Todo: Insert Sound
			cout << "Log: Player Move Sound Play" << endl;

			PlayerToMove -= HowMuchMove;
		}

		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			if (!bCanJump) return;
			if (PlayerToMove == HowMuchMove) return;
			if (!bCanMove) return;

			// Todo: Insert Sound
			cout << "Log: Player Move Sound Play" << endl;

			PlayerToMove += HowMuchMove;
		}

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			if (!bCanJump) return;

			// Todo: Insert Sound
			cout << "Log: Player Move Sound Play" << endl;

			CurrentVerticalSpeed = JumpForce;
			bCanJump = false;
		}
	}

	virtual void Update(float deltaTime) override
	{
		_animator->UpdateAnimation(deltaTime);
		PlayerMoveLerp();
		PlayerGravity(deltaTime);
	}

	virtual void Render(const glm::mat4& projection, const glm::mat4& view) override 
	{
		if (_shader == nullptr) return;
		_shader->use();
		_shader->setMat4("projection", projection);
		_shader->setMat4("view", view);

		auto transforms = (*_animator).GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			_shader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, _transform->getLocalPosition());
		model = glm::rotate(model, glm::radians(180.f), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(.5f, .5f, .5f));
		_shader->setMat4("model", model);
		_model->Draw(*_shader);
	}

	void PlayerMoveLerp()
	{
		if (abs(PlayerPosition - PlayerToMove) < 0.1f)
		{
			PlayerPosition = PlayerToMove;
			_transform->setLocalPosition(glm::vec3(PlayerPosition, _transform->getLocalPosition().y, _transform->getLocalPosition().z));
			bCanMove = true;
		}
		else
		{
			PlayerPosition = Lerp(PlayerPosition, PlayerToMove, 0.2f);
			_transform->setLocalPosition(glm::vec3(PlayerPosition, _transform->getLocalPosition().y, _transform->getLocalPosition().z));
			bCanMove = false;
		}
	}

	void PlayerGravity(float deltaTime)
	{
		if (!bCanJump)
		{
			PlayerY += deltaTime * CurrentVerticalSpeed;
			CurrentVerticalSpeed -= deltaTime * Gravity;
			_transform->setLocalPosition(glm::vec3(_transform->getLocalPosition().x, PlayerY, _transform->getLocalPosition().z));

			if (PlayerY < 0.f)
			{
				PlayerY = 0.f;
				CurrentVerticalSpeed = 0.f;
				_transform->setLocalPosition(glm::vec3(_transform->getLocalPosition().x, PlayerY, _transform->getLocalPosition().z));
				bCanJump = true;
			}
		}
	}
};