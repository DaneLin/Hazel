#include "Sandbox2D.h"
#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Hazel/Core/EntryPoint.h"

#include <chrono>

//#define PROFILE_SCOPE(name) Timer timer##__LINE__(name,[&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult);})


Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();
	m_CheckerBoardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	//m_SquareVA = Hazel::VertexArray::Create();
	//// Square info
	//float squareVertices[4 * 3] = {
	//	-0.5f, -0.5f, 0.0f, 
	//	 0.5f, -0.5f, 0.0f, 
	//	 0.5f,  0.5f, 0.0f, 
	//	-0.5f, 0.5f, 0.0f, 
	//};

	//Hazel::Ref<Hazel::VertexBuffer> m_SquareBuffer;
	//m_SquareBuffer = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

	//Hazel::BufferLayout squareLayout = {
	//	{ Hazel::ShaderDataType::Float3, "a_Position" },
	//};

	//m_SquareBuffer->SetLayout(squareLayout);
	//m_SquareVA->AddVertexBuffer(m_SquareBuffer);

	//uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	//Hazel::Ref<Hazel::IndexBuffer> m_SquareIB;
	//m_SquareIB = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	//m_SquareVA->SetIndexBuffer(m_SquareIB);

	//m_FlatColorShader = Hazel::Shader::Create("assets/shaders/FlatColor.glsl");
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
	
	{
		HZ_PROFILE_SCOPE("Renderer Setup");
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
	}
	
	{
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f,0.8f }, { 0.8f, 0.8f, 0.5f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 1.0f,0.5f }, { 0.1f, 0.1f, 0.5f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerBoardTexture);
		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();
	ImGui::Begin("Setting");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	m_ProfileResults.clear();
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
