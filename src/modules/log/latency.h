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

#ifndef XEFIS__MODULES__LOG__LATENCY_H__INCLUDED
#define XEFIS__MODULES__LOG__LATENCY_H__INCLUDED

// Standard:
#include <cstddef>

// Xefis:
#include <xefis/config/all.h>
#include <xefis/core/module.h>


class Latency:
	public QObject,
	public Xefis::Module
{
	Q_OBJECT

  public:
	// Ctor
	Latency (Xefis::ModuleManager*, QDomElement const& config);

  private slots:
	/**
	 * Log latencies on std::clog.
	 */
	void
	log_latency();

  private:
	QTimer*	_log_timer;
};

#endif
