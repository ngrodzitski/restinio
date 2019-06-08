/*
	restinio
*/

/*!
	HTTP server traits.
*/

#pragma once

#include <restinio/request_handler.hpp>
#include <restinio/asio_timer_manager.hpp>
#include <restinio/null_logger.hpp>
#include <restinio/connection_state_listener.hpp>

namespace restinio
{

//
// traits_t
//

template <
		typename Timer_Manager,
		typename Logger,
		typename Request_Handler = default_request_handler_t,
		typename Strand = asio_ns::strand< asio_ns::executor >,
		typename Socket = asio_ns::ip::tcp::socket >
struct traits_t
{
	//FIXME: document this!
	/*!
	 * @brief A type for HTTP methods mapping.
	 *
	 * If RESTinio is used with vanila version of http_parser then
	 * the default value of http_methods_mapper_t is enough. But if a user
	 * uses modified version of http_parser with support of additional,
	 * not-standard HTTP methods then the user should provide its
	 * http_methods_mapper. For example:
	 * \code
	 * // Definition for non-standard HTTP methods.
	 * // Note: values of HTTP_ENCODE and HTTP_DECODE are from modified
	 * // http_parser version.
	 * constexpr const restinio::http_method_id_t http_encode(HTTP_ENCODE, "ENCODE");
	 * constexpr const restinio::http_method_id_t http_decode(HTTP_DECODE, "DECODE");
	 *
	 * // Definition of non-standard http_method_mapper.
	 * struct my_http_method_mapper {
	 * 	inline constexpr restinio::http_method_id_t
	 * 	from_nodejs(int value) noexcept {
	 * 		switch(value) {
	 * 			case HTTP_ENCODE: return http_encode;
	 * 			case HTTP_DECODE: return http_decode;
	 * 			default: return restinio::default_http_methods_t::from_nodejs(value);
	 * 		}
	 * 	}
	 * };
	 * ...
	 * // Make a custom traits with non-standard http_method_mapper.
	 * struct my_server_traits : public restinio::default_traits_t {
	 * 	using http_methods_mapper_t = my_http_method_mapper;
	 * };
	 * \endcode
	 *
	 * @since v.0.5.0
	 */
	using http_methods_mapper_t = default_http_methods_t;

	//FIXME: document this!
	using connection_state_listener_t = connection_state::noop_listener_t;

	using timer_manager_t = Timer_Manager;
	using logger_t = Logger;
	using request_handler_t = Request_Handler;
	using strand_t = Strand;
	using stream_socket_t = Socket;
};

using noop_strand_t = asio_ns::executor;

//
// single_thread_traits_t
//

template <
		typename Timer_Manager,
		typename Logger,
		typename Request_Handler = default_request_handler_t >
using single_thread_traits_t =
	traits_t< Timer_Manager, Logger, Request_Handler, noop_strand_t >;

//
// default_traits_t
//

using default_traits_t = traits_t< asio_timer_manager_t, null_logger_t >;

/*!
 * \brief Default traits for single-threaded HTTP-server.
 *
 * Uses default timer manager. And null logger.
 *
 * Usage example:
 * \code
 * struct my_traits : public restinio::default_single_thread_traits_t {
 * 	using logger_t = my_special_single_threaded_logger_type;
 * };
 * \endcode
 *
 * \since
 * v.0.4.0
 */
using default_single_thread_traits_t = single_thread_traits_t<
		asio_timer_manager_t,
		null_logger_t >;

} /* namespace restinio */

