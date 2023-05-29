#pragma once

class App
{
public:
	App();
	~App();

	void Update();

	void PreRender();

	void Render();

	void PostRender();

	void ImGui();

	class IExample* example;


	bool show_demo_window = true;
	bool show_another_window = false;
};