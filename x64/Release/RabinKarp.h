#pragma once
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>

//custom type to store multiple ints in a queue
struct TextPos 
{
	TextPos(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	int x, y;
};


// hashing algorithm and Rabin Karp algorithm, inspiration taken from 
// https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm
// hash based on a simpler version of Rabin fingerprint


class RabinKarp
{
public:
	RabinKarp(std::string& pat, std::string& text, int split);

	void hash(int* hash, const std::string& input);
	void find_pattern();
	void split_text();

	//getters
	std::queue<int> getMatches()
	{
		return m_matches;
	}

	std::queue<TextPos> getSplitText()
	{
		return m_splitText;
	}

private:
	int m_textSpit;
	std::string m_pattern;
	std::string m_text;

	std::queue<int> m_matches;
	std::queue<TextPos> m_splitText;
	std::mutex rb_mtx;
	
};

