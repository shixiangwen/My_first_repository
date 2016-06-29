/*****main.cpp*****/
#include "d3dFramework.h"

/*****ȫ�ֱ���*****/
D3DXMATRIX        WorldMatrix;		
IDirect3DDevice9* Device = 0; 

/*****����ֱ���*****/
const int Width  = 800;
const int Height = 600;

/*****���㻺��*****/
IDirect3DVertexBuffer9* Triangle = 0;
IDirect3DIndexBuffer9*  IB       = 0;

/*****����ṹ*****/
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

/**************����ʵ�ֺ���**************/
void Construct()      
{
	/*****���㻺��*****/
	Device->CreateVertexBuffer(
		6 * sizeof(ColorVertex), 
		D3DUSAGE_WRITEONLY,
		ColorVertex::FVF,
		D3DPOOL_MANAGED,
		&Triangle,
		0);

	/*****��������*****/	
	Device->CreateIndexBuffer(
		24 * sizeof(ColorVertex), 
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IB,
		0);

	/*****��䶥�㻺��*****/		
	ColorVertex* v;
	Triangle->Lock(0, 0, (void**)&v, 0);

	v[0] = ColorVertex(0.0f, 1.4f, 0.0f,D3DCOLOR_XRGB(255,255,255));
	v[1] = ColorVertex(-1.0f,  0.0f, -1.0f,D3DCOLOR_XRGB(255,0,0));
	v[2] = ColorVertex(-1.0f,  0.0f, 1.0f,D3DCOLOR_XRGB(0,255,0));

	v[3] = ColorVertex(1.0f, 0.0f, 1.0f,D3DCOLOR_XRGB(0,0,255));
	v[4] = ColorVertex(1.0f,  0.0f, -1.0f,D3DCOLOR_XRGB(0,255,255));
	v[5] = ColorVertex(0.0f, -1.4f, 0.0f,D3DCOLOR_XRGB(255,255,0));
	
	Triangle->Unlock();

	/*****����������*****/	
	WORD* indices=0;
	IB->Lock(0, 0, (void**)&indices, 0);

	/*****�����������*****/	
	indices[0]=0; indices[1]=1; indices[2]=2; 
	indices[3]=0; indices[4]=2; indices[5]=3; 

	indices[6]=0; indices[7]=3; indices[8]=4; 
	indices[9]=0; indices[10]=4; indices[11]=1; 

	indices[12]=5; indices[13]= 1; indices[14]= 4; 
	indices[15]=5 ; indices[16]= 4; indices[17]= 3; 

	indices[18]= 5; indices[19]=3 ; indices[20]= 2; 
	indices[21]= 5; indices[22]=2 ; indices[23]= 1; 

	IB->Unlock();

    /*****���������*****/
	D3DXVECTOR3    pos(2.0f, 1.0f, -3.0f);
	D3DXVECTOR3		target(0.0, 0.0f, 0.0f);
	D3DXVECTOR3     up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &V);

    /*****ͶӰ�任*****/
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.5f, 
			(float)Width / (float)Height,
			1.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	/*****���ù���*****/
	Device->SetRenderState(D3DRS_LIGHTING, false);
}

/****************ģ�����ٺ���****************/
void Destruct()       
{
	Release<IDirect3DVertexBuffer9*>(Triangle);
	Release<IDirect3DIndexBuffer9* >(IB);
}

/****************���ƺ���****************/
bool Display(float time,IDirect3DDevice9* Device) 
{
		/*****��Y����ת*****/
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

		/*****����ƽ����ɫģʽ*****/
		if( ::GetAsyncKeyState('F') & 0x8000f )
			Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);	
		/*****���ø������ɫģʽ*****/
		if( ::GetAsyncKeyState('G') & 0x8000f )
			Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		/*****����phong��ɫģʽ*****/
		if( ::GetAsyncKeyState('P') & 0x8000f )
			Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);

		/*****������������*****/
		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0, 0,6,0, 8);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	
		return true;
}

/****************��Ϣ��Ӧ����****************/
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

/****************������****************/
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nShowCmd)
{
	/*****d3d�豸��������ʼ��*****/
	if(!d3d9Initial(hInstance,(WNDPROC)WndProc,Width,Height,false,D3DDEVTYPE_HAL,&Device))
	{
		MessageBox(0,"d3dInitial Failed.",0,0);
		return 0;
	}

	/*****ģ�ʹ���*****/
	Construct();

	/*****��Ϣѭ��*****/
	d3d9MessageLoop(Display,Device);

	/*****ģ������*****/
    Destruct();
	Device->Release();

	return 0;
}
