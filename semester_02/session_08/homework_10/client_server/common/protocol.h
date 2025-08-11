/// protocol.h
/// Данный файл protocol.h реализует протокол обмена данными между клиентом и сервером.

#ifndef INFORMATION_INTERACTION_PROTOCOL_H_IN_MY_PROJECT
#define INFORMATION_INTERACTION_PROTOCOL_H_IN_MY_PROJECT

#include <cstdint>
#include <limits>

#pragma pack(push, 1)

namespace protocol
{
	/// <summary>
	/// Константа идентификатора протокола.
	/// Используется для обозначения начала пакета данных.
	/// </summary>
	constexpr auto get_id() { return static_cast<uint16_t>(0xfffe); };

	/// <summary>
	/// Типы запросов к серверу.
	/// Определяют, какое действие требуется выполнить серверу.
	/// </summary>
	enum class RequestTypeMessages : uint8_t
	{
		GetTime = 0,  ///< Запрос текущего времени сервера
		GetFreeSpace, ///< Запрос размера данных
		GetStat,      ///< Запрос статистики сервера
		SendData,     ///< Отправка данных на сервер
		ClearData,    ///< Очистка данных на сервере
		Unknown = std::numeric_limits<uint8_t>::max()
	};

	/// <summary>
	/// Идентификаторы ошибок и статусов обработки сообщений.
	/// Используются для информирования клиента о результате обработки запроса.
	/// </summary>
	enum class StatusMessages : uint8_t
	{
		Succes = 0,           ///< Успешная обработка
		NoFreeSpace = 10,     ///< Недостаточно свободного места
		NoConnectToHost = 20, ///< Ошибка подключения к хосту
		NoImplFunc = 30       ///< Функционал не реализован
	};

	/// <summary>
	/// Формат служебного заголовока пакета.
	/// Содержит информацию о размере данных, идентификаторе пакета, типе сообщения и статусе обработки.
	/// </summary>
	struct ServiceHeader
	{
		uint16_t idHead{ get_id() };                                 ///< Идентификатор
		RequestTypeMessages typeMsg{ RequestTypeMessages::Unknown }; ///< Идентификатор типа данных
		StatusMessages statusMsg{ StatusMessages::Succes };          ///< Статус обработки сообщения
		uint32_t sizeMsg{};                                          ///< Размер данных, байт
	};

	/// <summary>
	/// Формат статистики сервера.
	/// Содержит информацию о количестве переданных и принятых данных, времени работы и количестве клиентов.
	/// </summary>
	struct StatServer
	{
		uint64_t bytesReceived{};      ///< Принято байт
		uint64_t bytesTransferred{};   ///< Передано байт
		uint64_t packetsReceived{};    ///< Принято пакетов
		uint64_t packetsTransferred{}; ///< Передано пакетов
		uint64_t serverUpTime{};       ///< Время работы сервера в секундах
		uint64_t connectedClients{};   ///< Количество подключенных клиентов
	};
} /// namespace protocol

#pragma pack(pop)

#endif // INFORMATION_INTERACTION_PROTOCOL_H_IN_MY_PROJECT
