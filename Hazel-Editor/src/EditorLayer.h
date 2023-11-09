#pragma once

#include <Hazel.h>
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_VertexArray;

		Ref<Texture2D> m_CheckerBoardTexture;
		Ref<Texture2D> m_GameTexture;
		Ref<SubTexture2D> m_RoleOneTexture;
		Ref<SubTexture2D> m_GrassTexture;

		bool m_ViewportFocused = false;
		bool m_ViewportHoverd = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		//Framebuffer
		Ref<Framebuffer> m_Framebuffer;

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};

		std::vector<ProfileResult> m_ProfileResults;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };


		uint32_t m_MapWidth, m_MapHeight;
		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;



	};
}

