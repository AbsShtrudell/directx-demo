#pragma once
#include "ConfWin.h"
#include "BasicException.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "DxgiInfoManager.h"
#include <DirectXMath.h>
#include <memory>
#include <random>
#include "ConditionalNoexcept.h"

namespace Bind
{
	class Bindable;
}

class Graphics
{
	friend Bind::Bindable;
public:
	class Exception : public BasicException
	{
		using BasicException::BasicException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void EndFrame();
	void BeginFrame(float red, float green, float blue)noexcept;
	void DrawIndexed(UINT count) noxnd;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetCamera(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;
	void SetStencile(bool skybox);
	void EnableImgui() noexcept;
	void DisableImgui() noexcept;
	bool IsImguiEnabled() const noexcept;

private:
	bool imguiEnabled = true;
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr <IDXGISwapChain> pSwap = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pTarget = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> pDSV = nullptr;
};

