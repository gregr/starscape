// texture.h
// Gregory Rosenblatt
// 5/12/05

#ifndef Starscape_Graphics_Texture_H_
#define Starscape_Graphics_Texture_H_

#include "graphics/config/surfacedata.h"
#include "graphics/config/opengl.h"

namespace Starscape {

	namespace Graphics {

		/** Returns the drawing mode to non-textured. */
		inline void UnbindTextures()	{ glBindTexture(GL_TEXTURE_2D, 0); }

		// mapper for textures that are used as simple orthogonal images
		struct ImageMapper {
			void SetParams() {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER
								,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER
								,GL_NEAREST);
			}
			void MapData(unsigned int width, unsigned int height
						,const GLvoid* data) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA
							,width, height, 0, GL_RGBA
							,GL_UNSIGNED_BYTE, data);
			}
		};

		// mapper for textures with multiple levels of detail
		struct MipMapper {
			void SetParams() {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER
								,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER
								,GL_NEAREST);
			}
			void MapData(unsigned int width, unsigned int height
						,const GLvoid* data) {
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA
								,width, height, GL_RGBA
								,GL_UNSIGNED_BYTE, data);
			}
		};

		/** Manages an OpenGL texture resource.
		 * The Mapper determines how the texture is created.
		 * MipMapping is the default.
		 */
		template <class MapperType = MipMapper>
		class Texture {
			// non-copyable
			Texture(const Texture&);
			void operator=(const Texture&);
		public:
			/** Creates a Texture using the provided surface data.
			 * @param surface The surface to use as texture data.
			 */
			Texture(const Raster::Surface& data)
				: id_(0)
				,rectWidth_(data.Width()), rectHeight_(data.Height())
				,widthRatio_(0.0f), heightRatio_(0.0f)
				,tu_(0.0f), tv_(0.0f)
			{
				glGenTextures(1, &id_);
				LoadData(data);
			}
			~Texture()	{ Release(); }
			/** Binds this texture as the current one for rendering. */
			void Bind() const	{ glBindTexture(GL_TEXTURE_2D, id_); }
			/** Releases this texture in preparation to reacquire. */
			void Release() {
				glDeleteTextures(1, &id_);
				id_ = 0;
			}
			/** The texture width coord of the meaningful data region. */
			GLfloat TextureWidth() const	{ return tu_; }
			/** The texture height coord of the meaningful data region. */
			GLfloat TextureHeight() const	{ return tv_; }
			/** Ratio for computing texture width from rect width. */
			GLfloat WidthRatio() const	{ return widthRatio_; }
			/** Ratio for computing texture height from rect height. */
			GLfloat HeightRatio() const	{ return heightRatio_; }
			/** The rect width of the meaningful data region. */
			const SizeScalar& RectWidth() const	{ return rectWidth_; }
			/** The rect height of the meaningful data region. */
			const SizeScalar& RectHeight() const	{ return rectHeight_; }
		private:
			// sets the opengl texture data to match the surface data
			void LoadData(const Raster::Surface& source) {
				Raster::ManagedSurface data =
					Raster::CreateSurfacePowerOfTwo(source);
				Bind();
				MapperType	mapper;
				mapper.SetParams();
				mapper.MapData(data.Width(), data.Height()
							,data.GetPixelData());
				widthRatio_ = 1.0 / data.Width();
				heightRatio_ = 1.0 / data.Height();
				tu_ = widthRatio_ * rectWidth_;
				tv_ = heightRatio_ * rectHeight_;
			}
			GLuint	id_;
			SizeScalar	rectWidth_, rectHeight_;
			GLfloat	widthRatio_, heightRatio_
					,tu_, tv_;
		};
	}
}

#endif
