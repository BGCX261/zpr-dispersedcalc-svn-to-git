#include <stdio.h>
#include <ctype.h>
#include <stack>
#include <iostream>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/thread/mutex.hpp>

#include "../libiqxmlrpc/libiqxmlrpc.h"


namespace dispcalc
{

class ServerStat
{
protected:
	//! Prosta metoda umieszczajaca status w mapie statusow
	/*!
		Metoda synchroznizuje dostep do globalnej skrzynki
		przechowujacej aktualnie wykonywana operacje;

		\param Wiadomosc do umieszczenia w mapie
		\param Identyfikator wykonujacego watku
	 */
	void postStat(const std::string&, const std::string&);

	std::string  identifier_;
};
//! Metoda serwera zwracajaca status
/*!
	Metoda wywolywana celem sprawdzenie czy serwer jest gotowy
    przyjac zadania do przetworzenie
*/
class ServerMethodStatus : public iqxmlrpc::Method, public ServerStat  {
public:
    void execute(const iqxmlrpc::Param_list& params, iqxmlrpc::Value& retval);
};

//! Metoda serwera odpowiedzielna za przeprowadzanie obliczen
/*!
	Do metody przekazywana jest poddrzewo nad ktorego wartosc ma
	zostac obliczona przez aplikacje serwera.
 */
class ServerMethodCompute : public iqxmlrpc::Method, public ServerStat {
public:
    void execute (const iqxmlrpc::Param_list& params, iqxmlrpc::Value& retval);
private:
	std::stack<int> numStack_;
};

//! Metoda serwera odpowiedzielna za sprawdzenie serwera
/*!
	Do metody przekazywany jest adres serwera, ktory ma sprawdzic
 */
class ServerMethodConnTest : public iqxmlrpc::Method {
public:
	void execute (const iqxmlrpc::Param_list& params, iqxmlrpc::Value& retval);

};

}
