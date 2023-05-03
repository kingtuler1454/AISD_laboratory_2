/*#Функции сортировки должны принимать std::vector<int> -сортируемый набор элементов.
#https://docs.google.com/document/d/1L-doO_8pcQJ2gBpHUw7w7zEiB3auwPjt/edit
#. Сортировка пузырьком
#  Шейкерная сортировка
#0. Сортировка расчёской
1 000, 2 000, 3 000, …, 10 000,  25000, 50000, 100000:
*/
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
using namespace std::chrono;

struct stats {
	long comparison_count = 0;
	long copy_count = 0;
	double time = 0;
};
size_t lcg() {
	static size_t x = 0;
	x = (1021 * x + 24631) % 116640;
	return x;
}

std::vector<int> create_vector(int N,int chose) {
	std::vector<int> TMP;
	if (chose == 1)
	{
		for (int i = 0; i < N; i++)
		{
			TMP.push_back(lcg());
		}
	}
	else if (chose == 2)
	{
		for (int i = 0; i < N; i++)
		{
			TMP.push_back(i);
		}
		
	}
	else {
		for (int i = N; i >0; i--)
		{
			TMP.push_back(i);
		}
		
	}
	return TMP;
}

stats sorting_bubble(std::vector<int> Vector){
	auto start = high_resolution_clock::now();
	bool needing_restart = false;
	stats A;
	do {
		needing_restart = false;
		for (int i = 0; i < Vector.size()-1; i++)
		{
			if (Vector[i + 1] < Vector[i]) {
				int tmp = Vector[i];
				Vector[i] = Vector[i + 1];
				Vector[i + 1] = tmp;
				A.copy_count += 3;
				needing_restart = true;
			}
			A.comparison_count++;

		}
	} while (needing_restart);
	auto stop = high_resolution_clock::now();
	duration<double> duration = stop - start;
	A.time=(double)duration.count();
	return A;
}
stats sorting_shaker(std::vector<int> Vector) {
	auto start = high_resolution_clock::now();
	int Left, Right, i;
	Left = 1;
	Right = Vector.size() - 1;
	stats A;
	while (Left <= Right)
	{
		for (i = Right; i >= Left; i--)
		{
			if (Vector[i - 1] > Vector[i]) {
				int tmp = Vector[i];
				Vector[i] = Vector[i - 1];
				Vector[i - 1] = tmp;
				A.copy_count += 3;
			}
				A.comparison_count++;
			
			};
		Left++;
		for (i = Left; i <= Right; i++)
		{
			if (Vector[i - 1] > Vector[i]) {
				int tmp = Vector[i];
				Vector[i] = Vector[i - 1];
				Vector[i - 1] = tmp;
				A.copy_count += 3;
			}
			A.comparison_count++;
			
			};
		Right--;
		
	}
	auto stop = high_resolution_clock::now();
	duration<double> duration = stop - start;
	A.time = (double)duration.count();
	
	return A; }
stats sorting_comb(std::vector<int> Vector) {
	auto start = high_resolution_clock::now();
	stats A;
	double factor = 1.2473309; // фактор уменьшения
	int step = Vector.size() - 1; // шаг сортировки

	//Последняя итерация цикла, когда step==1 эквивалентна одному проходу сортировки пузырьком
	while (step >= 1)
	{
		for (int i = 0; i + step < Vector.size(); i++)
		{
			if (Vector[i] > Vector[i + step])
			{
				int tmp = Vector[i + step];
				Vector[i + step] = Vector[i];
				Vector[i] = tmp;
				A.copy_count += 3;
			}
			A.comparison_count += 1;
		}
		step /= factor;
	}
	auto stop = high_resolution_clock::now();
	duration<double> duration = stop - start;
	A.time = (double)duration.count();
	return A;
}

void testing(int N,int chosing,int number_circle,int variant_create_vector)
{
	
		stats AVG;

		for (int i = 0; i < number_circle; i++)
		{
			std::vector<int> TMP = create_vector(N, variant_create_vector);
			stats tmp;
			if (chosing == 1) { // БАБЛ
				tmp = sorting_bubble(TMP);

				AVG.comparison_count += tmp.comparison_count;
				AVG.copy_count += tmp.copy_count ;
				AVG.time += tmp.time;
			}
			else if (chosing == 2) { // shake 
				 tmp = sorting_shaker(TMP);

				 AVG.comparison_count += tmp.comparison_count;
				 AVG.copy_count += tmp.copy_count;
				 AVG.time += tmp.time;
			}
			else { // raschoska 
				 tmp = sorting_comb(TMP);
				 AVG.comparison_count += tmp.comparison_count;
				 AVG.copy_count += tmp.copy_count;
				 AVG.time += tmp.time;
			}
			//AVG.comparison_count += tmp->comparison_count ;
			//AVG.copy_count += tmp->copy_count / 100;
			//AVG.time += tmp->time / 100;
		}
		std::ofstream out("log.txt", std::ios::app);
		if (chosing == 1) { // БАБЛ
			std::cout << "The  bubble sorting  on vector size " << N << "\t" << AVG.comparison_count/ number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
			if (out.is_open())
			{
				out << "The  bubble sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
			}
		}
		else if (chosing == 2) { // shake 
			std::cout << "The shaker sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count/ number_circle << "\t" << AVG.time/ number_circle << std::endl;
			if (out.is_open())
			{
				out << "The shaker sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
			}
		}
		else { // raschoska 
			std::cout << "The  comb sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
		
			if (out.is_open())
			{
				out << "The  comb sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
			}
			}
		out.close();


}
int main() {
	//testing (размер вектора,выбор сортировки, количество циклов,выбор генерации вектора)
	
	std::cout << "\nAverage value on 100 vectors\n";
	for (int i=1;i<4;i++)
	{
	testing(1000,  i,100,1);
	testing(2000,  i, 100,1);
	testing(3000,  i, 100,1);
	testing(10000, i, 100,1);
	testing(25000, i, 100,1);
	testing(50000, i, 100,1);
	testing(100000,i, 100,1);
	}
	
	std::cout << "\nValue on sorted vector\n";
	for (int i = 1; i < 4; i++)
	{
		testing(1000, i, 1, 2);
		testing(2000, i, 1, 2);
		testing(3000, i, 1, 2);
		testing(10000, i, 1, 2);
		testing(25000, i,1, 2);
		testing(50000, i, 1, 2);
		testing(100000, i, 1, 2);
	}

	std::cout << "\nValue on reverse-sorted vector\n";
	for (int i = 1; i < 4; i++)
	{
		testing(1000, i, 1, 3);
		testing(2000, i, 1, 3);
		testing(3000, i,1, 3);
		testing(10000, i, 1, 3);
		testing(25000, i, 1, 3);
		testing(50000, i, 1, 3);
		testing(100000, i, 1, 3);
	}
	


} 
