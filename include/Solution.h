/**
 * Solution.h
 *
 * File defining the class Solution.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_SOLUTION_H_
#define INCLUDE_SOLUTION_H_

class Solution {
public:
	virtual ~Solution(){};

	virtual double getFitness() const = 0;

	virtual void copy(Solution&) = 0;
};



#endif /* INCLUDE_SOLUTION_H_ */
