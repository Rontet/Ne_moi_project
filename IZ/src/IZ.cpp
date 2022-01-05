#define ZERO_MONTH 202109
#include"MyLib.hpp"
#include"MineLib.hpp"
#include<fstream>
#include<cstdio>
#include<iomanip>
using namespace std;

int main()
{
	//объявления, ввод, динамическая обработка---------------
	_s flats = gethome();
	_s taxes = gettaxes();

	ifstream indet("details.txt");
	int L;
	indet >> L;
	double live_s = 0.0;
	for (int i = 0; i < flats.n; i++)
	{
		_flat& flat = (static_cast<_flat*>(flats.mas))[i];
		live_s += flat.s;
		flat.ipu = new int* [L];
		flat.MonExp = new int* [L];
		flat.MonOdn = new int* [L];
		for (int j = 0; j < L; j++)
		{
			flat.ipu[j] = new int[taxes.n];
			flat.MonExp[j] = new int[taxes.n];
			flat.MonOdn[j] = new int[taxes.n];
		}

	}
	int** odpu = new int* [L],
		** ExpFlats = new int* [L];
	for (int i = 0; i < L; i++)
	{
		odpu[i] = new int[taxes.n];
		ExpFlats[i] = new int[taxes.n];
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
			ExpFlats[i][j] = 0;
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
			_flat& flat = (static_cast<_flat*>(flats.mas))[flatnum];
			auto dszm = date - ZERO_MONTH;

			indet >> flat.ipu[dszm][j];
			flat.MonExp[dszm][j] = (dszm == 0) ? 0 :
				flat.ipu[dszm][j] - flat.ipu[dszm - 1][j];

			ExpFlats[dszm][j] += flat.MonExp[dszm][j];
		}
	}
	indet.close();

	//TODO:
	freopen("input.txt", "r", stdin);
	//ifstream fin("input.txt");
	int N_flat;
	char taxname[6],
		period_L[8],
		period_R[8];
	//char c;
	//fin >> N_flat >> taxname >> period_L >> c >> period_R;
	scanf_s("%d %s %s-%s", &N_flat, taxname, period_L, period_R);
	fclose(stdin);
	//обработка и вывод---------------------------------------
	int per_L = dtoi(period_L) - ZERO_MONTH,
		per_R = dtoi(period_R) - ZERO_MONTH,
		N_tax = 0;


	//todo:
	/*_tax& tax = static_cast<_tax*>(taxes.mas)[N_tax];
	for (; strcmp(tax.name, taxname);)
	{
		tax = 
	}*/

	//todo: 
	static_cast<_flat*>(flats.mas)[N_flat].s /= live_s;


	ofstream out("output.txt");
	int SumExpFlat = 0,
		SumExpOdn = 0,
		Cost = static_cast<_tax*>(taxes.mas)[N_tax].price;
	out << per_R - per_L + 1 << '\n';
	for (int i = per_L; i <= per_R; i++)
	{
		auto& flat = static_cast<_flat*>(flats.mas)[N_flat];
		int ExpFlat = flat.MonExp[i][N_tax],
			ExpOdn = (odpu[i][N_tax] - ExpFlats[i][N_tax]) * flat.s;
		SumExpFlat += ExpFlat;
		SumExpOdn += ExpOdn;
		out << itod(i + ZERO_MONTH) << ' '
			<< ExpFlat * Cost << ' ' << ExpOdn * Cost << '\n';
	}
	out.seekp(0);
	int SumCostFlat = SumExpFlat * Cost,
		SumCostOdn = SumExpOdn * Cost;
	out << SumExpFlat << ' '
		<< SumCostFlat << ' '
		<< SumExpOdn << ' '
		<< SumCostOdn << '\n';
	out.close();
	//очистка-----------------------------------------------------
	for (int i = 0; i < L; i++)
	{
		delete[]odpu[i];
		delete[]ExpFlats[i];
	}
	for (int i = 0; i < flats.n; i++)
	{
		auto& flat = static_cast<_flat*>(flats.mas)[i];
		for (int j = 0; j < L; j++)
		{

			delete[] flat.ipu[j];
			delete[] flat.MonExp[j];
			delete[] flat.MonOdn[j];
		}
		delete[] flat.ipu;
		delete[] flat.MonExp;
		delete[] flat.MonOdn;
	}
	delete[]odpu;
	delete[]ExpFlats;
	delete[]flats.mas;
	delete[]taxes.mas;
	return 0;
}