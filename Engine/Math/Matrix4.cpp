//---------------------------------------------------------------------------
// Matrix4.cpp
//---------------------------------------------------------------------------


#include "Engine/Math/Vec4.hpp"
#include "Matrix4.hpp"


//---------------------------------------------------------------------------
Matrix4::Matrix4()
{
	m_matrix[ 0 ]	= 1.f;
	m_matrix[ 1 ]	= 0.f;
	m_matrix[ 2 ]	= 0.f;
	m_matrix[ 3 ]	= 0.f;

	m_matrix[ 4 ]	= 0.f;
	m_matrix[ 5 ]	= 1.f;
	m_matrix[ 6 ]	= 0.f;
	m_matrix[ 7 ]	= 0.f;

	m_matrix[ 8 ]	= 0.f;
	m_matrix[ 9 ]	= 0.f;
	m_matrix[ 10 ]	= 1.f;
	m_matrix[ 11 ]	= 0.f;

	m_matrix[ 12 ]	= 0.f;
	m_matrix[ 13 ]	= 0.f;
	m_matrix[ 14 ]	= 0.f;
	m_matrix[ 15 ]	= 1.f;
}


//---------------------------------------------------------------------------
Matrix4::Matrix4( const Vec3f& i, const Vec3f& j, const Vec3f& k )
{
	m_matrix[ 0 ]	= i.x;
	m_matrix[ 1 ]	= i.y;
	m_matrix[ 2 ]	= i.z;
	m_matrix[ 3 ]	= 0.f;

	m_matrix[ 4 ]	= j.x;
	m_matrix[ 5 ]	= j.y;
	m_matrix[ 6 ]	= j.z;
	m_matrix[ 7 ]	= 0.f;

	m_matrix[ 8 ]	= k.x;
	m_matrix[ 9 ]	= k.y;
	m_matrix[ 10 ]	= k.z;
	m_matrix[ 11 ]	= 0.f;

	m_matrix[ 12 ]	= 0.f;
	m_matrix[ 13 ]	= 0.f;
	m_matrix[ 14 ]	= 0.f;
	m_matrix[ 15 ]	= 1.f;
}


//---------------------------------------------------------------------------
Matrix4::Matrix4( const Matrix4& matrixToCopy )
{
	m_matrix[ 0 ]	= matrixToCopy.m_matrix[ 0 ];
	m_matrix[ 1 ]	= matrixToCopy.m_matrix[ 1 ];
	m_matrix[ 2 ]	= matrixToCopy.m_matrix[ 2 ];
	m_matrix[ 3 ]	= matrixToCopy.m_matrix[ 3 ];

	m_matrix[ 4 ]	= matrixToCopy.m_matrix[ 4 ];
	m_matrix[ 5 ]	= matrixToCopy.m_matrix[ 5 ];
	m_matrix[ 6 ]	= matrixToCopy.m_matrix[ 6 ];
	m_matrix[ 7 ]	= matrixToCopy.m_matrix[ 7 ];

	m_matrix[ 8 ]	= matrixToCopy.m_matrix[ 8 ];
	m_matrix[ 9 ]	= matrixToCopy.m_matrix[ 9 ];
	m_matrix[ 10 ]	= matrixToCopy.m_matrix[ 10 ];
	m_matrix[ 11 ]	= matrixToCopy.m_matrix[ 11 ];

	m_matrix[ 12 ]	= matrixToCopy.m_matrix[ 12 ];
	m_matrix[ 13 ]	= matrixToCopy.m_matrix[ 13 ];
	m_matrix[ 14 ]	= matrixToCopy.m_matrix[ 14 ];
	m_matrix[ 15 ]	= matrixToCopy.m_matrix[ 15 ];
}


//---------------------------------------------------------------------------
const Matrix4& Matrix4::operator=( const Matrix4& rightHandSide )
{
	//Matrix4 result;
	m_matrix[ 0 ]	= rightHandSide.m_matrix[ 0 ];
	m_matrix[ 1 ]	= rightHandSide.m_matrix[ 1 ];
	m_matrix[ 2 ]	= rightHandSide.m_matrix[ 2 ];
	m_matrix[ 3 ]	= rightHandSide.m_matrix[ 3 ];

	m_matrix[ 4 ]	= rightHandSide.m_matrix[ 4 ];
	m_matrix[ 5 ]	= rightHandSide.m_matrix[ 5 ];
	m_matrix[ 6 ]	= rightHandSide.m_matrix[ 6 ];
	m_matrix[ 7 ]	= rightHandSide.m_matrix[ 7 ];

	m_matrix[ 8 ]	= rightHandSide.m_matrix[ 8 ];
	m_matrix[ 9 ]	= rightHandSide.m_matrix[ 9 ];
	m_matrix[ 10 ]	= rightHandSide.m_matrix[ 10 ];
	m_matrix[ 11 ]	= rightHandSide.m_matrix[ 11 ];

	m_matrix[ 12 ]	= rightHandSide.m_matrix[ 12 ];
	m_matrix[ 13 ]	= rightHandSide.m_matrix[ 13 ];
	m_matrix[ 14 ]	= rightHandSide.m_matrix[ 14 ];
	m_matrix[ 15 ]	= rightHandSide.m_matrix[ 15 ];

	return *this;
}


//---------------------------------------------------------------------------
void Matrix4::SetToIdentity()
{
	m_matrix[ 0 ]	= 1.f;
	m_matrix[ 1 ]	= 0.f;
	m_matrix[ 2 ]	= 0.f;
	m_matrix[ 3 ]	= 0.f;

	m_matrix[ 4 ]	= 0.f;
	m_matrix[ 5 ]	= 1.f;
	m_matrix[ 6 ]	= 0.f;
	m_matrix[ 7 ]	= 0.f;

	m_matrix[ 8 ]	= 0.f;
	m_matrix[ 9 ]	= 0.f;
	m_matrix[ 10 ]	= 1.f;
	m_matrix[ 11 ]	= 0.f;

	m_matrix[ 12 ]	= 0.f;
	m_matrix[ 13 ]	= 0.f;
	m_matrix[ 14 ]	= 0.f;
	m_matrix[ 15 ]	= 1.f;
}


// code based on examples found at http://www.songho.ca/opengl/gl_matrix.html
//---------------------------------------------------------------------------
void Matrix4::SetFrustum( const float left, const float right, const float bottom, const float top, const float zNear, const float zFar )
{
	float oneOverRightLeftDifference = 1.f / ( right - left );
	float oneOverTopBottomDifference = 1.f / ( top - bottom );
	float oneOverFarNearDifference = 1.f / ( zFar - zNear );

	m_matrix[ 0 ] = 2.f * zNear * oneOverRightLeftDifference;
	m_matrix[ 5 ] = 2.f * zNear * oneOverTopBottomDifference;
	m_matrix[ 8 ] = ( right + left ) * oneOverRightLeftDifference;
	m_matrix[ 9 ] = ( top + bottom ) * oneOverTopBottomDifference;
	m_matrix[ 10 ] = - ( zFar + zNear ) * oneOverFarNearDifference;
	m_matrix[ 11 ] = -1.f;
	m_matrix[ 14 ] = - ( 2.f * zFar * zNear ) * oneOverFarNearDifference;
	m_matrix[ 15 ] = 0.f;
}


// code based on examples found at http://www.songho.ca/opengl/gl_matrix.html
//---------------------------------------------------------------------------
void Matrix4::SetPerspective( const float fovy, const float aspect, const float zNear, const float zFar )
{
	float tangent = tan( DegreesToRadians( fovy / 2.f ) );
	float height = zNear * tangent;
	float width = height * aspect;

	SetFrustum( -width, width, -height, height, zNear, zFar );
}


// code based on examples found at http://www.songho.ca/opengl/gl_matrix.html
//---------------------------------------------------------------------------
void Matrix4::SetOrtho( const float left, const float right, const float bottom, const float top, const float zNear, const float zFar )
{
	float oneOverRightMinusLeft = 1.f / ( right - left );
	float oneOverTopMinusBottom = 1.f / ( top - bottom );
	float oneOverFarMinusNear = 1.f / ( zFar - zNear );

	m_matrix[0]  = 2.f * oneOverRightMinusLeft;
	m_matrix[5]  = 2.f * oneOverTopMinusBottom;
	m_matrix[10] = - 2.f * oneOverFarMinusNear;
	m_matrix[12] = - ( right + left ) * oneOverRightMinusLeft;
	m_matrix[13] = - ( top + bottom ) * oneOverTopMinusBottom;
	m_matrix[14] = - ( zFar + zNear ) * oneOverFarMinusNear;
	m_matrix[15] = 1.f;
}


//---------------------------------------------------------------------------
void Matrix4::ApplyThisMatrixToVector( Vec2f& value ) const
{
	Vec4f temp = Vec4f( value.x, value.y, 0.f, 1.f );

	value.x = m_matrix[ 0 ] * temp.x + m_matrix[ 4 ] * temp.y + m_matrix[ 8 ] * temp.z + m_matrix[ 12 ] * temp.w;
	value.y = m_matrix[ 1 ] * temp.x + m_matrix[ 5 ] * temp.y + m_matrix[ 9 ] * temp.z + m_matrix[ 13 ] * temp.w;
}


//---------------------------------------------------------------------------
void Matrix4::ApplyThisMatrixToVector( Vec3f& value ) const
{
	Vec4f temp = Vec4f( value.x, value.y, value.z, 1.f );

	value.x = m_matrix[ 0 ] * temp.x + m_matrix[ 4 ] * temp.y + m_matrix[ 8 ] * temp.z + m_matrix[ 12 ] * temp.w;
	value.y = m_matrix[ 1 ] * temp.x + m_matrix[ 5 ] * temp.y + m_matrix[ 9 ] * temp.z + m_matrix[ 13 ] * temp.w;
	value.z = m_matrix[ 2 ] * temp.x + m_matrix[ 6 ] * temp.y + m_matrix[ 10 ] * temp.z + m_matrix[ 14 ] * temp.w;
}


//---------------------------------------------------------------------------
STATIC const Matrix4 Matrix4::GetRotationMatrix( const float rotationDegrees, const Vec3f& rotationAxis )
{
	Matrix4 result;
	Vec3f axis = rotationAxis;
	axis.Normalize();

	float cosAngle = CosDegrees( rotationDegrees );
	float oneMinusCosAngle = 1.f - cosAngle;
	float sinAngle = SinDegrees( rotationDegrees );

	result.m_matrix[ 0 ] = axis.x * axis.x * oneMinusCosAngle + cosAngle;
	result.m_matrix[ 1 ] = axis.x * axis.y * oneMinusCosAngle + axis.z * sinAngle;
	result.m_matrix[ 2 ] = axis.x * axis.z * oneMinusCosAngle - axis.y * sinAngle;

	result.m_matrix[ 4 ] = axis.y * axis.x * oneMinusCosAngle - axis.z * sinAngle;
	result.m_matrix[ 5 ] = axis.y * axis.y * oneMinusCosAngle + cosAngle;
	result.m_matrix[ 6 ] = axis.y * axis.z * oneMinusCosAngle + axis.x * sinAngle;

	result.m_matrix[ 8 ] = axis.z * axis.x * oneMinusCosAngle + axis.y * sinAngle;
	result.m_matrix[ 9 ] = axis.z * axis.y * oneMinusCosAngle - axis.x * sinAngle;
	result.m_matrix[ 10 ] = axis.z * axis.z * oneMinusCosAngle + cosAngle;

	return result;
}


//---------------------------------------------------------------------------
void Matrix4::ApplyTransformationMatrix( const Matrix4& newTransform )
{
	Matrix4 temp = *this;
	Matrix4& t = temp;
	const Matrix4& n = newTransform;

	m_matrix[ 0 ] = n.m_matrix[ 0 ] * t.m_matrix[ 0 ] + n.m_matrix[ 1 ] * t.m_matrix[ 4 ] + n.m_matrix[ 2 ] * t.m_matrix[ 8 ] + n.m_matrix[ 3 ] * t.m_matrix[ 12 ];
	m_matrix[ 1 ] = n.m_matrix[ 0 ] * t.m_matrix[ 1 ] + n.m_matrix[ 1 ] * t.m_matrix[ 5 ] + n.m_matrix[ 2 ] * t.m_matrix[ 9 ] + n.m_matrix[ 3 ] * t.m_matrix[ 13 ];
	m_matrix[ 2 ] = n.m_matrix[ 0 ] * t.m_matrix[ 2 ] + n.m_matrix[ 1 ] * t.m_matrix[ 6 ] + n.m_matrix[ 2 ] * t.m_matrix[ 10 ] + n.m_matrix[ 3 ] * t.m_matrix[ 14 ];
	m_matrix[ 3 ] = n.m_matrix[ 0 ] * t.m_matrix[ 3 ] + n.m_matrix[ 1 ] * t.m_matrix[ 7 ] + n.m_matrix[ 2 ] * t.m_matrix[ 11 ] + n.m_matrix[ 3 ] * t.m_matrix[ 15 ];

	m_matrix[ 4 ] = n.m_matrix[ 4 ] * t.m_matrix[ 0 ] + n.m_matrix[ 5 ] * t.m_matrix[ 4 ] + n.m_matrix[ 6 ] * t.m_matrix[ 8 ] + n.m_matrix[ 7 ] * t.m_matrix[ 12 ];
	m_matrix[ 5 ] = n.m_matrix[ 4 ] * t.m_matrix[ 1 ] + n.m_matrix[ 5 ] * t.m_matrix[ 5 ] + n.m_matrix[ 6 ] * t.m_matrix[ 9 ] + n.m_matrix[ 7 ] * t.m_matrix[ 13 ];
	m_matrix[ 6 ] = n.m_matrix[ 4 ] * t.m_matrix[ 2 ] + n.m_matrix[ 5 ] * t.m_matrix[ 6 ] + n.m_matrix[ 6 ] * t.m_matrix[ 10 ] + n.m_matrix[ 7 ] * t.m_matrix[ 14 ];
	m_matrix[ 7 ] = n.m_matrix[ 4 ] * t.m_matrix[ 3 ] + n.m_matrix[ 5 ] * t.m_matrix[ 7 ] + n.m_matrix[ 6 ] * t.m_matrix[ 11 ] + n.m_matrix[ 7 ] * t.m_matrix[ 15 ];

	m_matrix[ 8 ] = n.m_matrix[ 8 ] * t.m_matrix[ 0 ] + n.m_matrix[ 9 ] * t.m_matrix[ 4 ] + n.m_matrix[ 10 ] * t.m_matrix[ 8 ] + n.m_matrix[ 11 ] * t.m_matrix[ 12 ];
	m_matrix[ 9 ] = n.m_matrix[ 8 ] * t.m_matrix[ 1 ] + n.m_matrix[ 9 ] * t.m_matrix[ 5 ] + n.m_matrix[ 10 ] * t.m_matrix[ 9 ] + n.m_matrix[ 11 ] * t.m_matrix[ 13 ];
	m_matrix[ 10 ] = n.m_matrix[ 8 ] * t.m_matrix[ 2 ] + n.m_matrix[ 9 ] * t.m_matrix[ 6 ] + n.m_matrix[ 10 ] * t.m_matrix[ 10 ] + n.m_matrix[ 11 ] * t.m_matrix[ 14 ];
	m_matrix[ 11 ] = n.m_matrix[ 8 ] * t.m_matrix[ 3 ] + n.m_matrix[ 9 ] * t.m_matrix[ 7 ] + n.m_matrix[ 10 ] * t.m_matrix[ 11 ] + n.m_matrix[ 11 ] * t.m_matrix[ 15 ];

	m_matrix[ 12 ] = n.m_matrix[ 12 ] * t.m_matrix[ 0 ] + n.m_matrix[ 13 ] * t.m_matrix[ 4 ] + n.m_matrix[ 14 ] * t.m_matrix[ 8 ] + n.m_matrix[ 15 ] * t.m_matrix[ 12 ];
	m_matrix[ 13 ] = n.m_matrix[ 12 ] * t.m_matrix[ 1 ] + n.m_matrix[ 13 ] * t.m_matrix[ 5 ] + n.m_matrix[ 14 ] * t.m_matrix[ 9 ] + n.m_matrix[ 15 ] * t.m_matrix[ 13 ];
	m_matrix[ 14 ] = n.m_matrix[ 12 ] * t.m_matrix[ 2 ] + n.m_matrix[ 13 ] * t.m_matrix[ 6 ] + n.m_matrix[ 14 ] * t.m_matrix[ 10 ] + n.m_matrix[ 15 ] * t.m_matrix[ 14 ];
	m_matrix[ 15 ] = n.m_matrix[ 12 ] * t.m_matrix[ 3 ] + n.m_matrix[ 13 ] * t.m_matrix[ 7 ] + n.m_matrix[ 14 ] * t.m_matrix[ 11 ] + n.m_matrix[ 15 ] * t.m_matrix[ 15 ];
}


// sample found at http://www.gamedev.net/topic/648190-algorithm-for-4x4-matrix-inverse/
//---------------------------------------------------------------------------
void Matrix4::Invert()
{
// 	float det = GetDeterminant();
// 	float oneOverDet = 1.f / det;
// 
// 	m_matrix[ 0 ] =
// 		m_matrix[ 5 ] * m_matrix[ 10 ] * m_matrix[ 15 ] +
// 		m_matrix[ 6 ] * m_matrix[ 11 ] * m_matrix[ 12 ] +
// 		m_matrix[ 7 ] * m_matrix[ 9 ] * m_matrix[ 13 ];

	float* m = m_matrix;
	float inv[ 16 ];
	float det = 0.f;
	int i;

	inv[ 0 ] = m[ 5 ] * m[ 10 ] * m[ 15 ] -
		m[ 5 ] * m[ 11 ] * m[ 14 ] -
		m[ 9 ] * m[ 6 ] * m[ 15 ] +
		m[ 9 ] * m[ 7 ] * m[ 14 ] +
		m[ 13 ] * m[ 6 ] * m[ 11 ] -
		m[ 13 ] * m[ 7 ] * m[ 10 ];

	inv[ 4 ] = -m[ 4 ] * m[ 10 ] * m[ 15 ] +
		m[ 4 ] * m[ 11 ] * m[ 14 ] +
		m[ 8 ] * m[ 6 ] * m[ 15 ] -
		m[ 8 ] * m[ 7 ] * m[ 14 ] -
		m[ 12 ] * m[ 6 ] * m[ 11 ] +
		m[ 12 ] * m[ 7 ] * m[ 10 ];

	inv[ 8 ] = m[ 4 ] * m[ 9 ] * m[ 15 ] -
		m[ 4 ] * m[ 11 ] * m[ 13 ] -
		m[ 8 ] * m[ 5 ] * m[ 15 ] +
		m[ 8 ] * m[ 7 ] * m[ 13 ] +
		m[ 12 ] * m[ 5 ] * m[ 11 ] -
		m[ 12 ] * m[ 7 ] * m[ 9 ];

	inv[ 12 ] = -m[ 4 ] * m[ 9 ] * m[ 14 ] +
		m[ 4 ] * m[ 10 ] * m[ 13 ] +
		m[ 8 ] * m[ 5 ] * m[ 14 ] -
		m[ 8 ] * m[ 6 ] * m[ 13 ] -
		m[ 12 ] * m[ 5 ] * m[ 10 ] +
		m[ 12 ] * m[ 6 ] * m[ 9 ];

	inv[ 1 ] = -m[ 1 ] * m[ 10 ] * m[ 15 ] +
		m[ 1 ] * m[ 11 ] * m[ 14 ] +
		m[ 9 ] * m[ 2 ] * m[ 15 ] -
		m[ 9 ] * m[ 3 ] * m[ 14 ] -
		m[ 13 ] * m[ 2 ] * m[ 11 ] +
		m[ 13 ] * m[ 3 ] * m[ 10 ];

	inv[ 5 ] = m[ 0 ] * m[ 10 ] * m[ 15 ] -
		m[ 0 ] * m[ 11 ] * m[ 14 ] -
		m[ 8 ] * m[ 2 ] * m[ 15 ] +
		m[ 8 ] * m[ 3 ] * m[ 14 ] +
		m[ 12 ] * m[ 2 ] * m[ 11 ] -
		m[ 12 ] * m[ 3 ] * m[ 10 ];

	inv[ 9 ] = -m[ 0 ] * m[ 9 ] * m[ 15 ] +
		m[ 0 ] * m[ 11 ] * m[ 13 ] +
		m[ 8 ] * m[ 1 ] * m[ 15 ] -
		m[ 8 ] * m[ 3 ] * m[ 13 ] -
		m[ 12 ] * m[ 1 ] * m[ 11 ] +
		m[ 12 ] * m[ 3 ] * m[ 9 ];

	inv[ 13 ] = m[ 0 ] * m[ 9 ] * m[ 14 ] -
		m[ 0 ] * m[ 10 ] * m[ 13 ] -
		m[ 8 ] * m[ 1 ] * m[ 14 ] +
		m[ 8 ] * m[ 2 ] * m[ 13 ] +
		m[ 12 ] * m[ 1 ] * m[ 10 ] -
		m[ 12 ] * m[ 2 ] * m[ 9 ];

	inv[ 2 ] = m[ 1 ] * m[ 6 ] * m[ 15 ] -
		m[ 1 ] * m[ 7 ] * m[ 14 ] -
		m[ 5 ] * m[ 2 ] * m[ 15 ] +
		m[ 5 ] * m[ 3 ] * m[ 14 ] +
		m[ 13 ] * m[ 2 ] * m[ 7 ] -
		m[ 13 ] * m[ 3 ] * m[ 6 ];

	inv[ 6 ] = -m[ 0 ] * m[ 6 ] * m[ 15 ] +
		m[ 0 ] * m[ 7 ] * m[ 14 ] +
		m[ 4 ] * m[ 2 ] * m[ 15 ] -
		m[ 4 ] * m[ 3 ] * m[ 14 ] -
		m[ 12 ] * m[ 2 ] * m[ 7 ] +
		m[ 12 ] * m[ 3 ] * m[ 6 ];

	inv[ 10 ] = m[ 0 ] * m[ 5 ] * m[ 15 ] -
		m[ 0 ] * m[ 7 ] * m[ 13 ] -
		m[ 4 ] * m[ 1 ] * m[ 15 ] +
		m[ 4 ] * m[ 3 ] * m[ 13 ] +
		m[ 12 ] * m[ 1 ] * m[ 7 ] -
		m[ 12 ] * m[ 3 ] * m[ 5 ];

	inv[ 14 ] = -m[ 0 ] * m[ 5 ] * m[ 14 ] +
		m[ 0 ] * m[ 6 ] * m[ 13 ] +
		m[ 4 ] * m[ 1 ] * m[ 14 ] -
		m[ 4 ] * m[ 2 ] * m[ 13 ] -
		m[ 12 ] * m[ 1 ] * m[ 6 ] +
		m[ 12 ] * m[ 2 ] * m[ 5 ];

	inv[ 3 ] = -m[ 1 ] * m[ 6 ] * m[ 11 ] +
		m[ 1 ] * m[ 7 ] * m[ 10 ] +
		m[ 5 ] * m[ 2 ] * m[ 11 ] -
		m[ 5 ] * m[ 3 ] * m[ 10 ] -
		m[ 9 ] * m[ 2 ] * m[ 7 ] +
		m[ 9 ] * m[ 3 ] * m[ 6 ];

	inv[ 7 ] = m[ 0 ] * m[ 6 ] * m[ 11 ] -
		m[ 0 ] * m[ 7 ] * m[ 10 ] -
		m[ 4 ] * m[ 2 ] * m[ 11 ] +
		m[ 4 ] * m[ 3 ] * m[ 10 ] +
		m[ 8 ] * m[ 2 ] * m[ 7 ] -
		m[ 8 ] * m[ 3 ] * m[ 6 ];

	inv[ 11 ] = -m[ 0 ] * m[ 5 ] * m[ 11 ] +
		m[ 0 ] * m[ 7 ] * m[ 9 ] +
		m[ 4 ] * m[ 1 ] * m[ 11 ] -
		m[ 4 ] * m[ 3 ] * m[ 9 ] -
		m[ 8 ] * m[ 1 ] * m[ 7 ] +
		m[ 8 ] * m[ 3 ] * m[ 5 ];

	inv[ 15 ] = m[ 0 ] * m[ 5 ] * m[ 10 ] -
		m[ 0 ] * m[ 6 ] * m[ 9 ] -
		m[ 4 ] * m[ 1 ] * m[ 10 ] +
		m[ 4 ] * m[ 2 ] * m[ 9 ] +
		m[ 8 ] * m[ 1 ] * m[ 6 ] -
		m[ 8 ] * m[ 2 ] * m[ 5 ];

	det = m[ 0 ] * inv[ 0 ] + m[ 1 ] * inv[ 4 ] + m[ 2 ] * inv[ 8 ] + m[ 3 ] * inv[ 12 ];

	if ( det == 0.f ) return;

	float oneOverDet = 1.f / det;

	for ( i = 0; i < 16; ++ i )
	{
		m_matrix[ i ] = inv[ i ] * oneOverDet;
	}
}


//---------------------------------------------------------------------------
float Matrix4::GetDeterminant()
{
	return
		m_matrix[ 0 ] * m_matrix[ 5 ] * m_matrix[ 10 ] * m_matrix[ 15 ] +
		m_matrix[ 0 ] * m_matrix[ 6 ] * m_matrix[ 11 ] * m_matrix[ 13 ] +
		m_matrix[ 0 ] * m_matrix[ 7 ] * m_matrix[ 9 ] * m_matrix[ 14 ] +
		m_matrix[ 1 ] * m_matrix[ 4 ] * m_matrix[ 11 ] * m_matrix[ 14 ] +
		m_matrix[ 1 ] * m_matrix[ 6 ] * m_matrix[ 8 ] * m_matrix[ 15 ] +
		m_matrix[ 1 ] * m_matrix[ 7 ] * m_matrix[ 10 ] * m_matrix[ 12 ] +
		m_matrix[ 2 ] * m_matrix[ 4 ] * m_matrix[ 9 ] * m_matrix[ 15 ] +
		m_matrix[ 2 ] * m_matrix[ 5 ] * m_matrix[ 11 ] * m_matrix[ 12 ] +
		m_matrix[ 2 ] * m_matrix[ 7 ] * m_matrix[ 8 ] * m_matrix[ 13 ] +
		m_matrix[ 3 ] * m_matrix[ 4 ] * m_matrix[ 10 ] * m_matrix[ 13 ] +
		m_matrix[ 3 ] * m_matrix[ 5 ] * m_matrix[ 8 ] * m_matrix[ 14 ] +
		m_matrix[ 3 ] * m_matrix[ 6 ] * m_matrix[ 9 ] * m_matrix[ 12 ] -
		m_matrix[ 0 ] * m_matrix[ 5 ] * m_matrix[ 11 ] * m_matrix[ 14 ] -
		m_matrix[ 0 ] * m_matrix[ 6 ] * m_matrix[ 9 ] * m_matrix[ 15 ] -
		m_matrix[ 0 ] * m_matrix[ 7 ] * m_matrix[ 10 ] * m_matrix[ 13 ] -
		m_matrix[ 1 ] * m_matrix[ 4 ] * m_matrix[ 10 ] * m_matrix[ 15 ] -
		m_matrix[ 1 ] * m_matrix[ 6 ] * m_matrix[ 11 ] * m_matrix[ 12 ] -
		m_matrix[ 1 ] * m_matrix[ 7 ] * m_matrix[ 8 ] * m_matrix[ 14 ] -
		m_matrix[ 2 ] * m_matrix[ 4 ] * m_matrix[ 11 ] * m_matrix[ 13 ] -
		m_matrix[ 2 ] * m_matrix[ 5 ] * m_matrix[ 8 ] * m_matrix[ 15 ] -
		m_matrix[ 2 ] * m_matrix[ 7 ] * m_matrix[ 9 ] * m_matrix[ 12 ] -
		m_matrix[ 3 ] * m_matrix[ 4 ] * m_matrix[ 9 ] * m_matrix[ 14 ] -
		m_matrix[ 3 ] * m_matrix[ 5 ] * m_matrix[ 10 ] * m_matrix[ 12 ] -
		m_matrix[ 3 ] * m_matrix[ 6 ] * m_matrix[ 8 ] * m_matrix[ 13 ];
}


//---------------------------------------------------------------------------
const Matrix4 Matrix4::GetTransformedMatrix( const Matrix4& newTransform ) const
{
	Matrix4 result;
	Matrix4& r = result;
	const Matrix4& n = newTransform;

	r.m_matrix[ 0 ] = n.m_matrix[ 0 ] * r.m_matrix[ 0 ] + n.m_matrix[ 1 ] * r.m_matrix[ 4 ] + n.m_matrix[ 2 ] * r.m_matrix[ 8 ] + n.m_matrix[ 3 ] * r.m_matrix[ 12 ];
	r.m_matrix[ 1 ] = n.m_matrix[ 0 ] * r.m_matrix[ 1 ] + n.m_matrix[ 1 ] * r.m_matrix[ 5 ] + n.m_matrix[ 2 ] * r.m_matrix[ 9 ] + n.m_matrix[ 3 ] * r.m_matrix[ 13 ];
	r.m_matrix[ 2 ] = n.m_matrix[ 0 ] * r.m_matrix[ 2 ] + n.m_matrix[ 1 ] * r.m_matrix[ 6 ] + n.m_matrix[ 2 ] * r.m_matrix[ 10 ] + n.m_matrix[ 3 ] * r.m_matrix[ 14 ];
	r.m_matrix[ 3 ] = n.m_matrix[ 0 ] * r.m_matrix[ 3 ] + n.m_matrix[ 1 ] * r.m_matrix[ 7 ] + n.m_matrix[ 2 ] * r.m_matrix[ 11 ] + n.m_matrix[ 3 ] * r.m_matrix[ 15 ];

	r.m_matrix[ 4 ] = n.m_matrix[ 4 ] * r.m_matrix[ 0 ] + n.m_matrix[ 5 ] * r.m_matrix[ 4 ] + n.m_matrix[ 6 ] * r.m_matrix[ 8 ] + n.m_matrix[ 7 ] * r.m_matrix[ 12 ];
	r.m_matrix[ 5 ] = n.m_matrix[ 4 ] * r.m_matrix[ 1 ] + n.m_matrix[ 5 ] * r.m_matrix[ 5 ] + n.m_matrix[ 6 ] * r.m_matrix[ 9 ] + n.m_matrix[ 7 ] * r.m_matrix[ 13 ];
	r.m_matrix[ 6 ] = n.m_matrix[ 4 ] * r.m_matrix[ 2 ] + n.m_matrix[ 5 ] * r.m_matrix[ 6 ] + n.m_matrix[ 6 ] * r.m_matrix[ 10 ] + n.m_matrix[ 7 ] * r.m_matrix[ 14 ];
	r.m_matrix[ 7 ] = n.m_matrix[ 4 ] * r.m_matrix[ 3 ] + n.m_matrix[ 5 ] * r.m_matrix[ 7 ] + n.m_matrix[ 6 ] * r.m_matrix[ 11 ] + n.m_matrix[ 7 ] * r.m_matrix[ 15 ];

	r.m_matrix[ 8 ] = n.m_matrix[ 8 ] * r.m_matrix[ 0 ] + n.m_matrix[ 9 ] * r.m_matrix[ 4 ] + n.m_matrix[ 10 ] * r.m_matrix[ 8 ] + n.m_matrix[ 11 ] * r.m_matrix[ 12 ];
	r.m_matrix[ 9 ] = n.m_matrix[ 8 ] * r.m_matrix[ 1 ] + n.m_matrix[ 9 ] * r.m_matrix[ 5 ] + n.m_matrix[ 10 ] * r.m_matrix[ 9 ] + n.m_matrix[ 11 ] * r.m_matrix[ 13 ];
	r.m_matrix[ 10 ] = n.m_matrix[ 8 ] * r.m_matrix[ 2 ] + n.m_matrix[ 9 ] * r.m_matrix[ 6 ] + n.m_matrix[ 10 ] * r.m_matrix[ 10 ] + n.m_matrix[ 11 ] * r.m_matrix[ 14 ];
	r.m_matrix[ 11 ] = n.m_matrix[ 8 ] * r.m_matrix[ 3 ] + n.m_matrix[ 9 ] * r.m_matrix[ 7 ] + n.m_matrix[ 10 ] * r.m_matrix[ 11 ] + n.m_matrix[ 11 ] * r.m_matrix[ 15 ];

	r.m_matrix[ 12 ] = n.m_matrix[ 12 ] * r.m_matrix[ 0 ] + n.m_matrix[ 13 ] * r.m_matrix[ 4 ] + n.m_matrix[ 14 ] * r.m_matrix[ 8 ] + n.m_matrix[ 15 ] * r.m_matrix[ 12 ];
	r.m_matrix[ 13 ] = n.m_matrix[ 12 ] * r.m_matrix[ 1 ] + n.m_matrix[ 13 ] * r.m_matrix[ 5 ] + n.m_matrix[ 14 ] * r.m_matrix[ 9 ] + n.m_matrix[ 15 ] * r.m_matrix[ 13 ];
	r.m_matrix[ 14 ] = n.m_matrix[ 12 ] * r.m_matrix[ 2 ] + n.m_matrix[ 13 ] * r.m_matrix[ 6 ] + n.m_matrix[ 14 ] * r.m_matrix[ 10 ] + n.m_matrix[ 15 ] * r.m_matrix[ 14 ];
	r.m_matrix[ 15 ] = n.m_matrix[ 12 ] * r.m_matrix[ 3 ] + n.m_matrix[ 13 ] * r.m_matrix[ 7 ] + n.m_matrix[ 14 ] * r.m_matrix[ 11 ] + n.m_matrix[ 15 ] * r.m_matrix[ 15 ];

	return result;
}