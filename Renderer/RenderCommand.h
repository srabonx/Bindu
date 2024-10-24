#ifndef BINDU_RENDER_COMMAND_H
#define BINDU_RENDER_COMMAND_H

namespace BINDU
{
	class RenderCommand
	{
	public:
		virtual ~RenderCommand() = default;

		virtual void Execute() = 0;
	};
}

#endif