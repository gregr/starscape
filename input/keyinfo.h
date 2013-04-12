// keyinfo.h
// Gregory Rosenblatt
// 5/7/05

#ifndef Starscape_Input_KeyInfo_H_
#define Starscape_Input_KeyInfo_H_

#include "SDL.h"

namespace Starscape {

	namespace Input {

		typedef SDL_keysym	KeyInfo;

		/** Retrieves the pressed key symbol. */
		inline SDLKey GetSymbol(const KeyInfo& key) {
			return key.sym;
		}

		/** Retrieves key-modifier flags. */
		inline SDLMod GetModifier(const KeyInfo& key) {
			return key.mod;
		}

		/** Retrieves the unicode value. */
		inline unsigned short GetUnicode(const KeyInfo& key) {
			return key.unicode;
		}

		/** Determines if the unicode value is an ascii character. */
		inline bool IsUnicodeAscii(const KeyInfo& key) {
			return (((GetUnicode(key) & 0xFF80) == 0)
					&& (GetUnicode(key) >= 32));
		}

		/** Tests for a shift key being held. */
		inline bool IsShiftPressed() {
			return (SDL_GetModState() & KMOD_SHIFT);
		}

		/** Tests for a ctrl key being held. */
		inline bool IsCtrlPressed() {
			return (SDL_GetModState() & KMOD_CTRL);
		}

		/** Tests for an alt key being held. */
		inline bool IsAltPressed() {
			return (SDL_GetModState() & KMOD_ALT);
		}

		/** Enables Unicode translation for keyboard events. */
		inline void EnableUnicode()	{ SDL_EnableUNICODE(1); }

		/** Disables Unicode translation for keyboard events. */
		inline void DisableUnicode()	{ SDL_EnableUNICODE(0); }

		/** Enables keyboard event repeating when a key is held.
		 * @return 0 on success and -1 on failure.
		 */
		inline int EnableKeyRepeat() {
			return SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY
								,SDL_DEFAULT_REPEAT_INTERVAL);
		}

		/** Disables keyboard event repeating.
		 * @return 0 on success and -1 on failure.
		 */
		inline int DisableKeyRepeat()	{ return SDL_EnableKeyRepeat(0, 0); }
	}
}

#endif
