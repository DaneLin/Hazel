#pragma once

#include <glm/glm.hpp>

namespace Hazel
{

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		const glm::vec3& GetFrontDirection() const { return m_FrontDirection; }
		const glm::vec3& GetUpDirection() const { return m_UpDirection; }
		inline float GetCameraSpeed() { return m_CameraSpeed; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		// self settings
		glm::vec3 m_FrontDirection = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_UpDirection = {0.0f, 1.0f, 0.0f};

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		float m_CameraSpeed = 0.05f;
	};

}