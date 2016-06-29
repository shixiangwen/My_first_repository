/*****d3dFramework.cpp*****/
#include "d3dFramework.h"

bool d3d9Initial(HINSTANCE hInstance,
                 WNDPROC WndProc,
				 int width,
				 int height,
				 bool fullScreen,
				 D3DDEVTYPE deviceType,
				 IDirect3DDevice9** device)
{
	/*****创建窗口*****/
	HWND hwnd=d3d9CreateWindow(hInstance,WndProc,width,height,fullScreen);
	if(!hwnd)
	{
		MessageBox(0,"d3dCreateWindow Failed.",0,0);
		return false;
	}

	/*****创建D3D9接口*****/
	IDirect3D9* d3d9=Direct3DCreate9(D3D_SDK_VERSION);
	if(!d3d9)
	{
		MessageBox(0,"Direct3DCreate9 Failed.",0,0);
		return false;
	}

	/*****校验硬件设备是否支持顶点运算*****/
	int vp=d3d9CheckHardwareVP(d3d9,deviceType);

	/*****创建并填充D3DPRESENT_PARAMETERS结构*****/

	D3DPRESENT_PARAMETERS d3dpp=d3d9CreatePP(hwnd,width,height,fullScreen);

	/*****创建D3D9设备*****/
	return d3d9CreateDevice(d3d9,deviceType,hwnd,vp,d3dpp,device);
}

HWND d3d9CreateWindow(HINSTANCE hInstance,
					  WNDPROC WndProc,
				 	  int width,
					  int height,
					  bool fullScreen)
{
	/*****设计窗口类*****/
	WNDCLASS wc;
	wc.style         = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(0,IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "d3dWindow";

	/*****注册窗口类*****/
	if(!RegisterClass(&wc))
	{
		MessageBox(0,"RegisterClass Failed.",0,0);
		return 0;
	}

	/*****创建窗口*****/
	HWND hwnd = CreateWindow("d3dWindow",
		                   "d3dProject",
						   WS_EX_TOPMOST|WS_OVERLAPPEDWINDOW,
						   0,
						   0,
						   width,
						   height,
						   0,
						   0,
						   hInstance,
						   0);

	/*****显示及更新窗口*****/
	if(!hwnd)
	{
		MessageBox(0,"CreateWindow Failed.",0,0);
		return 0;
	}
	ShowWindow(hwnd,SW_SHOW);
	UpdateWindow(hwnd);

	return hwnd;
}

int d3d9CheckHardwareVP(IDirect3D9* d3d9,D3DDEVTYPE deviceType)
{
	int vp=0;
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT,deviceType,&caps);
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp=D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	return vp;
}

D3DPRESENT_PARAMETERS d3d9CreatePP(HWND hwnd,
								   int width,
								   int height,
								   bool fullScreen)
{
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            =width;
	d3dpp.BackBufferHeight           =height;
	d3dpp.BackBufferFormat           =D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            =1;
	d3dpp.MultiSampleType            =D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         =0;
	d3dpp.SwapEffect                 =D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow              =hwnd;
	d3dpp.Windowed                   =!fullScreen;
	d3dpp.EnableAutoDepthStencil     =true;
	d3dpp.AutoDepthStencilFormat     =D3DFMT_D24S8;
	d3dpp.Flags                      =0;
	d3dpp.FullScreen_RefreshRateInHz =D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       =D3DPRESENT_INTERVAL_IMMEDIATE;
	return d3dpp;
}

bool d3d9CreateDevice(IDirect3D9* d3d9,
				      D3DDEVTYPE deviceType,
					  HWND hwnd,
				      int vp,
				      D3DPRESENT_PARAMETERS d3dpp,
					  IDirect3DDevice9** device)
{
	HRESULT hr=0;
	hr=d3d9->CreateDevice(D3DADAPTER_DEFAULT,
		                  deviceType,
						  hwnd,
						  vp,
						  &d3dpp,
						  device);
	if(FAILED(hr))
	{
		d3dpp.AutoDepthStencilFormat=D3DFMT_D16;
	    hr=d3d9->CreateDevice(D3DADAPTER_DEFAULT,
		                      deviceType,
						      hwnd,
						      vp,
						      &d3dpp,
						      device);
		if(FAILED(hr))
		{
			MessageBox(0,"d3d9CreateDevice Failed.",0,0);
			d3d9->Release();
			return false;
		}
	}
	d3d9->Release();
	return true;
}

int d3d9MessageLoop(bool (* p_Display)(float time,IDirect3DDevice9* Device),
					IDirect3DDevice9* Device)
{
	MSG msg;
	ZeroMemory(&msg,sizeof(MSG));
	static float time_1=(float)(timeGetTime());
	while(msg.message!=WM_QUIT)
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float time_2=(float)timeGetTime();
			float intervalTime=time_2-time_1;
			p_Display(intervalTime,Device);
			time_1=time_2;
		}
	}
	return true;
}
