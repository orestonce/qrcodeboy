package qrcodeboy

import (
	"bytes"
	"github.com/makiuchi-d/gozxing"
	"github.com/makiuchi-d/gozxing/qrcode"
	"image/png"
)

type TextToQrcodePng_Resp struct {
	ErrMsg   string
	PngBytes string
}

func TextToQrcodePng(s string, sz int) (resp TextToQrcodePng_Resp) {
	// Generate a barcode image (*BitMatrix)
	enc := qrcode.NewQRCodeWriter()
	img, err := enc.Encode(s, gozxing.EncodeHintType_QR_MASK_PATTERN, sz, sz, nil)
	if err != nil {
		resp.ErrMsg = "enc.Encode " + err.Error()
		return resp
	}

	encoder := png.Encoder{CompressionLevel: png.BestCompression}

	var b bytes.Buffer
	err = encoder.Encode(&b, img)

	if err != nil {
		resp.ErrMsg = "encoder.Encode " + err.Error()
		return resp
	}
	resp.PngBytes = string(b.Bytes())
	return resp
}
