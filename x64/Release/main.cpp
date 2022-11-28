// includes
#include <iostream>
#include <queue>
#include <thread>
#include "utils.h"
#include "RabinKarp.h"
#include <condition_variable>
#include <chrono>




//function prototypes
void split_string();
void search(int id);
void find_matches();
void show_matches();
void calculate_timings();
void selection_menu();
void searching_algorithm();



// queues to store important information to loop through at the end
std::queue<int> allMatches;
std::queue<long long> timings;



// important thread related variables
int threadamount;
std::thread* threads;
std::condition_variable matches_cv, string_cv;
std::mutex mtx_matches, mtx_search;

// Algorithm related 
RabinKarp* rabinKarp;
bool isMatches_found = false;
bool isText_split = false;
std::string text;
std::string searchInput;
bool ran_once = false;

// benchmarking
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using the_clock = std::chrono::steady_clock;
the_clock::time_point bendTime;



int main()
{
	selection_menu();

	searching_algorithm();

	calculate_timings();

	return 1;
}


// --------- Thread Functions ---------



// splits text into sections to be searched through by each thread
void split_string()
{
	std::unique_lock<std::mutex> lck(mtx_search);
	rabinKarp->split_text();
	isText_split = true;
	string_cv.notify_one(); //notifies associated "mtx_search" to stop waiting
}


// splits work into multiple threads and searches for the pattern amongst them
void find_matches()
{


	std::unique_lock<std::mutex> lck(mtx_matches);
	the_clock::time_point bstartTime = the_clock::now();
	for (int i = 0; i < threadamount; i++)
	{
		threads[i] = std::thread(search, i);
	};



	for (int i = 0; i < threadamount; i++)
	{
		threads[i].join();

	}

	allMatches = rabinKarp->getMatches();
	bendTime = the_clock::now();
	auto btime_taken = duration_cast<milliseconds>(bendTime - bstartTime).count();
	timings.push(btime_taken);
	isMatches_found = true;
	matches_cv.notify_one();
}

//displays how many matches there are. 
void show_matches()
{
	std::unique_lock<std::mutex> lck(mtx_matches);
	while (!isMatches_found)
	{

		matches_cv.wait(lck);

	}
	int matches_found = 0;
	while (!allMatches.empty())
	{

		matches_found++;
		allMatches.pop();
	}

	if (!ran_once)
	{
		std::cout << "matches found: " << matches_found << std::endl;
		ran_once = true;
	}

}

//waits for the text to be fully split before searching through to find the patterns
void search(int id)
{
	std::unique_lock<std::mutex> lck(mtx_search);
	while (!isText_split)
	{
		string_cv.wait(lck);
	}



	rabinKarp->find_pattern();



}

//calculates the average time taken
void calculate_timings()
{
	float average = 0;
	for (int i = 0; i < timings.size(); i++)
	{
		average += timings.front();
		timings.pop();
	}

	average = average / timings.size();
	std::cout << average << "ms" << std::endl;
}

// menu and inputs
void selection_menu()
{
	std::cout << "how many threads to use?: " << std::endl;
	threadamount = 0;
	bool input = false;

	// selects how many threads to use
	do
	{
		std::cin >> threadamount;
		if (threadamount > std::thread::hardware_concurrency()) // using more threads than the system has available causes c++ 
		{                                                       // to reuse threads after they've finished with their tasks significantly increasing time taken
			std::cout << "your system can only handle " << std::thread::hardware_concurrency() << " threads without it decreasing performance significantly" << std::endl;
			std::cout << "please enter a different value" << std::endl;
		}
		else
		{
			input = true;
		}

	} while (!input);

	//select the file to search
	system("CLS");

	std::cout << "Rabin Karp MultiThreading" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Pick a file:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "1. Small File" << std::endl;
	std::cout << "2. Medium File" << std::endl;
	std::cout << "3. Large File" << std::endl;
	int userinput;
	std::cin >> userinput;

	switch (userinput)
	{
	case 1:
		load_small_text(text);
		break;
	case 2:
		load_medium_text(text);
		break;
	case 3:
		load_large_text(text);
		break;
	default:
		break;
	}

	//argument for what to search
	system("CLS");
	std::cout << "Rabin Karp MultiThreading" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "What would you like to search" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;

	std::cin.ignore();
	std::getline(std::cin, searchInput);

	threads = new std::thread[threadamount];
}

// main function that deals with initial threads and loops
void searching_algorithm()
{
	int repetitions;
	system("CLS");
	std::cout << "Rabin Karp MultiThreading" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "How many times would you like to test" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;

	std::cin >> repetitions;

	for (int i = 0; i < repetitions; i++)
	{
		rabinKarp = new RabinKarp(searchInput, text, threadamount);
		std::thread* thread = new std::thread(split_string);
		std::thread* thread2 = new std::thread(find_matches);
		std::thread* thread3 = new std::thread(show_matches);


		thread->join();
		thread2->join();
		thread3->join();
		delete rabinKarp;
	}
}

