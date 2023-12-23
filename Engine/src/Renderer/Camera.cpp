#include "pch.h"
#include "Camera.h"


Camera::Camera()
{
	SetPerspective(PerspectiveCameraSpecification{});
}

Camera::Camera(const PerspectiveCameraSpecification& specification)
{
	SetPerspective(specification);
}

Camera::Camera(const OrthographicCameraSpecification& specification)
{
	SetOrthographic(specification);
}

void Camera::SetPerspective(const PerspectiveCameraSpecification& specification)
{
	m_PerspectiveSpecification = specification;
	m_IsPerspective = true;

	m_Projection = glm::perspective(specification.Fov,
									specification.AspectRatio,
									specification.NearPlane,
									specification.FarPlane);
}

void Camera::SetOrthographic(const OrthographicCameraSpecification& specification)
{
	m_OrthographicSpecification = specification;
	m_IsPerspective = false;

	m_Projection = glm::ortho(specification.Width  * -0.5f,
						      specification.Width  *  0.5f,
						      specification.Height * -0.5f,
						      specification.Height *  0.5f,
						      specification.NearPlane,
						      specification.FarPlane);
}

