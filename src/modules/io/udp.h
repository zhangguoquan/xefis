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

#ifndef XEFIS__MODULES__IO__UDP_H__INCLUDED
#define XEFIS__MODULES__IO__UDP_H__INCLUDED

// Standard:
#include <cstddef>

// Qt:
#include <QtNetwork/QUdpSocket>

// Xefis:
#include <xefis/config/all.h>
#include <xefis/core/module.h>
#include <xefis/core/property.h>


class UDP:
	public QObject,
	public Xefis::Module
{
	Q_OBJECT

  public:
	// Ctor
	UDP (Xefis::ModuleManager*, QDomElement const& config);

	// Dtor
	~UDP();

  protected:
	void
	data_updated() override;

  private slots:
	/**
	 * Called whenever there's data ready to be read from socket.
	 */
	void
	got_udp_packet();

  private:
	void
	interfere (QByteArray& blob);

  private:
	Xefis::PropertyString	_send;
	Xefis::PropertyString	_receive;
	bool					_send_interference		= false;
	bool					_receive_interference	= false;
	QByteArray				_receive_datagram;
	bool					_udp_send_enabled		= false;
	QUdpSocket*				_udp_send				= nullptr;
	QString					_udp_send_host;
	int						_udp_send_port			= 0;
	bool					_udp_receive_enabled	= false;
	QUdpSocket*				_udp_receive			= nullptr;
	QString					_udp_receive_host;
	int						_udp_receive_port		= 0;
};

#endif
