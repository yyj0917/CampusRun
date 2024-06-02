#pragma once
#include "Item.h"
#include <keyframe.h>
#include <GLFW/glfw3.h>

class SpinItem : public Item
{
public:	
	glm::vec3 Scale;
	float yAngle;
	float timeT;
	float beginT;
	float animEndTime;
	KeyFraming* yRKF;

	SpinItem(const string& modelPath, int lineIndex, int score, glm::vec3 Scale)
	{
		SetModel(modelPath);
		this->lineIndex = lineIndex;
		this->score = score;
		this->bIsItem = true;
		this->Scale = Scale;
		animEndTime = 2.f;
		yRKF = new KeyFraming(5);
		initKeyframes();
	}

	~SpinItem()
	{
		if (yRKF != nullptr) delete yRKF;
	}

	void initKeyframes()
	{
		// y-rotation keyframes
		yRKF->setKey(0, 0, 0.0);
		yRKF->setKey(1, 0.5, 90.0);
		yRKF->setKey(2, 1.0, 180.0);
		yRKF->setKey(3, 1.5, 270.0);
		yRKF->setKey(4, animEndTime, 360.0);
	}

	void updateAnimData() {
		if (timeT > animEndTime) {
			SetAnimInit();
			return;
		}
		yAngle = yRKF->getValLinear(timeT);
	}


	virtual void RunKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

	}

	virtual void CollisionEvent() override
	{
		// Todo: Insert Sound
		bActivated = false;
	}

	void SetAnimInit()
	{
		beginT = (float)glfwGetTime();
	}

	virtual void SetModel(const string& modelPath)
	{
		GameObject::SetModel(modelPath);
	}

	virtual void Update(float deltaTime)
	{
		Item::Update(deltaTime);
	}

	virtual void Render(const glm::mat4& projection, const glm::mat4& view)
	{
		if (_shader == nullptr) return;
		_shader->use();
		_shader->setMat4("projection", projection);
		_shader->setMat4("view", view);

		float cTime = (float)glfwGetTime(); // current time
		timeT = cTime - beginT;
		updateAnimData();
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, _transform->getLocalPosition());
		glm::vec3 eulerAngles(0.f, glm::radians(yAngle), 0.f);
		glm::quat q(eulerAngles);
		glm::mat4 rotMatrix = q.operator glm::mat4x4();
		model = model * rotMatrix;
		model = glm::scale(model, Scale);
		_shader->setMat4("model", model);
		_model->Draw(*_shader);
	}
};