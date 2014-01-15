/* vim:ts=4
 *
 * Copyleft 2012…2013  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef XEFIS__CORE__SOUND_MANAGER_H__INCLUDED
#define XEFIS__CORE__SOUND_MANAGER_H__INCLUDED

// Standard:
#include <cstddef>
#include <set>

// Qt:
#include <QtCore/QProcess>

// Xefis:
#include <xefis/config/all.h>
#include <xefis/core/application.h>
#include <xefis/core/window.h>


namespace Xefis {

class SoundManager
{
  public:
	/**
	 * Represents sound process.
	 */
	class Sound
	{
	  public:
		// Ctor
		Sound (QString const& wav_file_name);

		/**
		 * Return true if sound has finished playing.
		 */
		bool
		finished() const noexcept;

	  private:
		Unique<QProcess>	_play_process;
		bool				_finished = false;
	};

  public:
	// Ctor
	SoundManager (Application*);

	/**
	 * Play sound.
	 */
	Shared<Sound>
	play (QString const& wav_file_name);

	/**
	 * Clean old finished sounds.
	 */
	void
	cleanup();

  private:
	Application*			_application;
	std::set<Shared<Sound>>	_sounds;
};


inline bool
SoundManager::Sound::finished() const noexcept
{
	return _finished;
}

} // namespace Xefis

#endif
