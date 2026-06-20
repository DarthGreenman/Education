/// filters.h

#ifndef FILTERS_H_FOR_THE_ARDUINO_UNO_PROJECT
#define FILTERS_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <stdint.h>

namespace uno {
	namespace filter {
		enum class reset : uint8_t { hard, soft };
		/****************************************************************************************************
		 * @brief Универсальный цифровой интегратор для фильтрации дребезга контактов.
		 * @note Архитектурные особенности фильтрации:
		 * 1. МЯГКАЯ. Счетчик инкрементируется/декрементируется пошагово. Алгоритм устойчив к естественному
		 * механическому дребезгу, но в условиях сильных циклических наводок (помех) может медленно накопить
		 * значение до предела и вызвать ложное срабатывание.
		 *
		 * 2. ЖЕСТКАЯ, для защиты от радиопомех. В этом случае событие активируется строго при получении пакета
		 * одинаковых сигналов подряд, без единого разрыва.
		 *
		 * @tparam Limit Порог фильтрации (количество тиков для стабилизации)
		 * @tparam Mode Режим сброса счетчика при обнаружении шума (reset::soft или reset::hard)
		 */
		template <uint8_t Limit = 5, reset Mode = reset::soft>
		struct integrator {
			uint8_t count{};
			bool operator()(uint8_t signal_value, uint8_t sample_signal_value, bool& state) noexcept {
				if(signal_value == sample_signal_value) {
					if(count < Limit) count++;
				} else {
					if constexpr(Mode == reset::hard) {
						count = 0; /// Жесткий режим: сброс в ноль
					} else {
						if(count > 0) count--; /// Мягкий режим: накопительный декремент
					}
				}
				/// Проверка фиксации стабильного состояния
				if(count == Limit && !state) {
					state = true;
					return true;
				} else if(count == 0 && state) {
					state = false;
					return true;
				}
				return false;
			}
		};

		// ... (после закрывающей фигурной скобки структуры integrator на странице 2)

		/****************************************************************************************************
		 * @brief Стратегия фильтрации: Гистерезис (Зона нечувствительности).
		 * @details Обрезает мелкие колебания и шум АЦП. Идеально для меню и настроек.
		 * @tparam Threshold Порог срабатывания в единицах АЦП (мертвая зона).
		 */
		template <uint16_t Threshold = 3>
		struct hysteresis {
			uint16_t last_stable_value{};

			bool operator()(uint16_t signal_value, uint16_t& out_signal_value) noexcept {
				const auto diff{ (signal_value > last_stable_value)
					? (signal_value - last_stable_value)
					: (last_stable_value - signal_value)
				};
				if(diff >= Threshold) {
					last_stable_value = signal_value;
					out_signal_value = signal_value;
					return true; // Значение изменилось существенно
				}
				return false;
			}
		};

		/****************************************************************************************************
		 * @brief Стратегия фильтрации: Экспоненциальное бегущее среднее (EMA).
		 * @details Плавное сглаживание сигнала. Реализовано полностью в целых числах с фиксированной точкой
		 *          (Zero-float) для экономии тактов процессора AVR.
		 * @tparam Shift Коэффициент сглаживания (битовый сдвиг масштаба, чем больше - тем плавнее).
		 */
		template <uint8_t Shift = 2>
		struct ema {
			uint32_t smoothed_accumulator{};
			uint16_t last_output{};

			bool operator()(uint16_t signal_value, uint16_t& out_signal_value) noexcept {
				if(smoothed_accumulator == 0) {
					smoothed_accumulator = static_cast<uint32_t>(signal_value) << Shift;
				}

				// Быстрая целочисленная математика EMA
				smoothed_accumulator = smoothed_accumulator + signal_value - (smoothed_accumulator >> Shift);
				const auto current_output = static_cast<uint16_t>(smoothed_accumulator >> Shift);

				if(current_output != last_output) {
					last_output = current_output;
					out_signal_value = current_output;
					return true; // Сглаженное значение изменилось
				}
				return false;
			}
		};


	} /// namespace filter
} /// namespace uno

#endif /// !FILTERS_H_FOR_THE_ARDUINO_UNO_PROJECT
