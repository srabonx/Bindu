#ifndef D3DUTILITY_H
#define D3DUTILITY_H

#include "../Error/ComException.h"

// Macro to throw DirectX HRESULT exceptions 
#ifndef DXThrowIfFailed
#define DXThrowIfFailed(x)                                              \
{                                                                       \
    HRESULT hr__ = (x);                                                 \
    if(FAILED(hr__)) { throw ComException(__FILE__, __LINE__); }         \
}
#endif



#endif