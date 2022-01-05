#define ZERO_MONTH 202109
#include"MyLib.hpp"
#include"MineLib.hpp"
#include<fstream>
#include<cstdio>
#include<iomanip>
using namespace std;

int main()
{
	//объявления и ввод
	_flat* flats = gethome();
	_s taxes = gettaxes();
	ifstream indet("details.txt");
	int L;
	indet >> L;
	int** odpu = new int*[L];
	for (int i = 0; i < L; i++)
	{
		odpu[i] = new int[taxes.n];
		flats[i].ipu = new int*[L];
		for (int j = 0; j < L; j++)
		{
			flats[i].ipu[j] = new int[taxes.n];
		}
	}
	int date;
	char dat[8];
	for (int i = 0; i < L; i++)
	{
		indet >> dat;
		date = dtoi(dat);
		for (int j = 0; j < taxes.n; j++)
		{
			indet >> odpu[date - ZERO_MONTH][j];
		}
	}
	int M;
	indet >> M;
	for (int i = 0, flatnum; i < M; i++)
	{
		indet >> dat >> flatnum;
		date = dtoi(dat);
		for (int j = 0; j < taxes.n; j++)
		{
			indet >> flats[flatnum].ipu[date - ZERO_MONTH][j];
		}
	}
	indet.close();
	freopen("input.txt", "r", stdin);
	int N_flat;
	char taxname[6],
		 period_L[8],
		 period_R[8];
	scanf_s("%d &s %s-%s", &N_flat, taxname, period_L, period_R);
	fclose(stdin);
	//обработка

	//вывод и очистка
	freopen("output.txt", "w", stdout);

	fclose(stdout);
	for (int i = 0; i < L; i++)
	{
		delete[]odpu[i];
		for (int j = 0; j < L; j++)
		{
			delete[]flats[i].ipu[j];
		}
		delete[]flats[i].ipu;
	}
	delete[]odpu;
	delete[]flats;
	delete[]taxes.mas;
	return 0;
}