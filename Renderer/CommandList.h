#ifndef BINDU_COMMAND_LIST_H
#define BINDU_COMMAND_LIST_H

#include <DirectXMath.h>
#include <cstdint>
#include <string>

namespace BINDU
{
	class Camera;
}

namespace BINDU
{
	class UniformBuffer;
}

namespace BINDU
{
	using namespace DirectX;

	class Texture;
	class Shader;
	class VertexBuffer;
	class IndexBuffer;

	struct MeshData;


	struct CommandListStats
	{
		std::uint16_t ClearCalls{ 0 };

		std::uint16_t DrawCalls{ 0 };
	};

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

		virtual bool BindShader(Shader* shader) = 0;

		virtual void BindTexture(std::string name, Texture* texture) = 0;

		virtual void DrawMesh(const MeshData& meshData) = 0;

		virtual void EndRender() = 0;
	};
}

#endif