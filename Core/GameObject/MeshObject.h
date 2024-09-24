#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include <cstdint>
#include <string>
#include "Buffers.h"
#include "GameObject.h"
#include "MeshManager.h"

namespace BINDU
{
	class D3DCommandContext;
	class D3DPipelineStateManager;

	class MeshObject : public GameObject
	{
	public:
		MeshObject() = default;

		MeshObject(MeshObject&& rhs) = default;

		MeshObject& operator = (MeshObject&& rhs) = default;

		void	SetMesh(const MeshData& meshData);

		virtual void Render(const D3DCommandContext& commandContext);

		virtual void Update();

	private:
		MeshData			m_meshData;
	};
}

#endif