#pragma once

#include "Components/PreviewTexture.hpp"

class RenderBuffer {
public:
	RenderBuffer();
	~RenderBuffer();

	void setRenderTarget_Texture(Cmp::Texture& texture);
	void setRenderTarget_Screen();

protected:
	unsigned int m_frameBufferId;
};