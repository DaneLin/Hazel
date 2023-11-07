#include "Sandbox2D.h"
#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Hazel/Core/EntryPoint.h"

#include <chrono>
//#define PROFILE_SCOPE(name) Timer timer##__LINE__(name,[&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult);})


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

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();
	m_CheckerBoardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_GameTexture = Hazel::Texture2D::Create("assets/games/tilemap_packed.png");

	m_GrassTexture = Hazel::SubTexture2D::CreateFromCoords(m_GameTexture, { 0, 7 }, { 16, 16 }, { 1, 1 });
	m_RoleOneTexture = Hazel::SubTexture2D::CreateFromCoords(m_GameTexture, { 0, 6 }, { 16, 16 }, {1, 1});

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / m_MapWidth;

	s_TextureMap['W'] = m_GrassTexture;
	s_TextureMap['D'] = m_RoleOneTexture;

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f , 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();
	{
		HZ_PROFILE_SCOPE("Sandbox2D::OnUpdate");
		//Update
		m_CameraController.OnUpdate(ts);
	}
	
	Hazel::Renderer2D::ResetStats();
	{
		HZ_PROFILE_SCOPE("Renderer Setup");
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
	}
	
	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f,0.8f}, {0.8f, 0.8f, 0.5f, 1.0f});
		Hazel::Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 1.0f,0.5f }, { 0.1f, 0.1f, 0.5f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerBoardTexture, 10.0f);
		Hazel::Renderer2D::DrawRotatedQuad({ 2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerBoardTexture, 10.0f);
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 1.0f }, { 1.0f, 2.0f }, m_RoleOneTexture);
		//Hazel::Renderer2D::EndScene();

		//Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		/*for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f,0.4f, (y + 5.0f) / 10.0f , 0.5f};
				Hazel::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}*/


		for (uint32_t y = 0; y < m_MapHeight; y++)
		{
			for (uint32_t x = 0; x < m_MapWidth; x++)
			{
				char tileType = s_MapTiles[x + y * m_MapWidth];
				Hazel::Ref<Hazel::SubTexture2D> texture;
				if (s_TextureMap.find(tileType) != s_TextureMap.end())
				{
					texture = s_TextureMap[tileType];
				}
				else
				{
					texture = m_RoleOneTexture;
				}

				Hazel::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, y - m_MapHeight / 2.0f, 0.5f }, { 1.0f, 1.0f }, texture);
			}
		}

		Hazel::Renderer2D::EndScene();
	}

	// particle system
	/*if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Hazel::Input::GetMousePosition();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();

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

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();
	
	ImGui::Begin("Settings");

	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
	
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
