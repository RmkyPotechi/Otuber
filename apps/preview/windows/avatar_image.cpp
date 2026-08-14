#include "avatar_image.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wincodec.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace otuber::preview {

bool load_png_wic(const std::wstring &path, AvatarImage &image)
{
    image = {};
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    const bool uninit = SUCCEEDED(hr);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE)
        return false;

    ComPtr<IWICImagingFactory> factory;
    hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                          IID_PPV_ARGS(&factory));
    if (FAILED(hr)) { if (uninit) CoUninitialize(); return false; }

    ComPtr<IWICBitmapDecoder> decoder;
    hr = factory->CreateDecoderFromFilename(path.c_str(), nullptr, GENERIC_READ,
                                            WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) { if (uninit) CoUninitialize(); return false; }

    ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) { if (uninit) CoUninitialize(); return false; }

    ComPtr<IWICFormatConverter> converter;
    hr = factory->CreateFormatConverter(&converter);
    if (FAILED(hr)) { if (uninit) CoUninitialize(); return false; }

    hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA,
                               WICBitmapDitherTypeNone, nullptr, 0.0,
                               WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) { if (uninit) CoUninitialize(); return false; }

    UINT width = 0, height = 0;
    converter->GetSize(&width, &height);
    if (!width || !height) { if (uninit) CoUninitialize(); return false; }

    image.width = static_cast<int>(width);
    image.height = static_cast<int>(height);
    image.rgba.resize(static_cast<std::size_t>(width) * height * 4);
    hr = converter->CopyPixels(nullptr, width * 4, static_cast<UINT>(image.rgba.size()), image.rgba.data());

    if (uninit) CoUninitialize();
    if (FAILED(hr)) image = {};
    return !image.empty();
}

} // namespace otuber::preview
