#pragma once

namespace nsK2EngineLow
{
	// シーンのディレクションライトの最大数
	const int MAX_DIRECTIONAL_LIGHT = 4;

	// シャドウマップ
	enum {
		SHADOW_MAP_AREA_NEAR,		//近影範囲のシャドウマップ。
		SHADOW_MAP_AREA_MIDDLE,		//中影範囲のシャドウマップ。
		SHADOW_MAP_AREA_FAR,		//遠影範囲のシャドウマップ。
		NUM_SHADOW_MAP
	};

	// レンダリングターゲットのフォーマット。
	struct RenderTargetFormat {
		DXGI_FORMAT colorBufferFormat;	// カラーバッファのフォーマット。
		DXGI_FORMAT depthBufferFormat;	// 深度バッファのフォーマット。
	};
	// メインレンダリングターゲットのフォーマット。
	const RenderTargetFormat g_mainRenderTargetFormat = {
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	};
}
