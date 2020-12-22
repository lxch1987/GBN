#pragma once
#ifndef STOP_WAIT_RDT_SENDER_H
#define STOP_WAIT_RDT_SENDER_H
#include "RdtSender.h"
class GBNRdtSender :public RdtSender
{
private:
	bool waitingState;				// �Ƿ��ڵȴ�Ack��״̬
	int nextseqnum;	// ��һ��������� 
	int base;						//�����
	int windowsize;//���ڴ�С
	int numsize;//���кŷ�Χ
	Packet *PacketsToSend;		//�ѷ��Ͳ��ȴ�Ack�����ݰ�
public:
	bool getWaitingState();
	bool send(const Message& message);						//����Ӧ�ò�������Message����NetworkServiceSimulator����,������ͷ��ɹ��ؽ�Message���͵�����㣬����true;�����Ϊ���ͷ����ڵȴ���ȷȷ��״̬���ܾ�����Message���򷵻�false
	void receive(const Packet& ackPkt);						//����ȷ��Ack������NetworkServiceSimulator����	
	void timeoutHandler(int seqNum);					//Timeout handler������NetworkServiceSimulator����
	void printwindow();
public:
	GBNRdtSender();
	virtual ~GBNRdtSender();
};
#endif

