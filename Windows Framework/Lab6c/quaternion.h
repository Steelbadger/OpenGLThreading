#pragma once
///////////////////////// By Ross Davies /////////////////////////
//					Quaternion Mathematics class				//
//	This class provides all basic quaternion functionality		//
//  necessary for quaternion rotation operations and can		//
//  interface with the Vector and Matrix classes				//
//////////////////////////////////////////////////////////////////

class Vector4;
class Vector3;
class Matrix4x4;


class Quaternion
{
public:
	//  Constructors
	Quaternion(void);
	//  Set individual values
	Quaternion(float s, float x, float y, float z);
	//  Set using values from a Vector4
	Quaternion(const Vector4 & rhs);
	//  Set using values from a Vector3
	Quaternion(const Vector3 & rhs);
	//  Set using a rotation axis vector and rotation value
	Quaternion(float s, Vector3 rhs);
	Quaternion(float s, Vector4 rhs);

	//  Destructor
	~Quaternion(void);

	//  Rotate by angle around direction vector
	void Rotation(float angle, float xComp, float yComp, float zComp);
	void Rotation(float angle, Vector3 axis);

	//  Create a Quaternion to align v1 to v2
	void Align(Vector3 v1, Vector3 v2);


	//  The modulus of the Quat
	float Modulus();
	//  Square of the modulus, use when actual modulus is uneccessary,
	//  no square root (less costly)
	float ModulusSqr();

	//  Return the Conjugate of the Quat (needed for Inverse)
	Quaternion Conjugate();
	//  Return the Inverse of the Quat (needed for Quat manipulation of vectors)
	Quaternion Inverse();

	//  return a normalized Quat with unit modulus
	Quaternion Normalize();
	//  Make the quat change to unit modulus quat
	void NormalizeSelf();
	//  Convert the vector part to a unit vector
	void NormalizeVector();

	//  Find the rotation angle
	float GetAngle();
	//  Find the axis of rotation
	Vector3 GetAxis();
	//  Convert the Quat to a rotation matrix for use in graphics pipeline
	Matrix4x4 GetRotationMatrix();

	float s, x, y, z;
};

//  Standard operators
inline Quaternion operator + (const Quaternion &q1,
						   const Quaternion &q2)
{
	return Quaternion(q1.s + q2.s, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z);
}

inline Quaternion operator - (const Quaternion &q1,
						   const Quaternion &q2)
{
	return Quaternion(q1.s - q2.s, q1.x - q2.x, q1.y - q2.y, q1.z - q2.z);
}

inline Quaternion operator * (const Quaternion &q1,
						   const Quaternion &q2)
{
	return Quaternion	( q1.s*q2.s+q1.x*-q2.x+q1.y*-q2.y+q1.z*-q2.z
						, q1.s*q2.x+q1.x*q2.s+q1.y*q2.z+q1.z*-q2.y
						, q1.s*q2.y+q1.x*-q2.z+q1.y*q2.s+q1.z*q2.x
						, q1.s*q2.z+q1.x*q2.y+q1.y*-q2.x+q1.z*q2.s);
}
inline Quaternion operator * (const Quaternion &q,
						   const float &c)
{
	return Quaternion(q.s*c, q.x*c, q.y*c, q.z*c);
}

inline Quaternion operator * (const float &c,
						   const Quaternion &q)
{
	return Quaternion(q.s*c, q.x*c, q.y*c, q.z*c);
}

inline Quaternion operator / (const Quaternion &q,
						   const float & c)
{
	return Quaternion(q.s/c, q.x/c, q.y/c, q.z/c);
}