///////////////////////////////////////
// PBR�x�[�X�̃f�B�t�@�[�h���C�e�B���O��SRV_UAV���W�X�^�ݒ�
///////////////////////////////////////

#ifndef _DEFERREDLIGHTING_SRV_UAV_REGISTER_H_
#define _DEFERREDLIGHTING_SRV_UAV_REGISTER_H_

Texture2D<float4> albedoTexture : register(t0);                                         // �A���x�h
Texture2D<float4> normalTexture : register(t1);                                         // �@��
Texture2D<float4> metallicShadowSmoothTexture : register(t2);                           // ���^���b�N�A�V���h�E�A�X���[�X�e�N�X�`���Br�ɋ����x�Ag�ɉe�p�����[�^�Aa�Ɋ��炩���B

#endif