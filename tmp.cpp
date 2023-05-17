
stats sorting_choice(std::vector<int> Vector) {
	auto start = high_resolution_clock::now();
	bool needing_restart = false;
	stats A;
	int min = 0; // для записи минимального значения
	int buf = 0; // для обмена значениями 

	/*********** Начало сортировки **************/
	for (int i = 0; i < Vector.size(); i++)
	{

		min = i; // запомним номер текущей ячейки, как ячейки с минимальным значением
		// в цикле найдем реальный номер ячейки с минимальным значением
		for (int j = i + 1; j < Vector.size(); j++)
			min = (Vector[j] < Vector[min]) ? j : min;
			A.copy_count +=1;
			A.comparison_count += 1;
		// cделаем перестановку этого элемента, поменяв его местами с текущим
		if (i != min)
		{
			buf = Vector[i];
			Vector[i] = Vector[min];
			Vector[min] = buf;
			A.copy_count += 3;
		}
		A.comparison_count += 1;
	}
	auto stop = high_resolution_clock::now();
	duration<double> duration = stop - start;
	A.time = (double)duration.count();
	return A;
}
stats sorting_shell(std::vector<int> Vector) {
	auto start = high_resolution_clock::now();
	stats A;
	for (int s = Vector.size() / 2; s > 0; s /= 2) {
		for (int i = s; i < Vector.size(); ++i) {
			for (int j = i - s; j >= 0 && Vector[j] > Vector[j + s]; j -= s) {
				int temp = Vector[j];
				Vector[j] = Vector[j + s];
				Vector[j + s] = temp;
				A.comparison_count += 1;
				A.copy_count += 3;
			}
		}
	}
	auto stop = high_resolution_clock::now();
	duration<double> duration = stop - start;
	A.time = (double)duration.count();

	return A;
}
template <typename Iterator>
stats merge(Iterator begin, Iterator middle, Iterator end)
{

	vector<typename Iterator::value_type> left_vec(begin, middle), right_vec(middle, end);
	Iterator left = left_vec.begin(), right = right_vec.begin(), temp = begin;

	stats tmp;

	while (left < left_vec.end() && right < right_vec.end())
	{
		tmp.comparison_count++;
		if (*left <= *right)
		{
			*temp = *left;
			left++;
		}
		else
		{
			*temp = *right;
			right++;
		}
		temp++;
		tmp.copy_count++;
	}
	while (left < left_vec.end())
	{
		*temp = *left;
		left++;
		temp++;
		tmp.copy_count++;
	}
	while (right < right_vec.end())
	{
		*temp = *right;
		right++;
		temp++;
		tmp.copy_count++;
	}
	return tmp;
}

template <typename Iterator>
stats natural_merge_sort(Iterator begin, Iterator end)
{
	auto start = high_resolution_clock::now();
	stats empty;
	if (end - begin <= 1)
	{
		return empty;
	}
	Iterator middle = begin + (end - begin) / 2;
	stats left_stats = natural_merge_sort(begin, middle);
	stats right_stats = natural_merge_sort(middle, end);
	stats result = merge(begin, middle, end);
	result.comparison_count += left_stats.comparison_count + right_stats.comparison_count;
	result.copy_count += left_stats.copy_count + right_stats.copy_count;

	auto finish = high_resolution_clock::now();
	auto times = duration_cast<microseconds>(finish - start);

	result.time_count = (int)times.count();

	return result;
}
void testing(int N, int chosing, int number_circle, int variant_create_vector)
{

	stats AVG;

	for (int i = 0; i < number_circle; i++)
	{
		std::vector<int> TMP = create_vector(N, variant_create_vector);
		stats tmp;
		if (chosing == 1) { // choice
			tmp = sorting_choice(TMP);

			AVG.comparison_count += tmp.comparison_count;
			AVG.copy_count += tmp.copy_count;
			AVG.time += tmp.time;
		}
		else if (chosing==2)  { // shell 
			tmp = sorting_shell(TMP);

			AVG.comparison_count += tmp.comparison_count;
			AVG.copy_count += tmp.copy_count;
			AVG.time += tmp.time;
		}
		else {
			auto iterator1{ TMP.begin() };
			auto iterator2{ TMP.end() };
			tmp = natural_merge_sort(iterator1, iterator2);

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
		std::cout << "The  choice sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
		if (out.is_open())
		{
			out << "The  choice sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
		}
	}
	else if (chosing == 2) { // shake 
		std::cout << "The shell sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
		if (out.is_open())
		{
			out << "The shell sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
		}
	}
	else  { // merge 
		std::cout << "The natural_merge_sort sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
		if (out.is_open())
		{
			out << "The natural_merge_sort sorting  on vector size " << N << "\t" << AVG.comparison_count / number_circle << " \t" << AVG.copy_count / number_circle << "\t" << AVG.time / number_circle << std::endl;
		}
	}
	
	out.close();


}
int main() {
	//testing (размер вектора,выбор сортировки, количество циклов,выбор генерации вектора)

	std::cout << "\nAverage value on 100 vectors\n";
	for (int i = 1; i < 4; i++)
	{//testing (размер вектора,выбор сортировки, количество циклов,выбор генерации вектора)
		testing(1000, i, 100, 1);
		testing(2000, i, 100, 1);
		testing(3000, i, 100, 1);
		testing(10000, i, 100, 1);
		testing(25000, i, 100, 1);
		testing(50000, i, 100, 1);
		testing(100000, i, 100, 1);
	}

	std::cout << "\nValue on sorted vector\n";
	for (int i = 1; i <4; i++)
	{
		testing(1000, i, 1, 2);
		testing(2000, i, 1, 2);
		testing(3000, i, 1, 2);
		testing(10000, i, 1, 2);
		testing(25000, i, 1, 2);
		testing(50000, i, 1, 2);
		testing(100000, i, 1, 2);
	}

	std::cout << "\nValue on reverse-sorted vector\n";
	for (int i = 1; i < 4; i++)
	{
		testing(1000, i, 1, 3);
		testing(2000, i, 1, 3);
		testing(3000, i, 1, 3);
		testing(10000, i, 1, 3);
		testing(25000, i, 1, 3);
		testing(50000, i, 1, 3);
		testing(100000, i, 1, 3);
	}



}
