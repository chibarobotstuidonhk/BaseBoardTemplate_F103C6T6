#pragma once

#include <variant>
#include <utility>

#include <CRSLibtmp/utility.hpp>

namespace Ijector
{
	struct Error final
	{
		const char *const message{nullptr};

		explicit operator bool() const noexcept
		{
			return message;
		}
	};

	template<class T>
	class Result final
	{
		std::variant<T, Error> value;
		using Value = std::variant<T, Error>;

		public:
		Result()
		requires std::is_default_constructible_v<T>:
			value{std::in_place_index<0>}
		{}

		Result(const Result&) = default;
		Result(Result&&) = default;

		template<class U>
		Result(CRSLib::cvref_same<Result<U>> auto&& other):
			value{other.value.index() == 0 ? Value{std::in_place_index<0>, CRSLib::forward_like<decltype(other)>(std::get<0>(other.value))} : Value{std::in_place_index<1>, CRSLib::forward_like<decltype(other)>(std::get<1>(other.value))}}
		{}

		Result(const Error error):
			value{std::in_place_index<1>, error}
		{}

		Result(std::in_place_t, auto&& ... args):
			value{std::in_place_index<0>, std::forward<decltype(args)>(args)...}
		{}

		Result& operator=(const Result&) = default;
		Result& operator=(Result&&) = default;

		template<class U>
		Result& operator=(CRSLib::cvref_same<Result<U>> auto&& other)
		{
			value = other.value.index() == 0 ? Value{std::in_place_index<0>, CRSLib::forward_like<decltype(other)>(std::get<0>(other.value))} : Value{std::in_place_index<1>, CRSLib::forward_like<decltype(other)>(std::get<1>(other.value))};
			return *this;
		}

		Result& operator=(const Error error)
		{
			value = Value{std::in_place_index<1>, error};
			return *this;
		}

		explicit operator bool() const noexcept
		{
			return value.index() == 0;
		}

		auto operator->() const noexcept -> const T *
		{
			return std::get_if<0>(&value);
		}

		auto operator->() noexcept -> T *
		{
			return std::get_if<0>(&value);
		}

		auto operator*() const noexcept -> const T&
		{
			return std::get<0>(value);
		}

		auto operator*() noexcept -> T&
		{
			return std::get<0>(value);
		}
	};

	template<>
	class Result<void> final
	{
		std::variant<std::monostate, Error> value;
		using Value = std::variant<std::monostate, Error>;

		public:
		Result():
			value{std::in_place_index<0>}
		{}

		Result(const Result&) = default;
		Result(Result&&) = default;

		template<class U>
		Result(CRSLib::cvref_same<Result<U>> auto&& other):
			value{other.value.index() == 0 ? Value{std::in_place_index<0>} : Value{std::in_place_index<1>, CRSLib::forward_like<decltype(other)>(std::get<1>(other.value))}}
		{}

		Result(const Error error):
			value{std::in_place_index<1>, error}
		{}

		Result& operator=(const Result&) = default;
		Result& operator=(Result&&) = default;

		template<class U>
		Result& operator=(CRSLib::cvref_same<Result<U>> auto&& other)
		{
			value = other.value.index() == 0 ? Value{std::in_place_index<0>} : Value{std::in_place_index<1>, CRSLib::forward_like<decltype(other)>(std::get<1>(other.value))};
			return *this;
		}

		Result& operator=(const Error error)
		{
			value = Value{std::in_place_index<1>, error};
			return *this;
		}

		explicit operator bool() const noexcept
		{
			return value.index() == 0;
		}
	};
}