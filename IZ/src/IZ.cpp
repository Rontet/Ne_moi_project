#include"MyLib.hpp"
#include"MineLib.hpp"
#include<fstream>
#include<cstring>
using namespace std;

int main()
{
	//объявления, ввод, динамическая обработка---------------
	_s flats = gethome();
	_s taxes = gettaxes();
	ifstream indet("details.txt");
	int L;
	indet >> L;
	for (int i = 1; i < flats.n; i++)
	{
		_flat& flat = (static_cast<_flat*>(flats.mas))[i];
		flat.ipu = new int* [L];
		flat.MonExp = new int* [L];
		for (int j = 0; j < L; j++)
		{
			flat.ipu[j] = new int[taxes.n];
			flat.MonExp[j] = new int[taxes.n];
		}
	}
	int** odpu = new int* [L],
		** ExpFlats = new int* [L];
	for (int i = 0; i < L; i++)
	{
		odpu[i] = new int[taxes.n];
		ExpFlats[i] = new int[taxes.n];
	}
	int* Monpool = new int[L],
		* tmps = new int[taxes.n],
		MIN_MONTH = 999999;
	for (int i = 0; i < L; i++)
	{
		indet >> Monpool[i];
		for (int j = 0; j < taxes.n; j++)
		{
			indet >> tmps[j];
		}
		if (Monpool[i] < MIN_MONTH) MIN_MONTH = Monpool[i];
	}
	delete[] Monpool;
	delete[] tmps;
	indet.seekg(0);
	indet >> L;
	int date;
	char dat[MMoYYYY];
	for (int i = 0; i < L; i++)
	{
		indet >> dat;
		date = dtoi(dat);
		for (int j = 0; j < taxes.n; j++)
		{
			indet >> odpu[date - MIN_MONTH][j];
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
			auto dszm = date - MIN_MONTH;
			indet >> flat.ipu[dszm][j];
			flat.MonExp[dszm][j] = (dszm == 0) ?
				0 : flat.ipu[dszm][j] - flat.ipu[dszm - 1][j];
			ExpFlats[dszm][j] += flat.MonExp[dszm][j];
		}
	}
	indet.close();
	ifstream fin("input.txt");
	int N_flat;
	char taxname[6],
		period_L[MMoYYYY],
		period_R[MMoYYYY];
	char c;
	fin >> N_flat >> taxname >> period_L >> period_R;
	fin.close();
	//обработка и вывод---------------------------------------
	int per_L = dtoi(period_L) - MIN_MONTH,
		per_R = dtoi(period_R) - MIN_MONTH,
		N_tax = 0;
	#define taxcast _tax& tax = static_cast<_tax*>(taxes.mas)[N_tax]
	taxcast;
	while (strcmp(tax.name, taxname))
	{
		N_tax++;
		taxcast;
	}
	#undef taxcast
	ofstream fout("output.txt");
	int SumExpFlat = 0,
		SumExpOdn = 0,
		Cost = static_cast<_tax*>(taxes.mas)[N_tax].cost;
	/*vvvvvvvvvv поправить vvvvvvvvv*/
	for (int i = per_L; i <= per_R; i++)
	{
		auto& flat = static_cast<_flat*>(flats.mas)[N_flat];
		int ExpFlat = flat.MonExp[i][N_tax],
			ExpOdn = (odpu[i][N_tax] - ExpFlats[i][N_tax]) * flat.s;
		SumExpFlat += ExpFlat;
		SumExpOdn += ExpOdn;

	}
	int SumCostFlat = SumExpFlat * Cost,
		SumCostOdn = SumExpOdn * Cost;
	fout << SumExpFlat << ' '
		<< SumCostFlat << ' '
		<< SumExpOdn << ' '
		<< SumCostOdn << '\n'
		<< per_R - per_L + 1 << '\n';
	for (int i = per_L; i <= per_R; i++)
	{
		auto& flat = static_cast<_flat*>(flats.mas)[N_flat];
		char* MonthYear = itod(i + MIN_MONTH);
		fout << MonthYear << ' '
			<< flat.MonExp[i][N_tax] << ' '
			<< flat.MonExp[i][N_tax] * Cost << ' '
			<< (odpu[i][N_tax] - ExpFlats[i][N_tax]) * flat.s << ' '
			<< (odpu[i][N_tax] - ExpFlats[i][N_tax]) * flat.s * Cost << '\n';
		delete[] MonthYear;
	}
	/*^^^^^^^^^^^ поправить ^^^^^^^^^^^*/
	fout.close();
	//очистка-----------------------------------------------------
	for (int i = 0; i < L; i++)
	{
		delete[] odpu[i];
		delete[] ExpFlats[i];
	}
	for (int i = 1; i < flats.n; i++)
	{
		auto& flat = static_cast<_flat*>(flats.mas)[i];
		for (int j = 0; j < L; j++)
		{

			delete[] flat.ipu[j];
			delete[] flat.MonExp[j];
		}
		delete[] flat.ipu;
		delete[] flat.MonExp;
	}
	delete[] odpu;
	delete[] ExpFlats;
	delete[] flats.mas;
	delete[] taxes.mas;
	return 0;
}