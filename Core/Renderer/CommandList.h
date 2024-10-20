#ifndef BINDU_COMMAND_LIST_H
#define BINDU_COMMAND_LIST_H

#include <DirectXMath.h>

namespace BINDU
{
	using namespace DirectX;

	class Texture;
	class Shader;
	class VertexBuffer;
	class IndexBuffer;

	class CommandList
	{
	public:
		virtual ~CommandList() = default;

		virtual void BeginRender() = 0;

		virtual void BeginDrawOnTexture(Texture* texture) = 0;

		virtual void EndDrawOnTexture() = 0;

		virtual void SetViewPort(float width, float height) = 0;

		virtual void SetScissorRect(int x, int y, int width, int height) = 0;

		virtual void Clear(float r, float g, float b, float a) = 0;

		virtual void CopyRenderTextureDataToBackBuffer(Texture* renderTexture) = 0;

		virtual void SetViewProjectionMatrix(const XMMATRIX& viewProjMat) = 0;

		virtual void BindShader(Shader* shader) = 0;

		virtual void BindTexture(int slot, Texture* texture) = 0;

		virtual void DrawMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer) = 0;

		virtual void EndRender() = 0;
	};
}

#endif