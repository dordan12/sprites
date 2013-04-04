/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    Keyboard Demo - Demo used to perform keyboard input.
*/


#ifndef _KEYBOARD_DEMO_H_
#define _KEYBOARD_DEMO_H_

#include"Dx11DemoBase.h"
#include"GameSprite.h"

class KeyboardDemo : public Dx11DemoBase
{
    public:
        KeyboardDemo( );
        virtual ~KeyboardDemo( );

        bool LoadContent( );
		bool LoadContent2( );
        void UnloadContent( );

        void Update( float dt );
		void Update2( float dt );
        void Render( );
		void Render2( );
		int selectedColor();
		int selectedColor_;
		float w;
		float y;
		float z;
    private:
		//Drawing the Game Sprites
        ID3D11VertexShader* solidColorVS_;
        ID3D11PixelShader* solidColorPS_;

        ID3D11InputLayout* inputLayout_;
        ID3D11Buffer* vertexBuffer_;

		ID3D11ShaderResourceView* colorMap_;
        ID3D11SamplerState* colorMapSampler_;
        ID3D11BlendState* alphaBlendState_;

        GameSprite sprites_[50];
		GameSprite sprites2_[30];
		GameSprite sprites3_[5];
        ID3D11Buffer* mvpCB_;
        XMMATRIX vpMatrix_;

		ID3D11Buffer* colorCB_;
		//Drawing the bars for board
		ID3D11VertexShader* solidColorVS2_;
        ID3D11PixelShader* solidColorPS2_;

        ID3D11InputLayout* inputLayout2_;
        ID3D11Buffer* vertexBuffer2_;

		ID3D11ShaderResourceView* colorMap2_;
        ID3D11SamplerState* colorMapSampler2_;
        ID3D11BlendState* alphaBlendState2_;

		ID3D11Buffer* mvpCB2_;
        XMMATRIX vpMatrix2_;

		//drawing for horizontal bars
		ID3D11ShaderResourceView* colorMap3_;
		ID3D11Buffer* vertexBuffer3_;
		XMMATRIX vpMatrix3_;

		//Box
		ID3D11ShaderResourceView* colorMap4_;
		ID3D11Buffer* vertexBuffer4_;
		XMMATRIX vpMatrix4_;
		ID3D11Buffer* mvpCB3_;
		//Keyboard
		
};

#endif