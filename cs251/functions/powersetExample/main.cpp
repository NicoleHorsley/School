/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Evaluate relations between domain (set A) and codomain(set B)
 *
 *        Version:  1.0
 *        Created:  04/24/2015 12:38:31 AM
 *       Revision:  none
 *       Compiler:  g++ -lgmp -lgmpxx
 *
 *         Author:  Jonathon Sonesen
 *   Organization:  PCC CS251
 *
 * =====================================================================================
 */
#include <iostream>                                             //I/O
#include <algorithm>                                            
#include <string>
#include <set>
#include <tuple>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

            
typedef string elementtype;                                     //Datatpe for domain and codomain elements
typedef set<elementtype> settype;                               //Sets of strings 
typedef set<tuple<elementtype, elementtype>> crosstype;         //Cross product is a tuple
typedef set<crosstype> powertype;                               //Power set is a set of sets

//Functions to generate sets
powertype * powerset(crosstype *);                             //power set generator 
crosstype * crossproduct(settype *, settype *);                //cross product generator
settype * loadSet(string);                                     //load domain or codomain from user

//Functions to check properties of power set elements
//elements are ordered pairs
bool isFunction(crosstype relation, settype * domain);
bool isInjective(crosstype relation, settype *domain, settype *codomain);
bool isSurjective(crosstype relation, settype * domain, settype *codomain);
bool isBijective(crosstype relation, settype * domain, settype *codomain);
int main ()
{
    settype * domain   = loadSet("DOMAIN");                       //load domain
    settype * codomain = loadSet("CODOMAIN");                     //load codomain
    crosstype * cross  = crossproduct(domain, codomain);          //generate cross product
    powertype * power  = powerset(cross);                         //generate the power set

    //Print info on sets
    cout << "Domain has " << domain->size() << " elements" << endl;
    cout << "CoDomain has " << codomain->size() << " elements" << endl;
    cout << "Power set of cross product has " << power->size() << " elements";

    //Print coss product roster. The cross product is a set of ordered pairs. They
    //are represented with the <tuple> container. get is used to access the 0 or
    //1 element of the ordered pair. Then conatiner iterators can be used to run through
    cout << "Cross Product: {";
    for(auto ptr = cross->begin(); ptr != cross->end(); ptr++)
    {
        cout << " (" << std::get<0>(*ptr) << "," << std::get<1>(*ptr) << ") ";
    }
    cout << "}" << endl;

    //Print out the power set roster. The power set is a set of sets. In other words
    //each element of the powerset is a set of tuple. So once you access a particular
    //element of that power set you need a second loop to acess the the tuples of the element
    //
    //we also use filters to identify properties of the elements, later I will add a filter for
    //surjective and bijective
    //This prints the number of functions and tests the power set
    cout << "\nThere are " << pow(domain->size(), codomain->size())
         << " functions from A to B: " <<  endl;
    int row = 0;
    for (auto ptrA = power->begin(); ptrA != power->end(); ptrA++)
    {
    
        auto testF = isFunction(*ptrA, domain) ? 'F' : ' ';
        auto testS = isSurjective(*ptrA, domain, codomain) ? 'S' : ' ';
        auto testB = isBijective(*ptrA, domain, codomain) ? 'B' : ' ';
        if(testF == 'F')
        {
            cout << setw(3) << setfill (' ') << ++row << ". " << "{";
            
            //Loop fo inner tuples
            for(auto ptr = ptrA->begin(); ptr != ptrA->end();ptr++)
            {
                cout << " (" << std::get<0>(*ptr) << "," << std::get<1>(*ptr) << ") ";
            }
            cout << "}" << endl;
        }
    }
    cout << "}" << endl;
    

    cout << "\nThere are " << pow(domain->size(), codomain->size())
         << " injective functions from A to B: " <<  endl;
    row = 0;
    for (auto ptrA = power->begin(); ptrA != power->end(); ptrA++)
    {
        auto testI = isInjective(*ptrA, domain, codomain) ? 'I' : ' ';
    
        if(testI == 'I')
        {
            cout << setw(3) << setfill (' ') << ++row << ". " << "{";
            
            //Loop fo inner tuples
            for(auto ptr = ptrA->begin(); ptr != ptrA->end();ptr++)
            {
                cout << " (" << std::get<0>(*ptr) << "," << std::get<1>(*ptr) << ") ";
            }
            cout << "}" << endl;
        }
    }
    cout << "}" << endl;

    //Surjective

        auto testI = isInjective(*ptrA, domain, codomain) ? 'I' : ' ';
    //clean up them leaks boy
    delete domain;
    delete codomain;
    delete cross;
    delete power;
    return 0;
}
/*
 * Filter: Surjective Function
 * Parm: crosstype powerset, settype domain,settype codomain
 */
bool isSurjective( crosstype  relation, settype * domain, settype * codomain)
{
    if(!isFunction(relation, domain)) return false;
    if(domain->size() !=  codomain->size()) return false;
   
    //Check that appears all elements of codomain are in relation set
    set<elementtype> relem;
    for(auto ptr = relation.begin(); ptr != relation.end(); ptr++)
    {
        //get codomain element
        auto buffer = std::get<1>(*ptr);
        if ( relem.find(buffer) == relem.end() ) return false;
        relem.insert(buffer);
    }
    return true;
}
/* Filter: Function
 * Parm: crosstype powerset, settype domain
 * Return: true is relation is a function false otherwise
 * Check domain consumption and unique image 
 * */
bool isFunction( crosstype relation, settype * domain)
{
   set<elementtype> relem;                                              //Set of elements from the tuples
   for (auto ptr = relation.begin();ptr != relation.end(); ptr++)
   {
        auto buffer = std::get<0>(*ptr);                                //Get first element of ordered pair (the domain contribution)
        relem.insert(buffer);                                           //Add to tuples
   }
   if (relem.size() != domain->size()) return false;
   if (relation.size() != domain->size()) return false;
   return true;
}

bool isInjective( crosstype  relation, settype * domain, settype * codomain)
{
    if(isFunction(relation, domain) == false) return false;
    if(codomain->size() < domain->size()) return false;


    //Make sure codomain element appears once
    set<elementtype> relem;
    for(auto ptr = relation.begin(); ptr != relation.end(); ptr++)
    {
        auto buffer = std::get<1>(*ptr);
        //Get codomain cotributed to ordered pair
        if (relem.find(buffer) != relem.end()) return false;
        relem.insert(buffer);
    }
    return true;
}

/*Filter: Bijective function
 * Parm: powerset, domain, codomain
 * Return: true function is both injective and surjective
 */
bool isBijective( crosstype relation, settype *domain, settype * codomain)
{
    if( !isFunction(relation, domain)) return false;
    if(!isSurjective(relation, domain, codomain)) return false;
    if(!isInjective(relation, domain, codomain)) return false;

    
    return true;
}

/*Create power set
 * Parm: Address of cross product set
 * Return: Address of power set of cross product
 * 
 */
powertype * powerset(crosstype * cross)
{
    typedef crosstype::iterator sPtr;
    typedef std::vector<sPtr>vec;
    typedef vec::iterator vPtr;
    struct local
    {
        static tuple<elementtype, elementtype> dereference(sPtr v)
        {
            return *v; 
        }
    };
    powertype * result = new powertype;                                 //the resulting powersey
    vec elements;                                                       //element stack

    do 
    {
        crosstype tmp;

        //copy initial elemens and insert them into the power set element
        std::transform(elements.begin(), elements.end(), std::inserter(tmp, tmp.end()), local::dereference);
        result->insert(tmp);

        //Check the stack for more elements. If more exist, pump them to the stack
        if(!elements.empty() && ++elements.back() == cross->end())
        {
            elements.pop_back();
        } else {
            sPtr iter;
            if(elements.empty() == true)
            {
                iter = cross->begin();
            } else {
                iter = elements.back();
                ++iter;
            }
            for(; iter != cross->end(); ++iter)
            {
                elements.push_back(iter);
            }
        }
    } while (!elements.empty());
    return result;
}

/* Load set
 * Parm: Name of set
 * Return: Address of set
 */
settype * loadSet(string parm)
{
    settype * result = new settype;                             //Result
    elementtype input;                                          //user input buffer

    cout << "Enter elements for " << parm << " (space seperated): ";    //prompt
    while (cin.peek() != '\n') 
    {
        cin >> input;                                           //Read element
        result->insert(input);                                  //Add to set
    }
    cin.ignore(8192, '\n');                                     //eat end line
    return result;
}

/* Generate cross product
 * Parm: domain, co-domain sets
 * Return: address of cross product
 */
crosstype * crossproduct(settype * domain, settype * codomain)
{
    crosstype * result = new crosstype;
    elementtype first, second;

    for(settype::iterator ptrA = domain->begin(); ptrA != domain->end(); ptrA++)
    {
        for (settype::iterator ptrB = codomain->begin(); ptrB != codomain->end(); ptrB++)
        {
            auto element = make_tuple(*ptrA, *ptrB);
            result->insert(element);
        }
    }
    return result;
}

