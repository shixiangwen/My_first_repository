/*****main.cpp*****/
#include "d3dFramework.h"

/*****全局变量*****/
D3DXMATRIX        WorldMatrix;		
IDirect3DDevice9* Device = 0; 

/*****定义分辨率*****/
const int Width  = 800;
const int Height = 600;

/*****顶点缓存*****/
IDirect3DVertexBuffer9* Triangle = 0;
IDirect3DIndexBuffer9*  IB       = 0;

/*****顶点结构*****/
struct ColorVertex
{	
	float _x, _y, _z;
	D3DCOLOR _color;
	ColorVertex(){}
	ColorVertex(float x, float y, float z, D3DCOLOR color)
	{
		_x = x;  _y = y;  _z = z;  _color = color; 
	}
	static const DWORD FVF;
};
const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

/**************功能实现函数**************/
void Construct()      
{
	/*****顶点缓存*****/
	Device->CreateVertexBuffer(
		6 * sizeof(ColorVertex), 
		D3DUSAGE_WRITEONLY,
		ColorVertex::FVF,
		D3DPOOL_MANAGED,
		&Triangle,
		0);

	/*****索引缓存*****/	
	Device->CreateIndexBuffer(
		24 * sizeof(ColorVertex), 
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IB,
		0);

	/*****填充顶点缓存*****/		
	ColorVertex* v;
	Triangle->Lock(0, 0, (void**)&v, 0);

	v[0] = ColorVertex(0.0f, 1.4f, 0.0f,D3DCOLOR_XRGB(255,255,255));
	v[1] = ColorVertex(-1.0f,  0.0f, -1.0f,D3DCOLOR_XRGB(255,0,0));
	v[2] = ColorVertex(-1.0f,  0.0f, 1.0f,D3DCOLOR_XRGB(0,255,0));

	v[3] = ColorVertex(1.0f, 0.0f, 1.0f,D3DCOLOR_XRGB(0,0,255));
	v[4] = ColorVertex(1.0f,  0.0f, -1.0f,D3DCOLOR_XRGB(0,255,255));
	v[5] = ColorVertex(0.0f, -1.4f, 0.0f,D3DCOLOR_XRGB(255,255,0));
	
	Triangle->Unlock();

	/*****定义三角形*****/	
	WORD* indices=0;
	IB->Lock(0, 0, (void**)&indices, 0);

	/*****填充索引缓存*****/	
	indices[0]=0; indices[1]=1; indices[2]=2; 
	indices[3]=0; indices[4]=2; indices[5]=3; 

	indices[6]=0; indices[7]=3; indices[8]=4; 
	indices[9]=0; indices[10]=4; indices[11]=1; 

	indices[12]=5; indices[13]= 1; indices[14]= 4; 
	indices[15]=5 ; indices[16]= 4; indices[17]= 3; 

	indices[18]= 5; indices[19]=3 ; indices[20]= 2; 
	indices[21]= 5; indices[22]=2 ; indices[23]= 1; 

	IB->Unlock();

    /*****定义摄像机*****/
	D3DXVECTOR3    pos(2.0f, 1.0f, -3.0f);
	D3DXVECTOR3		target(0.0, 0.0f, 0.0f);
	D3DXVECTOR3     up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &V);

    /*****投影变换*****/
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.5f, 
			(float)Width / (float)Height,
			1.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	/*****禁用光照*****/
	Device->SetRenderState(D3DRS_LIGHTING, false);
}

/****************模型销毁函数****************/
void Destruct()       
{
	Release<IDirect3DVertexBuffer9*>(Triangle);
	Release<IDirect3DIndexBuffer9* >(IB);
}

/****************绘制函数****************/
bool Display(float time,IDirect3DDevice9* Device) 
{
		/*****绕Y轴旋转*****/
		D3DXMATRIX Ry;
		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry, y);
		y+=time*0.002;
		if ( y>6.28f )
			y = 0.0f;
		Device->SetTransform(D3DTS_WORLD, &Ry);

        Device->Clear(0,0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(230,230,230),1.0f,0);
        Device->BeginScene();
	
		Device->SetStreamSource(0, Triangle, 0, sizeof(ColorVertex));
		Device->SetIndices(IB);
		Device->SetFVF(ColorVertex::FVF);

		/*****设置平面着色模式*****/
		if( ::GetAsyncKeyState('F') & 0x8000f )
			Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);	
		/*****设置高洛德着色模式*****/
		if( ::GetAsyncKeyState('G') & 0x8000f )
			Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		/*****设置phong着色模式*****/
		if( ::GetAsyncKeyState('P') & 0x8000f )
			Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);

		/*****绘制正八面体*****/
		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0, 0,6,0, 8);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	
		return true;
}

/****************消息响应函数****************/
LRESULT CALLBACK WndProc(HWND hwnd,
						 UINT msg,
						 WPARAM wParam,
						 LPARAM lParam)
{
    switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			DestroyWindow(hwnd);
		break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

/****************主函数****************/
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nShowCmd)
{
	/*****d3d设备创建及初始化*****/
	if(!d3d9Initial(hInstance,(WNDPROC)WndProc,Width,Height,false,D3DDEVTYPE_HAL,&Device))
	{
		MessageBox(0,"d3dInitial Failed.",0,0);
		return 0;
	}

	/*****模型创建*****/
	Construct();

	/*****消息循环*****/
	d3d9MessageLoop(Display,Device);

	/*****模型销毁*****/
    Destruct();
	Device->Release();

	return 0;
}
