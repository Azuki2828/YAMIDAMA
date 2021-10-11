#pragma once
class FXAA
{
private:
	struct SFXAABuffer {
		float bufferW;
		float bufferH;
	};
public:
	void Init();
	void Render(RenderContext& rc);

	Sprite& GetSprite() { return m_finalSprite; }

private:
	SFXAABuffer m_buffer;
	Sprite m_finalSprite;
};

