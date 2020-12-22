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
	printf("当前窗口状态：");
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
		this->PacketsToSend[nextseqnum % windowsize].acknum = -1; //忽略该字段
		this->PacketsToSend[nextseqnum % windowsize].seqnum = this->nextseqnum;//数据包的序号
		memcpy(this->PacketsToSend[nextseqnum % windowsize].payload, message.data, sizeof(message.data));//将来自应用层的数据写入数据包内
		this->PacketsToSend[nextseqnum % windowsize].checksum = pUtils->calculateCheckSum(this->PacketsToSend[nextseqnum % windowsize]);
		pUtils->printPacket("发送方发送报文", this->PacketsToSend[nextseqnum % windowsize]);
		pns->sendToNetworkLayer(RECEIVER, this->PacketsToSend[nextseqnum % windowsize]);								//调用模拟网络环境的sendToNetworkLayer，通过网络层发送到对方
		if (base == nextseqnum) {
			//启动发送方定时器
			pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
		}
		nextseqnum = (nextseqnum+1) % numsize;
		if (abs(nextseqnum - base) >= windowsize) waitingState = true;//如果窗口已满，则进入等待状态，应用层停止向运输层发送数据
		return true;//发送数据成功
	}
	else {
		waitingState = true;//如果窗口已满，则进入等待状态，应用层停止向运输层发送数据
		return false;//发送数据失败
	}
}
void GBNRdtSender::receive(const Packet& ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	if (checkSum == ackPkt.checksum) {//检查校验和是否正确
		pUtils->printPacket("发送方正确收到确认", ackPkt);
		base = (ackPkt.acknum + 1)%numsize;//更新base为累计确认号+1
		printwindow();
		if (base == nextseqnum) {//所有数据已经发送完毕
			pns->stopTimer(SENDER, 0);		//关闭定时器
		}
		else {
			pns->stopTimer(SENDER, 0);		//关闭定时器
			pns->startTimer(SENDER, Configuration::TIME_OUT, 0);//重新开始计时
		}
	}
	if (abs(nextseqnum - base) < windowsize) waitingState = false;//更新后的窗口未满
}
void GBNRdtSender::timeoutHandler(int seqNum) {
	//唯一一个定时器,无需考虑seqNum
	pns->stopTimer(SENDER, 0);		//关闭定时器
	pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
	for (int i = base;i != nextseqnum;i = (i+1)%numsize) {//回退N步重新发送
		pUtils->printPacket("发送方定时器时间到，重发上次发送的报文", this->PacketsToSend[i % windowsize]);
		pns->sendToNetworkLayer(RECEIVER, this->PacketsToSend[i%windowsize]);			//重新发送数据包
	}
}
