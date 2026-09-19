#pragma once
#include <Core/Core.hpp>


template<typename type>
class List
{
public:
	type get( std::uint32_t idx )
	{
		const auto internal_list = reinterpret_cast< std::uintptr_t >( this + 0x20 );
		return *reinterpret_cast< type* >( internal_list + idx * sizeof( type ) );
	}

	type value( std::uint32_t idx )
	{
		const auto list = *reinterpret_cast< std::uintptr_t* >( this + 0x10 );
		const auto internal_list = list + 0x20;
		return *reinterpret_cast< type* >( internal_list + idx * sizeof( type ) );
	}

	auto size( ) -> const std::uint32_t { return *reinterpret_cast< std::uint32_t* >( this + 0x18 ); }
};


class ListDictionary
{
public:
	template <typename type>
	auto value( ) -> type
	{
		auto _list = *reinterpret_cast< uintptr_t* >( this + 0x10 );
		if ( !_list )
			return { };

		auto value = *reinterpret_cast< type* >( _list + 0x28 );
		if ( !value )
			return {};

		return value;
	}

	auto size( ) -> int
	{
		auto val = value< std::uintptr_t >( );
		if ( !val )
			return {};

		auto size = *reinterpret_cast< int* >( val + 0x10 );
		if ( !size )
			return {};

		return size;
	}

	template <typename type>
	auto buffer( ) -> type
	{
		auto val = value< std::uintptr_t >( );
		return *reinterpret_cast< std::uintptr_t* >( val + 0x18 );
	}
};


/*
class ListDictionary
{
public:
	template <typename type>
	auto value( ) -> type
	{
		auto list = *reinterpret_cast< std::uintptr_t* >( this + 0x10 );
		if ( !list )
			return {};

		auto value = *reinterpret_cast< type* >( list + 0x28 );
		if ( !value )
			return {};

		return value;
	}

	auto size( ) -> int
	{
		auto val = value< std::uintptr_t >( );
		if ( !val )
			return {};

		auto size = *reinterpret_cast< int* >( val + 0x10 );
		if ( !size )
			return {};

		return size;
	}

	template <typename type>
	auto buffer( ) -> type
	{
		auto val = value< std::uintptr_t >( );
		return *reinterpret_cast< std::uintptr_t* >( val + 0x18 );
	}

};

*/

class String
{
public:
	CLASS( "System", "String" );
public:
	char zpad[ 0x10 ] { };
	int size { };
	wchar_t buffer[ 128 + 1 ];
public:
	String( const wchar_t* st )
	{
		size = min( Crt::wcslen( st ), 128 );
		for ( int idx = 0; idx < size; idx++ )
		{
			buffer[ idx ] = st[ idx ];
		}
		buffer[ size ] = 0;
	}

	auto Contains( String value ) -> bool
	{
		METHOD( bool( ** )( String*, String ), Contains );
		return Contains( this, value );
	}
};




class Vector3
{
private:
	CLASS( "UnityEngine", "Vector3" );
public:
	float x;
	float y;
	float z;
#define _powf(n) (n)*(n)
	Vector3( )
	{}

	Vector3( float x, float y, float z )
		: x( x ), y( y ), z( z )
	{}

	Vector3 operator+( const Vector3& vector ) const
	{
		return Vector3( x + vector.x, y + vector.y, z + vector.z );
	}

	Vector3 operator-( const Vector3& vector ) const
	{
		return Vector3( x - vector.x, y - vector.y, z - vector.z );
	}

	Vector3 operator * ( float number ) const
	{
		return Vector3( x * number, y * number, z * number );
	}

	Vector3 operator/( float number ) const
	{
		return Vector3( x / number, y / number, z / number );
	}

	Vector3& operator/=( float number )
	{
		x /= number;
		y /= number;
		z /= number;

		return *this;
	}

	static float sqrtf( float number )
	{
		long        i;
		float       x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y = number;
		i = *( long* ) &y; // floating point bit level hacking [sic]
		i = 0x5f3759df - ( i >> 1 ); // Newton's approximation
		y = *( float* ) &i;
		y = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
		y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration

		return 1 / y;
	}

	auto magnitude( ) -> float
	{
		METHOD( float( ** )( Vector3* ), get_magnitude );
		return get_magnitude( this );
	}

	auto empty( ) -> bool { return x == 0 && y == 0 && z == 0; }

	auto length( ) -> float { return sqrtf( ( x * x ) + ( y * y ) + ( z * z ) ); }

	Vector3 normalized( )
	{
		float len = length( );
		return Vector3( x / len, y / len, z / len );
	}

	auto distance_3d( const Vector3& vector ) -> float { return ( *this - vector ).length( ); }
	auto dot( const Vector3& vector ) -> float { return x * vector.x + y * vector.y + z * vector.z; }
};

class Vector2
{
public:
	float x;
	float y;

	Vector2( )
	{}

	Vector2( float x, float y )
		: x( x ), y( y )
	{}

	float distance_2d( Vector2 input ) {
		return sqrt( powf( x - input.x, 2 ) + powf( y - input.y, 2 ) );
	}

	bool empty( ) const {
		return x == 0.f && y == 0.f;
	}
};


class Matrix4x4
{
public:
	Matrix4x4( )
		: m { { 0, 0, 0, 0 },
			 { 0, 0, 0, 0 },
			 { 0, 0, 0, 0 },
			 { 0, 0, 0, 0 } }
	{}

	Matrix4x4( const Matrix4x4& ) = default;

	auto transpose( ) -> Matrix4x4 {
		Matrix4x4 m;

		for ( int i = 0; i < 4; i++ )
			for ( int j = 0; j < 4; j++ )
				m.m[ i ][ j ] = this->m[ j ][ i ];

		return m;
	}

	auto is_empty( ) -> bool {
		if ( !m[ 3 ][ 0 ] && !m[ 3 ][ 1 ] && !m[ 3 ][ 2 ] && !m[ 2 ][ 1 ] && !m[ 2 ][ 0 ] && !m[ 2 ][ 2 ] )
			return true;

		return false;
	}

	Vector3 operator*( const Vector3& vec ) {
		Matrix4x4 m;

		m[ 3 ][ 0 ] = vec.x;
		m[ 3 ][ 1 ] = vec.y;
		m[ 3 ][ 2 ] = vec.z;

		m[ 0 ][ 0 ] = 1;
		m[ 1 ][ 1 ] = 1;
		m[ 2 ][ 2 ] = 1;


		m[ 0 ][ 3 ] = 0.0f;
		m[ 1 ][ 3 ] = 0.0f;
		m[ 2 ][ 3 ] = 0.0f;
		m[ 3 ][ 3 ] = 1.0f;

		auto result = m * ( *this );

		return Vector3 { result[ 3 ][ 0 ], result[ 3 ][ 1 ], result[ 3 ][ 2 ] };
	}

	Matrix4x4 operator*( const Matrix4x4& _m2 ) {
		auto _m = *this;

		Matrix4x4 out;
		out[ 0 ][ 0 ] = _m[ 0 ][ 0 ] * _m2[ 0 ][ 0 ] + _m[ 0 ][ 1 ] * _m2[ 1 ][ 0 ] + _m[ 0 ][ 2 ] * _m2[ 2 ][ 0 ] + _m[ 0 ][ 3 ] * _m2[ 3 ][ 0 ];
		out[ 0 ][ 1 ] = _m[ 0 ][ 0 ] * _m2[ 0 ][ 1 ] + _m[ 0 ][ 1 ] * _m2[ 1 ][ 1 ] + _m[ 0 ][ 2 ] * _m2[ 2 ][ 1 ] + _m[ 0 ][ 3 ] * _m2[ 3 ][ 1 ];
		out[ 0 ][ 2 ] = _m[ 0 ][ 0 ] * _m2[ 0 ][ 2 ] + _m[ 0 ][ 1 ] * _m2[ 1 ][ 2 ] + _m[ 0 ][ 2 ] * _m2[ 2 ][ 2 ] + _m[ 0 ][ 3 ] * _m2[ 3 ][ 2 ];
		out[ 0 ][ 3 ] = _m[ 0 ][ 0 ] * _m2[ 0 ][ 3 ] + _m[ 0 ][ 1 ] * _m2[ 1 ][ 3 ] + _m[ 0 ][ 2 ] * _m2[ 2 ][ 3 ] + _m[ 0 ][ 3 ] * _m2[ 3 ][ 3 ];
		out[ 1 ][ 0 ] = _m[ 1 ][ 0 ] * _m2[ 0 ][ 0 ] + _m[ 1 ][ 1 ] * _m2[ 1 ][ 0 ] + _m[ 1 ][ 2 ] * _m2[ 2 ][ 0 ] + _m[ 1 ][ 3 ] * _m2[ 3 ][ 0 ];
		out[ 1 ][ 1 ] = _m[ 1 ][ 0 ] * _m2[ 0 ][ 1 ] + _m[ 1 ][ 1 ] * _m2[ 1 ][ 1 ] + _m[ 1 ][ 2 ] * _m2[ 2 ][ 1 ] + _m[ 1 ][ 3 ] * _m2[ 3 ][ 1 ];
		out[ 1 ][ 2 ] = _m[ 1 ][ 0 ] * _m2[ 0 ][ 2 ] + _m[ 1 ][ 1 ] * _m2[ 1 ][ 2 ] + _m[ 1 ][ 2 ] * _m2[ 2 ][ 2 ] + _m[ 1 ][ 3 ] * _m2[ 3 ][ 2 ];
		out[ 1 ][ 3 ] = _m[ 1 ][ 0 ] * _m2[ 0 ][ 3 ] + _m[ 1 ][ 1 ] * _m2[ 1 ][ 3 ] + _m[ 1 ][ 2 ] * _m2[ 2 ][ 3 ] + _m[ 1 ][ 3 ] * _m2[ 3 ][ 3 ];
		out[ 2 ][ 0 ] = _m[ 2 ][ 0 ] * _m2[ 0 ][ 0 ] + _m[ 2 ][ 1 ] * _m2[ 1 ][ 0 ] + _m[ 2 ][ 2 ] * _m2[ 2 ][ 0 ] + _m[ 2 ][ 3 ] * _m2[ 3 ][ 0 ];
		out[ 2 ][ 1 ] = _m[ 2 ][ 0 ] * _m2[ 0 ][ 1 ] + _m[ 2 ][ 1 ] * _m2[ 1 ][ 1 ] + _m[ 2 ][ 2 ] * _m2[ 2 ][ 1 ] + _m[ 2 ][ 3 ] * _m2[ 3 ][ 1 ];
		out[ 2 ][ 2 ] = _m[ 2 ][ 0 ] * _m2[ 0 ][ 2 ] + _m[ 2 ][ 1 ] * _m2[ 1 ][ 2 ] + _m[ 2 ][ 2 ] * _m2[ 2 ][ 2 ] + _m[ 2 ][ 3 ] * _m2[ 3 ][ 2 ];
		out[ 2 ][ 3 ] = _m[ 2 ][ 0 ] * _m2[ 0 ][ 3 ] + _m[ 2 ][ 1 ] * _m2[ 1 ][ 3 ] + _m[ 2 ][ 2 ] * _m2[ 2 ][ 3 ] + _m[ 2 ][ 3 ] * _m2[ 3 ][ 3 ];
		out[ 3 ][ 0 ] = _m[ 3 ][ 0 ] * _m2[ 0 ][ 0 ] + _m[ 3 ][ 1 ] * _m2[ 1 ][ 0 ] + _m[ 3 ][ 2 ] * _m2[ 2 ][ 0 ] + _m[ 3 ][ 3 ] * _m2[ 3 ][ 0 ];
		out[ 3 ][ 1 ] = _m[ 3 ][ 0 ] * _m2[ 0 ][ 1 ] + _m[ 3 ][ 1 ] * _m2[ 1 ][ 1 ] + _m[ 3 ][ 2 ] * _m2[ 2 ][ 1 ] + _m[ 3 ][ 3 ] * _m2[ 3 ][ 1 ];
		out[ 3 ][ 2 ] = _m[ 3 ][ 0 ] * _m2[ 0 ][ 2 ] + _m[ 3 ][ 1 ] * _m2[ 1 ][ 2 ] + _m[ 3 ][ 2 ] * _m2[ 2 ][ 2 ] + _m[ 3 ][ 3 ] * _m2[ 3 ][ 2 ];
		out[ 3 ][ 3 ] = _m[ 3 ][ 0 ] * _m2[ 0 ][ 3 ] + _m[ 3 ][ 1 ] * _m2[ 1 ][ 3 ] + _m[ 3 ][ 2 ] * _m2[ 2 ][ 3 ] + _m[ 3 ][ 3 ] * _m2[ 3 ][ 3 ];

		return out;
	}

	float* operator[]( size_t i ) { return m[ i ]; }
	const float* operator[]( size_t i ) const { return m[ i ]; }

	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[ 4 ][ 4 ];
	};
};

class Rect
{
public:
	float x, y, w, h;
	Rect( float _x, float _y, float _w, float _h ) : x( _x ), y( _y ), w( _w ), h( _h ) { }
	Rect( ) : x( 0 ), y( 0 ), w( 0 ), h( 0 ) { }

	auto contains( Vector2 point ) -> bool
	{
		return point.x >= x && point.x < ( x + w ) && point.y >= y && point.y < ( y + h );
	}
};

class Color
{
public:
	float r, g, b, a;
	Color( float _r, float _g, float _b, float _a ) : r( _r / 255 ), g( _g / 255 ), b( _b / 255 ), a( _a / 255 ) {}
	Color( float _r, float _g, float _b ) : r( _r / 255 ), g( _g / 255 ), b( _b / 255 ), a( 1 ) {}
};

class Transform
{
public:
	CLASS( "UnityEngine", "Transform" );

	auto position( ) -> Vector3
	{
		METHOD( Vector3( ** ) ( Transform* ), get_position );
		return get_position( this );
	}

	auto set_position( Vector3 position ) -> void
	{
		METHOD( void( ** )( Transform*, Vector3 ), set_position );
		set_position( this, position );
	}
};

enum class KeyCode 
{
	Backspace = 8, Delete = 127, Tab = 9, Clear = 12,
	Return = 13, Pause = 19, Escape = 27, Space = 32,
	Keypad0 = 256, Keypad1 = 257, Keypad2 = 258, Keypad3 = 259,
	Keypad4 = 260, Keypad5 = 261, Keypad6 = 262, Keypad7 = 263,
	Keypad8 = 264, Keypad9 = 265, KeypadPeriod = 266, KeypadDivide = 267,
	KeypadMultiply = 268, KeypadMinus = 269, KeypadPlus = 270, KeypadEnter = 271,
	KeypadEquals = 272, UpArrow = 273, DownArrow = 274, RightArrow = 275,
	LeftArrow = 276, Insert = 277, Home = 278, End = 279,
	PageUp = 280, PageDown = 281, F1 = 282, F2 = 283,
	F3 = 284, F4 = 285, F5 = 286, F6 = 287,
	F7 = 288, F8 = 289, F9 = 290, F10 = 291,
	F11 = 292, F12 = 293, F13 = 294, F14 = 295,
	F15 = 296, Alpha0 = 48, Alpha1 = 49, Alpha2 = 50,
	Alpha3 = 51, Alpha4 = 52, Alpha5 = 53, Alpha6 = 54,
	Alpha7 = 55, Alpha8 = 56, Alpha9 = 57, Exclaim = 33,
	DoubleQuote = 34, Hash = 35, Dollar = 36, Percent = 37,
	Ampersand = 38, Quote = 39, LeftParen = 40, RightParen = 41,
	Asterisk = 42, Plus = 43, Comma = 44, Minus = 45,
	Period = 46, Slash = 47, Colon = 58, Semicolon = 59,
	Less = 60, Equals = 61, Greater = 62, Question = 63,
	At = 64, LeftBracket = 91, Backslash = 92, RightBracket = 93,
	Caret = 94, Underscore = 95, BackQuote = 96, A = 97,
	B = 98, C = 99, D = 100, E = 101,
	F = 102, G = 103, H = 104, I = 105,
	J = 106, K = 107, L = 108, M = 109,
	N = 110, O = 111, P = 112, Q = 113,
	R = 114, S = 115, T = 116, U = 117,
	V = 118, W = 119, X = 120, Y = 121,
	Z = 122, LeftCurlyBracket = 123, Pipe = 124, RightCurlyBracket = 125, Tilde = 126,
	Numlock = 300, CapsLock = 301, ScrollLock = 302, RightShift = 303,
	LeftShift = 304, RightControl = 305, LeftControl = 306, RightAlt = 307,
	LeftAlt = 308, LeftCommand = 310, LeftApple = 310, LeftWindows = 311,
	RightCommand = 309, RightApple = 309, RightWindows = 312, AltGr = 313,
	Help = 315, Pr = 316, SysReq = 317, Break = 318,
	Menu = 319, Mouse0 = 323, Mouse1 = 324, Mouse2 = 325,
	Mouse3 = 326, Mouse4 = 327, Mouse5 = 328, Mouse6 = 329
};

class Input
{
public:
	CLASS( "UnityEngine", "Input" );


	static auto GetKey( KeyCode key ) -> bool
	{
		METHOD( bool( ** )( KeyCode ), GetKey );
		return GetKey( key );
	}

	static auto GetMouseButton( ) -> bool
	{
		METHOD( bool( ** )( ), GetMouseButton );
		return GetMouseButton( );
	}

	static auto mousePosition( ) -> Vector3
	{
		METHOD( Vector3( ** )( ), get_mousePosition );
		return get_mousePosition( );
	}
};

class Object : public Il2CppObject
{
public:
	CLASS( "UnityEngine", "Object" );

	static auto DontDestroyOnLoad( Object* object ) -> void
	{
		METHOD( void( ** )( Object* ), DontDestroyOnLoad );
		return DontDestroyOnLoad( object );
	}

	auto name( ) -> String*
	{
		METHOD( String*( ** )( Object* ), get_name );
		return get_name( this );
	}

	auto IsA( Il2CppClass* object ) -> bool
	{
		if ( !this )
			return false;

		if ( ( Il2CppClass* ) this == object )
			return true;

		return false;
	}
};

class GameObject : public Object
{
public:
	CLASS( "UnityEngine", "GameObject" );

	static auto Internal_CreateGameObject( GameObject* self, String name ) -> void
	{
		METHOD( void( ** )( GameObject*, String ), Internal_CreateGameObject );
		return Internal_CreateGameObject( self, name );
	}

	auto AddComponent( void* componentType ) -> class Component*
	{
		METHOD( Component * ( ** )( GameObject*, void* ), AddComponent );
		return AddComponent( this, componentType );
	}
};

class Component : public Object
{
public:
	CLASS( "UnityEngine", "Component" );

	auto ClassHash( ) -> uint64_t
	{
		auto name = *reinterpret_cast< const char** >( this + 0x10 );
		if ( !name )
			return 0;

		return HASH( name );
	}

	auto transform( ) -> Transform*
	{
		METHOD( Transform * ( ** )( Component* ), get_transform );
		return get_transform( this );
	}

	auto gameObject( ) -> GameObject*
	{
		METHOD( GameObject * ( ** )( Component* ), get_gameObject );
		return get_gameObject( this );
	}

	auto GetComponentsInChildren( void* type ) -> Component*
	{
		METHOD( Component * ( ** )( Component*, void* ), GetComponentsInChildren );
		return GetComponentsInChildren( this, type );
	}
};
;

class Shader : public Object
{
public:
	CLASS( "UnityEngine", "Shader" );
};

class Material : public Object
{
public:
	CLASS( "UnityEngine", "Material" );

	auto set_shader( Shader* shader ) -> void
	{
		METHOD( void( ** )( Material*, Shader* ), set_shader );
		return set_shader( this, shader );
	}

	auto SetFloat( String name, float value ) -> void
	{
		METHOD( void( ** )( Material*, String, float ), SetFloat );
		return SetFloat( this, name, value );
	}

	auto SetInt( String name, int value ) -> void
	{
		METHOD( void( ** )( Material*, String, int ), SetInt );
		return SetInt( this, name, value );
	}

	auto SetColor( String name, Color value ) -> void
	{
		METHOD( void( ** )( Material*, String, Color ), SetColor );
		return SetColor( this, name, value );
	}
};


class Renderer
{
public:
	CLASS( "UnityEngine", "Renderer" );

	auto material( ) -> Material*
	{
		METHOD( Material * ( ** )( Renderer* ), get_material );
		return get_material( this );
	}
};

class DDraw
{
public:
	CLASS( "UnityEngine", "DDraw" );

	static auto Get( ) -> DDraw*
	{
		METHOD( DDraw * ( ** )( ), Get );
		return Get( );
	}

	static auto Sphere( Vector3 vPos, float fRadius, Color color, float fDuration, bool distanceFade ) -> void
	{
		METHOD( void( ** )( Vector3, float, Color, float, bool ), Sphere );
		return Sphere( vPos, fRadius, color, fDuration, distanceFade );
	}

	static auto Text( String text, Vector3 vPos, Color color, float fDuration ) -> void
	{
		METHOD( void( ** )( String, Vector3, Color, float ), Text );
		return Text( text, vPos, color, fDuration );
	}


	static auto Line( Vector3 vPos, Vector3 vPosB, Color color, float fDuration, bool distanceFade, bool ztest  ) -> void
	{
		METHOD( void( ** )( Vector3, Vector3, Color, float, bool, bool ), Line );
		return Line( vPos, vPosB, color, fDuration, distanceFade, ztest );
	}

	static auto Arrow( Vector3 vPos, Vector3 vPosB, float headSize, Color color, float fDuration ) -> void
	{
		METHOD( void( ** )( Vector3, Vector3, float, Color, float ), Arrow );
		return Arrow( vPos, vPosB, headSize, color, fDuration );
	}
};

class GL
{
public:
	CLASS( "UnityEngine", "GL" );
};


class Texture
{
public:

};

class Font
{

};

class Texture2D : public Texture
{
public:
	CLASS( "UnityEngine", "Texture2D" );

	static auto whiteTexture( ) -> Texture2D*
	{
		METHOD( Texture2D * ( ** )( ), get_whiteTexture );
		return get_whiteTexture( );
	}
};

class GUIContent
{
public:
	CLASS( "UnityEngine", "GUIContent" );

	static auto Temp( String* text ) -> GUIContent*
	{
		METHOD( GUIContent * ( ** )( String* ), Temp );
		return Temp( text );
	}
};

enum class TextAnchor : int
{
	UpperLeft = 0,
	MiddleCenter = 4
};

class GUIStyle
{
public:
	CLASS( "UnityEngine", "GUIStyle" );

	auto set_font( Font* font ) -> void
	{
		METHOD( void( ** )( GUIStyle*, Font* ), set_font );
		return set_font( this, font );
	}

	auto set_fontSize( int size ) -> void
	{
		METHOD( void( ** )( GUIStyle*, int ), set_fontSize );
		return set_fontSize( this, size );
	}

	auto set_alignment( TextAnchor alignment ) -> void
	{
		METHOD( void( ** )( GUIStyle*, TextAnchor ), set_alignment );
		return set_alignment( this, alignment );
	}

	auto CalcSize( GUIContent* content ) -> Vector2
	{
		METHOD( Vector2( ** )( GUIStyle*, GUIContent* ), CalcSize );
		return CalcSize( this, content );
	}
};

class GUISkin
{
public:
	CLASS( "UnityEngine", "GUISkin" );

	auto label( ) -> GUIStyle*
	{
		METHOD( GUIStyle * ( ** )( GUISkin* ), get_label );
		return get_label( this );
	}
};

class GUI
{
public:
	CLASS( "UnityEngine", "GUI" );


	static auto skin( ) -> GUISkin*
	{
		METHOD( GUISkin * ( ** )( ), get_skin );
		return get_skin( );
	}

	static auto set_color( Color color ) -> void
	{
		METHOD( void ( ** )( Color ), set_color );
		return set_color( color );
	}

	static auto Label( Rect position, String text, GUIStyle* style ) -> void
	{
		METHOD( void( ** )( Rect, String, GUIStyle* ), Label );
		return Label( position, text, style );
	}

	static auto Label( Rect position, GUIContent* content, GUIStyle* style ) -> void
	{
		static auto Label = *reinterpret_cast< void( ** )( Rect, GUIContent*, GUIStyle* ) >( StaticClass( )->GetMethodFromName( "Label", 3, "content", 2 ) );
		return Label( position, content, style );
	}

	static auto Label( Rect position, String text ) -> void
	{
		METHOD( void( ** )( Rect, String ), Label );
		return Label( position, text );
	}

	static auto DrawTexture( Rect position, Texture* texture ) -> void
	{
		METHOD( void( ** )( Rect, Texture* ), DrawTexture );
		return DrawTexture( position, texture );
	}
};

class Sprite
{
public:
	CLASS( "UnityEngine", "Sprite" );

	auto texture( ) -> Texture2D*
	{
		METHOD( Texture2D * ( ** )( Sprite* ), get_texture );
		return get_texture( this );
	}
};

class Bounds
{
public:
	CLASS( "UnityEngine", "Bounds" );

	auto center( ) -> Vector3
	{
		METHOD( Vector3( ** )( Bounds* ), get_center );
		return get_center( this );
	}

	auto size( ) -> Vector3
	{
		METHOD( Vector3( ** )( Bounds* ), get_size );
		return get_size( this );
	}

	auto extents( ) -> Vector3
	{
		METHOD( Vector3( ** )( Bounds* ), get_extents );
		return get_extents( this );
	}

	auto get_min( ) -> Vector3
	{
		METHOD( Vector3( ** )( Bounds* ), get_min );
		return get_min( this );
	}

	auto get_max( ) -> Vector3
	{
		METHOD( Vector3( ** )( Bounds* ), get_max );
		return get_max( this );
	}
};

enum class EventType : int
{
	MouseDown = 0,
	MouseUp = 1,
	MouseDrag = 3,
	KeyDown = 4,
	KeyUp = 5,
	Repaint = 7
};

class Event
{
public:
	CLASS( "UnityEngine", "Event" );

	static auto current( ) -> Event*
	{
		METHOD( Event * ( ** )( ), get_current );
		return get_current( );
	}

	auto type( ) -> EventType
	{
		METHOD( EventType( ** )( Event* ), get_type );
		return get_type( this );
	}

	auto keyCode( ) -> KeyCode
	{
		METHOD( KeyCode( ** )( Event* ), get_keyCode );
		return get_keyCode( this );
	}
};

class Animator
{
public:
	CLASS( "UnityEngine", "Animator" );

	auto speed( ) -> float
	{
		METHOD( float( ** )( Animator* ), get_speed );
		return get_speed( this );
	}

	auto set_speed( float value ) -> void
	{
		METHOD( void( ** )( Animator*, float ), set_speed );
		return set_speed( this, value );
	}
};

class Time
{
public:
	CLASS( "UnityEngine", "Time" );

	static auto time( ) -> float
	{
		METHOD( float( ** )( ), get_time );
		return get_time( );
	}

	static auto realtimeSinceStartup( ) -> float
	{
		METHOD( float( ** )( ), get_realtimeSinceStartup );
		return get_realtimeSinceStartup( );
	}
};

class AssetBundle : public Object
{
public:
	CLASS( "UnityEngine", "AssetBundle" );

	static auto LoadFromFile( String path, unsigned int crc, unsigned long offset ) -> AssetBundle*
	{
		METHOD( AssetBundle * ( ** )( String, unsigned int, unsigned long ), LoadFromFile_Internal );
		return LoadFromFile_Internal( path, crc, offset );
	}

	template< typename t >
	auto LoadAsset( String name, void* type ) -> t
	{
		METHOD( t ( ** )( AssetBundle*, String, void* ), LoadAsset_Internal );
		return LoadAsset_Internal( this, name, type );
	}
};

class Screen
{
public:
	CLASS( "UnityEngine", "Screen" );

	static auto width( ) -> int
	{
		METHOD( int( ** )( ), get_width );
		return get_width( );
	}

	static auto height( ) -> int
	{
		METHOD( int( ** )( ), get_height );
		return get_height( );
	}
};

class Camera
{
public:
	CLASS( "UnityEngine", "Camera" );

	static auto main( ) -> Camera*
	{
		METHOD( Camera * ( ** )( ), get_main );
		return get_main( );
	}
};

class Debug
{
public:
	CLASS( "UnityEngine", "Debug" );

	static auto Log( String message ) -> void
	{
		METHOD( void( ** )( String ), Log );
		return Log( message );
	}
};

class RaycastHit
{
public:
};

class Physics
{
public:
	CLASS( "UnityEngine", "Physics" );


	static auto Linecast( Vector3 start, Vector3 end, RaycastHit* hit, int layerMask ) -> bool
	{
		METHOD( bool( ** )( Vector3, Vector3, RaycastHit*, int ), Linecast );
		return Linecast( start, end, hit, layerMask );
	}

}; 

class GamePhysics
{
public:
	CLASS( "", "GamePhysics" );

	static auto LineOfSight( Vector3 source, Vector3 dest, int layer, float padding ) -> bool
	{
		static auto LineOfSight = *reinterpret_cast< bool( ** )( Vector3, Vector3, int, float, uintptr_t ) >( LI_MODULE_SAFE_( "GameAssembly.dll" ) + 0x52A090 );
		return LineOfSight( source, dest, layer, padding, 0 );
	}
};

namespace ConVar
{
	class Graphics
	{
	public:		
		static auto StaticClass( ) -> uintptr_t
		{
			static auto GameAssembly = LI_MODULE_SAFE_( "GameAssembly.dll" );
			return *reinterpret_cast< uintptr_t* >( GameAssembly + 0x32192E8 );
		}
	};
}

class SkinnedMultiMesh
{
public:
	CLASS( "", "SkinnedMultiMesh" );

	auto Renderers( ) -> List< Renderer*>*
	{
		METHOD( List< Renderer*>*( ** )( SkinnedMultiMesh* ), get_Renderers );
		return get_Renderers( this );
	}
};

class BaseNetwork
{
public:
};

class Connection
{
public:
	CLASS( "", "Connection" );

	MEMBER( bool, connected );
	MEMBER( unsigned long, userid );
	MEMBER( unsigned long, ownerid );
	MEMBER( String*, ipaddress );

};

class Server
{

};

class Client : public BaseNetwork
{
public:
	CLASS( "", "Client" );

	auto connection( ) -> Connection*
	{
		METHOD( Connection * ( ** )( Client* ), get_Connection );
		return get_Connection( this );
	}

};

class Networkable
{
public:
	CLASS( "", "Networkable" );

	MEMBER( Server*, sv );
	MEMBER( Client*, cl );

};

class EntityRealm
{
public:
	CLASS( "", "BaseNetworkable" )
	MEMBER( ListDictionary*, entityList );
};


class BaseNetworkable : public Component
{
public:
	CLASS( "", "BaseNetworkable" );

	static auto clientEntities( ) -> EntityRealm*
	{
		static auto clientEntities = reinterpret_cast< EntityRealm* >( Il2CppWrapper::GetClassFromName( "", "BaseNetworkable" )->GetFieldFromName( "clientEntities", false ) );
		return clientEntities;
	}
};


class Model
{
public:
	CLASS( "", "Model" );

	MEMBER( Transform*, headBone );
	MEMBER( Transform*, eyeBone );
	MEMBER( Animator*, animator );
	MEMBER( uintptr_t, boneTransforms );
};

class BaseEntity : public BaseNetworkable
{
public:
	CLASS( "", "BaseEntity" );

	MEMBER( Model*, model );
	MEMBER( uintptr_t, itemSkin )
	MEMBER( Bounds*, bounds )

	auto Health( ) -> float
	{
		METHOD( float( ** )( BaseEntity* ), Health );
		return Health( this );
	}

	auto MaxHealth( ) -> float
	{
		METHOD( float( ** )( BaseEntity* ), MaxHealth );
		return MaxHealth( this );
	}

	auto MaxVelocity( ) -> float
	{
		METHOD( float( ** )( BaseEntity* ), MaxVelocity );
		return MaxVelocity( this );
	}

	auto SetNetworkPosition( Vector3 position ) -> void
	{
		METHOD( void( ** )( BaseEntity*, Vector3 ), SetNetworkPosition );
		return SetNetworkPosition( this, position );
	}

	template< typename T>
	auto ServerRPC( String name, T args ) -> void
	{
		METHOD( void( ** )( BaseEntity*, String, T ), ServerRPC );
		return ServerRPC( this, name, args );
	}
};

enum class LifeState : int
{
	Alive = 0,
	Dead = 1
};

class BaseCombatEntity : public BaseEntity
{
public:
	CLASS( "", "BaseCombatEntity" );
	MEMBER( LifeState, lifestate );
	MEMBER( float, _health );
	MEMBER( float, _maxHealth );

};


class HeldEntity : public BaseEntity
{
public:
	CLASS( "", "HeldEntity" );
};

class AttackEntity : public HeldEntity
{
public:
	CLASS( "", "AttackEntity" );

	MEMBER( float, deployDelay );
	MEMBER( float, repeatDelay );
	MEMBER( float, timeSinceDeploy );

	auto HasAttackCooldown( ) -> bool
	{
		METHOD( bool( ** )( AttackEntity* ), HasAttackCooldown );
		return HasAttackCooldown( this );
	}
};


class BaseMelee : public AttackEntity
{
public:
	CLASS( "", "BaseMelee" );

	MEMBER( bool, canThrowAsProjectile );
	MEMBER( float, maxDistance );
	MEMBER( float, attackRadius );
	MEMBER( bool, isAutomatic );
	MEMBER( bool, blockSprintOnAttack );
	MEMBER( float, aiStrikeDelay );


	auto DoThrow( ) -> void
	{
		METHOD( void( ** )( BaseMelee* ), DoThrow );
		return DoThrow( this );
	}

	auto DoAttack( ) -> void
	{
		METHOD( void( ** )( BaseMelee* ), DoAttack );
		return DoAttack( this );
	}
};

class RecoilProperties
{
public:
	CLASS( "", "RecoilProperties" );

	MEMBER( RecoilProperties*, newRecoilOverride );
	MEMBER( float, recoilYawMin );
	MEMBER( float, recoilYawMax );
	MEMBER( float, recoilPitchMin );
	MEMBER( float, recoilPitchMax );
};


class BaseProjectile : public AttackEntity
{
public:
	CLASS( "", "BaseProjectile" );

	MEMBER( float, NoiseRadius );
	MEMBER( float, projectileVelocityScale );
	MEMBER( float, automatic );
	MEMBER( float, reloadTime );
	MEMBER( float, aimSway );
	MEMBER( float, aimSwaySpeed );
	MEMBER( RecoilProperties*, recoil );
	MEMBER( bool, isReloading );
	MEMBER( float, nextReloadTime );
	MEMBER( float, lastShotTime );
	MEMBER( Transform*, MuzzlePoint );

	auto DoReload( ) -> void
	{
		METHOD( void( ** )( BaseProjectile* ), DoReload );
		return DoReload( this );
	}

	auto DoAttack( ) -> void
	{
		METHOD( void( ** )( BaseProjectile* ), DoAttack );
		return DoAttack( this );
	}

	auto BeginCycle( ) -> void
	{
		METHOD( void( ** )( BaseProjectile* ), BeginCycle );
		return BeginCycle( this );
	}

	auto LaunchProjectile( ) -> void
	{
		METHOD( void( ** )( BaseProjectile* ), LaunchProjectile );
		return LaunchProjectile( this );
	}

	auto UpdateAmmoDisplay( ) -> void
	{
		METHOD( void( ** )( BaseProjectile* ), UpdateAmmoDisplay );
		return UpdateAmmoDisplay( this );
	}

	auto ShotFired( ) -> void
	{
		METHOD( void( ** )( BaseProjectile* ), ShotFired );
		return ShotFired( this );
	}

	auto DidAttackClientside( ) -> void
	{
		METHOD( void( ** )( BaseProjectile* ), DidAttackClientside );
		return DidAttackClientside( this );
	}
};

class BowWeapon : public BaseProjectile
{
public:
	CLASS( "", "BowWeapon" );

	MEMBER( bool, attackReady );
	MEMBER( float, arrowBack );


	auto TryReload( ) -> void
	{
		METHOD( void( ** )( BowWeapon* ), TryReload );
		return TryReload( this );
	}
};

enum class HitTest_Type : int
{
	Generic,
	ProjectileEffect,
	Projectile,
	MeleeAttack,
	Use
};

class HitInfo
{
public:
	CLASS( "", "HitInfo" );

	MEMBER( Vector3, PointStart );
	MEMBER( Vector3, PointEnd );
};


class HitTest
{
public:
	CLASS( "", "HitTest" );

	MEMBER( float, Radius );
	MEMBER( float, Forgiveness );
	MEMBER( float, MaxDistance );
	MEMBER( bool, DidHit );
	MEMBER( BaseEntity*, HitEntity );
	MEMBER( Vector3, HitPoint );
	MEMBER( Vector3, HitNormal );
	MEMBER( float, HitDistance );
	MEMBER( Transform*, HitTransform );

	auto HitPointWorld( ) -> Vector3
	{
		METHOD( Vector3( ** )( HitTest* ), HitPointWorld );
		return HitPointWorld( this );
	}

	auto HitNormalWorld( ) -> Vector3
	{
		METHOD( Vector3( ** )( HitTest* ), HitNormalWorld );
		return HitNormalWorld( this );
	}
};


class Projectile : public Component
{
public:
	CLASS( "", "Projectile" );

	MEMBER( float, thickness );
	MEMBER( Vector3, currentPosition );
	MEMBER( Vector3, currentVelocity );
	MEMBER( HitTest*, hitTest );
	MEMBER( float, traveledTime );
	
	auto isAuthoritative( ) -> bool
	{
		METHOD( bool( ** )( Projectile* ), get_isAuthoritative );
		return get_isAuthoritative( this );
	}

	auto DoRicochet( HitTest* test, Vector3 point, Vector3 normal ) -> bool
	{
		METHOD( bool( ** )( Projectile*, HitTest*, Vector3, Vector3 ), DoRicochet );
		return DoRicochet( this, test, point, normal );
	}

	auto DoHit( HitTest* test, Vector3 point, Vector3 normal ) -> bool
	{
		METHOD( bool( ** )( Projectile*, HitTest*, Vector3, Vector3 ), DoHit );
		return DoHit( this, test, point, normal );
	}

	auto isAlive( ) -> bool
	{
		METHOD( bool( ** )( Projectile* ), get_isAlive );
		return get_isAlive( this );
	}
};


class ItemModProjectile : public Component
{
public:
	CLASS( "", "ItemModProjectile" );
};

enum class ModelStateFlag : int
{
	OnGround = 4,
	Sprinting = 16
};

class ModelState
{
public:
	CLASS( "", "ModelState" );

	auto SetFlag( ModelStateFlag flag ) -> void
	{
		METHOD( void( ** )( ModelState*, ModelStateFlag, bool ), SetFlag );
		return SetFlag( this, flag, true );
	}
};


class PlayerEyes
{
public:
	CLASS( "", "PlayerEyes" );

	MEMBER( Vector3, viewOffset );

	auto MovementRight( ) -> Vector3
	{
		METHOD( Vector3( ** )( PlayerEyes* ), MovementRight );
		return MovementRight( this );
	}

	auto BodyRight( ) -> Vector3
	{
		METHOD( Vector3( ** )( PlayerEyes* ), BodyRight );
		return BodyRight( this );
	}

	auto BodyForward( ) -> Vector3
	{
		METHOD( Vector3( ** )( PlayerEyes* ), BodyForward );
		return BodyForward( this );
	}

	auto BodyUp( ) -> Vector3
	{
		METHOD( Vector3( ** )( PlayerEyes* ), BodyUp );
		return BodyUp( this );
	}

	auto MovementForward( ) -> Vector3
	{
		METHOD( Vector3( ** )( PlayerEyes* ), MovementForward );
		return MovementForward( this );
	}

	auto position( ) -> Vector3
	{
		METHOD( Vector3( ** )( PlayerEyes* ), get_position );
		return get_position( this );
	}
};

enum class ItemCategory : int
{
	Weapon,
	Construction,
	Items,
	Resources,
	Attire,
	Tool,
	Medical,
	Food
};

class ItemDefinition
{
public:
	CLASS( "", "ItemDefinition" );

	MEMBER( Sprite*, iconSprite );
	MEMBER( ItemCategory, category );
	MEMBER( uintptr_t, skins );

	auto FindIconSprite( int skinid ) -> Sprite*
	{
		METHOD( Sprite * ( ** )( ItemDefinition*, int ), FindIconSprite );
		return FindIconSprite( this, skinid );
	}
};


class Item
{
public:
	CLASS( "", "Item" );

	MEMBER( String*, name );
	MEMBER( String*, text );
	MEMBER( unsigned long, skin );
	MEMBER( uint32_t, uid );
	MEMBER( ItemDefinition*, info );
	MEMBER( BaseProjectile*, heldEntity );
};


class ItemContainer
{
public:
	CLASS( "", "ItemContainer" );

	MEMBER( List<Item*>*, itemList );
};

class PlayerInventory
{
public:
	CLASS( "", "PlayerInventory" );

	MEMBER( ItemContainer*, containerBelt );
};

class PlayerLoot
{
public:

};

enum class BUTTON : int
{
	USE = 256,
	FIRE_PRIMARY = 1024,
	FIRE_SECONDARY = 2048,
	RELOAD = 8192
};

class InputState
{
public:
	CLASS( "", "InputState" );

	auto IsDown( BUTTON button ) -> bool
	{
		METHOD( bool( ** )( InputState*, BUTTON ), IsDown );
		return IsDown( this, button );
	}
};

class PlayerInput
{
public:
	CLASS( "", "PlayerInput" );

	MEMBER( InputState*, state );
};


class PlayerModel
{
public:
	CLASS( "", "PlayerModel" );

	MEMBER( SkinnedMultiMesh*, _multiMesh );
	MEMBER( Animator*, _animator );
};

class BaseMovement : public Component
{
public:
	CLASS( "", "BaseMovement" );
};

class PlayerWalkMovement : public BaseMovement
{
public:
	CLASS( "", "PlayerWalkMovement" );

	MEMBER( float, capsuleHeight );
	MEMBER( float, capsuleCenter );
	MEMBER( float, capsuleHeightDucked );
	MEMBER( float, capsuleCenterDucked );
	MEMBER( float, capsuleHeightCrawling );
	MEMBER( float, capsuleCenterCrawling );
	MEMBER( float, gravityMultiplier );
	MEMBER( float, gravityMultiplierSwimming );
	MEMBER( float, maxVelocity );
};


enum class CameraMode : int
{
	FirstPerson,
	ThirdPerson,
	Eyes,
	FirstPersonWithArms,
	DeathCamClassic
};

enum class PlayerFlags : int
{
	IsAdmin = 4,
	Sleeping = 16,
	Wounded = 64,
	IsDeveloper = 128,
	Connected = 256,
	Aiming = 16384,
};


class BasePlayer : public BaseCombatEntity
{
public:
	CLASS( "", "BasePlayer" );

	MEMBER( CameraMode, currentViewModel );
	MEMBER( PlayerModel*, playerModel );
	MEMBER( PlayerInput*, input );
	MEMBER( BaseMovement*, movement );
	MEMBER( uint32_t, clActiveItem );
	MEMBER( ModelState*, modelState );
	MEMBER( int, clientTickRate );
	MEMBER( String*, _displayName );
	MEMBER( float, serverTickInterval );
	MEMBER( float, clientTickInterval );
	MEMBER( float, lastSentTickTime );
	MEMBER( PlayerEyes*, eyes );
	MEMBER( PlayerInventory*, inventory );
	MEMBER( uintptr_t, playerFlags );
	MEMBER( unsigned long, userID );



	auto MaxHealth( ) -> float
	{
		METHOD( float( ** )( BasePlayer* ), MaxHealth );
		return MaxHealth( this );
	}

	auto HasFlag( PlayerFlags flag ) -> bool
	{
		return ( playerFlags( ) & uintptr_t( flag ) ) == uintptr_t( flag );
	}

	auto item( ) -> Item*
	{
		auto ActiveUid = this->clActiveItem( );
		if ( !ActiveUid )
			return { };

		auto inventory = this->inventory( );
		if ( !inventory )
			return { };

		auto belt = inventory->containerBelt( );
		if ( !belt )
			return { };

		auto itemList = belt->itemList( );
		if ( !itemList )
			return { };

		auto items = *reinterpret_cast< uint64_t* >( itemList + 0x10 );
		if ( !items )
			return { };

		for ( int i = 0; i < 6; i++ )
		{
			auto item = *reinterpret_cast< Item** > ( items + 0x20 + ( i * 0x8 ) );
			if ( !item )
				continue;

			if ( ActiveUid == item->uid( ) )
				return item;
		}

		return { };
	}

	auto alive( ) -> bool
	{
		auto lifeState = this->lifestate( );
		return lifeState == LifeState::Alive;
	}

	auto BoneIndex( int index ) -> Transform*
	{
		auto model = this->model( );
		if ( !model )
			return { };

		auto boneTransforms = model->boneTransforms( );
		if ( !boneTransforms )
			return { };

		return *reinterpret_cast< Transform** >( boneTransforms + 0x20 + (  index * 0x8 ) );
	}

	auto ChatMessage( ) -> void
	{
		METHOD( void( ** )( BasePlayer* ), ChatMessage );
		return ChatMessage( this );
	}

	auto GetMaxSpeed( ) -> float
	{
		METHOD( float( ** )( BasePlayer* ), GetMaxSpeed );
		return GetMaxSpeed( this );
	}
};

class LocalPlayer
{
public:
	CLASS( _( "" ), _( "LocalPlayer" ) );


	static auto Entity( ) -> BasePlayer*
	{
		METHOD( BasePlayer * ( ** )( ), get_Entity );
		return get_Entity( );
	}
};

class ViewmodelSway
{
public:
	CLASS( "", "ViewmodelSway" );


	MEMBER( float, positionalSwaySpeed );
	MEMBER( float, positionalSwayAmount );
	MEMBER( float, rotationSwaySpeed );
	MEMBER( float, rotationSwayAmount );
};

class ViewmodelLower
{
public:
	CLASS( "", "ViewmodelLower" );

	MEMBER( bool, lowerOnSprint );
	MEMBER( bool, lowerWhenCantAttack );
	MEMBER( float, lowerScale );
	MEMBER( bool, shouldLower );
};

class ViewmodelBob
{
public:
	CLASS( "", "ViewmodelBob" );

	MEMBER( float, bobSpeedWalk );
	MEMBER( float, bobSpeedRun );
	MEMBER( float, bobAmountWalk );
	MEMBER( float, bobAmountRun );

};

class BaseViewModel : public Component
{
public:
	CLASS( "", "BaseViewModel" );

	MEMBER( ViewmodelSway*, sway );
	MEMBER( ViewmodelLower*, lower )
	MEMBER( ViewmodelBob*, bob );
	MEMBER( Animator*, animator );



	static auto ActiveModel( ) -> BaseViewModel*
	{
		METHOD( BaseViewModel * ( ** )( ), get_ActiveModel );
		return get_ActiveModel( );
	}
};

class TOD_DayParameters
{
public:
	CLASS( "", "TOD_DayParameters" );
};

class TOD_NightParameters
{
public:
	CLASS( "", "TOD_NightParameters" );

	MEMBER( float, LightIntensity );
	MEMBER( float, ShadowStrength );
	MEMBER( float, AmbientMultiplier );
	MEMBER( float, ReflectionMultiplier );
};

class TOD_Sky
{
public:
	CLASS( "", "TOD_Sky" );
	
	MEMBER( TOD_DayParameters*, Day );
	MEMBER( TOD_NightParameters*, Night );

	static auto Instance( ) -> TOD_Sky*
	{
		METHOD( TOD_Sky * ( ** )( ), get_Instance );
		return get_Instance( );
	}
};


namespace ProtoBuf
{
	class Attack
	{
	public:
		CLASS( "ProtoBuf", "Attack" );

		MEMBER( unsigned int, hitID );
		MEMBER( Vector3, pointStart );
		MEMBER( Vector3, pointEnd );
		MEMBER( unsigned int, hitBone );
	};


	class PlayerAttack
	{
	public:
		CLASS( "ProtoBuf", "PlayerAttack" );

		MEMBER( Attack*, attack );
	};

	class PlayerProjectileAttack
	{
	public:
		CLASS( "ProtoBuf", "PlayerProjectileAttack" );

		MEMBER( PlayerAttack*, playerAttack );
		MEMBER( Vector3, hitVelocity );
		MEMBER( float, hitDistance );
		MEMBER( float, travelTime );

	};
}


class BaseNpc
{
public:
	CLASS( "", "BaseNpc" );
};