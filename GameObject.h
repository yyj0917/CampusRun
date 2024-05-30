#pragma once
#include <learnopengl/entity.h>
#include <learnopengl/animator.h>
#include <GLFW/glfw3.h>

class GameObject
{
public:
	Transform* _transform;
	Model* _model;
	Shader* _shader;

	GameObject() 
	{
		_transform = new Transform();
	}

	~GameObject()
	{
		if (_transform != nullptr) delete _transform;
		if (_model != nullptr) delete _model;
		if (_shader != nullptr) delete _shader;
	}
	
	virtual void SetModel(const string& modelPath)
	{
		_model = new Model(modelPath);
	}

	void SetShader(const string& vs, const string& fs)
	{
		_shader = new Shader(vs.c_str(), fs.c_str());
	}

	virtual void RunKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

	}

	virtual void Update(float deltaTime)
	{

	}

	virtual void Render(const glm::mat4& projection,const glm::mat4& view)
	{
		if (_shader == nullptr) return;
		_shader->use();
		_shader->setMat4("projection", projection);
		_shader->setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model,_transform->getLocalPosition()); 
		_shader->setMat4("model", model);
		_model->Draw(*_shader);
	}
};