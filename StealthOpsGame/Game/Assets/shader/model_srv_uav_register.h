//////////////////////////////////////////////////////////////////////////////
// フォワードレンダリングで使用されるモデルシェーダーのSRV_UAVレジスタ設定
//////////////////////////////////////////////////////////////////////////////

#ifndef _MODEL_SRV_UAV_REGISTER_H_
#define _MODEL_SRV_UAV_REGISTER_H_

Texture2D<float4> albedoTexture : register(t0);                 // アルベド
Texture2D<float4> normalTexture : register(t1);                 // 法線
Texture2D<float4> metallicShadowSmoothTexture : register(t2);   // メタリック、シャドウ、スムーステクスチャ。rに金属度、gに影パラメータ、aに滑らかさ。

#endif