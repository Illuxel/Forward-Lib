<<<<<<< Updated upstream
#pragma once

#include "fl/net/Core.hpp"

namespace Forward::Net {

	class TcpSocket 
	{
	private:

	public:

	};
=======
#pragma once

#include "fl/net/Endpoint.hpp"

namespace Forward::Net {

	class TcpSocket 
	{
	public:
		using OnWriteDataCallFunc = std::function<void(Core::mutable_buffer const&)>;

	private:
		Core::Tcp::socket socket_;
		Core::Tcp::resolver resolver_;

		Core::io_context io_context_;

		OnWriteDataCallFunc on_write_data_;

	public:
		TcpSocket(Endpoint const& address);
		TcpSocket(Core::Tcp::socket&& socket);

		virtual ~TcpSocket();

		void SetWriteDataCallback(OnWriteDataCallFunc const& callback);

	protected:
		virtual void OnReadData(Core::mutable_buffer const&);

	};
>>>>>>> Stashed changes
}