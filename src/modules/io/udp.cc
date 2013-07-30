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

// Standard:
#include <cstddef>
#include <random>

// Qt:
#include <QtXml/QDomElement>

// Lib:
#include <boost/endian/conversion.hpp>

// Xefis:
#include <xefis/config/all.h>
#include <xefis/utility/qdom.h>

// Local:
#include "udp.h"


XEFIS_REGISTER_MODULE_CLASS ("io/udp", UDP);


UDP::UDP (Xefis::ModuleManager* module_manager, QDomElement const& config):
	Module (module_manager, config)
{
	for (QDomElement& e: config)
	{
		if (e == "settings")
		{
			parse_settings (e, {
				{ "send.host", _udp_send_host, false },
				{ "send.port", _udp_send_port, false },
				{ "send.interference", _send_interference, false },
				{ "receive.host", _udp_receive_host, false },
				{ "receive.port", _udp_receive_port, false },
				{ "receive.interference", _receive_interference, false },
			});
		}
		else if (e == "properties")
		{
			parse_properties (e, {
				{ "send", _send, true },
				{ "receive", _receive, true }
			});
		}
	}

	if (has_setting ("send.host") && has_setting ("send.port"))
	{
		_udp_send_enabled = true;
		_udp_send = new QUdpSocket();
	}

	if (has_setting ("receive.host") && has_setting ("receive.port"))
	{
		_udp_receive_enabled = true;
		_udp_receive = new QUdpSocket();
		if (!_udp_receive->bind (QHostAddress (_udp_receive_host), _udp_receive_port, QUdpSocket::ShareAddress))
			log() << "failed to bind to address " << _udp_receive_host.toStdString() << ":" << _udp_receive_port << std::endl;
		QObject::connect (_udp_receive, SIGNAL (readyRead()), this, SLOT (got_udp_packet()));
	}
}


UDP::~UDP()
{
	delete _udp_receive;
	delete _udp_send;
}


void
UDP::data_updated()
{
	if (!_send.is_singular() && _send.fresh())
	{
		std::string data = *_send;
		QByteArray blob (data.data(), data.size());

		if (_send_interference)
			interfere (blob);

		if (_udp_send)
			_udp_send->writeDatagram (blob.data(), blob.size(), QHostAddress (_udp_send_host), _udp_send_port);
	}
}


void
UDP::got_udp_packet()
{
	while (_udp_receive->hasPendingDatagrams())
	{
		int datagram_size = _udp_receive->pendingDatagramSize();
		if (_receive_datagram.size() < datagram_size)
			_receive_datagram.resize (datagram_size);
		_udp_receive->readDatagram (_receive_datagram.data(), datagram_size, nullptr, nullptr);
	}

	if (_receive_interference)
		interfere (_receive_datagram);

	if (!_receive.is_singular())
	{
		_receive.write (std::string (_receive_datagram.data(), _receive_datagram.size()));
		signal_data_updated();
	}
}


void
UDP::interfere (QByteArray& blob)
{
	if (rand() % 3 == 0)
	{
		// Erase random byte from the input sequence:
		int i = rand() % blob.size();
		blob.remove (i, 1);
	}
}

