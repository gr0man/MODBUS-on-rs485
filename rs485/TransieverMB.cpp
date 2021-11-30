/*
	Author:		R.Gorohov (K1 SOFT)

	TransieverMB.cpp
	����������� ������ TransieverMB.
*/

#include "StdAfx.h"
#include "TransieverMB.h"
#include "BehaviorMB.h"
#include "FrameMB.h"
#include "rs485.h"

#include "ErrorCodes.h"

#include "rs485.h"

/**
 *	�����������.
 *	\param portName ��� �����.
 *	\param baudrate ������� ������.
 *	\param parity �������� ��������.
 *	\param charTimeout ������� ������ (������) �������.
 *	\param behavior ��������� �������� �������.
 *	\param processor ���������� ������.
 */
TransieverMB::TransieverMB(LPCTSTR portName,
						   DWORD baudrate,
						   BYTE parity,
						   int charTimeout,
						   BehaviorMB& behavior,
						   CmdProcessor& processor)
: comPort_(portName)
, behavior_(behavior)
, processor_(processor)
, workThread_(NULL)
, frameForSend_(0)
, hStopWorkThreadEvent_(CreateEvent(NULL, TRUE, FALSE, NULL))
{
	Start(baudrate, parity, charTimeout);
}

/**
 *	����������.
 */
TransieverMB::~TransieverMB(void)
{
	//LOG_Info("TransieverMB destructor on port \"%s\"\n", GetPortName().c_str());
	//Stop();
	LOG_Info("TransieverMB destructor complete on port \"%s\"\n", GetPortName().c_str());
}

/**
 *	��������� ������� ������ ���������� � ����� MODBUS.
 *	\param baudrate ������� ������.
 *	\param parity �������� ��������.
 *	\param charTimeout ������� ������ (������) �������.
 */
void TransieverMB::Start(DWORD baudrate, BYTE parity, int charTimeout)
{
	if( Settings(baudrate, parity) != OPERATION_COMPLETE )
		throw comPort_.GetSystemErrorCode();


	if( (workThread_=CreateThread(NULL, 0, TransieverMB::WorkThread, this, 0, NULL)) == NULL )
	{
		throw GetLastError();
	}
}

/**
 *	������������� ������� ������ ���������� � ����� MODBUS.
 */
void TransieverMB::Stop()
{
	comPort_.Close();

	if( WaitForSingleObject(hStopWorkThreadEvent_, 5000) == WAIT_TIMEOUT )
	{
		//LOG_Info("TransieverMB wait close on port \"%s\" timeout\n", GetPortName().c_str());
	}
	
	if( frameForSend_ )
	{
		delete frameForSend_;
		frameForSend_ = 0;
	}

	LOG_Info("TransieverMB closed on port \"%s\"\n", GetPortName().c_str());
}

/**
 *	������������� ��������� COM-�����.
 *	\param baudrate ������� ������.
 *	\param parity �������� ��������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD TransieverMB::Settings(DWORD baudrate, BYTE parity)
{
	return comPort_.Settings(baudrate, 7, (parity == NOPARITY) ? TWOSTOPBITS : ONESTOPBIT, parity);
}

/**
 *	������� ������� � ������� ��� �������� � ��������� � �������.
 *	\param address ����� ����������.
 *	\param data ��������� �� ������ � �������.
 *	\param dataLen ������ �������.
 *	\return ��� ������ (������ OPERATION_COMPLETE).
 */
DWORD TransieverMB::NewSend(BYTE address, const BYTE* data, int dataLen)
{
	DataCommand* dataCmd = new DataCommand(comPort_.GetPortName(), address, data, dataLen);
	AddToSend(dataCmd);

	return OPERATION_COMPLETE;
}

/**
 *	������������� ��������.
 *	\param charTimeout ������� ������ (������) �������.
 *	\param responceTimeout ������� �������� ������ �� ������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD TransieverMB::SetTimeouts(int charTimeout, int responceTimeout)
{
	behavior_.SetResponceTimeout(responceTimeout);
	return SetTimeout(charTimeout);
}

/**
 *	������� ����� �������� ������.
 */
DWORD TransieverMB::WorkThread(LPVOID lpParam)
{
	TransieverMB* master = reinterpret_cast<TransieverMB*>(lpParam);

	LOG_Info("InitSend 1\n");

	master->InitSend();
	master->InitRead();

	try
	{
		for(;;)
		{
			master->Send();
			
			if( master->IsNeedToRead() )
			{
				DataCommand* dataCmd = master->Receive();
				if( dataCmd )
				{
					master->AddRecCmd(dataCmd);
					master->ProcessRecCommands();
				}
			}
		}
	}
	catch( ErrorCommand* cmd )
	{
		LOG_Info("TrancieverMaster::WorkThread catch ErrorCommand\n");
		// ���������� ������
		delete cmd;
	}
	catch(...)
	{

	}


	SetEvent(master->hStopWorkThreadEvent_);

	return 0;
}

/**
 *	���������� ��������� ������.
 */
void TransieverMB::Send()
{
	if( comPort_.IsClosed(0) )
	{
		ErrorCommand* errCmd = new ErrorCommand(GetPortName(), 0, PORT_IS_CLOSED); 
		LOG_Info("TransieverMB::Send: ComPort is Closed\n");
		throw errCmd;
	}

	if( frameForSend_ == 0 )
	{
		if( !sendQueue_.IsEmpty() )
		{
			DataCommand* dataCmd = reinterpret_cast<DataCommand*>(sendQueue_.GetFront());
			frameForSend_ = new FrameMB(dataCmd->GetAddress(), dataCmd->GetBuffer(), dataCmd->GetLen());
			sendQueue_.PopFront();
			delete dataCmd;
		}
	}

	if( frameForSend_ )
	{
		// ���� ���������
		for(;;)
		{
			int written = 0;
			BYTE transByte = frameForSend_->GetAsciiFrame()[sendCounter_];
			DWORD err = comPort_.Write(&transByte, 1, &written);
			if( err == OPERATION_COMPLETE )
			{
				sendCounter_++;
				if( sendCounter_ == frameForSend_->GetAsciiFrameLen() )
				{
					LOG_Info("End of sending frame\n");				
					// ��������� ���������
					SendCommand* sendCmd = new SendCommand(comPort_.GetPortName(), frameForSend_->GetAddress());
					AddRecCmd(sendCmd);

					// ����������� ����� ��������� ��������.
					behavior_.FinishSend(frameForSend_->GetAddress());

					LOG_Info("InitSend 2\n");
					InitSend();

					break;
				}
			}
			else
			{
				ProcessError(err);
			}
		}
	}
}

/**
 *	��������� ��������� ������.
 *	\return ��������� �� ������� � ������� � ������ ��������� ������ ������ ��� 0. 
 */
DataCommand* TransieverMB::Receive()
{
	//LOG_Info("TransieverMB on port receive start\n");

	DWORD err = OPERATION_COMPLETE;

	DataCommand* dataCmd = 0;

	//if( behavior_.IsNeedToRead() )
	{
		for(;;)
		{
			// �������� ��������
			if( behavior_.IsTimeoutExpiration() )
			{
				err = RESPONSE_TIMEOUT;

				LOG_Info("TransieverMB::Receive break on timeout\n");

				break;
			}

			dataCmd = ReceiveSymbol(err);
			ProcessError(err);

			if( dataCmd )
			{
				LOG_Info("TransieverMB::Receive break on receive dataCommand\n");
				break;
			}

			if( err == CHARACTER_TIMEOUT )
			{
				LOG_Info("TransieverMB::Receive break on char timeout\n");
				break;
			}
		}

	}

	if(err)
		ProcessError(err);

	LOG_Info("TransieverMB on port \"%s\" receive end\n", GetPortName().c_str());
	return dataCmd;
}


DataCommand* TransieverMB::ReceiveSymbol(DWORD& errCode)
{
	DataCommand* dataCmd = 0;

	int read = 0;
	BYTE rd;
	errCode = comPort_.Read(&rd, 1, &read );
	if( errCode == OPERATION_COMPLETE && read == 1 )
	{
		// �������� �������
		switch( rd )
		{
		case ':': // ������ ���������
			LOG_Info("TransieverMB::Receive ':'\n");

			recCounter_ = 0;
			recBuffer_[recCounter_] = ':';
			recCounter_++;

			break;
	
		case '\r': // ������ ����������� ������
			LOG_Info("TransieverMB::Receive '\\r'\n");
		
			if( recCounter_ > 4 )
			{
				recBuffer_[recCounter_] = rd;
				recCounter_++;
			}

			break;

		case '\n': // ����� ���������
			{
				LOG_Info("TransieverMB::Receive '\\n' on port \"%s\"\n", GetPortName().c_str());

				if( recCounter_ < 5 )
				{
					errCode = FRAME_DATA_ERROR;
					LOG_Info("TransieverMB::Receive cmall frame\n");
						
					InitRead();
					break;
				}

				if( recBuffer_[recCounter_-1] != '\r' )
				{
					errCode = FRAME_DATA_ERROR;
					LOG_Info("TransieverMB::Receive FRAME_DATA_ERROR\n");

					InitRead();
					break;
				}

				recBuffer_[recCounter_] = rd;
				recCounter_++;

				if( recCounter_ < 9 )
				{
					errCode = FRAME_DATA_ERROR;
					LOG_Info("TransieverMB::Receive cmall frame\n");
					InitRead();
					break;
				}

				FrameMB frame(recBuffer_, recCounter_);
	
				// �������� ������
				if( !behavior_.IsAddressValid(frame.GetAddress()) )
				{
					errCode = FRAME_ADDRESS_ERROR;
					LOG_Info("TransieverMB::Receive FRAME_ADDRESS_ERROR (\"%s\")\n", GetPortName().c_str());
					InitRead();
					break;
				}

				// �������� LRC
				if( !frame.IsLRCValid() )
				{
					errCode = LRC_ERROR;
					LOG_Info("TransieverMB::Receive LRC_ERROR\n");
					InitRead();
					break;
				}

				LOG_Info("TransieverMB::Receive on port \"%s\" complete\n", GetPortName().c_str());
				dataCmd = new DataCommand(comPort_.GetPortName(), frame.GetAddress(), frame.GetBinaryData(), frame.GetBinaryLen());

				behavior_.FinishReceive(frame.GetAddress());
	
				errCode = OPERATION_COMPLETE;
				InitRead();

				break;
			}
			break;

		default:
			if( recCounter_ > 0 )
			{
				if( FrameMB::IsASCIISymbolValid(rd) )
				{
					LOG_Error("TransieverMB::Receive '%x'\n",rd);
					recBuffer_[recCounter_] = rd;
					recCounter_++;
				}
				else
				{
					LOG_Info("TransieverMB::Receive invalid symbol\n");
				}
			}
			break;
		}

		if( recCounter_ > sizeof(recBuffer_) )
		{
			LOG_Info("TransieverMB::Receive FRAME_DATA_ERROR\n");
			errCode = FRAME_DATA_ERROR;
		}
	}
	else if( errCode == CHARACTER_TIMEOUT ) // �������� �������
	{
		//LOG_Info("TransieverMB::Receive: CHARACTER_TIMEOUT");
		InitRead();
	}

	return dataCmd;

}

/**
 *	������������ �������� �������.
 */
void TransieverMB::ProcessRecCommands()
{
	while( !recQueue_.IsEmpty() )
	{
		LOG_Info("TransieverMB on port process rec: ");

		Command* cmd = recQueue_.GetFront();
		recQueue_.PopFront();

#ifdef USE_LOG
		LOG_Info("\"%s\"\n", cmd->GetName());
#endif

		cmd->Process(&processor_);
		delete cmd;
	}
}

/**
 *	��������� ������� � �������� �������.
 *	\param cmd ��������� �� ����������� �������.
 */
void TransieverMB::AddRecCmd(Command* cmd)
{
	//LOG_Info("TransieverMB on port \"%s\" receive command ", GetPortName().c_str());
	//LOG_Info(" \"%s\"", cmd->GetName());
	recQueue_.PushBack(cmd);
}

/**
 *	�������������� ����� ��������.
 */
void TransieverMB::InitSend()
{
	if( frameForSend_ )
	{
		LOG_Info("TransieverMB on port \"%s\" init send\n", GetPortName().c_str());

		delete frameForSend_;
		frameForSend_ = 0;
	}

	sendCounter_ = 0;
}

/**
 *	�������������� ����� �����.
 */
void TransieverMB::InitRead()
{
	//LOG_Info("TransieverMB on port \"%s\" init read\n", GetPortName().c_str());
	recCounter_ = 0;
}

/** 
 *	������������ ��������� ������ ������.
 *	\param err ��� ������.
 */
void TransieverMB::ProcessError(DWORD err)
{
	if( err == OPERATION_COMPLETE )
		return;

	LOG_Error("TransieverMB process error: %u\n", err);
	//LOG_Info("on port \"%s\"\n", GetPortName().c_str());

	if( err == SYSTEM_ERROR )
	{
		if( GetSystemErrorCode() == ERROR_INVALID_HANDLE )
		{
			//LOG_Info("INVALID_HANDLE_VALUE on port \"%s\"\n", GetPortName().c_str());
			ErrorCommand* cmd = new ErrorCommand(comPort_.GetPortName(), 0, err);
			throw cmd;
		}
		else
		{
			LOG_Error("SYSTEM_ERROR = %u\n", GetSystemErrorCode());
		}
	}
	else if( err == PORT_IS_CLOSED )
	{
		//LOG_Info("Port Is Closed \"%s\"\n", GetPortName().c_str());
		ErrorCommand* cmd = new ErrorCommand(comPort_.GetPortName(), 0, err);
		throw cmd;
	}
}

/**
 *	��������� ������� � ������� ��� ���������.
 *	\param cmd ��������� �� ����������� �������.
 */
void TransieverMB::AddToSend(DataCommand *cmd)
{
	//LOG_Info("TransieverMB on port \"%s\" add to send cpmmand\n", GetPortName().c_str());

	sendQueue_.PushBack(cmd);
}

bool TransieverMB::IsNeedToRead()
{
	if( behavior_.IsNeedToRead() )
		LOG_Info("need read on \"%s\"", GetPortName().c_str());

	return behavior_.IsNeedToRead();
}