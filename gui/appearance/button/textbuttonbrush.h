// textbuttonbrush.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_TextButtonBrush_H_
#define Starscape_Gui_TextButtonBrush_H_

#include "gui/appearance/button/buttonbrush.h"
#include "gui/config/font.h"

namespace Starscape {

	namespace Gui {

		template <class BaseButtonBrushType>
		class TextButtonBrush : public ButtonBrush {
		public:
			enum { MARGIN_WIDTH = 20, MARGIN_HEIGHT = 10 };
			TextButtonBrush(const BaseButtonBrushType& baseBrush
							,const Graphics::Color& textColor
							,const Graphics::ImageFontHandle& font
							,const SizeScalar& marginWidth = MARGIN_WIDTH
							,const SizeScalar& marginHeight = MARGIN_HEIGHT)
				: baseBrush_(baseBrush), textColor_(textColor), font_(font)
				,captionIndent_(baseBrush.GetCaptionOffset())
				,marginWidth_(marginWidth), marginHeight_(marginHeight)	{}
			Size GetPreferredSize(const char* caption) const {
				return Size(GetFontTextWidth(*font_, caption)
								+(marginWidth_+marginWidth_)
							,font_->GetCharHeight()
								+(marginHeight_+marginHeight_));
			}
			void DrawUnpressed(const Rect& region
							,const char* caption) const {
				baseBrush_.DrawUnpressed(region, caption);
				textColor_.MakeCurrent();
				font_->Bind();
				Graphics::GetFontTextWidth(*font_, caption);
				Graphics::DrawFontText(*font_, caption
									,region.x+marginWidth_
									,region.y+marginHeight_);
			}
			void DrawPressed(const Rect& region
							,const char* caption) const {
				baseBrush_.DrawPressed(region, caption);
				textColor_.MakeCurrent();
				font_->Bind();
				Graphics::DrawFontText(*font_, caption
									,region.x+marginWidth_+captionIndent_.x
									,region.y+marginHeight_+captionIndent_.y);
			}
			void DrawHover(const Rect& region
						,const char* caption) const {
				baseBrush_.DrawHover(region, caption);
				textColor_.MakeCurrent();
				font_->Bind();
				Graphics::GetFontTextWidth(*font_, caption);
				Graphics::DrawFontText(*font_, caption
									,region.x+marginWidth_
									,region.y+marginHeight_);
			}
		private:
			BaseButtonBrushType			baseBrush_;
			Graphics::Color				textColor_;
			Graphics::ImageFontHandle	font_;
			Coord						captionIndent_;
			SizeScalar					marginWidth_, marginHeight_;
		};
	}
}

#endif
