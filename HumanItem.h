#pragma once
#include "Item.h"
#include "Player.h"

class HumanItem : public Item
{
public:

	Animation* _anim;
	Animator* _animator;
	glm::vec3 Scale;

	HumanItem(const string& modelPath, int lineIndex, int score, glm::vec3 Scale)
	{
		SetModel(modelPath);
		this->lineIndex = lineIndex;
		this->score = score;
		this->bIsItem = true;
		this->Scale = Scale;
	}

	~HumanItem()
	{
		if (_anim != nullptr) delete _anim;
		if (_animator != nullptr) delete _animator;
	}

	virtual void SetModel(const string& modelPath)
	{
		_model = new Model(modelPath);

		_anim = new Animation(modelPath, _model);
		_animator = new Animator(_anim);
	}

	virtual void RunKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

	}

	virtual void CollisionEvent() override
	{
		// Todo: Insert Sound
		bActivated = false;
	}

	virtual void Update(float deltaTime)
	{
		Item::Update(deltaTime);
		_animator->UpdateAnimation(deltaTime);
	}

	virtual void Render(const glm::mat4& projection, const glm::mat4& view)
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
		model = glm::scale(model, Scale);
		_shader->setMat4("model", model);
		_model->Draw(*_shader);
	}
};