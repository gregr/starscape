// slidertrack.h
// Gregory Rosenblatt
// 5/20/05

#ifndef Starscape_Gui_SliderTrack_H_
#define Starscape_Gui_SliderTrack_H_

#include "gui/appearance/scroll/slidertrackbrush.h"
#include "gui/control/scroll/scrollcontroller.h"
#include "gui/window.h"

namespace Starscape {

	namespace Gui {

		/** A set of functions for dealing with a vertical orientation. */
		struct VerticalOrientation {
			typedef ScrollController::RatioType	RatioType;
			static const SizeScalar& GetLength(const Window& target) {
				return target.Height();
			}
			static void SetLength(Window& target
								,const SizeScalar& length) {
				target.SetSize(target.Width(), length);
			}
			static void SetThickness(Window& target
									,const SizeScalar& thickness) {
				target.SetSize(thickness, target.Height());
			}
			static void SetLayout(Window& track
								,Window& incButton
								,Window& decButton) {
				incButton.SetPos(0, 0);
				track.SetPos(0, incButton.Height());
				decButton.SetPos(0, track.PosY()+track.Height());
			}
			static void MoveToPos(Window& target
							,const Window& parent
							,const Coord& pos) {
				SizeScalar ypos
					= std::min(UsableHeight(target, parent)
							,static_cast<SizeScalar>(pos.y));
				target.SetPos(target.PosX(), ypos);
			}
			static RatioType GetPosRatio(const Window& target
										,const Window& parent) {
				SizeScalar total = UsableHeight(target, parent);
				return (static_cast<RatioType>(total - target.PosY())
												/(total));
			}
			static void RefreshPosition(Window& target
								,const Window& parent
								,const RatioType& posRatio) {
				SizeScalar total = UsableHeight(target, parent);
				target.SetPos(target.PosX()
					,total-static_cast<CoordScalar>(posRatio*(total)));
			}
		private:
			static SizeScalar UsableHeight(const Window& target
										,const Window& parent) {
				return (parent.Height()-target.Height());
			}
		};

		/** A set of functions for dealing with a horizontal orientation. */
		struct HorizontalOrientation {
			typedef ScrollController::RatioType	RatioType;
			static const SizeScalar& GetLength(const Window& target) {
				return target.Width();
			}
			static void SetLength(Window& target, const SizeScalar& length) {
				target.SetSize(length, target.Height());
			}
			static void SetThickness(Window& target
									,const SizeScalar& thickness) {
				target.SetSize(target.Width(), thickness);
			}
			static void SetLayout(Window& track
								,Window& incButton
								,Window& decButton) {
				decButton.SetPos(0, 0);
				track.SetPos(decButton.Width(), 0);
				incButton.SetPos(track.PosX()+track.Width(), 0);
			}
			static void MoveToPos(Window& target
							,const Window& parent
							,const Coord& pos) {
				SizeScalar xpos
					= std::min(UsableWidth(target, parent)
							,static_cast<SizeScalar>(pos.x));
				target.SetPos(xpos, target.PosY());
			}
			static RatioType GetPosRatio(const Window& target
									,const Window& parent) {
				SizeScalar total = UsableWidth(target, parent);
				return (static_cast<RatioType>(target.PosX())
										/(total));
			}
			static void RefreshPosition(Window& target
								,const Window& parent
								,const RatioType posRatio) {
				SizeScalar total = UsableWidth(target, parent);
				target.SetPos(static_cast<CoordScalar>(posRatio*(total))
							,target.PosY());
			}
		private:
			static SizeScalar UsableWidth(const Window& target
										,const Window& parent) {
				return (parent.Width()-target.Width());
			}
		};

		/** A grippable slider that moves along a track. */
		template <class OrientationType>
		class Slider : public Window {
		public:
			Slider(const SliderBrush& brush)
				: brush_(brush), grabbed_(false)
			{
				Size size = brush.GetPreferredSize();
				SetSize(size.x, size.y);
			}
			const Window* OnMouseGrab(int button) {
				if (button == STARSCAPE_BUTTON_LEFT)
					return &GetParent();
				return 0;
			}
			bool OnMouseRelease(int button) {
				if (button == STARSCAPE_BUTTON_LEFT)
					return true;
				return false;
			}
			void OnButtonDown(int button) {
				if (button == STARSCAPE_BUTTON_LEFT)
					grabbed_ = true;
				else
					GetParent().OnButtonDown(button);
			}
			void OnButtonUp(int button) {
				if (button == STARSCAPE_BUTTON_LEFT)
					grabbed_ = false;
				else
					GetParent().OnButtonUp(button);
			}
			void OnMouseMove(const Coord& delta) {
				if (grabbed_) {
					Coord target(PosX()+delta.x, PosY()+delta.y);
					if (target.x < 0)
						target.x = 0;
					if (target.y < 0)
						target.y = 0;
					OrientationType::MoveToPos(*this, GetParent()
							,target);
				}
			}
			void OnMouseLeave() {
				grabbed_ = false;
			}
			bool IsGrabbed() const	{ return grabbed_; }
		protected:
			void OnRender() {
				brush_.Draw(GetRenderingRect());
			}
		private:
			const SliderBrush&	brush_;
			bool				grabbed_;
		};

		/** Controls a value via the position of its slider. */
		template <class OrientationType>
		class SliderTrack : public Window {
		public:
			SliderTrack(const SliderTrackBrush& brush
						,const ScrollController& controller)
				: slider_(brush.GetSliderBrush())
				,brush_(brush), controller_(controller)
				,stateChanged_(true)	// have it update initially
			{
				AddChild(slider_);
				Size size = brush_.GetPreferredSize();
				SetSize(size.x, size.y);
			}
			void OnButtonDown(int button) {
				switch (button) {
				case STARSCAPE_BUTTON_LEFT:
					MoveSliderToPosition();
					break;
				case STARSCAPE_BUTTON_WHEELUP:
					Increment();
					break;
				case STARSCAPE_BUTTON_WHEELDOWN:
					Decrement();
					break;
				}
			}
			void OnUpdate(unsigned int deltaTime) {
				if (stateChanged_) {
					OrientationType::RefreshPosition(slider_, *this
							,controller_.GetRefreshedRatio());
					stateChanged_ = false;
				}
				else if (slider_.IsGrabbed())
					SlideToRatio();
			}
			void SetLength(const SizeScalar& length) {
				OrientationType::SetLength(*this, length);
			}
			void AlertStateChanged()	{ stateChanged_ = true; }
			void Increment() {
				controller_.Increment();
				AlertStateChanged();
			}
			void Decrement() {
				controller_.Decrement();
				AlertStateChanged();
			}
		protected:
			void OnRender() {
				brush_.Draw(GetRenderingRect());
			}
		private:
			void SlideToRatio() {
				controller_.SlideToRatio(
						OrientationType::GetPosRatio(slider_, *this));
			}
			void MoveSliderToPosition() {
				OrientationType::MoveToPos(slider_, *this, CursorCoord());
				SlideToRatio();
			}
			Slider<OrientationType>	slider_;
			const SliderTrackBrush& brush_;
			const ScrollController&	controller_;
			bool					stateChanged_;
		};

		typedef SliderTrack<VerticalOrientation>	VSliderTrack;
		typedef SliderTrack<HorizontalOrientation>	HSliderTrack;
	}
}

#endif
