#ifndef TOOL_H
#define TOOL_H
#include "DataStructure.h"
#include <ctime>
struct Tool{
	/* ��ӡPacket����Ϣ*/
	virtual void printPacket(const char * description, const Packet &packet) = 0;
	/*����һ��Packet��У���*/
	virtual int calculateCheckSum(const Packet &packet) = 0;
	/*����һ�����ȷֲ���[0-1]��������*/
	virtual double random() = 0;
	virtual ~Tool() = 0;
};
//void Tool::printPacket(const char* description, const Packet& packet) {
//	cout << description;
//	cout << "acknum" << packet.acknum << endl;
//	cout << "seqnum" << packet.seqnum << endl;
//	(*(Packet*)&packet).print();
//}
//double Tool::random() {
//	srand((unsigned int) time(0));
//	double d = rand() / (double)RAND_MAX;
//	return d;
//}
//int Tool::calculateCheckSum(const Packet& packet){
//	int sum = 0;
//	for (int i = 0;i < Configuration::PAYLOAD_SIZE;i++)
//		sum += packet.payload[i];
//	while (sum >> 16)
//		sum = (sum >> 16) + (sum & 0xffff);
//	return ~sum;
//}
#endif