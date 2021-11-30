/*
	Author:		R.Gorohov (K1 SOFT)

	Rs485Manager.cpp
	Определение класса Rs485Manager.
*/

#include "StdAfx.h"
#include "Rs485Manager.h"

#include "PortRS485.h"
#include "MasterMB.h"
#include "SlaveMB.h"

#include "errorCodes.h"

/**
 *	Конструктор.
 */
Rs485Manager::Rs485Manager(void)
{

}

/**
 *	Деструктор.
 */
Rs485Manager::~Rs485Manager(void)
{
}

/**
 *	Открывает заданный COM-порт.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::OpenRs485(LPCTSTR portName)
{
	DWORD ret;

	comPortManager_.Lock();

	if( comPortManager_.IsExist(portName) )
	{
		ret = PORT_ALREADY_OPEN;
	}
	else
	{
		if(masterMbManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
		else
			ret = comPortManager_.Open(portName);
	}

	comPortManager_.Unlock();

	return ret;
}


/**
 *	Закрывает заданный COM-порт.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::CloseRs485(LPCTSTR portName)
{
	DWORD ret;

	comPortManager_.Lock();

	ret = comPortManager_.Close(portName);
	if( ret == PORT_NOT_OPEN )
	{
		if( masterMbManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
	}

	comPortManager_.Unlock();

	return ret;

}

/**
 *	Читает из COM-порта данные.
 *	\param portName имя порта.
 *	\param data указатель на буфер для приема данных.
 *	\param len размер приемного буфера.
 *	\param nmbRead указатель на переменную, в котолрой сохраняется количество прочитанных байт.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::ReadRs485(LPCTSTR portName, BYTE* data, int len, int* nmbRead)
{
	DWORD ret = comPortManager_.Read(portName, data, len, nmbRead);
	if( ret == PORT_NOT_OPEN )
	{
		if( masterMbManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
	}

	return ret;
}

/**
 *	Записывает в COM-порт данные. 
 *	\param portName имя порта.
 *	\param data указатель на данных для записи.
 *	\param len размер буфера.
 *	\param nmbRead указатель на переменную, в котолрой сохраняется количество записанных байт.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::WriteRs485(LPCTSTR portName, const BYTE* data, int len, int* nmbWritten)
{
	DWORD ret = comPortManager_.Write(portName, data, len, nmbWritten);
	if( ret == PORT_NOT_OPEN )
	{
		if( masterMbManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			return PORT_IS_BUSY;
	}

	return ret; 
}

/**
 *	Устанавливает настройки COM-порта.
 *	\param portName имя порта.
 *	\param baudrate скорость обмена.
 *	\param byteSize размер байта в битах.
 *	\param stopBits количество стоповых бит.
 *	\param parity контроль четности.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SettingsRs485(LPCTSTR portName, DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity)
{
	return comPortManager_.Settings(portName, baudrate, byteSize, stopBits, parity);
}

/**
 *	Устанавливает таймаут чтения (записи) COM-порта.
 *	\param portName имя порта.
 *	\param timeout таймаут чтения (записис) порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SetTimeoutRs485(LPCTSTR portName, int timeout)
{
	return comPortManager_.SetTimeout(portName, timeout);
}

/**
 *	Запускает мастер MODBUS.
 *	\param portName имя порта.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\param responceTimeout время ожидания ответа от слейва.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::StartMaster(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout)
{
	DWORD ret;

	masterMbManager_.Lock();

	if( masterMbManager_.IsExist(portName) )
	{
		ret = PORT_ALREADY_OPEN;
	}
	else
	{
		if( comPortManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
		else
			ret = masterMbManager_.Start(portName, baudrate, parity, charTimeout, responceTimeout);
	}

	masterMbManager_.Unlock();

	return ret;
}

/**
 *	Посылает unicast-запрос мастера MODBUS.
 *	\param portName имя порта.
 *	\param address адрес получателя.
 *	\param data указатель на массив с данными.
 *	\param dataLen размер массива в байтах.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SendRequestMaster(LPCTSTR portName, BYTE address, const BYTE* data, int dataLen)
{
	LOG_Info("SendRequestMasterMB on port \"%s\"\n", portName);

	DWORD ret = masterMbManager_.SendRequest(portName, address, data, dataLen);
	if( ret == PORT_NOT_OPEN )
	{
		if( comPortManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			return PORT_IS_BUSY;
	}

	LOG_Info("SendRequestMasterMB complete\n");
	return ret; 
}

/**
 *	Устанавливает настройки мастера MODBUS.
 *	\param portName имя порта.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SettingsMaster(LPCTSTR portName, DWORD baudrate, BYTE parity)
{
	DWORD ret = masterMbManager_.Settings(portName, baudrate, parity);
	if( ret == PORT_NOT_OPEN )
	{
		if( comPortManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			return PORT_IS_BUSY;
	}

	return ret;
}

/**
 *	Устанавливает таймауты мастера MODBUS.
 *	\param portName имя порта.
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\param responceTimeout время ожидания ответа от слейва.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SetTimeoutsMaster(LPCTSTR portName, int charTimeout, int responceTimeout)
{
	DWORD ret = masterMbManager_.SetTimeouts(portName, charTimeout, responceTimeout);

	if( ret == PORT_NOT_OPEN )
	{
		if( comPortManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			return PORT_IS_BUSY;
	}

	return ret;
}

/**
 *	Завершает работу мастера MODBUS.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::StopMaster(LPCTSTR portName)
{
	DWORD ret;
	
	masterMbManager_.Lock();

	ret = masterMbManager_.Stop(portName);
	if( ret == PORT_NOT_OPEN )
	{
		if( comPortManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
	}

	masterMbManager_.Unlock();

	return ret;
}


/**
 *	Инициализирует указатель на callback-функцию получения ответа от слейва.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SetReceiveMasterCallback(pReceiveMasterMBCallback callback)
{
	return masterMbManager_.SetReceiveCallback(callback);
}

/**
 *	Инициализирует указатель на callback-функцию истечения таймаута ответа слейва.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SetTimeoutMasterCallback(pTimeoutMasterMBCallback callback)
{
	return masterMbManager_.SetTimeoutCallback(callback);
}

/**
 *	Инициализирует указатель на callback-функцию окончания пересылки данных мастером MODBUS.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SetSendMasterCallback(pSendMasterMBCallback callback)
{
	return masterMbManager_.SetSendCallback(callback);
}


/**
 *	Запускает слейв MODBUS.
 *	\param portName имя порта.
 *	\param adddress адрес слейва.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::StartSlave(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout)
{
	DWORD ret;

	slaveMbManager_.Lock();

	if( slaveMbManager_.IsExist(portName) )
	{
		ret = PORT_ALREADY_OPEN;
	}
	else
	{
		if( comPortManager_.IsExist(portName) || masterMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
		else
			ret = slaveMbManager_.Start(portName, address, baudrate, parity, charTimeout);
	}

	slaveMbManager_.Unlock();

	return ret;
}

/**
 *	Посылает ответ на запрос мастера MODBUS.
 *	\param portName имя порта.
 *	\param data указатель на массив с данными.
 *	\param dataLen размер массива в байтах.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SendReplySlave(LPCTSTR portName, const BYTE* data, int dataLen)
{
	DWORD ret = slaveMbManager_.SendReply(portName, data, dataLen);
	if( ret == PORT_NOT_OPEN )
	{
		if( comPortManager_.IsExist(portName) || masterMbManager_.IsExist(portName) )
			return PORT_IS_BUSY;
	}

	return ret;
}

/**
 *	Завершает работу слейва MODBUS.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::StopSlave(LPCTSTR portName)
{
	DWORD ret;

	slaveMbManager_.Lock();

	ret = slaveMbManager_.Stop(portName);
	if( ret == PORT_NOT_OPEN )
	{
		if( comPortManager_.IsExist(portName) || masterMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
	}

	slaveMbManager_.Unlock();

	return ret;
}

/**
 *	Устанавливает настройки слейва MODBUS.
 *	\param portName имя порта.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SettingsSlave(LPCTSTR portName, DWORD baudrate, BYTE parity)
{
	return slaveMbManager_.Settings(portName, baudrate, parity);
}

/**
 *	Устанавливает таймаут на чтение/запись символа.
 *	\param portName имя порта.
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SetTimeoutSlave(LPCTSTR portName, int charTimeout)
{
	return slaveMbManager_.SetTimeout(portName, charTimeout);
}

/**
 *	Инициализирует указатель на callback-функцию получения запроса от мастера MODBUS.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SetReceiveSlaveCallback(pReceiveSlaveMBCallback callback)
{
	return slaveMbManager_.SetReceiveCallback(callback);
}

/**
 *	Инициализирует указатель на callback-функцию окончания пересылки данных слейвом MODBUS.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD Rs485Manager::SetSendSlaveCallback(pSendSlaveMBCallback callback)
{
	return slaveMbManager_.SetSendCallback(callback);
}


/**
 *	Возвращает код системной ошибки RS485.
 *	\param portName имя порта.
 *	\return код системной ошибки.
 */
DWORD Rs485Manager::GetSystemErrorRs485(LPCTSTR portName)
{
	return comPortManager_.GetSystemError(portName);
}

/**
 *	Возвращает код системной ошибки мастера MODBUS.
 *	\param portName имя порта.
 *	\return код системной ошибки.
 */
DWORD Rs485Manager::GetSystemErrorMaster(LPCTSTR portName)
{
	return masterMbManager_.GetSystemError(portName);
}

/**
 *	Возвращает код системной ошибки слейва MODBUS.
 *	\param portName имя порта.
 *	\return код системной ошибки.
 */
DWORD Rs485Manager::GetSystemErrorSlave(LPCTSTR portName)
{
	return slaveMbManager_.GetSystemError(portName);
}

