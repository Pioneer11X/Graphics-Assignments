#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// 
// ఇక్కడ ముందు వున్న క్లాసు లో వేరియబుల్స్ ఈ క్లాస్స్లో పెడతాం.
// 
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	// మనకి కన్సోల్ విండో కావాలంటే ఉంచు. లేకపోతే తీసెయ్యి.
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// 
//	- అన్నీ డిలీట్ చెయ్యాలి.
// 
// --------------------------------------------------------
Game::~Game()
{
	// అన్నీ డిలీట్ చెయ్యాలి.
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	delete triangleMesh;
	delete squareMesh;
	delete cubeMesh;
	delete coneMesh;
	delete cylinderMesh;
	delete helixMesh;
	delete sphereMesh;
	delete torusMesh;

	delete triangleEntity;
	delete triangleEntity1;
	delete triangleEntity2;
	delete triangleEntity3;
	delete triangleEntity4;
	delete squareEntity;

	delete newCamera;
	delete newMaterial;
	delete anotherMaterial;

	//std::vector<Entity*>::iterator entityIterator;
	//for (entityIterator = entityVector.begin(); entityIterator < entityVector.end(); entityIterator++)
	//{
	//	(*entityIterator)->UpdateWorldMatrix();
	//	delete (*entityIterator);
	//}

	SRV->Release();
	anotherSRV->Release();
	sampler->Release();

	//delete SRV;
	//delete sampler;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	LoadShaders();

	DirectionalLight newDirectionalLight = {
		XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f),
		XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT3(1.0f, 0.3f, 0.2f)
	};

	DirectionalLight anotherDirectionalLight = {
		XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f),
		XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		XMFLOAT3(0.3f, -1.0f, -1.0f)
	};

	directionalLights.push_back(newDirectionalLight);
	directionalLights.push_back(anotherDirectionalLight);

	//pixelShader->SetData("dlight", &newDirectionalLight, sizeof(DirectionalLight));
	

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	
	// మాట్రిసీస్ ని తయారు చెయ్యి.
	CreateMatrices();

	// ఆబ్జెక్ట్లను వాడటానికి తయారు చెయ్యి.
	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	if (!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	// Material ముందుగా texture తయారుచేస్తే, దానికి  వాడుకోవచ్చు.
	// We want to create a texture just before creating a Material.
	CreateWICTextureFromFile(device, context, L"Debug/Textures/calinou/egypt01_c.png", 0, &SRV);
	CreateWICTextureFromFile(device, context, L"Debug/Textures/rock.jpg", 0, &anotherSRV);

	D3D11_SAMPLER_DESC sampleDesc = {};
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampleDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampleDesc.MaxAnisotropy = 16;

	device->CreateSamplerState(&sampleDesc, &sampler);

	newMaterial = new Material(vertexShader, pixelShader, SRV, sampler);
	anotherMaterial = new Material(vertexShader, pixelShader, anotherSRV, sampler);

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	XMFLOAT3 cameraPos = XMFLOAT3(0.0f, 0.0f, -5.0f);
	newCamera = new Camera( cameraPos, 0.0f, 0.0f, width, height );


}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{

	// ఇందులో అన్నిటినీ తయారు చేసుకో.

	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	XMFLOAT3 defaultNormal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT2 defaultUV = XMFLOAT2(0.0f, 0.0f);

	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), defaultNormal, defaultUV },
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), defaultNormal, defaultUV },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), defaultNormal, defaultUV }
	};

	int indices[] = { 0, 1, 2 };

	triangleMesh = new Mesh(vertices, 3, indices, 3, device);

	Vertex squareVertices[] = {
		{ XMFLOAT3(+2.0f, +1.0f, +0.0f), defaultNormal, defaultUV },
		{ XMFLOAT3(+2.0f, +2.0f, +0.0f), defaultNormal, defaultUV },
		{ XMFLOAT3(+3.0f, +2.0f, +0.0f), defaultNormal, defaultUV },
		{ XMFLOAT3(+3.0f, +1.0f, -0.0f), defaultNormal, defaultUV }
	};

	int squareIndices[] = { 0, 1, 2 , 0, 2, 3 };

	squareMesh = new Mesh(squareVertices, 4, squareIndices, 6, device);

	cubeMesh = new Mesh("Debug/cube.obj", device);
	coneMesh = new Mesh("Debug/cone.obj", device);
	cylinderMesh = new Mesh("Debug/cylinder.obj", device);
	helixMesh = new Mesh("Debug/helix.obj", device);
	sphereMesh = new Mesh("Debug/sphere.obj", device);
	torusMesh = new Mesh("Debug/torus.obj", device);
	

	triangleEntity  = new Entity(triangleMesh, newMaterial);
	entityVector.push_back(triangleEntity);
	triangleEntity1 = new Entity(coneMesh, newMaterial, XMFLOAT3(0.0f, 1.0f, 0.0f));
	entityVector.push_back(triangleEntity1);
	triangleEntity2 = new Entity(cylinderMesh, anotherMaterial, XMFLOAT3(1.0f, 0.0f, 0.0f));
	entityVector.push_back(triangleEntity2);
	triangleEntity3 = new Entity(helixMesh, newMaterial, XMFLOAT3(0.0f, -1.0f, 0.0f));
	entityVector.push_back(triangleEntity3);
	triangleEntity4 = new Entity(torusMesh, newMaterial, XMFLOAT3(-1.0f, 0.0f, 0.0f));
	entityVector.push_back(triangleEntity4);

	squareEntity = new Entity(cubeMesh, newMaterial);

}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// రీసైజ్ చేసినప్పుడు ఎం చెయ్యాలి?
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	//// Update our projection matrix since the window size changed
	//XMMATRIX P = XMMatrixPerspectiveFovLH(
	//	0.25f * 3.1415926535f,	// Field of View Angle
	//	(float)width / height,	// Aspect ratio
	//	0.1f,				  	// Near clip plane distance
	//	100.0f);			  	// Far clip plane distance
	//XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!

	newCamera->SetProjectionMatrix(width, height);

}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// యూనిటీ లో అప్డేట్ లూప్ లాంటిది.
	float sinTime = (sin(totalTime * 2) + 2.0f) / 10.0f;

	triangleEntity->SetRotation(sinTime);
	triangleEntity->SetScale(sinTime, sinTime, sinTime);
	triangleEntity->SetTranslation(sinTime, sinTime, sinTime);

	//squareEntity->SetTranslation(0.5f, 1.0f, 0.0f);

	triangleEntity1->MoveRight(5);
	triangleEntity2->MoveRight(-5);
	triangleEntity3->MoveUp(5);
	triangleEntity4->MoveUp(-5);

	/*
	triangleEntity1->MoveRightUsingMatrix(5);
	triangleEntity2->MoveRightUsingMatrix(-5);
	triangleEntity3->MoveUpUsingMatrix(5);
	triangleEntity4->MoveUpUsingMatrix(-5);
	*/

	squareEntity->UpdateWorldMatrix();

	float speed = 2.0f;

	std::vector<Entity*>::iterator entityIterator;
	for (entityIterator = entityVector.begin(); entityIterator < entityVector.end(); entityIterator++)
	{
		(*entityIterator)->UpdateWorldMatrix();
	}

	if (GetAsyncKeyState('W') & 0x8000) {
		newCamera->MoveForward(deltaTime * speed);
	}

	if (GetAsyncKeyState('S') & 0x8000) {
		newCamera->MoveForward(-1.0f * deltaTime * speed);
	}

	if (GetAsyncKeyState('D') & 0x8000) {
		newCamera->MoveLeft(-1.0f * deltaTime * speed);
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		newCamera->MoveLeft(deltaTime * speed);
	}

	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	newCamera->Update();

}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// కాంటెక్స్ట్ లో డెప్త్ ఇంకా, రెండర్ టార్గెట్లను క్లియర్ చెయ్యాలి.
	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	std::vector<Entity*>::iterator entityIterator;
	for (entityIterator = entityVector.begin(); entityIterator < entityVector.end() ; entityIterator++ )
	{
		DrawEntity(*entityIterator);
	}

	DrawEntity(squareEntity);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}

void Game::DrawEntity(Entity * _entity)
{

	_entity->PrepareShaders(newCamera, directionalLights);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ID3D11Buffer* vertexBuffer = _entity->GetMesh()->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(_entity->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		_entity->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{


	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if ( buttonState & 0x0001 ) {
		float diffX = x - float(prevMousePos.x);
		float diffY = y - float(prevMousePos.y);

		newCamera->IncrementRotationX(diffY * 0.001f);
		newCamera->IncrementRotationY(diffX * 0.001f);
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion