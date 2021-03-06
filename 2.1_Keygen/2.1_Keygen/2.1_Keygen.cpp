// 2.1_Keygen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <cstring>
#include <string>
#include <vector>
#include "QInt.h"
#include <sstream>
#include <stack>

//61626364->64636261
string dump(string s)
{
	string res = "";
	while (s.length() < 8)
	{
		s = "0" + s;
	}
	int n = s.length();
	for (int i = 0; i < n / 2; i++)
	{
		res += s.substr(s.length() - 2, 2);
		s.erase(s.length() - 2, 2);
	}
	return res;
}

string textToHex(string text)
{
	stringstream ss;
	for (int i = 0; i < text.length(); i++)
	{
		ss << std::hex << (int)text[i];
	}
	string res = ss.str();
	for (int i = 0; i < res.length(); i++)
	{
		if (res[i] >= 'a'&&res[i] <= 'f')
		{
			res[i] = res[i] - 32;
		}
	}
	return res;
}

//lưu theo kiểu thanh ghi, không phải vùng nhớ
class Keygen
{
private:
	string name;//8 ô => 16 ký tự
	string seed1;//4 ô => 8 ký tự
	string seed2;//4 ô
	string size;
	char **maze;
	int size_int;
	int x_begin, y_begin, x_end, y_end;
	string key;

	void init()
	{
		string s = textToHex(this->name);
		s = dump(s);
		while (s.length() < 16)
		{
			s = "0" + s;
		}
		seed2 = s.substr(0, 8);
		seed1 = s.substr(8, 8);
	}
	string getBlock(string stack)
	{
		QInt x(seed1, "16");
		QInt y("5DC1", "16");
		string res1 = (x * y).toString("16");
		while (res1.length() < 16) { res1 = "0" + res1; }
		res1 = res1.substr(res1.length() - 8, 8);

		x.setData(res1, "16");
		y.setData("560B", "16");
		res1 = (x % y).toString("16");

		seed1 = res1;

		x.setData(seed2, "16");
		y.setData("549D", "16");
		string res2 = (x * y).toString("16");
		while (res2.length() < 16) { res2 = "0" + res2; }
		res2 = res2.substr(res2.length() - 8, 8);

		x.setData(res2, "16");
		y.setData("51A1", "16");
		res2 = (x % y).toString("16");

		seed2 = res2;

		x.setData(res1, "16");
		y.setData(res2, "16");
		string res = (x + y).toString("16");

		x.setData(res, "16");
		y.setData(stack, "16");

		return (x % y).toString("16");
	}
	void generate()
	{
		QInt x(size, "16");
		string square = (x*x).toString("16");//bình phương
											 
		int square_int = atoi((x*x).toString("10").c_str());
		char *temp = new char[square_int];
		//133311
		int i = 0;
		QInt y;
		string eax = "";
		while (i < square_int)
		{
			eax = getBlock("4");
			y.setData(eax, "16");

			int test = atoi((y & y).toString("10").c_str());

			if (test != 0)
			{
				temp[i++] = '.';
			}
			else
			{
				temp[i++] = '#';
			}
		}

		string x_start, y_start, x_finish, y_finish;
		x_start = getBlock(size);
		y_start = getBlock(size);
		x_finish = getBlock(size);
		while (x_start == x_finish)
		{
			x_finish = getBlock(size);
		}
		y_finish = getBlock(size);
		while (y_start == y_finish)
		{
			y_finish = getBlock(size);
		}
		x.setData(x_start, "16");
		y.setData(size, "16");
		string res = (x*y).toString("16");
		x.setData(res, "16");
		y.setData(y_start, "16");
		//res = (x + y).toString("16");
		int pos = atoi((x + y).toString("10").c_str());
		temp[pos] = 's';

		x.setData(x_finish, "16");
		y.setData(size, "16");
		res = (x*y).toString("16");
		x.setData(res, "16");
		y.setData(y_finish, "16");
		//res = (x + y).toString("16");
		pos = atoi((x + y).toString("10").c_str());
		temp[pos] = 'f';

		i = 0; int j = 0;
		for (int k = 0; k < square_int; k++)
		{
			if (k % size_int == 0 && k != 0)
			{
				j = 0;
				i++;
			}
			if (temp[k] == 's') { this->x_begin = i; this->y_begin = j; }
			if (temp[k] == 'f') { this->x_end = i; this->y_end = j; }
			maze[i][j++] = temp[k];
		}
		delete[] temp;
		temp = NULL;
	}
public:
	Keygen(string name)
	{
		this->name = name;
		init();
		QInt x(getBlock("14"), "16");
		QInt y("14", "16");
		size = (x + y).toString("16");

		QInt z(size, "16");
		size_int = atoi(z.toString("10").c_str());
		while (size.length() < 8) { size = "0" + size; }

		maze = new char*[size_int];
		for (int i = 0; i < size_int; i++)
		{
			maze[i] = new char[size_int];
		}

		generate();
	}
	~Keygen()
	{
		for (int i = 0; i < size_int; i++)
		{
			delete[] maze[i];
			maze[i] = NULL;
		}
		delete[] maze;
		maze = NULL;
	}
	void printMaze()
	{
		for (int i = 0; i < size_int; i++)
		{
			for (int j = 0; j < size_int; j++)
			{
				cout << maze[i][j];
			}
			cout << endl;
		}
	}
	bool solveMaze(int x, int y)
	{
		if (x < 0 || x > size_int - 1 || y < 0 || y > size_int - 1) return false;
		if (maze[x][y] == 'f') return true;
		if (maze[x][y] != '.' && maze[x][y] != 's') return false;
		maze[x][y] = '+';
		/*	r xuống
		l lên
		d phải
		u trái
		*/
		if (solveMaze(x - 1, y) == true)
		{
			key += 'l';
			return true;
		}
		if (solveMaze(x + 1, y) == true)
		{
			key += 'r';
			return true;
		}
		if (solveMaze(x, y + 1) == true)
		{
			key += 'd';
			return true;
		}
		if (solveMaze(x, y - 1) == true)
		{
			key += 'u';
			return true;
		}
		maze[x][y] = ' ';
		return false;
	}
	string getKey()
	{
		if (solveMaze(x_begin, y_begin))
		{
			string tmp = "";
			for (int i = key.length() - 1; i >= 0; i--)
			{
				tmp += key[i];
			}
			return tmp;
		}
		else
		{
			return "unsolvable";
		}
	}
};

int main()
{
	cout << "1612774 - 1612829 - 1612842" << endl;
	cout << "Keygen for 2.1" << endl << endl;
	while (1)
	{
		string username;
		while (1)
		{
			fflush(stdin);
			cout << "Username (5 - 8 chars): ";
			getline(cin, username);
			int n = username.length();
			if (n >= 5 && n <= 8)break;
		}
		Keygen k(username);
		//k.printMaze();
		//cout << endl;
		cout << "Serial: " << k.getKey() << endl;
	}
	
	system("pause");
	return 0;
}

