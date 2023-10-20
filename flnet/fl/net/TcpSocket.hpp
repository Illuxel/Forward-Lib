#pragma once

#include "fl/net/Endpoint.hpp"

namespace Forward::Net {

	class TcpSocket 
	{
	public:
		using OnWriteDataCallFunc = std::function<void(Core::MutableBuffer const&)>;

	private:
		Core::IOContext io_context_;

		Core::TcpSocketBase socket_;
		Core::TcpResolver resolver_;

		OnWriteDataCallFunc on_write_data_;

	public:
		TcpSocket(Endpoint const& address);
		//TcpSocket(Core::TcpSocketBase socket);

		virtual ~TcpSocket();

		void SetReadDataCallback(OnWriteDataCallFunc const& callback);

	protected:
		virtual void OnReadData(Core::MutableBuffer const&);

	};
}