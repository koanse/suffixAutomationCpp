#include "stdafx.h"
#include <map>
#include <set>
#include <iostream>

struct AutomationState {
	int Length;
	int Link;
	std::map<char, int> Next;
};

const int MAXLEN = 100000;
AutomationState states[MAXLEN * 2];
int sz, last;
int cnt[MAXLEN * 2];

std::set<std::pair<int, int>> base;

void InitSuffixAutomation()
{
	sz = last = 0;
	states[0].Length = 0;
	states[0].Link = -1;
	sz++;
}

void AddCharacterToSuffixAutomation(char c)
{
	auto cur = sz++;
	states[cur].Length = states[last].Length + 1;
	int p;
	for (p = last; p != -1 && !states[p].Next.count(c); p = states[p].Link)
		states[p].Next[c] = cur;
	if (p == -1)
	{
		states[cur].Link = 0;
	}
	else {
		auto q = states[p].Next[c];
		if (states[p].Length + 1 == states[q].Length)
		{
			states[cur].Link = q;
			cnt[cur] = 1;
			base.insert(std::make_pair(states[cur].Length, cur));
		}
		else {
			auto clone = sz++;
			states[clone].Length = states[p].Length + 1;
			states[clone].Next = states[q].Next;
			states[clone].Link = states[q].Link;
			for (; p != -1 && states[p].Next[c] == q; p = states[p].Link)
				states[p].Next[c] = clone;
			states[q].Link = states[cur].Link = clone;

			cnt[clone] = 0;
			base.insert(std::make_pair(states[clone].Length, clone));
		}
	}
	last = cur;
}

void CalculateAllAccurences(std::string pattern)
{
	for (size_t i = 1; i < last; i++)
	{
		cnt[i] = 1;
	}

	std::reverse_iterator<std::set<std::pair<int, int>>::iterator> it;
	for (it = base.rbegin(); it != base.rend(); ++it)
	{
		auto index = it->second;
		cnt[states[index].Link] += cnt[index];
	}

	auto vtx = 0;
	auto patternIndex = 0;

	while (patternIndex <= pattern.length() - 1)
	{
		vtx = states[vtx].Next[pattern[patternIndex] - 'a'];
		patternIndex++;
	}
}

int main()
{
	InitSuffixAutomation();
	std::string s = "ababdffgfghfabababdgfghhab";

	
	for (auto i = 0; i < s.length(); i++)
	{
		AddCharacterToSuffixAutomation(s[i]);
	}
	CalculateAllAccurences("ab");

	return 0;
}

