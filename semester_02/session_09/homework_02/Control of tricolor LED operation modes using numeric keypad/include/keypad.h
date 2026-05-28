// keypad.h

#ifndef KEYPAD_H_FOR_THE_ARDUINO_UNO_PROJECT
#define KEYPAD_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "callbacks_table.h"
#include "events.h"
#include "fox.h"
#include "hardware.h"

namespace uno
{
template <typename K> class keypad
{
public:
	using pointer_to_callback_function = typename uno::table_of_callback_functions<uno::key_events>::pointer_to_callback_function;
	using table_of_pointers_to_callback_function = uno::table_of_callback_functions<uno::key_events>;

	using event = uno::key_events;
	using elem_row = uno::electronic_component<uno::signal_digital, uno::signal_direction::output>;
	using elem_col = uno::electronic_component<uno::signal_digital, uno::signal_direction::input_pullup>;

	constexpr keypad() = default;
	/**
	 * @brief Construct a new keypad object
	 *
	 * @tparam Binds
	 * @param binds
	 */
	template <typename... Binds> explicit keypad(Binds&&... binds)
	{
		fox::expand_parameter_package{(construct(binds.first, binds.second), 1)...};
	}
	keypad(const keypad&) = default;
	keypad(keypad&&) = default;
	~keypad() = default;

	keypad& operator=(const keypad&) = default;
	keypad& operator=(keypad&&) = default;

	/**
	 * @brief
	 *
	 * @tparam Binds
	 * @param binds
	 */
	template <typename... Binds> void assign_names(Binds... binds)
	{
		static_assert(sizeof...(Binds) <= K::number_of_rows * K::number_of_columns, "Number of elements shall not exceed sixteen.");
		fox::expand_parameter_package{(assign(binds), 1)...};
	}

	void key_down(const pointer_to_callback_function action) { _handlers.add(event::down, action); }
	uint8_t get_key() const { return _key_id; }

	/**
	 * @brief
	 *
	 */
	void update()
	{
		static auto prev_key_id = _key_id;
		for (uint8_t row{}; row < K::number_of_rows; ++row)
		{
			_channels.first[row].write(uno::signal_digital::low);
			for (uint8_t col{}; col < K::number_of_columns; ++col)
			{
				if (!_channels.second[col].read())
				{
					if (_handlers.call(event::down))
					{
						_key_id = _keys[row][col];
						_handlers.call(event::down)();
					}
				}
			}
			_channels.first[row].write(uno::signal_digital::high);
		}
		if (_key_id != prev_key_id)
			prev_key_id = _key_id;
	}

private:
	/**
	 * @brief
	 *
	 * @param pin_number
	 * @param bind
	 */
	void construct(uint8_t pin_number, uint8_t bind)
	{
		if (K::unpack_dir(bind) == K::id::row)
		{
			_channels.first[K::unpack_num(bind)] = elem_row{pin_number};
			_channels.first[K::unpack_num(bind)].write(uno::signal_digital::high);
		}
		else
			_channels.second[K::unpack_num(bind)] = elem_col{pin_number};
	}
	/**
	 * @brief
	 *
	 * @param value
	 */
	void assign(uint8_t value)
	{
		static fox::pair<uint8_t, uint8_t> number{};
		_keys[number.first][number.second++] = value;

		if (number.second == K::number_of_columns)
		{
			number.second = 0u;
			++number.first;
		}
	}

	table_of_pointers_to_callback_function _handlers{};
	fox::pair<elem_row[K::number_of_rows], elem_col[K::number_of_columns]> _channels{};
	uint8_t _keys[K::number_of_rows][K::number_of_columns]{};
	uint8_t _key_id{static_cast<uint8_t>(-1)};
};
} // namespace uno
#endif // !KEYPAD_H_FOR_THE_ARDUINO_UNO_PROJECT