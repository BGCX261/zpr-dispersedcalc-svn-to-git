/*
 *  MessageBox.h
 *
 *  Created on: 2009-01-08
 *  Author: Piotr Doniec
 *  		Lukasz Trzaska
 */

#ifndef _DISPCALC_MESSAGEBOX_H_
#define _DISPCALC_MESSAGEBOX_H_

#include <boost/thread/mutex.hpp>
#include <map>

namespace dispcalc
{
//! Klasa reprezentuje skrzynke do komunikacji posredniej miedzy watkami
/*
 * przechowuje wiadomosci oraz historie wiadomosci do debugowania
 */
template<typename T>
class  MessageBox {
public:
	MessageBox() {};

	//! Bezpieczny sposob umieszczania wiadomosci w skrzynce
	/*
	 * \param d destination-do kogo wysylamy
	 */

	void postMessage(std::string d, T m)
	{
		boost::interprocess::scoped_lock<boost::mutex> lock(messageInboxMutex_);
		inbox_.insert(std::make_pair(d, m));
		inboxHistory_.push_back(m);
	}

	//! Bezpieczny sposob pobierania wiadomosci ze skrzynki
	/*!
	   \param m referencja do obiektu przez ktory zostanie
	  		  zwrocona wiadomosc tzw. reference return type
	   \return False jezeli skrzynka jest pusta
	*/
	bool getMessage(std::string d, T* m)
	{
		boost::interprocess::scoped_lock<boost::mutex> lock(messageInboxMutex_);
		if(!inbox_.empty()) {
//			std::cout << "Poszukiwanie wiadomosci dla " << d << std::endl;
			T copy;
			typename std::multimap<std::string, T>::iterator iter;
			iter = inbox_.find(d);
			if(iter != inbox_.end())
			{
				copy = (*iter).second;
				*m = copy;
				//memcpy(m, &copy, sizeof(T));
				inbox_.erase(iter);
				return true;
			}
			return false;
		}
		return false;
	}

private:
	std::multimap<std::string, T> inbox_; ///<  <cel, wiadomosc>
	std::vector<T> inboxHistory_; ///< historia wiadomosci, kolejnosc hronologiczna
	boost::mutex messageInboxMutex_; ///< synchronizacja dostepu do skrzynki
};

}

#endif /* _DISPCALC_MESSAGEBOX_H_ */
