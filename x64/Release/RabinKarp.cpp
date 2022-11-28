#include "RabinKarp.h"
RabinKarp::RabinKarp(std::string& pat, std::string& text, int split)
{
	//variable initilization
	m_pattern = pat;
	m_text = text;
	m_textSpit = split;


}

void RabinKarp::hash(int* hash, const std::string& input)
{
	*hash = input[0];

	int counter = 1;

	//based on formula // ASCII a = 97, b = 98, r = 114. 
	//eg: hash("abr") = [([([(97 × 256) % 101 + 98] % 101) × 256] % 101) + 114] % 101 = 4
	for (int a : input)
	{
		if (counter < input.length())
		{
			*hash *= 256;
			*hash %= 101;

			*hash += input[counter];
			*hash %= 101;
			counter++;
		}

	}
}

void RabinKarp::find_pattern()
{
	int hash_pattern;
	hash(&hash_pattern, m_pattern);
	int pat_size = m_pattern.size();
	int hash_text;
	std::string subString;




	for (int i = m_splitText.front().x; i <= (m_splitText.front().y - pat_size) + 1; i++)
	{
		
		subString = m_text.substr(i, pat_size);
		hash(&hash_text, subString); //scrolls through text using a substring only looking at the same amount of characters as the pattern
		if (hash_text == hash_pattern)
		{
			if (subString == m_pattern) //makes sure the match is an actual match and not a duplicate hash number
			{
				m_matches.push(i); //push match to data structure

			}
		}
	}
	m_splitText.pop();


}


void RabinKarp::split_text()
{

	int N = m_text.length();


	// algorithm to parse the text into equal subsections
	int currentPos, previousPos = 0, i;

	for (previousPos = i = 0; i < m_textSpit; ++i) {
		currentPos = (N + N * i) / m_textSpit;

		//ensures that the subsection doesn't cut off a word by incrementing the current position until it's a whitespace
		while (m_text.substr(previousPos, currentPos - previousPos).back() != 32 && currentPos != N) //note '32' is the char code for a whitespace
		{
			currentPos++;

		}


		m_splitText.push(TextPos(previousPos, currentPos)); //pushes coordinates 

		previousPos = currentPos;
	}

}