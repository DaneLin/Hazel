#include "Hazel.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"
#include "Hazel/Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		:Layer("Example"),m_Camera(-1.6f, 1.6f, -1.0f, 1.0f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());

		// Vertex information
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		Hazel::Ref<Hazel::VertexBuffer> m_VertexBuffer;
		Hazel::Ref<Hazel::IndexBuffer> m_IndexBuffer;
		// using reset to set m_VertexBuffer pointer
		m_VertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		// set vertex layout
		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// the order of drawing triangles
		// make sure it's counterclockwise
		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_SquareArray.reset(Hazel::VertexArray::Create());
		// Square info
		float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> m_SquareBuffer;
		m_SquareBuffer.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Hazel::BufferLayout squareLayout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
		};

		m_SquareBuffer->SetLayout(squareLayout);
		m_SquareArray->AddVertexBuffer(m_SquareBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Hazel::Ref<Hazel::IndexBuffer> m_SquareIB;
		m_SquareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareArray->SetIndexBuffer(m_SquareIB);


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;			
			
			out vec4 v_Color;
			
			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;			
			
			uniform mat4 u_ViewProjection;	
			uniform mat4 u_Transform;
			
			out vec2 v_TexCoord;
			
			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
			}
		)";

		std::string squareFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			uniform sampler2D u_Texture;
			in vec2 v_TexCoord;
			
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_SquareShader.reset(Hazel::Shader::Create(squareVertexSrc, squareFragmentSrc));
		
		m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		//HZ_TRACE("Delta Time: {0}s, ({1}ms)", ts.GetSecond(), ts.GetMilliSeconds());

		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation += m_CameraRotateSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation -= m_CameraRotateSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
			m_SquarePosition.x -= m_SquareMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
			m_SquarePosition.x += m_SquareMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
			m_SquarePosition.y += m_SquareMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
			m_SquarePosition.y -= m_SquareMoveSpeed * ts;

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_Camera);

		/*glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition);*/

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		/*std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->UploadUniformFloat3("u_Color", m_SquareColor);*/
		m_Texture->Bind();
		/*for (int x = 0; x < 20; x++)
		{
			for (int y = 0; y < 20; y++) 
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				
			}
		}*/
		Hazel::Renderer::Submit(m_SquareShader, m_SquareArray);
		
		//Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	void OnEvent(Hazel::Event& event) override
	{
		/*Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));*/
	}

	

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
private:
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::Shader> m_SquareShader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;
	Hazel::Ref<Hazel::VertexArray> m_SquareArray;

	Hazel::Ref<Hazel::Texture2D> m_Texture;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotateSpeed = 180.0f;
	float m_CameraRotation = 0.0f;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.8f, 0.8f };
};

class SandBox : public Hazel::Application
{
public:
	SandBox() {
		PushLayer(new ExampleLayer());
	}
	~SandBox()
	{

	}

private:

};

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBox();
}