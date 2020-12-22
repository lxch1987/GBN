// GBN.cpp : 定义控制台应用程序的入口点。
//
#define  _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "Global.h"
#include "RdtSender.h"
#include "RdtReceiver.h"
#include "GBNRdtSender.h"
#include "GBNRdtReceiver.h"
#include <cstdio>
int main(int argc, char* argv[])
{
	RdtSender *ps = new GBNRdtSender();
	RdtReceiver * pr = new GBNRdtReceiver();
	freopen("log.txt", "w+", stdout);
	//pns->setRunMode(0);  //VERBOS模式
	pns->setRunMode(1);  //安静模式
	pns->init();
	pns->setRtdSender(ps);

	pns->setRtdReceiver(pr);
	pns->setInputFile("D:\\VSprojects\\NET2\\input.txt");
	pns->setOutputFile("D:\\VSprojects\\NET2\\output.txt");
	
	pns->start();
	delete ps;
	delete pr;
	delete pUtils;									//指向唯一的工具类实例，只在main函数结束前delete
	delete pns;										//指向唯一的模拟网络环境类实例，只在main函数结束前delete
	return 0;
}

