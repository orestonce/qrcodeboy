#pragma once

#include <string>
#include <vector>
#include <cstdint>

struct QrcodePngToText_Resp{
	std::string ErrMsg;
	std::string Text;
};
QrcodePngToText_Resp QrcodePngToText(std::string in0);
struct TextToQrcodePng_Resp{
	std::string ErrMsg;
	std::string PngBytes;
};
TextToQrcodePng_Resp TextToQrcodePng(std::string in0, int32_t in1);
