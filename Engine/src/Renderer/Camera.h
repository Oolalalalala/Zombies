#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


struct PerspectiveCameraSpecification
{
	float Fov = 100.0f / 180.0f * 3.14f, AspectRatio = 16.0f / 9.0f, NearPlane = 0.1f, FarPlane = 1000.0f;
};

struct OrthographicCameraSpecification
{
	float Width = 1600, Height = 900, NearPlane = -1, FarPlane = 1;
};

class Camera
{
public:
	Camera();
	Camera(const PerspectiveCameraSpecification& specification);
	Camera(const OrthographicCameraSpecification& specification);
	~Camera() {}

	void SetPerspective(const PerspectiveCameraSpecification& specification);
	void SetOrthographic(const OrthographicCameraSpecification& specification);

	void SetProjectionMatrix(glm::mat4 projection) { m_Projection = projection; }
	void SetPosition(glm::vec3 position) { m_Position = position; }
	void SetDirection(glm::vec3 direction) { m_Direction = direction; }
	void SetUpDirection(glm::vec3 upDirection) { m_UpDirection = upDirection; }

	glm::vec3 GetDirection() const { return m_Direction; }
	glm::vec3 GetPosition() const { return m_Position; }
	glm::vec3 GetUpDirection() const { return m_UpDirection; }
	glm::mat4 GetProjectionMatrix() const { return m_Projection; }
	glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Direction, m_UpDirection); }
	glm::mat4 GetViewProjectionMatrix() const { return GetProjectionMatrix() * GetViewMatrix(); }

	bool IsPerspective() const { return m_IsPerspective; }
	bool IsOrthographic() const { return !m_IsPerspective; }

	const PerspectiveCameraSpecification& GetPerspectiveCameraSpecification() { return m_PerspectiveSpecification; }
	const OrthographicCameraSpecification& GetOrthographicCameraSpecification() { return m_OrthographicSpecification; }

private:
	glm::mat4 m_Projection = glm::mat4(1.0f);
	glm::vec3 m_Position = glm::vec3(0.0f);
	glm::vec3 m_Direction = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_UpDirection = glm::vec3(0.0f, 1.0f, 0.0f);

	union
	{
		PerspectiveCameraSpecification m_PerspectiveSpecification;
		OrthographicCameraSpecification m_OrthographicSpecification;
	};

	bool m_IsPerspective;
};
