#ifndef DXPARTICLESYSTEM_H
#define DXPARTICLESYSTEM_H	

#include <D3D11.h>
#include <d3dx11effect.h>
#include <DirectXMath.h>
#include <string>
#include "DXTextureManager.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "Effects11D.lib")


class DXParticleSystem
{
	struct Particle
	{
		DirectX::XMFLOAT3 initialPos;
		DirectX::XMFLOAT3 initialVel;
		DirectX::XMFLOAT2 size;
		float age;
		unsigned int type;
	};

	DirectX::XMFLOAT3 *m_position, *m_velocity;


	static const int					MAX_VERTICES = 240001;
	bool								firstRun;
	float								time;

	//ID3D11Device*				mDevice;
	//ID3D11DeviceContext*		mDeviceContext;
	//Buffer*						InitVertexBuffer;
	ID3D11Buffer*				InitVertexBuffer;
	ID3D11Buffer*				DrawVertexBuffer;
	ID3D11Buffer*				SteamOutVertexBuffer;

	ID3D11InputLayout*			InputLayout;
	ID3DX11EffectTechnique*		StreamOutTech;
	ID3DX11EffectTechnique*		DrawTech;

	ID3DX11EffectMatrixVariable* mfxViewProjVar;
	ID3DX11EffectScalarVariable* mfxTimeVar;
	ID3DX11EffectScalarVariable* mfxDeltaTimeVar;
	ID3DX11EffectScalarVariable* mfxNsVar;
	ID3DX11EffectScalarVariable* mfxKsVar;
	ID3DX11EffectVectorVariable* mfxCameraPosVar;
	ID3DX11EffectVectorVariable* mfxEmitPosVar;
	ID3DX11EffectVectorVariable* mfxEmitVelVar;

	ID3D11ShaderResourceView*	RandomTexRV;
	ID3D11ShaderResourceView*	TextureRV;
public:
	float	Ks;
	float	Ns;

	DXParticleSystem(void);
	~DXParticleSystem(void);

	void Reset();

	void Init(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, std::string _effect, DXTextureManager *_texMgr);
	void DXParticleSystem::SetEmitPosition(DirectX::XMFLOAT3 *_position);
	void DXParticleSystem::SetEmitVelocity(DirectX::XMFLOAT3 *_velocity);
	DirectX::XMFLOAT3 GetEmitPosition();
	DirectX::XMFLOAT3 GetEmitVelocity();

	void Render(ID3D11DeviceContext* _dc, float deltaTime, DirectX::XMFLOAT4X4& ViewProjection, DirectX::XMFLOAT3& cameraPos);

};
#endif
