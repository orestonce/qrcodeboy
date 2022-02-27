package qrcodeboy

import (
	"github.com/makiuchi-d/gozxing"
	"github.com/makiuchi-d/gozxing/qrcode"
	"image"
	_ "image/jpeg"
	_ "image/png"
	"strings"
)

type QrcodePngToText_Resp struct {
	ErrMsg string
	Text   string
}

func QrcodePngToText(data string) (resp QrcodePngToText_Resp) {
	img, _, err := image.Decode(strings.NewReader(data))
	if err != nil {
		resp.ErrMsg = "image.Decode " + err.Error()
		return resp
	}
	bmp, err := gozxing.NewBinaryBitmapFromImage(img)
	if err != nil {
		resp.ErrMsg = "gozxing.NewBinaryBitmapFromImage " + err.Error()
		return resp
	}
	qrReader := qrcode.NewQRCodeReader()
	result, err := qrReader.Decode(bmp, nil)
	if err != nil {
		resp.ErrMsg = "qrReader.Decode " + err.Error()
		return resp
	}
	resp.Text = result.String()
	return resp
}
