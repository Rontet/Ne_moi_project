#define SUBDATE(A, B) ((A) - ((A) / 100L - (B) / 100L) * 88L - (B))
#define DATEMOVE(A, B) ((B) + (A) / 12L * 100L + (((B) % 100L + (A) % 12L > 12L) ? (88L + (A) % 12L) : (A) % 12L))
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
	ifstream indet("res\\details.txt");
	int L;
	indet >> L;
	for (int i = 1; i <= flats.n; i++)
	{
		_flat& flat = (static_cast<_flat*>(flats.mas))[i];
		flat.ipu = new int* [L];
		flat.MonExp = new int[L];
		flat.MonExp[0] = 0;
		for (int j = 0; j < L; j++)
		{
			flat.ipu[j] = new int[taxes.n];
		}
	}
	int** odpu = new int* [L],
		* ExpFlats = new int[L],
		* ExpHome = new int[L];
	for (int i = 0; i < L; i++)
	{
		odpu[i] = new int[taxes.n];
	}
	int* Monpool = new int[L],
		* tmps = new int[taxes.n],
		MIN_MONTH = 999999;
	for (int i = 0; i < L; i++)
	{
		char dat[MMYYYY];
		indet >> dat;
		Monpool[i] = dtoi(dat);
		for (int j = 0; j < taxes.n; j++)
		{
			indet >> tmps[j];
		}
		if (Monpool[i] < MIN_MONTH)
		{
			MIN_MONTH = Monpool[i];
		}
	}
	delete[] tmps;
	delete[] Monpool;
	indet.seekg(0);
	indet >> L;
	int date;
	char dat[MMYYYY];
	for (int i = 0; i < L; i++)
	{
		indet >> dat;
		date = dtoi(dat);
		for (int j = 0; j < taxes.n; j++)
		{
			indet >> odpu[SUBDATE(date, MIN_MONTH)][j];
		}
		ExpFlats[i] = ExpHome[i] = 0;
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
			auto dszm = SUBDATE(date, MIN_MONTH);
			indet >> flat.ipu[dszm][j];
		}
	}

	indet.close();
	ifstream fin("res\\input.txt");
	int N_flat;
	char taxname[6],
		period_L[MMYYYY],
		period_R[MMYYYY];
	fin >> N_flat >> taxname;
	fin.get();
	fin.get(period_L, MMYYYY);
	fin.ignore(1);
	fin>>period_R;
	fin.close();
	//обработка и вывод---------------------------------------
	int per_L = SUBDATE(dtoi(period_L), MIN_MONTH),
		per_R = SUBDATE(dtoi(period_R), MIN_MONTH),
		N_tax = 0;

	_tax* tax_ptr = static_cast<_tax*>(taxes.mas);
	while (strcmp(tax_ptr->name, taxname))
	{
		N_tax++;
		tax_ptr++;
	}
	for (int i = 1; i <= flats.n; i++)
	{
		_flat& flat = (static_cast<_flat*>(flats.mas))[i];
		for (int j = (per_L > 0 ? per_L : 1); j <= per_R; j++)
		{
			flat.MonExp[j] = flat.ipu[j][N_tax] - flat.ipu[j - 1][N_tax];
			ExpFlats[j] += flat.MonExp[j];
		}
	}
	ofstream fout("output.txt");
	int SumExpFlat = 0,
		SumExpOdn = 0,
		Cost = static_cast<_tax*>(taxes.mas)[N_tax].cost;
	for (int i = per_L; i <= per_R; i++)
	{
		auto& flat = static_cast<_flat*>(flats.mas)[N_flat];
		ExpHome[i] = odpu[i][N_tax] - odpu[(i - 1 >= 0 ? i - 1 : i)][N_tax];
		int ExpFlat = flat.MonExp[i],
			ExpOdn = static_cast<int>((ExpHome[i] - ExpFlats[i]) * flat.s);
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
		char* MonthYear = itod(DATEMOVE(i, MIN_MONTH));
		fout << MonthYear << ' '
			<< flat.MonExp[i] << ' '
			<< flat.MonExp[i] * Cost << ' '
			<< static_cast<int>((ExpHome[i] - ExpFlats[i]) * flat.s) << ' '
			<< static_cast<int>((ExpHome[i] - ExpFlats[i]) * flat.s) * Cost << '\n';
		delete[] MonthYear;
	}
	fout.close();
	//очистка-----------------------------------------------------
	for (int i = 0; i < L; i++)
	{
		delete[] odpu[i];
	}
	for (int i = 1; i <= flats.n; i++)
	{
		auto& flat = static_cast<_flat*>(flats.mas)[i];
		for (int j = 0; j < L; j++)
		{
			delete[] flat.ipu[j];
		}
		delete[] flat.ipu;
		delete[] flat.MonExp;
	}
	delete[] odpu;
	delete[] ExpFlats;
	delete[] ExpHome;
	delete[] flats.mas;
	delete[] taxes.mas;
	return 0;
}