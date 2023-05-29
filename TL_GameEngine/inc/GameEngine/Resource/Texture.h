#pragma once

#include "Common_min.h"
#include "Graphics/RenderSystem.h"

namespace TL_GameEngine
{
	class Texture
	{
		friend class Resources;

	public:
		Texture() :
			m_Texture(nullptr) { }

		~Texture()
		{
			TL_Graphics::RenderSystem::Get()->Return(m_Texture);
		}

		TL_Graphics::ITexture* m_Texture;

	public:
		auto GetTexture() const { return m_Texture; }
	};
}