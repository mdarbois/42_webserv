#include "PmergeMe.hpp"

void PmergeMe::printDeque(int arguments) {
	std::deque<int>::iterator it = _sortedDeque.begin();
	std::cout << "After: ";
	for (; it < _sortedDeque.end(); it++) std::cout << *it << " ";
	std::cout << std::endl;
    std::cout << "Time to process a range of " << arguments << " elements with std::deque : "<< std::fixed
			  << std::setprecision(2)
			  << static_cast<double>(_endDeque - _startDeque) /
					 (CLOCKS_PER_SEC / 1000000.0)
			  << " us" << std::endl; 
}

int PmergeMe::getUpBoundDeque(const std::deque<int>& largeDeque, unsigned long range) {
	if (largeDeque.size() < static_cast<unsigned long>(range)) {
		return largeDeque.size();
	} else
		return static_cast<int>(range);
}

void PmergeMe::BinaryDeque(std::deque<int>& largeDeque, int value, int range) {
	int low = 0, up = getUpBoundDeque(largeDeque, range);

	while (low <= up) {
		int middle = low + (up - low) / 2;

		if (largeDeque[middle] == value) {
			largeDeque.insert(largeDeque.begin() + middle + 1, value);
			return;
		} else if (largeDeque[middle] < value) {
			low = middle + 1;
		} else
			up = middle - 1;
	}
	largeDeque.insert(largeDeque.begin() + low, value);
}

unsigned long PmergeMe::getUpIndexDeque(const std::deque<int>& smallDeque) {
	if (smallDeque.size() > 2) 
        return 2;
	return smallDeque.size() - 1;
}

unsigned long PmergeMe::getNextUpIndexDeque(const std::deque<int>& smallDeque, int up) {
	if (getNextJacobsthal(up + 1) > static_cast<int>(smallDeque.size()))
		return smallDeque.size() - 1;
	return getNextJacobsthal(up + 1) - 1;
}


void PmergeMe::sortJacobsthalDeque(std::deque<int>& largeDeque, std::deque<int>& smallDeque)
{
    unsigned long low = 1, up = getUpIndexDeque(smallDeque), count = up;

	largeDeque.insert(largeDeque.begin(), smallDeque.at(0));
	for (; low < smallDeque.size();) {
		BinaryDeque(largeDeque, smallDeque.at(count), up + low - 1);
		if (count == low) {
			low = up + 1;
			up = getNextUpIndexDeque(smallDeque, up);
			count = up;
		} else
			count--;
	}
	if (largeDeque.front() == -1)
        largeDeque.erase(largeDeque.begin());
	_sortedDeque = largeDeque;
}

void PmergeMe::findLimitsDeque() {
	std::deque<std::pair<int, int> >::iterator it = _pairDeque.begin();
	for (; it < _pairDeque.end(); it++) {
		_smallDeque.push_back(it->first);
		_largeDeque.push_back(it->second);
	}
}

void PmergeMe::mergeDeque(std::deque<std::pair<int, int> >& pairDeque, int lhs, int middle, int rhs) {
	int left = middle - lhs + 1, right = rhs - middle;
	std::deque<std::pair<int, int> > leftPairDeque(left), rightPairDeque(right);
	for (int i = 0; i < left; ++i) leftPairDeque[i] = pairDeque[lhs + i];	  
	for (int j = 0; j < right; ++j) rightPairDeque[j] = pairDeque[middle + 1 + j]; 
	int i = 0, j = 0, newLeft = lhs;

	while (i < left && j < right) {
		if (leftPairDeque[i].second <= rightPairDeque[j].second) {
			pairDeque[newLeft] = leftPairDeque[i++];
		} else
			pairDeque[newLeft] = rightPairDeque[j++];
		newLeft++;
	}
	while (i < left) pairDeque[newLeft++] = leftPairDeque[i++];
	while (j < right) pairDeque[newLeft++] = rightPairDeque[j++];
}

void PmergeMe::sortLargestDeque(std::deque<std::pair<int, int> > &pairDeque, int lhs, int rhs)
{
    if (lhs < rhs) {
		int middle = lhs + (rhs - lhs) / 2;
		sortLargestDeque(pairDeque, lhs, middle);	   
		sortLargestDeque(pairDeque, middle + 1, rhs); 
		mergeDeque(pairDeque, lhs, middle, rhs);
	}
}

void PmergeMe::comparePairsDeque()
{
	for (std::deque<std::pair<int, int> >::iterator it = _pairDeque.begin(); it != _pairDeque.end(); it++ )
	{
		if(it->first > it->second)
		{
            std::swap(it->first, it->second);
		}
	}
}

void PmergeMe::splitPairsDeque()
{
    std::deque<int>::iterator prev = _mDeque.begin(), next = _mDeque.begin();

	for (; next < _mDeque.end(); next++) {
		if (std::distance(_mDeque.begin(), next) % 2 != 0)
			_pairDeque.push_back(std::make_pair(*prev, *next));
		prev = next;
	}
    if (_mDeque.size() % 2 != 0)
        _pairDeque.push_back(std::make_pair(*prev, -1));
}

void PmergeMe::sortDeque()
{
    _startDeque = clock();
    if (_mDeque.size() <= 1)
        return;
    splitPairsDeque();
    comparePairsDeque();
    sortLargestDeque(_pairDeque, 0, _pairDeque.size() - 1);
    findLimitsDeque();
    sortJacobsthalDeque(_largeDeque, _smallDeque);
    _endDeque = clock();

}