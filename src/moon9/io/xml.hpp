/*
 * XML DOM parser/XPath queries/tree builder class

 * Copyright (c) 2010 Mario 'rlyeh' Rodriguez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.

 * To do:
 * - SAX parser
 * - subtree insertion, removal and reallocation (doable thru xPath syntax?)

 * - rlyeh ~~ listening to The Cure / A forest (live 1984)
 */

#pragma once

#include <vector>
#include <string>

#include <pugixml/pugixml.hpp>

#include "string.hpp"

namespace moon9
{
    class xml
    {
        public:

        enum { verbose = false };

        xml();
        xml( const xml &t );
        xml( const std::string &t );
        xml &operator=( const xml &t );

        // declarative construction

        // push a level of depth, then insert node
        xml &push( const moon9::iostring &key, const moon9::iostring &text = "", const moon9::iostring &comment = "" );
        xml &push( const pugi::xml_node &node );

        // insert node at current depth
        xml &node( const moon9::iostring &key, const moon9::iostring &text = "", const moon9::iostring &comment = "" );
        xml &node( const pugi::xml_node &node );

        // set attribute for current node
        xml &set( const moon9::iostring &key, const moon9::iostring &value );
        xml &set( /*const*/ pugi::xml_attribute &attribute );

        // pop a level of depth back
        xml &back();

        // XPath query api
#if 0
        template <typename T>    T query(       const std::string &XPath_fmt, ... ) const;
        template <typename T> void query( T &t, const std::string &XPath_fmt, ... ) const;
#else
        template <typename T>
        T query( const std::string &XPath ) const
        {
            try
            {
                std::string result = pugi::xpath_query( XPath.c_str() ).evaluate_string( doc );

                return moon9::iostring( result ).as<T>();
            }
            catch (const pugi::xpath_exception& e)
            {
                std::cerr << "Select failed: " << e.what() << std::endl;
            }
            catch( ... )
            {
                std::cerr << "Exception raised!" << std::endl;
            }

            return T();
        }

        template <typename T>
        bool query( T &t, const std::string &XPath ) const
        {
            try
            {
                std::string result = pugi::xpath_query( XPath.c_str() ).evaluate_string( doc );

                t = moon9::iostring( result ).as<T>();
                return true;
            }
            catch (const pugi::xpath_exception& e)
            {
                std::cerr << "Select failed: " << e.what() << std::endl;
            }
            catch( ... )
            {
                std::cerr << "Exception raised!" << std::endl;
            }

            t = T();
            return false;
        }

        template <typename T, typename T1>
        bool query( T &t, const std::string &XPath, const T1 &t1 ) const
        {
            return query( t, moon9::iostring( XPath, t1 ) );
        }

        template <typename T, typename T1, typename T2>
        bool query( T &t, const std::string &XPath, const T1 &t1, const T2 &t2 ) const
        {
            return query( t, moon9::iostring( XPath, t1, t2 ) );
        }

        template <typename T, typename T1, typename T2, typename T3>
        bool query( T &t, const std::string &XPath, const T1 &t1, const T2 &t2, const T3 &t3 ) const
        {
            return query( t, moon9::iostring( XPath, t1, t2, t3 ) );
        }

        template <typename T, typename T1, typename T2, typename T3, typename T4>
        bool query( T &t, const std::string &XPath, const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4 ) const
        {
            return query( t, moon9::iostring( XPath, t1, t2, t3, t4 ) );
        }

#endif
        // to unify, replace & deprecate. allow xml arithmetic

                xml subtree( const std::string &XPath ) const;
        std::string subtext( const std::string &XPath ) const;
                xml tree( const std::string &XPath ) const;
        std::string text( const std::string &XPath ) const;

        // serialization

        std::string str() const;
               void str( const std::string &data );

        // iteration
        // iterators

        typedef pugi::xml_node_iterator      iterator;
        typedef pugi::xml_attribute          attribute;
        typedef pugi::xml_attribute_iterator attribute_iterator;

                  iterator begin() const;
                  iterator end() const;
        attribute_iterator attribute_begin() const;
        attribute_iterator attribute_end() const;

        template <typename T> void walker( T &t )
        //;
        //template <typename T>   //@todo: attr
        //void xml::walker( T &t )
        {
            simple_walker<T> walker(t);
            doc.traverse(walker);
        }


        protected:

        //details:

        pugi::xml_document doc;
        pugi::xml_node latest;
        std::vector< pugi::xml_node > parent;

        void update_node( pugi::xml_node &node, const moon9::iostring &key, const moon9::iostring &text, const moon9::iostring &comment);
        void update_node( pugi::xml_node &node, const pugi::xml_node &content );

        template <typename T>
        struct simple_walker : pugi::xml_tree_walker
        {
            T &t;

            simple_walker( T &_t ) : t(_t)
            {}

            virtual bool for_each(pugi::xml_node& node)
            {
                static const char* node_types[] =
                {
                    "null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"
                };

                return t.operator()( depth(), node_types[node.type()], node.name(), node.value() );
            }
        };
    };

} //namespace moon9

namespace pack
{
    // note: <moon9/io/bistring.hpp> defines also a compatible 'bistring(s)' type
    typedef std::pair<std::string,std::string> bistring;
    typedef std::vector< bistring > bistrings;
    std::string xml( const bistrings &bs );
}

namespace unpack
{
    // note: <moon9/io/bistring.hpp> defines also a compatible 'bistring(s)' type
    typedef std::pair<std::string,std::string> bistring;
    typedef std::vector< bistring > bistrings;
    bistrings xml( const std::string &s );
}

