// switchbuttonbrush.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_SwitchButtonBrush_H_
#define Starscape_Gui_SwitchButtonBrush_H_

#include "gui/appearance/button/imagebuttonbrush.h"
#include "gui/config/font.h"

namespace Starscape {

	namespace Gui {

		class SwitchButtonBrush : public ImageButtonBrush {
		public:
			SwitchButtonBrush(const StaticImage& unpressed
							,const StaticImage& pressed
							,const StaticImage& hover
							,const Graphics::Color& textColor
							,const Graphics::ImageFontHandle& font)
				: ImageButtonBrush(unpressed, pressed, hover)
				,textColor_(textColor), font_(font)	{}
			Size GetPreferredSize(const char* caption) const {
				return Size((imageUnpressed_.Width()
								+GetFontTextWidth(*font_, caption))
							,std::max(imageUnpressed_.Height()
								,font_->GetCharHeight()));
			}
			void DrawUnpressed(const Rect& region
							,const char* caption) const {
				ImageButtonBrush::DrawUnpressed(region, caption);
				DrawCaption(region, caption);
			}
			void DrawPressed(const Rect& region
							,const char* caption) const {
				ImageButtonBrush::DrawPressed(region, caption);
				DrawCaption(region, caption);
			}
			void DrawHover(const Rect& region
						,const char* caption) const {
				ImageButtonBrush::DrawHover(region, caption);
				DrawCaption(region, caption);
			}
			void DrawCaption(const Rect& region
							,const char* caption) const {
				textColor_.MakeCurrent();
				font_->Bind();
				Graphics::DrawFontText(*font_, caption
									,region.x+imageUnpressed_.Width()
									,region.y);
			}
			Graphics::Color				textColor_;
			Graphics::ImageFontHandle	font_;
		};
	}
}

#endif
