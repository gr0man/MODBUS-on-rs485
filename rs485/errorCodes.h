/*
	Author:		R.Gorohov (K1 SOFT)

	errorCodes.h
	Объявления кодов ошибок.
*/

#pragma once

/** Коды ошибок библиотеки. */
enum ErrorCode
{
	OPERATION_COMPLETE=0,	// операция завершена успешно
	SYSTEM_ERROR,			// системная ошибка
	PORT_NOT_OPEN,			// порт не был открыт	
	PORT_ALREADY_OPEN,		// данный порт уже используется (открыт)
	PORT_IS_CLOSED,			// порт закрыт
	PORT_IS_BUSY,			// данный порт используется по другому назначению.
	WRONG_ADDRESS,			// передан неправильный адрес
	FRAME_DATA_ERROR,		// неправильный формат фрейма
	FRAME_ADDRESS_ERROR,	// принят фрейм, предназначенный для другого слейва
	CHARACTER_TIMEOUT,			// превышен таймаут получения символа
	RESPONSE_TIMEOUT, // превышен таймаут ожидания ответа от слейва
	BUFFER_SIZE_ERROR,		// переданный буфер слишком мал, чтобы вместить полученное сообщение
	LRC_ERROR				// неправильная LRC у принятого фрейма
};