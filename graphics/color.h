// color.h
// Gregory Rosenblatt
// 5/19/05

#ifndef Starscape_Graphics_Color_H_
#define Starscape_Graphics_Color_H_

#include "graphics/config/opengl.h"
#include <boost/cstdint.hpp>

namespace Starscape {

	namespace Graphics {

		typedef GLfloat	ColorScalar;

		/** Represents a color with an OpenGL format. */
		class Color {
		public:
			/** Creates a default blank color. */
			Color() : r_(0), g_(0), b_(0), a_(0)	{}
			/** Creates a color with the specified RGBA values. */
			Color(const ColorScalar& r, const ColorScalar& g
				,const ColorScalar& b, const ColorScalar& a = GLfloat(1.0))
				: r_(r), g_(g), b_(b), a_(a)	{}
			/** Sets this color as the current drawing color. */
			void MakeCurrent() const	{ glColor4f(r_, g_, b_, a_); }
			/** Interpolates this color with the target color. */
			void SetInterpolation(const Color& target) {
				r_ = 0.5*(r_+target.r_);
				g_ = 0.5*(g_+target.g_);
				b_ = 0.5*(b_+target.b_);
			}
			/** Interpolates this color and alpha with the target's. */
			void SetInterpolationAlpha(const Color& target) {
				SetInterpolation(target);
				a_ = 0.5*(a_+target.a_);
			}
			// basic colors
			static Color Black(const ColorScalar& alpha = 1.0) {
				return Color(0.0, 0.0, 0.0, alpha);
			}
			static Color White(const ColorScalar& alpha = 1.0) {
				return Color(1.0, 1.0, 1.0, alpha);
			}
			static Color Red(const ColorScalar& alpha = 1.0) {
				return Color(1.0, 0.0, 0.0, alpha);
			}
			static Color Green(const ColorScalar& alpha = 1.0) {
				return Color(0.0, 1.0, 0.0, alpha);
			}
			static Color Blue(const ColorScalar& alpha = 1.0) {
				return Color(0.0, 0.0, 1.0, alpha);
			}
		private:
			ColorScalar	r_, g_, b_, a_;
		};

		/** Creates a color from raw data. */
		inline Color MakeColorFromData(boost::uint32_t colorData) {
			return Color((static_cast<ColorScalar>(
								((colorData & 0xff0000) >> 16))/0xff)
						,(static_cast<ColorScalar>(
								((colorData & 0x00ff00) >> 8))/0xff)
						,(static_cast<ColorScalar>(
								(colorData & 0x0000ff))/0xff));
		}

		/** Constructs a color using streamed data. */
		template <class StreamType>
		Color MakeColorFromStream(StreamType& in) {
			ColorScalar r, g, b, a;
			in >> r >> g >> b >> a;
			return Color(r, g, b, a);
		}

		/** Sets the current drawing color to white. */
		inline void ResetColor()	{ glColor4f(1.0, 1.0, 1.0, 1.0); }

		/** Returns the provided color interpolated towards white. */
		inline Color GetLighterColor(Color color) {
			color.SetInterpolation(Color::White());
			return color;
		}
		
		/** Returns the provided color interpolated towards black. */
		inline Color GetDarkerColor(Color color) {
			color.SetInterpolation(Color::Black());
			return color;
		}
	}
}

#endif
