/* vim:ts=4
 *
 * Copyleft 2012…2016  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef XEFIS__UTILITY__TIME_HELPER_H__INCLUDED
#define XEFIS__UTILITY__TIME_HELPER_H__INCLUDED

// Standard:
#include <cstddef>

// System:
#include <sys/time.h>

// Xefis:
#include <xefis/config/all.h>


namespace Xefis {

class TimeHelper
{
  public:
	static Time
	now() noexcept;

	static Time
	epoch() noexcept;

	static Time
	measure (std::function<void()> callback) noexcept;
};


inline Time
TimeHelper::now() noexcept
{
	struct timeval tv;
	::gettimeofday (&tv, 0);
	return 1_us * static_cast<Time::Value> (tv.tv_sec * 1000000ull + tv.tv_usec);
}


inline Time
TimeHelper::epoch() noexcept
{
	return 0_s;
}


inline Time
TimeHelper::measure (std::function<void()> callback) noexcept
{
	Time t = now();
	callback();
	return now() - t;
}

} // namespace Xefis

#endif

