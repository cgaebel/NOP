#include "Core.h"
#include "config.h"

typedef HRESULT (WINAPI* CreateDevice_Prototype)        (LPDIRECT3D9, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, LPDIRECT3DDEVICE9*);
typedef HRESULT (WINAPI* Reset_Prototype)               (LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
typedef HRESULT (WINAPI* BeginScene_Prototype)           (LPDIRECT3DDEVICE9);
typedef HRESULT (WINAPI* EndScene_Prototype)            (LPDIRECT3DDEVICE9);
static CreateDevice_Prototype         CreateDevice_Pointer         = NULL;
static Reset_Prototype                Reset_Pointer                = NULL;
static EndScene_Prototype             EndScene_Pointer             = NULL;

static HRESULT WINAPI Direct3DCreate9_VMTable    ();
static HRESULT WINAPI CreateDevice_Detour        (LPDIRECT3D9, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, LPDIRECT3DDEVICE9*);
static HRESULT WINAPI Reset_Detour               (LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
static HRESULT WINAPI EndScene_Detour            (LPDIRECT3DDEVICE9);

static DWORD* Direct3D_VMTable = NULL;

static ID3DXFont* font = NULL;

int __stdcall VirtualMethodTableRepatchingLoopToCounterExtensionRepatching()
{
	for(;;)
	{
		Sleep(100);

		*(DWORD*)&Direct3D_VMTable[42] = (DWORD)EndScene_Detour;
		*(DWORD*)&Direct3D_VMTable[16] = (DWORD)Reset_Detour;
	}
}

static void PrintText(LPD3DXFONT Font, int x, int y, int Red, int Green, int Blue, int Alpha, const char *text, ...)
{
	D3DCOLOR fontColor = D3DCOLOR_ARGB(Alpha, Red, Green, Blue);
	RECT rct;
	rct.left = x;
	rct.top = y;
	rct.right = rct.left + 1000;
	rct.bottom = rct.top + 1000;
	va_list va_alist;

	char logbuf[0x1000];
	va_start(va_alist, text);
	_vsnprintf_s(logbuf, sizeof(logbuf), sizeof(logbuf), text, va_alist);
	va_end(va_alist);

	Font->DrawText(NULL, logbuf, -1, &rct, 0, fontColor);
}

static DWORD WINAPI HookD3D9()
{
	return Direct3DCreate9_VMTable() == D3D_OK;
}


static HRESULT WINAPI Direct3DCreate9_VMTable()
{
	auto Direct3D_Object = Direct3DCreate9(D3D_SDK_VERSION);

	if(Direct3D_Object == NULL)
	{
		OnFailure("Could not create D3D9.");
		return D3DERR_INVALIDCALL;
	}

	Direct3D_VMTable = (DWORD*)*(DWORD*)Direct3D_Object;
	Direct3D_Object->Release();

	DWORD dwProtect;

	if(VirtualProtect(&Direct3D_VMTable[16], sizeof(DWORD), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(DWORD*)&CreateDevice_Pointer = Direct3D_VMTable[16];
		*(DWORD*)&Direct3D_VMTable[16] = (DWORD)CreateDevice_Detour;

		if(VirtualProtect(&Direct3D_VMTable[16], sizeof(DWORD), dwProtect, &dwProtect) == 0)
		{
			OnFailure("Direct3DCreate9_VMTable: Could not re-protect CreateDevice.");
			return D3DERR_INVALIDCALL;
		}
	}
	else
	{
		OnFailure("Direct3DCreate9_VMTable: Could un-protect CreateDevice.");
		return D3DERR_INVALIDCALL;
	}

	return D3D_OK;
}

static HRESULT WINAPI CreateDevice_Detour(LPDIRECT3D9 Direct3D_Object, UINT Adapter, D3DDEVTYPE DeviceType, HWND FocusWindow,
					DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* PresentationParameters,
					LPDIRECT3DDEVICE9* Returned_Device_Interface)
{
	auto Returned_Result = CreateDevice_Pointer(Direct3D_Object, Adapter, DeviceType, FocusWindow, BehaviorFlags,
												PresentationParameters, Returned_Device_Interface);

	DWORD dwProtect;

	if(VirtualProtect(&Direct3D_VMTable[16], sizeof(DWORD), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PDWORD)&Direct3D_VMTable[16] = *(PDWORD)&CreateDevice_Pointer;
		CreateDevice_Pointer           = NULL;

		if(VirtualProtect(&Direct3D_VMTable[16], sizeof(DWORD), dwProtect, &dwProtect) == 0)
		{
			OnFailure("CreateDevice_Detour: Could not re-protect CreateDevice.");
			return D3DERR_INVALIDCALL;
		}
	}
	else
	{
		OnFailure("CreateDevice_Detour: Could not un-protect CreateDevice.");
		return D3DERR_INVALIDCALL;
	}

	if(Returned_Result == D3D_OK)
	{
		Direct3D_VMTable = (DWORD*)*(DWORD*)*Returned_Device_Interface;

		*(DWORD*)&Reset_Pointer                = (DWORD)Direct3D_VMTable[16];
		*(DWORD*)&EndScene_Pointer             = (DWORD)Direct3D_VMTable[42];

		if(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)VirtualMethodTableRepatchingLoopToCounterExtensionRepatching, NULL, NULL, NULL) == NULL)
		{
			OnFailure("Could not start the repatching thread.");
			return D3DERR_INVALIDCALL;
		}
	}

	return Returned_Result;
}

static HRESULT WINAPI Reset_Detour(LPDIRECT3DDEVICE9 Device_Interface, D3DPRESENT_PARAMETERS* PresentationParameters)
{
	font->Release(); // Same story for the font. We have to Release it before we create n new one else it WILL crash.
	D3DXCreateFontA(
			Device_Interface,
			15,
			0,
			FW_NORMAL,
			0,
			TRUE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			TEXT("Arial"),
			&font
		);

	return Reset_Pointer(Device_Interface, PresentationParameters);
}

//=====================================================================================


static HRESULT WINAPI EndScene_Detour(LPDIRECT3DDEVICE9 Device_Interface)
{
	D3DVIEWPORT9 Viewport;

	Device_Interface->GetViewport(&Viewport);

	ONCE(
		D3DXCreateFontA(
			Device_Interface,
			15,
			0,
			FW_NORMAL,
			0,
			TRUE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			TEXT("Arial"),
			&font
		);
	);

	PrintText(font, OVERLAY_POSITION_X, OVERLAY_POSITION_Y, 255, 255, 0, 128, OVERLAY_TEXT);

	return EndScene_Pointer(Device_Interface);
}

INITIALIZER(Overlay, "Loading the NOP overlay...")
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)VirtualMethodTableRepatchingLoopToCounterExtensionRepatching, NULL, NULL, NULL);
}
