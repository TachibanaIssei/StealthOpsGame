#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// レンダーラーの基底クラス
	/// </summary>
	class IRenderer : public Noncopyable
	{
	public:
		/// <summary>
		/// フォワードレンダーパスから呼ばれる処理
		/// </summary>
		/// <param name="rc"></param>
		virtual void  OnForwardRender(RenderContext& rc)
		{

		}
		/// <summary>
		/// 2Dオブジェクトから呼ばれる処理
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}
	};
}