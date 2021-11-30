/*
	Author:		R.Gorohov (K1 SOFT)

	BehaviorMB.h
	Объявление класса BehaviorMB.
*/

#pragma once

/** Базовый абстрактный класс поведения устройства MODBUS */
class BehaviorMB
{
public:
	/** Конструктор. */
	BehaviorMB(void);
	/** Деструктор. */
	virtual ~BehaviorMB(void);

	/** Устанавливает таймаут ожидания ответа. */
	virtual void SetResponceTimeout(int responceTimeout)=0;
	/** Фиксирует момент окончания пересылки фрейма. */
	virtual void FinishSend(BYTE address)=0;
	/** Сравнивает адрес полученного фрейма с адресом запрашиваемого устройства. */
	virtual bool IsAddressValid(BYTE address)=0;
	/** Проверяет не превышено ли время ожидания ответа. */
	virtual bool IsTimeoutExpiration()=0;
	/** Сбрасывает настройки. */
	virtual void ResetSettings()=0;
	/** Возвращает текущее системное время. */
	ULONGLONG GetCurrentTime();
	/** Проверяет, ожидается ли ответ в данное время. */
	virtual bool IsNeedToRead()=0;

	virtual void FinishReceive(BYTE address)=0;
	virtual void ReceiveTimeout()=0;

};
