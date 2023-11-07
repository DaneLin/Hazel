#include "Hazel.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"
#include "Hazel/Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace Hazel
{
	class HazelEditor : public Application
	{
	public:
		HazelEditor() 
			:Application("Hazel Editor")
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}
		~HazelEditor()
		{

		}

	private:

	};

	Application* CreateApplication()
	{
		return new HazelEditor();
	}
}

