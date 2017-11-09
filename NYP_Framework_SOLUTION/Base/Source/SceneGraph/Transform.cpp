#include "Transform.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include <windows.h> // Header File For Windows
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

CTransform::CTransform(void)
{
	Mtx.SetToIdentity();
	Update_Mtx.SetToIdentity();
}

void CTransform::reset(void){
	Mtx.SetToIdentity();
	Update_Mtx.SetToIdentity();
}

CTransform::CTransform( const float dx, const float dy, const float dz )
{
	Mtx.SetToTranslation( dx, dy, dz );
}

CTransform::~CTransform(void)
{
}

void CTransform::PreRendering()
{
	glPushMatrix();
		glMultMatrixf(Mtx.a);
}

void CTransform::Draw(void)
{
}

void CTransform::PostRendering()
{
	glPopMatrix();
}

void CTransform::SetTranslate( const float dx, const float dy, const float dz  )
{
	Mtx44 TempMtx;
	TempMtx.SetToTranslation( dx, dy, dz );
	Mtx = Mtx*TempMtx;
}

//void CTransform::SetRotate( const float angle, const float rx, const float ry, const float rz  )
//{
//	Mtx44 TempMtx;
//	TempMtx.Rotatef( angle, rx, ry, rz );
//	
//	Mtx44 MtxBackToOrigin;
//
//	MtxBackToOrigin.matrix[ MtxBackToOrigin.INDEX4( 0, 3 ) ] = -Mtx.matrix[ Mtx.INDEX4( 0, 3 ) ];
//	MtxBackToOrigin.matrix[ MtxBackToOrigin.INDEX4( 1, 3 ) ] = -Mtx.matrix[ Mtx.INDEX4( 1, 3 ) ];
//	MtxBackToOrigin.matrix[ MtxBackToOrigin.INDEX4( 2, 3 ) ] = -Mtx.matrix[ Mtx.INDEX4( 2, 3 ) ];
//
//	Mtx44 MtxBackToPosition;
//	MtxBackToPosition.matrix[ MtxBackToPosition.INDEX4( 0, 3 ) ] = Mtx.matrix[ Mtx.INDEX4( 0, 3 ) ];
//	MtxBackToPosition.matrix[ MtxBackToPosition.INDEX4( 1, 3 ) ] = Mtx.matrix[ Mtx.INDEX4( 1, 3 ) ];
//	MtxBackToPosition.matrix[ MtxBackToPosition.INDEX4( 2, 3 ) ] = Mtx.matrix[ Mtx.INDEX4( 2, 3 ) ];
//
//	Mtx = Mtx * MtxBackToOrigin;
//	Mtx = Mtx * TempMtx;
//	Mtx = Mtx * MtxBackToPosition;
//}
void CTransform::SetRotate( const float angle, const float rx, const float ry, const float rz  ){
	Mtx44 TempMtx;
	TempMtx.SetToRotation( angle, rx, ry, rz );
	Mtx = Mtx*TempMtx;
}


//void CTransform::SetRotate2( const float angle, const float rx, const float ry, const float rz, 
//							 const float offset_x, const float offset_y, const float offset_z )
//{
//	Mtx44 TempMtx;
//	TempMtx.Rotatef( angle, rx, ry, rz );
//	
//	Mtx44 MtxBackToOrigin;
//
//	MtxBackToOrigin.matrix[ MtxBackToOrigin.INDEX4( 0, 3 ) ] = -offset_x;
//	MtxBackToOrigin.matrix[ MtxBackToOrigin.INDEX4( 1, 3 ) ] = -offset_y;
//	MtxBackToOrigin.matrix[ MtxBackToOrigin.INDEX4( 2, 3 ) ] = -offset_z;
//
//	Mtx44 MtxBackToPosition;
//	MtxBackToPosition.matrix[ MtxBackToPosition.INDEX4( 0, 3 ) ] = offset_x;
//	MtxBackToPosition.matrix[ MtxBackToPosition.INDEX4( 1, 3 ) ] = offset_y;
//	MtxBackToPosition.matrix[ MtxBackToPosition.INDEX4( 2, 3 ) ] = offset_z;
//
//	Mtx = Mtx * MtxBackToOrigin;
//	Mtx = Mtx * TempMtx;
//	Mtx = Mtx * MtxBackToPosition;
//}

void CTransform::SetScale( const float sx, const float sy, const float sz  )
{
	Mtx.SetToScale( sx, sy, sz );
}

void CTransform::ApplyTransform( Mtx44 newMTX )
{
	Mtx = Mtx * newMTX;
}

void CTransform::GetOffset( float& x, float& y, float& z )
{
	/*
	x = Mtx.a[ Mtx.INDEX4( 0, 3 ) ];
	y = Mtx.a[ Mtx.INDEX4( 1, 3 ) ];
	z = Mtx.a[ Mtx.INDEX4( 2, 3 ) ];
	*/
}

// Get the transformation matrix
Mtx44 CTransform::GetTransform(void)
{
	return Mtx;
}