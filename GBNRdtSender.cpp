#include "stdafx.h"
#include "Global.h"
#include "GBNRdtSender.h"
#include <cmath>
GBNRdtSender::GBNRdtSender() :waitingState(false), nextseqnum(0), base(0), windowsize(4), numsize(8), PacketsToSend(new Packet[windowsize])
{
}
GBNRdtSender::~GBNRdtSender()
{
	//delete PacketsToSend;
}
void GBNRdtSender::printwindow() {
	printf("��ǰ����״̬��");
	printf("[ ");
	for (int i = 0;i < numsize;i++) {
		if (i == base) printf("%c%d ",'(',i);
		else if (i == (base+windowsize-1)%numsize) printf("%d%c ",i,')');
		else printf("%d ",i);
	}
	printf("]\n");
}
bool GBNRdtSender::getWaitingState() {
	return waitingState;
}
bool GBNRdtSender::send(const Message& message) {
	if (abs(nextseqnum-base) < windowsize) {
		this->PacketsToSend[nextseqnum % windowsize].acknum = -1; //���Ը��ֶ�
		this->PacketsToSend[nextseqnum % windowsize].seqnum = this->nextseqnum;//���ݰ������
		memcpy(this->PacketsToSend[nextseqnum % windowsize].payload, message.data, sizeof(message.data));//������Ӧ�ò������д�����ݰ���
		this->PacketsToSend[nextseqnum % windowsize].checksum = pUtils->calculateCheckSum(this->PacketsToSend[nextseqnum % windowsize]);
		pUtils->printPacket("���ͷ����ͱ���", this->PacketsToSend[nextseqnum % windowsize]);
		pns->sendToNetworkLayer(RECEIVER, this->PacketsToSend[nextseqnum % windowsize]);								//����ģ�����绷����sendToNetworkLayer��ͨ������㷢�͵��Է�
		if (base == nextseqnum) {
			//�������ͷ���ʱ��
			pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
		}
		nextseqnum = (nextseqnum+1) % numsize;
		if (abs(nextseqnum - base) >= windowsize) waitingState = true;//������������������ȴ�״̬��Ӧ�ò�ֹͣ������㷢������
		return true;//�������ݳɹ�
	}
	else {
		waitingState = true;//������������������ȴ�״̬��Ӧ�ò�ֹͣ������㷢������
		return false;//��������ʧ��
	}
}
void GBNRdtSender::receive(const Packet& ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	if (checkSum == ackPkt.checksum) {//���У����Ƿ���ȷ
		pUtils->printPacket("���ͷ���ȷ�յ�ȷ��", ackPkt);
		base = (ackPkt.acknum + 1)%numsize;//����baseΪ�ۼ�ȷ�Ϻ�+1
		printwindow();
		if (base == nextseqnum) {//���������Ѿ��������
			pns->stopTimer(SENDER, 0);		//�رն�ʱ��
		}
		else {
			pns->stopTimer(SENDER, 0);		//�رն�ʱ��
			pns->startTimer(SENDER, Configuration::TIME_OUT, 0);//���¿�ʼ��ʱ
		}
	}
	if (abs(nextseqnum - base) < windowsize) waitingState = false;//���º�Ĵ���δ��
}
void GBNRdtSender::timeoutHandler(int seqNum) {
	//Ψһһ����ʱ��,���迼��seqNum
	pns->stopTimer(SENDER, 0);		//�رն�ʱ��
	pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
	for (int i = base;i != nextseqnum;i = (i+1)%numsize) {//����N�����·���
		pUtils->printPacket("���ͷ���ʱ��ʱ�䵽���ط��ϴη��͵ı���", this->PacketsToSend[i % windowsize]);
		pns->sendToNetworkLayer(RECEIVER, this->PacketsToSend[i%windowsize]);			//���·������ݰ�
	}
}
