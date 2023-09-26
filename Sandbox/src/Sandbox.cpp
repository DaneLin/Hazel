#include "Hazel.h"

class SandBox : public Hazel::Application
{
public:
	SandBox() {

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