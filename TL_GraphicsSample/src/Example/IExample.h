#pragma once

class IExample
{
public:
	IExample() {}
	virtual ~IExample() {}

	virtual void Init() = 0;
	virtual void UnInit() = 0;

	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;

	virtual void ImGui() = 0;

	virtual void Update() = 0;
};