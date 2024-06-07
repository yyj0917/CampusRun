#pragma once
#include "GameObject.h"

class Road : public GameObject
{
private:
    float moveSpeed;
    int position; // -1 : left, 0 : middle, 1: right
    int order;
public:
    Road(const std::string& modelPath, float speed, int position, int order)
    {
        SetModel(modelPath);
        moveSpeed = speed;
        this->position = position;
        this->order = order;
    }
    int GetPosition() {
        return this->position;
    }
    
    int GetOrder() {
        return this->order;
    }

    void SetMoveSpeed(float speed)
    {
        moveSpeed = speed;
    }

    void SetInitialPosition(float x, float z)
    {
        _transform->setLocalPosition(glm::vec3(x, -2.0f, z));
    }

    float GetMoveSpeed() const
    {
        return moveSpeed;
    }

    virtual void Update(float deltaTime) override
    {
        // 도로를 플레이어 쪽으로 이동시키는 로직
        _transform->setLocalPosition(glm::vec3(
            _transform->getLocalPosition().x,
            _transform->getLocalPosition().y,
            _transform->getLocalPosition().z + moveSpeed * deltaTime
        ));
    }

    virtual void Render(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& cameraPos) override
    {
        if (_shader == nullptr) return;
        _shader->use();
        _shader->setMat4("projection", projection);
        _shader->setMat4("view", view);
        SetUniformVar(direction, ambient, diffuse, specular, cameraPos);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, _transform->getLocalPosition());
        _shader->setMat4("model", model);
        _model->Draw(*_shader);
    }
};
