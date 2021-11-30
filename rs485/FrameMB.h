/*
	Author:		R.Gorohov (K1 SOFT)

	FrameMB.h
	Объявление класса FrameMB.
*/

#pragma once

/** Инкапсулирует функционал фрейма MODBUS. */
class FrameMB
{
public:
	/** Конструктор. */
	FrameMB(BYTE address, const BYTE* data, int len);
	/** Конструктор. */
	FrameMB(const BYTE* data, int len);

	/** Деструктор. */
	virtual ~FrameMB(void);

	/** Возвращает указатель на буфер с ASCII фреймом. */
	const BYTE* GetAsciiFrame();
	/** Возвращает размер ASCII фрейма. */
	unsigned short GetAsciiFrameLen();

	/** Возвращает указатель на буфер с бинарными данными фрейма. */
	const BYTE* GetBinaryData();
	/** Возвращает размер буфера с бинарными данными. */
	unsigned short  GetBinaryLen();

	/** Возвращает адрес из поля адреса фрейма. */
	BYTE GetAddress();

	/** Проверяет LRC ASCII фрейма. */
	bool IsLRCValid(){ return isLRCValid_; };

	static bool IsASCIISymbolValid(BYTE ch);

private:
	/** Флаг правильности ASCII фрейма. */
	bool isLRCValid_;
	/** Таблица для конвертации банарных данных в ASCII. */
	static unsigned char s_hexTable[16];

	/** ASCII представление фрейма. */
	BYTE* asciiData_;
	/** Размер ASCII фрейма. */
	unsigned short asciiDataLen_;

	/** Бинарные данные. */
	BYTE* binData_;
	/** Размер буфера с бинарными данными. */
	unsigned short binDataLen_;

	/** Адрес в бинарном виде. */
	BYTE address_;

	/** Сбрасывает буфер с бинарными данными. */
	void FreeBinaryData();
	/** Сбрасывает буфер с ASCII представлением фрейма. */
	void FreeAsciiData();
	/** Выделяет буфер под бинарные данные. */
	void AllocBinaryData(unsigned short len);
	/** Выделяет буфер под ASCII фрейм. */
	void AllocAsciiData(unsigned short len);

	/** Выделяет бинарные данные из ASCII фрейма. */
	void SetBinaryDataFromAsciiFrame();
	/** Формирует ASCII фрейм из бинарных данных. */
	void SetAsciiFrameFromBinaryData();

	/** Конвертирует бинарный данные в ASCII представление. */
	void ConvertBin2Ascii(BYTE* dst, int dstLen, const BYTE* src, int srcLen);
	/** Конвертирует ASCII данные в бинарные. */
	void ConvertAscii2Bin(BYTE* dst, int dstLen, const BYTE* src, int srcLen);

	/** Вычисляет LRC заданного массива. */
	BYTE CalculateLRC(const BYTE* data, int len);

};
