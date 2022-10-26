#pragma once

#include <string>
#include <vector>
#include "Vector.h"
#include "GameObject.h"

#include <GLFW/glfw3.h>
using namespace std;

class Input
{
public:
	enum KBButtons
	{
		NONE = -1,
		SPACE = 32,
		APOSTROPHE = 39,
		COMMA = 44,
		MINUS = 45,
		PERIOD = 46,
		SLASH = 47,
		ZERO = 48,
		ONE = 49,
		TWO = 50,
		THREE = 51,
		FOUR = 52,
		FIVE = 53,
		SIX = 54,
		SEVEN = 55,
		EIGHT = 56,
		NINE = 57,
		SEMICOLON = 59,
		EQUAL = 61,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72, 
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LEFT_BRACKET = 91,
		BACKSLASH = 92,
		RIGHT_BRACKET = 93,
		GRAVE_ACCENT = 96,
		WORLD_1 = 161,
		WORLD_2 = 162,
		ESCAPE = 256,
		ENTER = 257,
		TAB = 258,
		BACKSPACE = 259,
		INSERT = 260,
		DELETE = 261,
		RIGHT = 262,
		LEFT = 263,
		DOWN = 264,
		UP = 265,
		PGUP = 266,
		PGDOWN = 267,
		HOME = 268,
		END = 269,
		CAPS_LOCK = 280,
		SCROLL_LOCK = 281,
		NUM_LOCK = 282,
		PRINT_SCREEN = 283,
		PAUSE = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
		NUM0 = 320,
		NUM1 = 321,
		NUM2 = 322,
		NUM3 = 323,
		NUM4 = 324,
		NUM5 = 325,
		NUM6 = 326,
		NUM7 = 327,
		NUM8 = 328,
		NUM9 = 329,
		NUM_DECIMAL = 330,
		NUM_DIVIDE = 331,
		NUM_MULTIPLY = 332,
		NUM_SUBSTRACT = 333,
		NUM_ADD = 334,
		NUM_ENTER = 335,
		NUM_EQUAL = 336,
		LSHIFT = 340,
		LCTRL = 341,
		LALT = 342,
		LSUPER = 343,
		RSHIFT = 344,
		RCTRL = 345,
		RALT = 346,
		RSUPER = 347,
		MENU = 348
	};

	enum JoyButtons
	{

	};

	enum JoyAxis
	{
		
	};

	enum MouseAxis
	{
		MOUSE_X,
		MOUSE_Y
	};

	enum MouseButtons {
		LMB = GLFW_MOUSE_BUTTON_LEFT,
		RMB = GLFW_MOUSE_BUTTON_RIGHT,
		MMB = GLFW_MOUSE_BUTTON_MIDDLE
	};

	enum Devices
	{
		UNKNOWN = -1,
		JOY1 = 0,
		JOY2 = 1,
		JOY3 = 2,
		JOY4 = 3,
		Mouse = 1000,
		MouseButton = 1001,
		Keyboard = 100
	};

	class Axis
	{
	public:
		struct Mapping
		{
		public:
			int device = Devices::UNKNOWN;
			int positiveKey = KBButtons::NONE;
			int negativeKey = KBButtons::NONE;

			Mapping() {}
			Mapping(int device, int positiveKey, int negativeKey);
		};

		struct Event
		{
		public:
			void(GameObject::* func)(Axis*) = nullptr;
			GameObject* object;

			Event(GameObject* object = nullptr, void(GameObject::* func)(Axis*) = nullptr);
			void Exec(Axis* axis);
		};
	protected:
		float lastValue = 0;
		float value = 0;

		Event OnButtonUp;
		Event OnButtonDown;
		Event OnButton;
		Event OnValueChange;		
	public:
		vector<Mapping> mappings;

		string name;

		/// <summary>
		/// Copies lastValue to value, use this every Input update before updating input values - to update value use Update() method
		/// </summary>
		void SwitchBuffers();

		/// <summary>
		/// Updates value without switching buffers, useful when updating from more than 1 device
		/// </summary>
		/// <param name="value">New value</param>
		void UpdateValue(float value);

		/// <summary>
		/// Function that returns current axis value
		/// </summary>
		/// <returns>Axis value</returns>
		float GetValue();

		/// <summary>
		/// Check if button was pressed this frame
		/// </summary>
		/// <returns>True if pressed this frame, false otherwise</returns>
		bool GetDown();


		void AddMapping(Mapping mapping);
		
		Axis(string name, int device, int positiveKey, int negativeKey = Input::KBButtons::NONE);
	};

protected:

	static GLFWwindow* lastWindow;

	static Axis EmptyAxis;

	static vector<Axis> axisMappings;

	static Axis FindAxis(string name);

	static Vector3 lastMousePos;
	static Vector3 mousePos;
	static Vector3 mouseMove;

	static float GetKeyValue(GLFWwindow* window, int device, int positiveKey, int negativeKey);
public:
	static bool GetKey(Devices device, int key);

	static void RegisterAxis(Axis axis);
	static bool GetButtonDown(string name);
	static bool GetButton(string name);
	static float GetAxis(string name);

	/// <summary>
	/// Updates mouse position
	/// </summary>
	/// <param name="xPos">MouseX pos</param>
	/// <param name="yPos">MouseY pos</param>
	/// <param name="ignoreChange">True to ignore mouse poition change</param>
	static void __SetMousePos(double xPos, double yPos, bool ignoreChange = false);
	static void __Update(GLFWwindow* window);
};