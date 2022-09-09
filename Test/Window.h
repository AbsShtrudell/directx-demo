#pragma once
#include "ConfWin.h"
#include "BasicException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <optional>

class Window
{
public:

	class Exception : public BasicException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr);
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};

private:

	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
		WindowClass(const WindowClass&) = delete;
		~WindowClass();

		WindowClass& operator= (const WindowClass&) = delete;

	private:
		HINSTANCE hInst;
		static constexpr const char* wndClassName = "WinAPIWindowClass";

		static WindowClass wndClass;
	};

public:
	Window(int width, int height, const char* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;

	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessages();

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	Keyboard kbd;
	Mouse mouse;

private:
	int width;
	int height;
	HWND hWnd;
};

#define WND_EXCEPT(hr) Window::Exception(__LINE__,__FILE__,hr)
#define WND_LAST_EXCEPT(hr) Window::Exception(__LINE__,__FILE__,GetLastError())