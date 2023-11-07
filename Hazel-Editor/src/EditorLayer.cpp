#include "EditorLayer.h"
#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Hazel/Core/EntryPoint.h"

#include <chrono>
//#define PROFILE_SCOPE(name) Timer timer##__LINE__(name,[&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult);})

namespace Hazel
{
	static const uint32_t s_MapWidth = 24;
	static const char* s_MapTiles = "WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWDDDDDDDDDDDDDDDDDWWWW"
		"WWDDDDDDDDDDDDDDDDDDWWWW"
		"WWDDDDDDDDDDDDDDDWDDDDWW"
		"WWDDDDDDDDDDDDDDDDDDDWWW"
		"WWWDDDDDDDDDDDDDDDDDDWWW"
		"WWWWDDDDDDDDDDDDDDDWWWWW"
		"WWWWDDDDDDDDDDDDDDDDDWWW"
		"WWWWDDDDDDDDDDDDDDDDDWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW";

	EditorLayer::EditorLayer()
		:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
	{

	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();
		m_CheckerBoardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_GameTexture = Texture2D::Create("assets/games/tilemap_packed.png");

		m_GrassTexture = SubTexture2D::CreateFromCoords(m_GameTexture, { 0, 7 }, { 16, 16 }, { 1, 1 });
		m_RoleOneTexture = SubTexture2D::CreateFromCoords(m_GameTexture, { 0, 6 }, { 16, 16 }, { 1, 1 });

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / m_MapWidth;

		s_TextureMap['W'] = m_GrassTexture;
		s_TextureMap['D'] = m_RoleOneTexture;

		FramebufferSpecification framebufferSpec;
		framebufferSpec.Width = 1280;
		framebufferSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(framebufferSpec);
	}

	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();
		{
			HZ_PROFILE_SCOPE("Sandbox2D::OnUpdate");
			//Update
			m_CameraController.OnUpdate(ts);
		}

		Renderer2D::ResetStats();
		{
			HZ_PROFILE_SCOPE("Renderer Setup");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
		}

		{

			static float rotation = 0.0f;
			rotation += ts * 50.0f;

			HZ_PROFILE_SCOPE("Renderer Draw");
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f,0.8f }, { 0.8f, 0.8f, 0.5f, 1.0f });
			Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 1.0f,0.5f }, { 0.1f, 0.1f, 0.5f, 1.0f });
			Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerBoardTexture, 10.0f);
			Renderer2D::DrawRotatedQuad({ 2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerBoardTexture, 10.0f);
			Renderer2D::DrawQuad({ 0.0f, 0.0f, 1.0f }, { 1.0f, 2.0f }, m_RoleOneTexture);
			//Renderer2D::EndScene();

			//Renderer2D::BeginScene(m_CameraController.GetCamera());
			/*for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = { (x + 5.0f) / 10.0f,0.4f, (y + 5.0f) / 10.0f , 0.5f};
					Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
				}
			}*/


			/*for (uint32_t y = 0; y < m_MapHeight; y++)
			{
				for (uint32_t x = 0; x < m_MapWidth; x++)
				{
					char tileType = s_MapTiles[x + y * m_MapWidth];
					Ref<SubTexture2D> texture;
					if (s_TextureMap.find(tileType) != s_TextureMap.end())
					{
						texture = s_TextureMap[tileType];
					}
					else
					{
						texture = m_RoleOneTexture;
					}

					Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, y - m_MapHeight / 2.0f, 0.5f }, { 1.0f, 1.0f }, texture);
				}
			}*/

			Renderer2D::EndScene();
			m_Framebuffer->UnBind();
		}

		// particle system
		/*if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Input::GetMousePosition();
			auto width = Application::Get().GetWindow().GetWidth();
			auto height = Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();

			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 5; i++)
				m_ParticleSystem.Emit(m_Particle);
		}



		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());*/
	}

	void EditorLayer::OnImGuiRender()
	{
		HZ_PROFILE_FUNCTION();
		//ImGui::Begin("Setting");

		// Switch this to true to enable dockspace
		static bool dockingEnabled = true;
		if (dockingEnabled)
		{
			static bool dockspaceOpen = true;
			static bool opt_fullscreen_persistent = true;
			bool opt_fullscreen = opt_fullscreen_persistent;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			if (dockspace_flags & ImGuiDockNodeFlags_PassthruDockspace)
				window_flags |= ImGuiWindowFlags_NoBackground;

			/*
			* Important: note that we proceed even if Begin() returns false (aka window is collapsed)
			* This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			* all active windows docked into it will lose their parent and become undocked
			* we cannot preserve the docking relationship between an active window and an inactive docking ,
			* otherwise, any change of dockspace/settings would lead to windows begin stuck in limbo and never being visible
			*/
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			//DockSpace
			ImGuiIO& io = ImGui::GetIO();

			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit")) Application::Get().Close();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImGui::Begin("Settings");

			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

			
			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport");
			ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2*)&ViewportPanelSize))
			{
				m_ViewportSize = { ViewportPanelSize.x, ViewportPanelSize.y };
				m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

				m_CameraController.ResizeBounds(m_ViewportSize.x, m_ViewportSize.y);
			}
			
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{0,1}, ImVec2{1, 0});
			ImGui::PopStyleVar();
			ImGui::End();

			ImGui::End();

		}
		else
		{
			ImGui::Begin("Settings");

			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

			uint32_t textureID = m_CheckerBoardTexture->GetRendererID();
			ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f }, ImVec2{0,1}, ImVec2{1, 0});
			ImGui::End();
		}
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}


