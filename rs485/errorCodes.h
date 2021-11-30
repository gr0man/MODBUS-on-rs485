/*
	Author:		R.Gorohov (K1 SOFT)

	errorCodes.h
	���������� ����� ������.
*/

#pragma once

/** ���� ������ ����������. */
enum ErrorCode
{
	OPERATION_COMPLETE=0,	// �������� ��������� �������
	SYSTEM_ERROR,			// ��������� ������
	PORT_NOT_OPEN,			// ���� �� ��� ������	
	PORT_ALREADY_OPEN,		// ������ ���� ��� ������������ (������)
	PORT_IS_CLOSED,			// ���� ������
	PORT_IS_BUSY,			// ������ ���� ������������ �� ������� ����������.
	WRONG_ADDRESS,			// ������� ������������ �����
	FRAME_DATA_ERROR,		// ������������ ������ ������
	FRAME_ADDRESS_ERROR,	// ������ �����, ��������������� ��� ������� ������
	CHARACTER_TIMEOUT,			// �������� ������� ��������� �������
	RESPONSE_TIMEOUT, // �������� ������� �������� ������ �� ������
	BUFFER_SIZE_ERROR,		// ���������� ����� ������� ���, ����� �������� ���������� ���������
	LRC_ERROR				// ������������ LRC � ��������� ������
};