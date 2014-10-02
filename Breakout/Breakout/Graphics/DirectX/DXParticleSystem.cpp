#include "DXParticleSystem.h"
#include <iostream>
#include <fstream>
#include <vector>


DXParticleSystem::DXParticleSystem(void)
{	
}


DXParticleSystem::~DXParticleSystem(void)
{
	ReleaseCOM(InitVertexBuffer);
	ReleaseCOM(DrawVertexBuffer);
	ReleaseCOM(SteamOutVertexBuffer);
	ReleaseCOM(InputLayout);
	ReleaseCOM(m_effect);

}

void DXParticleSystem::Init(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, std::string _effect, DXTextureManager *_texMgr)
{
	firstRun		= true;
	time			= 0;
	Ks				= 0;
	Ns				= 0;

	Particle *partilce		= new Particle();
	partilce->age			= 0.0f;
	partilce->type			= 0;

	//skapar start-vertexbuffern som endast innehåller en emit-partikel (en vertex).
	D3D11_BUFFER_DESC initVertexBufferDesc;
	initVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	initVertexBufferDesc.ByteWidth = sizeof(Particle);
	initVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	initVertexBufferDesc.CPUAccessFlags = 0;
	initVertexBufferDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = partilce;

	if (FAILED(_device->CreateBuffer(&initVertexBufferDesc, &data, &InitVertexBuffer)))
	{
		::MessageBox(0, "Failed to initalize SteamOutVertexBuffer(ParticleSystem)", "Error", MB_OK);
	}

	//skapar två stycken tomma streamoutvertexbuffern.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth		= sizeof(Particle) * MAX_VERTICES;
	vertexBufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	vertexBufferDesc.CPUAccessFlags	= 0;
	vertexBufferDesc.MiscFlags		= 0;
	if (FAILED(_device->CreateBuffer(&vertexBufferDesc, 0, &SteamOutVertexBuffer)))
	{
		::MessageBox(0, "Failed to initalize SteamOutVertexBuffer(ParticleSystem)", "Error", MB_OK);
	}

	if (FAILED(_device->CreateBuffer(&vertexBufferDesc, 0, &DrawVertexBuffer)))
	{
		::MessageBox(0, "Failed to initalize DrawVertexBuffer(ParticleSystem)", "Error", MB_OK);
	}



	ID3DBlob*	pBlobEffect = NULL;
	ID3DBlob*	pBlobErrors = NULL;
	DWORD dwShaderFlags		= D3D10_SHADER_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
#endif

	
	std::string path = "Graphics/DirectX/Shaders/ParticleSystems/" + _effect + ".fxo";
	std::ifstream fin(path.c_str(), std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();


	HRESULT(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, _device, &m_effect));


	//dwShaderFlags |= D3D10_SHADER_DEBUG;
	//D3DX11CompileFromFile(effectFilePath,
	//						NULL,
	//						NULL,
	//						"",
	//						"fx_5_0",
	//						dwShaderFlags,
	//						NULL,
	//						NULL,
	//						&pBlobEffect,
	//						&pBlobErrors,
	//						NULL);
	


	//D3DX11CreateEffectFromMemory(pBlobEffect->GetBufferPointer(),
	//								pBlobEffect->GetBufferSize(),
	//								dwShaderFlags,
	//								_device,
	//								&effect);
		

	//laddar effectteknikerna från effektfilen.
	StreamOutTech = m_effect->GetTechniqueByName("StreamOutTech");
	DrawTech = m_effect->GetTechniqueByName("DrawTech");

	//laddar variabler från effektfilen.
	mfxViewProjVar = m_effect->GetVariableByName("gViewProjection")->AsMatrix();
	mfxTimeVar = m_effect->GetVariableByName("gTime")->AsScalar();
	mfxDeltaTimeVar = m_effect->GetVariableByName("gDeltaTime")->AsScalar();
	mfxCameraPosVar = m_effect->GetVariableByName("gCameraPos")->AsVector();
	mfxEmitPosVar = m_effect->GetVariableByName("gEmitPosW")->AsVector();
	mfxEmitVelVar = m_effect->GetVariableByName("gEmitVelW")->AsVector();

	RandomTexRV = _texMgr->GetRandomTexture();

	//laddar textur till partikelsystemet, t.ex. eldtextur.

	TextureRV = _texMgr->CreateTexture(_effect + ".png");

	//if (D3DX11CreateShaderResourceViewFromFile(_device, charToWChar(texturePath), 0, 0, &TextureRV, 0))
	//{
	//	::MessageBox(0, L"Failed to create ShaderResourceView(ParticleSystem)", L"Error", MB_OK);
	//}

	//skickar texturerna till gpu.
	mfxTexture = m_effect->GetVariableByName("Texture")->AsShaderResource();
	mfxRandomTex = m_effect->GetVariableByName("randomTex")->AsShaderResource();
	//effect->GetVariableByName("randomTex")->AsShaderResource()->SetResource(RandomTexRV);
	//effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(TextureRV);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TYPE", 0, DXGI_FORMAT_R8_UINT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	D3DX11_PASS_DESC PassDesc;
	DrawTech->GetPassByIndex(0)->GetDesc(&PassDesc);
	_device->CreateInputLayout(inputDesc,
								5,
								PassDesc.pIAInputSignature,
								PassDesc.IAInputSignatureSize,
								&InputLayout);
}

//startar om partikelsystemet
void DXParticleSystem::Reset()
{
	time = 0;
	firstRun = true;
}

//sätter positionen där nya partiklar tillkommer.
void DXParticleSystem::SetEmitPosition(DirectX::XMFLOAT3 *_position)
{
	m_position = _position;
}

//hämtar positionen där nya partiklar tillkommer.
DirectX::XMFLOAT3 DXParticleSystem::GetEmitPosition()
{
	return *m_position;
}

//sätter positionen där nya partiklar tillkommer.
void DXParticleSystem::SetEmitVelocity(DirectX::XMFLOAT3 *_velocity)
{
	m_velocity = _velocity;
}

//hämtar positionen där nya partiklar tillkommer.
DirectX::XMFLOAT3 DXParticleSystem::GetEmitVelocity()
{
	return *m_velocity;
}



//uppdaterar och målar ut partikelsystemt.
void DXParticleSystem::Render(ID3D11DeviceContext* _dc, float deltaTime, DirectX::XMFLOAT4X4& ViewProjection, DirectX::XMFLOAT3& cameraPos)
{
	time += deltaTime;

	mfxEmitPosVar->SetFloatVector(reinterpret_cast<const float*>(m_position));
	mfxEmitVelVar->SetFloatVector(reinterpret_cast<const float*>(m_velocity));


	//skickar variabler till GPU.
	mfxViewProjVar->SetMatrix((float*)&ViewProjection);
	mfxTimeVar->SetFloat(time);
	mfxDeltaTimeVar->SetFloat(deltaTime);
	mfxTexture->SetResource(TextureRV);
	mfxRandomTex->SetResource(RandomTexRV);

	DirectX::XMFLOAT4 camPos = DirectX::XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 0);
	mfxCameraPosVar->SetFloatVector(reinterpret_cast<const float*>(&camPos));

	_dc->IASetInputLayout(InputLayout);
	_dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(Particle);
	UINT offset = 0;

	//Laddar start-vertexbuffern om det är första gången uppdateringen görs.
	if (firstRun)
	{
		_dc->IASetVertexBuffers(0, 1, &InitVertexBuffer, &stride, &offset);
	}

	//Annars sätter den DrawVertexBuffer som vertexbuffer.
	else
		_dc->IASetVertexBuffers(0, 1, &DrawVertexBuffer, &stride, &offset);

	//Sätter SteamOutVertexBuffer som StreamOutTarget.
	_dc->SOSetTargets(1, &SteamOutVertexBuffer, &offset);


	//kör draw metoden med StreamOutTech, vilket innebär att GPU:n kommer att uppdatera och skapa nya partiklar som sparas i StreamOutTarget.
	D3DX11_TECHNIQUE_DESC techDesc;
	StreamOutTech->GetDesc(&techDesc);
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		StreamOutTech->GetPassByIndex(p)->Apply(0, _dc);
		//"Målar ut" en vertex om det är första uppdateringen (emit-partiklen).
		if( firstRun )
		{

			_dc->Draw(1, 0);
			firstRun = false;
		}
		//"Målar" automatikst ut alla vertexpunkter i den satta vertexbuffern.
		else
		{
			_dc->DrawAuto();
		}
	}
	
	//un-binding streamouttarget-buffern.
	ID3D11Buffer* bufferArray[1] = {0};
	_dc->SOSetTargets(1, bufferArray, &offset);

	//byter så att DrawVertexBuffer blir SteamOutVertexBuffer och tvärt om.
	std::swap(DrawVertexBuffer, SteamOutVertexBuffer);

	//sätter DrawVertexBuffer (gamla SteamOutVertexBuffer) som vertexbuffer och målar ut den på skärmen med DrawTech-tekniken.
	_dc->IASetVertexBuffers(0, 1, &DrawVertexBuffer, &stride, &offset);
	DrawTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		DrawTech->GetPassByIndex(p)->Apply(0, _dc);
		_dc->DrawAuto();
	}
}
