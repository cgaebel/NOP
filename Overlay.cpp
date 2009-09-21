#include "Overlay.h"
#include "defs.h"

//=====================================================================================

typedef HRESULT (WINAPI* CreateDevice_Prototype)        (LPDIRECT3D9, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, LPDIRECT3DDEVICE9*);
typedef HRESULT (WINAPI* Reset_Prototype)               (LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
typedef HRESULT (WINAPI* EndScene_Prototype)            (LPDIRECT3DDEVICE9);
typedef HRESULT (WINAPI* DrawIndexedPrimitive_Prototype)(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);

static CreateDevice_Prototype         CreateDevice_Pointer         = NULL;
static Reset_Prototype                Reset_Pointer                = NULL;
static EndScene_Prototype             EndScene_Pointer             = NULL;
static DrawIndexedPrimitive_Prototype DrawIndexedPrimitive_Pointer = NULL;

static HRESULT WINAPI Direct3DCreate9_VMTable    (VOID);
static HRESULT WINAPI CreateDevice_Detour        (LPDIRECT3D9, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, LPDIRECT3DDEVICE9*);
static HRESULT WINAPI EndScene_Detour            (LPDIRECT3DDEVICE9);

static DWORD WINAPI VirtualMethodTableRepatchingLoopToCounterExtensionRepatching(LPVOID);
static PDWORD Direct3D_VMTable = NULL;


static void PrintText(LPD3DXFONT Font, int x, int y, int Red, int Green, int Blue, int Alpha, const char *text, ...)
{
	D3DCOLOR fontColor = D3DCOLOR_ARGB(Alpha, Red, Green, Blue);
	RECT rct;
	rct.left=x;
	rct.top=y;
	rct.right=rct.left+1000;
	rct.bottom=rct.top+1000;
	va_list va_alist;
	char logbuf[256] = {0};
	va_start (va_alist, text);
	_vsnprintf_s(logbuf+strlen(logbuf), sizeof(logbuf), sizeof(logbuf) - strlen(logbuf), text, va_alist);
	va_end (va_alist);
	Font->DrawText(NULL, logbuf, -1, &rct, 0, fontColor );
}

static void WINAPI HookD3D9()
{
	Direct3DCreate9_VMTable();
}

static HRESULT WINAPI Direct3DCreate9_VMTable()
{
	LPDIRECT3D9 Direct3D_Object = Direct3DCreate9(D3D_SDK_VERSION);

	if(Direct3D_Object == NULL)
		return D3DERR_INVALIDCALL;

	Direct3D_VMTable = (PDWORD)*(PDWORD)Direct3D_Object;
	Direct3D_Object->Release();

	DWORD dwProtect;

	if(VirtualProtect(&Direct3D_VMTable[16], sizeof(DWORD), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PDWORD)&CreateDevice_Pointer = Direct3D_VMTable[16];
		*(PDWORD)&Direct3D_VMTable[16] = (DWORD)CreateDevice_Detour;
		*(PDWORD)&Direct3D_VMTable[42] = (DWORD)EndScene_Detour;

		if(VirtualProtect(&Direct3D_VMTable[16], sizeof(DWORD), dwProtect, &dwProtect) == 0)
			return D3DERR_INVALIDCALL;
		else return D3D_OK;
	}
	else
		return D3DERR_INVALIDCALL;
}

static HRESULT WINAPI CreateDevice_Detour(LPDIRECT3D9 Direct3D_Object, UINT Adapter, D3DDEVTYPE DeviceType, HWND FocusWindow,
					DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* PresentationParameters,
					LPDIRECT3DDEVICE9* Returned_Device_Interface)
{
	HRESULT Returned_Result = CreateDevice_Pointer(Direct3D_Object, Adapter, DeviceType, FocusWindow, BehaviorFlags,
											  PresentationParameters, Returned_Device_Interface);

	DWORD dwProtect;

	if(VirtualProtect(&Direct3D_VMTable[16], sizeof(DWORD), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PDWORD)&Direct3D_VMTable[16] = *(PDWORD)&CreateDevice_Pointer;
		CreateDevice_Pointer           = NULL;

		if(VirtualProtect(&Direct3D_VMTable[16], sizeof(DWORD), dwProtect, &dwProtect) == 0)
			return D3DERR_INVALIDCALL;
	}
	else
		return D3DERR_INVALIDCALL;

	if(Returned_Result == D3D_OK)
	{
		Direct3D_VMTable = (PDWORD)*(PDWORD)*Returned_Device_Interface;
		*(PDWORD)&EndScene_Pointer = (DWORD)Direct3D_VMTable[42];
	}

	return Returned_Result;
}

static HRESULT WINAPI EndScene_Detour(LPDIRECT3DDEVICE9 Device_Interface)
{
	static LPD3DXFONT font;
	static D3DVIEWPORT9 Viewport;

	Device_Interface->GetViewport(&Viewport);

	ONCE(
		D3DXCreateFontA(
			Device_Interface,
			10,
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

	PrintText(font, 5, (Viewport.Height - 25), 255, 255, 0, 128, OVERLAY_TEXT);

	return EndScene_Pointer(Device_Interface);
}

void InitOverlay()
{
	HookD3D9();
}