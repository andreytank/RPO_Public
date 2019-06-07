/**
 * RPOObjectAssignmentOperation.h
 *
 * File defining the class RPOObjectAssignmentOperation.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef __RPOOBJECTASSIGNMENTOPERATION_H__
#define __RPOOBJECTASSIGNMENTOPERATION_H__

#include <RPOChangeOperation.h>
#include <RPOSolution.h>

/**
 * Class that codifies an operation of assigning an object to a knapsack.
 * The knapsack can be 0, that is, the object is removed from the knapsack
 * in which it is currently stored.
 */
class RPOObjectAssignmentOperation : public RPOChangeOperation {

protected:
	/*
	 * Properties of this class, according to the indications in the
	 * lab assignment (_indexObj, _indexKnapsack, _deltaFitness)
	 */
	int _indexParam;
	int _paramValue;
	double _deltaFitness;

public:

	/**
	 * Constructor
	 */
	RPOObjectAssignmentOperation();

	/**
	 * Destructor
	 */
	virtual ~RPOObjectAssignmentOperation();

	/**
	 * Function which applies the change defined by this object to the solution
	 * received as an argument
	 * @param[in, out] solution Solution over which the change will be applied
	 */
	virtual void apply(RPOSolution &solution);

	/**
	 * Function for assigning the values of the operation
	 * @param[in] indexObject Index of the object which is going to be changed
	 * @param[in] indexKnapsack Index of the knapsack where the object will be introduced.
	 * @param[in] deltaFitness Difference in fitness of the solution after applying the change,
	 *                         provided that the solution has not changed since the object
	 *                         of this class was created
	 */
	void setValues(int indexParam, int paramValue, double deltaFitness);

	unsigned getParam(){
		return _indexParam;
	}

	/**
	 * Function which returns the difference of fitness by the operation
	 * @return Difference of fitness of the operation (if the current solution has not changed
	 *         when the difference was calculated)
	 */
	double getDeltaFitness(){
		return _deltaFitness;
	}

	/**
	 * Function returning the knapsack considered by the operation
	 * @return Index of the knapsack considered by the operation
	 */
	unsigned getparamValue(){
		return _paramValue;
	}

};

#endif
