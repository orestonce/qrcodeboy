package main

import (
	"github.com/orestonce/go2cpp"
	"github.com/orestonce/qrcodeboy"
)

func main() {
	ctx := go2cpp.NewGo2cppContext(go2cpp.NewGo2cppContextReq{
		CppBaseName:       "qrcodeboygo",
		EnableQt:          false,
		QtExtendBaseClass: "",
		QtIncludeList:     nil,
	})
	ctx.Generate1(qrcodeboy.QrcodePngToText)
	ctx.Generate1(qrcodeboy.TextToQrcodePng)
	ctx.MustCreate386LibraryInDir("qrcodeboy-qt")
}