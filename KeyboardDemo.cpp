/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    GameSpriteDemo - Demo used to show how to create and display 2D game sprites.
*/


#include"KeyboardDemo.h"
#include"GameSprite.h"




struct VertexPos
{
    XMFLOAT3 pos;
    XMFLOAT2 tex0;
};


KeyboardDemo::KeyboardDemo( ) : solidColorVS_( 0 ), solidColorPS_( 0 ),
                                    inputLayout_( 0 ), vertexBuffer_( 0 ),
                                    colorMap_( 0 ), colorMapSampler_( 0 ),
                                    mvpCB_( 0 ), alphaBlendState_( 0 ), w(320.0), y(5.0),z(350.0), i(0.0), y2(250.0), direction(0),
									xCoordinate(220.0), yCoordinate(400.0)
{

}


KeyboardDemo::~KeyboardDemo( )
{

}


bool KeyboardDemo::LoadContent( )
{
    ID3DBlob* vsBuffer = 0;

    bool compileResult = CompileD3DShader( "TextureMap.fx", "VS_Main", "vs_4_0", &vsBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling the vertex shader!" );
        return false;
    }

    HRESULT d3dResult;

    d3dResult = d3dDevice_->CreateVertexShader( vsBuffer->GetBufferPointer( ),
        vsBuffer->GetBufferSize( ), 0, &solidColorVS_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the vertex shader!" );

        if( vsBuffer )
            vsBuffer->Release( );

        return false;
    }

    D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    unsigned int totalLayoutElements = ARRAYSIZE( solidColorLayout );

    d3dResult = d3dDevice_->CreateInputLayout( solidColorLayout, totalLayoutElements,
        vsBuffer->GetBufferPointer( ), vsBuffer->GetBufferSize( ), &inputLayout_ );

    vsBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the input layout!" );
        return false;
    }

    ID3DBlob* psBuffer = 0;

    compileResult = CompileD3DShader( "TextureMap.fx", "PS_Main", "ps_4_0", &psBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling pixel shader!" );
        return false;
    }

    d3dResult = d3dDevice_->CreatePixelShader( psBuffer->GetBufferPointer( ),
        psBuffer->GetBufferSize( ), 0, &solidColorPS_ );

    psBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating pixel shader!" );
        return false;
    }

	d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_,
        "ball.dds", 0, 0, &colorMap_, 0 );
	d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_,
        "picture1.dds", 0, 0, &colorMap2_, 0 );
	d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_,
        "ponghitbox.dds", 0, 0, &colorMap3_, 0 );
	d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_,
        "racket.dds", 0, 0, &colorMap4_, 0 );
    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to load the texture image!" );
        return false;
    }

    D3D11_SAMPLER_DESC colorMapDesc;
    ZeroMemory( &colorMapDesc, sizeof( colorMapDesc ) );
    colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

    d3dResult = d3dDevice_->CreateSamplerState( &colorMapDesc, &colorMapSampler_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create color map sampler state!" );
        return false;
    }

    ID3D11Resource* colorTex;

    colorMap2_->GetResource( &colorTex );

    D3D11_TEXTURE2D_DESC colorTexDesc;
    ( ( ID3D11Texture2D* )colorTex )->GetDesc( &colorTexDesc );
    colorTex->Release( );

    float halfWidth = ( float )colorTexDesc.Width / 2.0f;
    float halfHeight = ( float )colorTexDesc.Height / 2.0f;

	
     VertexPos vertices[] =
    {
        { XMFLOAT3(  halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },

		{ XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
    };


    D3D11_BUFFER_DESC vertexDesc;
    ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof( VertexPos ) * 6;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory( &resourceData, sizeof( resourceData ) );
    resourceData.pSysMem = vertices;



    d3dResult = d3dDevice_->CreateBuffer( &vertexDesc, &resourceData, &vertexBuffer_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create vertex buffer!" );
        return false;
    }


    D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( XMMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &mvpCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

	for(int i = 0.0; i < 1;i++)
	{
		for(int j = 0; j < 17;j++)
		{
			XMFLOAT2 sprite1Pos( (float)((50*j)), 300.0f );
			sprites_[j].SetPosition( sprite1Pos );
		}
	}

    XMMATRIX view = XMMatrixIdentity( );
    XMMATRIX projection = XMMatrixOrthographicOffCenterLH( 0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f );
    vpMatrix_ = XMMatrixMultiply( view, projection );

	//3rd
	ID3D11Resource* colorTex2;

    colorMap3_->GetResource( &colorTex2 );

    D3D11_TEXTURE2D_DESC colorTexDesc2;
    ( ( ID3D11Texture2D* )colorTex2 )->GetDesc( &colorTexDesc2 );
    colorTex2->Release( );

   float halfWidth2 = ( float )colorTexDesc2.Width / 2.0f;
    float halfHeight2 = ( float )colorTexDesc2.Height / 2.0f;

	
     VertexPos testing[] =
    {
        { XMFLOAT3(  halfWidth2,  halfHeight2, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth2, -halfHeight2, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth2, -halfHeight2, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -halfWidth2, -halfHeight2, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth2,  halfHeight2, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth2,  halfHeight2, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },

		{ XMFLOAT3( -halfWidth2, -halfHeight2, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth2,  halfHeight2, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth2,  halfHeight2, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
    };


    D3D11_BUFFER_DESC vertexDesc2;
    ZeroMemory( &vertexDesc2, sizeof( vertexDesc2 ) );
    vertexDesc2.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc2.ByteWidth = sizeof( VertexPos ) * 6;

    D3D11_SUBRESOURCE_DATA resourceData2;
    ZeroMemory( &resourceData2, sizeof( resourceData2 ) );
    resourceData2.pSysMem = testing;

    d3dResult = d3dDevice_->CreateBuffer( &vertexDesc2, &resourceData2, &vertexBuffer3_ );

	XMFLOAT2 sprite2Pos(xCoordinate, yCoordinate);
	sprites2_[0].SetPosition(sprite2Pos);
	XMFLOAT2 sprite3Pos(xCoordinate+105.0f, yCoordinate);
	sprites2_[1].SetPosition(sprite3Pos);
	XMFLOAT2 sprite4Pos(xCoordinate, yCoordinate-17.0f);
	sprites2_[2].SetPosition(sprite4Pos);
	XMFLOAT2 sprite5Pos(xCoordinate+105.0f, yCoordinate-17.0f);
	sprites2_[3].SetPosition(sprite5Pos);
	//end of 3rd

    D3D11_BLEND_DESC blendDesc;
    ZeroMemory( &blendDesc, sizeof( blendDesc ) );
    blendDesc.RenderTarget[0].BlendEnable = FALSE;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    d3dDevice_->CreateBlendState( &blendDesc, &alphaBlendState_ );
    d3dContext_->OMSetBlendState( alphaBlendState_, blendFactor, 0xFFFFFFFF );

    return true;
}


void KeyboardDemo::UnloadContent( )
{
    if( colorMapSampler_ ) colorMapSampler_->Release( );
    if( colorMap_ ) colorMap_->Release( );
    if( solidColorVS_ ) solidColorVS_->Release( );
    if( solidColorPS_ ) solidColorPS_->Release( );
    if( inputLayout_ ) inputLayout_->Release( );
    if( vertexBuffer_ ) vertexBuffer_->Release( );
    if( mvpCB_ ) mvpCB_->Release( );
    if( alphaBlendState_ ) alphaBlendState_->Release( );

    colorMapSampler_ = 0;
    colorMap_ = 0;
    solidColorVS_ = 0;
    solidColorPS_ = 0;
    inputLayout_ = 0;
    vertexBuffer_ = 0;
    mvpCB_ = 0;
    alphaBlendState_ = 0;
}


void KeyboardDemo::Update( float dt )
{
    keyboardDevice_->GetDeviceState( sizeof( keyboardKeys_ ), ( LPVOID )&keyboardKeys_ );

    // Button press event.
    if( GetAsyncKeyState( VK_ESCAPE ) )
	{ 
		PostQuitMessage( 0 );
	}

    // Button up event.
	if( KEYDOWN( prevKeyboardKeys_, DIK_DOWN ) && !KEYDOWN( keyboardKeys_, DIK_DOWN ) )
	{ 
		selectedColor_=1;
		y= y - 20.0;
		//q=q+10.0f;
		//e=e+10.0f;
	}

    // Button up event.
	if( KEYDOWN( prevKeyboardKeys_, DIK_RIGHT ) && !KEYDOWN( keyboardKeys_, DIK_RIGHT ) )
	{ 
		selectedColor_=1;
		w= w + 20.0;
	}
	if( KEYDOWN( prevKeyboardKeys_, DIK_UP ) && !KEYDOWN( keyboardKeys_, DIK_UP ) )
	{ 
		selectedColor_=1;
		y= y + 20.0;
	}
	if( KEYDOWN( prevKeyboardKeys_, DIK_LEFT ) && !KEYDOWN( keyboardKeys_, DIK_LEFT ) )
	{ 
		selectedColor_=1;
		w= w - 20.0;
	}
	/*
	Directions:
	0 = South
	1 = NorthWest
	2 = NorthEast
	3 = SouthEast
	4 = SouthWest
	5 = North
	*/
	if(direction == 0)
	{
		y2 -= 5.0f;
	}
	//right side of paddle hit
	for(int i = 0; i < 3; i++)
	{

		if((z > sprites2_[i].GetPositionX()-50 && z < sprites2_[i].GetPositionX()+50.0) && (y2 > sprites2_[i].GetPositionY() && y2 < sprites2_[i].GetPositionY() + 20))
	{
		XMFLOAT2 sprite2Pos(-500.0f, -500.0f);
		sprites2_[i].SetPosition(sprite2Pos);
	}
	}
	if((z > w && z < w+37.5) && (y2 > y && y2 < y + 10) && z > 794.9 && z < 800.0)
	{
		direction = 1;
	}
	if((z > w && z < w+37.5) && (y2 > y && y2 < y + 10) && direction == 3)
	{
		direction = 1;
	}
	if((z > w && z < w+37.5) && (y2 > y && y2 < y + 10))
	{
		direction = 1;
	}
	if((z > w-37.5 && z < w) && (y2 > y && y2 < y + 10) && direction != 4)
	{
		direction = 2;
	}
	if((z > w-37.5 && z < w) && (y2 > y && y2 < y + 10) && direction == 4)
	{
		direction =1;
	}
	//if(direction == 1 && y2 <= 600 && z != 0.0)
	//{
	//	z -=5.0f;
	//	y2 += 5.0f;
	//}
	//paddle hit right side from left side
	if(direction == 1 && z != -1.0)
	{
		if(z == 0.0)
		{
		direction = 2;
		z -= 5.0f;
		y2 +=5.0f;
		}
		if(y2 == 600)
		{
			direction = 4;
		}
		z -= 5.0f;
		y2 +=5.0f;
	}
	if(direction == 2 && y2 !=605.0)
		{
			if(z == 800.0)
			{
				direction = 1;
			}
			if(y2 == 600.0)
			{
				direction = 3;
			}
		z += 5.0f;
		y2 +=5.0f;
		}
	// top of the screen hit from left side
	if(direction == 3 && z != 805.0)
	{
		if(z==800.0)
		{
			direction = 4;
		}
		z += 5.0f;
		y2 -= 5.0f;
	}
	// top of the screen hit from left side
	if(direction == 4)
	{
		if(z == 0.0)
		{
			direction = 3;
		}
		y2 -= 5.0f;
		z -= 5.0f;
	}
	//right side hit from left side
	if(direction==6)
	{
		z+= 5.0f;
		y2 +=5.0f;
	}
	if(i == 7)
	{
		z -= 5.0f;
		y2 -= 5.0f;
	}
	if(i == 8)
	{
		
		z -= 5.0f;
		y2 -= 5.0f;
	}
	/*if(i >= 0.0 && i < 500.0)
	{
			//z += 5.0f;
			y2 -= 5.0f;
			i += 5.0f;
	}
	if(i > 499.9 && i < 1000.0)
	{
		//z -= 5.0f;
		y2 -= 5.0f;
		i += 5.0f;
	}*/
    memcpy( prevKeyboardKeys_, keyboardKeys_, sizeof( keyboardKeys_ ) );


}
void KeyboardDemo::Update2( float dt )
{

	if(i == 750.0)
	{
		z += 40.0f;
	}
}
void KeyboardDemo::Render( )
{
    if( d3dContext_ == 0 )
        return;
    float clearColor[4] = { 0.112f, 0.176f, 0.25f, 1.0f };
    d3dContext_->ClearRenderTargetView( backBufferTarget_, clearColor );

    unsigned int stride = sizeof( VertexPos );
    unsigned int offset = 0;

    d3dContext_->IASetInputLayout( inputLayout_ );
    d3dContext_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    d3dContext_->VSSetShader( solidColorVS_, 0, 0 );
    d3dContext_->PSSetShader( solidColorPS_, 0, 0 );
    d3dContext_->PSSetShaderResources( 0, 1, &colorMap2_ );
    d3dContext_->PSSetSamplers( 0, 1, &colorMapSampler_ );
	

	d3dContext_->PSSetShaderResources( 0, 1, &colorMap3_ );
	d3dContext_->IASetVertexBuffers( 0, 1, &vertexBuffer3_, &stride, &offset );
	//Horizontal Lines
	for(int i = 0; i < 3; i++)
	{
        XMMATRIX world = sprites2_[i].GetWorldMatrix( );
        XMMATRIX mvp = XMMatrixMultiply( world, vpMatrix_ );
        mvp = XMMatrixTranspose( mvp );

        d3dContext_->UpdateSubresource( mvpCB_, 0, 0, &mvp, 0, 0 );
        d3dContext_->VSSetConstantBuffers( 0, 1, &mvpCB_ );

        d3dContext_->Draw( 6, 0 );
	}
	
	//Box Position


			ID3D11Resource* colorTex;
    colorMap_->GetResource( &colorTex );

    D3D11_TEXTURE2D_DESC colorTexDesc;
    ( ( ID3D11Texture2D* )colorTex )->GetDesc( &colorTexDesc );
    colorTex->Release( );

    float halfWidth = ( float )colorTexDesc.Width / 2.0f;
    float halfHeight = ( float )colorTexDesc.Height / 2.0f;

	
     VertexPos pogim[] =
    {
        { XMFLOAT3(  halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },

		{ XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
    };
    D3D11_BUFFER_DESC vertexDesc;
    ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof( VertexPos ) * 6;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory( &resourceData, sizeof( resourceData ) );
    resourceData.pSysMem = pogim;


		HRESULT d3dResult = d3dDevice_->CreateBuffer( &vertexDesc, &resourceData, &vertexBuffer2_ );
		d3dContext_->PSSetShaderResources( 0, 1, &colorMap_ );
		d3dContext_->IASetVertexBuffers( 0, 1, &vertexBuffer2_, &stride, &offset );
		XMFLOAT2 sprite4Pos( z, y2  );
		sprites3_[0].SetPosition( sprite4Pos );
		XMMATRIX world = sprites3_[0].GetWorldMatrix( );
        XMMATRIX mvp = XMMatrixMultiply( world, vpMatrix_ );
        mvp = XMMatrixTranspose( mvp );
        d3dContext_->UpdateSubresource( mvpCB_, 0, 0, &mvp, 0, 0 );
        d3dContext_->VSSetConstantBuffers( 0, 1, &mvpCB_ );
		d3dContext_->Draw( 6, 0 );
	
	//end of Box Position
	
	// Item to get the box
		if(selectedColor_=1)
		{
		ID3D11Resource* colorTex;
    colorMap4_->GetResource( &colorTex );

    D3D11_TEXTURE2D_DESC colorTexDesc;
    ( ( ID3D11Texture2D* )colorTex )->GetDesc( &colorTexDesc );
    colorTex->Release( );

    float halfWidth = ( float )colorTexDesc.Width / 2.0f;
    float halfHeight = ( float )colorTexDesc.Height / 2.0f;

	
     VertexPos pogim[] =
    {
        { XMFLOAT3(  halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },

		{ XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
    };
    D3D11_BUFFER_DESC vertexDesc;
    ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof( VertexPos ) * 6;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory( &resourceData, sizeof( resourceData ) );
    resourceData.pSysMem = pogim;
	
    HRESULT d3dResult = d3dDevice_->CreateBuffer( &vertexDesc, &resourceData, &vertexBuffer2_ );
		d3dContext_->PSSetShaderResources( 0, 1, &colorMap4_ );
		d3dContext_->IASetVertexBuffers( 0, 1, &vertexBuffer2_, &stride, &offset );
		XMFLOAT2 sprite2Pos( w, y );
    sprites_[49].SetPosition( sprite2Pos );
		XMMATRIX world = sprites_[49].GetWorldMatrix( );
        XMMATRIX mvp = XMMatrixMultiply( world, vpMatrix_ );
        mvp = XMMatrixTranspose( mvp );

        d3dContext_->UpdateSubresource( mvpCB_, 0, 0, &mvp, 0, 0 );
        d3dContext_->VSSetConstantBuffers( 0, 1, &mvpCB_ );

        d3dContext_->Draw( 6, 0 );


		}
		//End of item
    swapChain_->Present( 0, 0 );
}
int KeyboardDemo::selectedColor( )
{
	return selectedColor_;
}