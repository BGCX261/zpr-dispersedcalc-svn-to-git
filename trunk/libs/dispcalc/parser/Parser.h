#include <boost/spirit/core.hpp>
#include <iostream>
#include <string>
#include <list>
#include "../libiqxmlrpc/value.h"

using namespace std;
using namespace boost::spirit;

namespace parser_product
{
	extern std::list<std::string> __expressionStack;
}

//namespace
//{
			void    do_int(char const* str, char const* end);
			void    do_add(char const*, char const*);
			void    do_subt(char const*, char const*);
			void    do_mult(char const*, char const*);
			void    do_div(char const*, char const*);
			void    do_neg(char const*, char const*);
//}

struct Parser : public grammar<Parser>
{
    template <typename ScannerT>
    struct definition {

        definition(Parser const&)
        {
            first = (

                expression =
                    term
                    >> *(   ('+' >> term)[&do_add]
                        |   ('-' >> term)[&do_subt]
                        )
                ,

                term =
                    factor
                    >> *(   ('*' >> factor)[&do_mult]
                        |   ('/' >> factor)[&do_div]
                        )
                ,

                factor
                    =   lexeme_d[(+digit_p)[&do_int]]
                    |   '(' >> expression >> ')'
                    |   ('-' >> factor)[&do_neg]
                    |   ('+' >> factor)
            );

            BOOST_SPIRIT_DEBUG_NODE(first);
            BOOST_SPIRIT_DEBUG_NODE(expression);
            BOOST_SPIRIT_DEBUG_NODE(term);
            BOOST_SPIRIT_DEBUG_NODE(factor);
        }

        subrule<0>  expression;
        subrule<1>  term;
        subrule<2>  factor;

        rule<ScannerT> first;
        rule<ScannerT> const&
        start() const { return first; }
    };
};



