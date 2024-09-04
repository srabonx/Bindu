#ifndef IRENDERDEVICE_H
#define IRENDERDEVICE_H

namespace BINDU
{
	class IRenderDevice
	{
	public:
		~IRenderDevice() {}

		// Initialize the RenderDevice
		virtual void		Initialize() = 0;

		// Begin rendering
		virtual void		BeginRender() = 0;

		// Clear the screen with specified color
		// TODO: Implement Vec4
		virtual void		ClearScreen() = 0;

		// End rendering
		virtual void		EndRender() = 0;

	};
}

#endif