#pragma once

#include "Components/PreviewTexture.hpp"

class RenderBuffer {
public:
	RenderBuffer();
	~RenderBuffer();

	void setRenderTarget_Texture(Cmp::Texture& texture);
	void setRenderTarget_Screen();

	inline unsigned int previewWidth() { return m_previewWidth; }
	inline unsigned int previewHeight() { return m_previewHeight; }

protected:
	unsigned int m_previewWidth = 1000;
	unsigned int m_previewHeight = 1000;

	unsigned int m_frameBufferId;
};