/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    GameSprite - Used to represent a 2D sprite's transformation.
*/


#include<d3d11.h>
#include<d3dx11.h>
#include"GameSprite.h"


GameSprite::GameSprite( ) : rotation_( 0 )
{
    scale_.x = scale_.y = 1.0f;
}


GameSprite::~GameSprite( )
{

}


XMMATRIX GameSprite::GetWorldMatrix( )
{
    XMMATRIX translation = XMMatrixTranslation( position_.x, position_.y, 0.5f );
    XMMATRIX rotationZ = XMMatrixRotationZ( rotation_ );
    XMMATRIX scale = XMMatrixScaling( scale_.x, scale_.y, 1.0f );

    return translation * rotationZ * scale;
}
/*XMMATRIX GameSprite::GetWorldMatrix2( )
{
    XMMATRIX translation = XMMatrixTranslation( w , z, 0.5f );
    XMMATRIX rotationZ = XMMatrixRotationZ( rotation_ );
    XMMATRIX scale = XMMatrixScaling( scale_.x, scale_.y, 1.0f );

    return translation * rotationZ * scale;
}
*/
void GameSprite::SetPosition( XMFLOAT2& position )
{
    position_ = position;
}


void GameSprite::SetRotation( float rotation )
{
    rotation_ = rotation;
}


void GameSprite::SetScale( XMFLOAT2& scale )
{
    scale_ = scale;
}
int GameSprite::GetPositionX( )
{
	return position_.x;
}
int GameSprite::GetPositionY( )
{
	return position_.y;
}