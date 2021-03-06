// 2.2_Keygen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

unsigned int getInt(string s)
{
	unsigned int res = 0;
	for (int i = s.length() - 1; i >= 0; i--)
	{
		res = res << 8;
		res += (s[i]);
	}
	return res;
}

void createTable(vector<unsigned int> &table)
{
	unsigned int v = 0;
	for (int i = 0; i < 0x100; i++)
	{
		v = i;
		for (int j = 0; j < 8; j++)
		{
			if (v % 2 != 0)
			{
				v = (v >> 1) ^ 0xEDB88320;
			}
			else
			{
				v = v >> 1;
			}
		}
		table.push_back(v);
	}
}

void printTable(vector<unsigned int> table)
{
	for (int i = 0; i < 0x100; i++)
	{
		printf("%x", table[i]);
		printf(" ");
		if (i % 8 == 0 && i!= 0)printf("\n");
	}
}

unsigned char* convert4bytesTo8bytes(unsigned int edx)
{
	unsigned int eax = 0xd1fc1e8f, ecx = 0xc6ef3720, ebx = 0x3beabc9a, bc = 0x9e3779b9;
	unsigned int esi, edi;
	while (ecx != 0)
	{
		esi = ebx;
		esi = esi << 4;
		edi = ebx;
		edi = edi >> 5;
		esi = esi xor edi;
		esi = esi + ebx;
		edi = ecx;
		edi = edi >> 11;
		edi = edi & 3;
		if (edi) edi = 0;
		else edi = edx;
		edi = edi + ecx;
		esi = esi xor edi;
		eax = eax - esi;
		ecx = ecx - bc;
		esi = eax;
		esi = esi << 4;
		edi = eax;
		edi = edi >> 5;
		esi = esi xor edi;
		esi = esi + eax;
		edi = ecx;
		edi = edi & 3;
		if (edi) edi = 0;
		else edi = edx;
		edi = edi + ecx;
		esi = esi xor edi;
		ebx = ebx - esi;
	}

	unsigned char *res = new unsigned char[9];

	for (int i = 0; i < 4; i++)
	{
		res[i] = ebx & 0xff;
		ebx = ebx >> 8;
		res[i + 4] = eax & 0xff;
		eax = eax >> 8;
	}
	res[8] = '\0';
	return res;
}

unsigned int crc32(unsigned char ESI[9], vector<unsigned int> table)
{
	unsigned int ECX, EDX;
	unsigned int EBX = 0xffffffff;
	for (EDX = 0; EDX < 8; EDX++)
	{
		ECX = 0;
		ECX = ESI[EDX];
		ECX = ECX ^ EBX;
		ECX = ECX & 0xff;
		EBX = EBX >> 8;
		EBX = EBX ^ table[ECX];
	}
	EBX = ~EBX;
	return EBX;
}

string upper(string s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] >= 'a'&&s[i] <= 'z')
		{
			s[i] = s[i] - 32;
		}
	}
	return s;
}

bool checkKey(string s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i]<'0')
		{
			return false;
		}
		else if (s[i] > '9')
		{
			if (s[i]<'A' || s[i]>'F')
			{
				return false;
			}
		}
	}
	return true;
}

int main()
{
	vector<unsigned int> table;
	createTable(table);
	//printTable(table);
	unsigned char* s;
	s = convert4bytesTo8bytes(getInt("PSk="));
	unsigned int cmp1 = crc32(s, table);
	printf("%x", cmp1);

	/*for (int i = 0; i < 8; i++)
	{
		cout << s[i];
	}*/
	/*unsigned char s2[9] = "4C4F4C1D";
	unsigned int cmp2 = crc32(s2, table);
	printf("%x", cmp2);*/
	
    return 0;
}

