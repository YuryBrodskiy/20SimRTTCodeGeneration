/**********************************************************
 * This file is generated by 20-sim C++ Code Generator
 *
 *  file:  %FILE_NAME%
 *  subm:  %SUBMODEL_NAME%
 *  model: %MODEL_NAME%
 *  expmt: %EXPERIMENT_NAME%
 *  date:  %GENERATION_DATE%
 *  time:  %GENERATION_TIME%
 *  user:  %USER_NAME%
 *  from:  %COMPANY_NAME%
 *  build: %GENERATION_BUILD%
 *
 **********************************************************/

/* This file describes the model functions
   that are supplied for computation.

   The model itself is the %SUBMODEL_NAME%.cpp file
*/

#ifndef %SUBMODEL_NAME%_H
#define %SUBMODEL_NAME%_H

/* 20-sim include files */
#include "xxfuncs.h"
#include "xxmatrix.h"
#include "xxmodel.h"
#include "xxinteg.h"

/* parameter parsing include */
#include "tinyxml.h"

/* OROCOS include files */
#include <rtt/TaskContext.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Port.hpp>
#include <rtt/Activity.hpp>
#include <rtt/RTT.hpp>
#include <string>
#include <vector>
#include <rtt/Property.hpp>
#include <rtt/PropertyBag.hpp>
#include <rtt/Time.hpp>

class %SUBMODEL_NAME%: virtual Submodel20sim , public RTT::TaskContext
{
	public:
		enum stateflags_%SUBMODEL_NAME% {initialrun, mainrun, finished};

		/**
		 * %SUBMODEL_NAME% constructor
		 */
		%SUBMODEL_NAME%(string name = "my%SUBMODEL_NAME%");

		/**
		 * %SUBMODEL_NAME% destructor
		 */
		virtual ~%SUBMODEL_NAME%(void);

		/**
		 * %SUBMODEL_NAME% configuration code and returns false if startup fails
		*/
		bool configureHook ();

		/**
		 * %SUBMODEL_NAME% startUp code and returns false if startup fails 
		*/
		bool startHook ();

		/**
		 * %SUBMODEL_NAME% Calculation executed in this Hook.
		 */
		void updateHook ();

		/**
		 * %SUBMODEL_NAME% Terminate
		 */
		void stopHook ();

		XXDouble GetTime(void)
		{
			return %VARPREFIX%%XX_TIME%;
		}

		stateflags_%SUBMODEL_NAME% state;

		virtual bool setPeriod(RTT::Seconds s);

	protected:
		/**
		 * CalculateDynamic()
		 * This function calculates the dynamic equations of the model.
		 * These equations are called from the integration method
		 * to calculate the new model rates (that are then integrated).
		 */
		void CalculateDynamic (void);

	private:
		/* internal submodel computation methods */

		XXDouble u [%NUMBER_INPUTS% + 1]; // Optimization for CopyInputsToVariables
		XXDouble y [%NUMBER_OUTPUTS% + 1]; //Optimization for CopyVariablesToOutputs

		/**
		 * CalculateInitial()
		 * This function calculates the initial equations of the model.
		 * These equations are calculated before anything else
		 */
		void CalculateInitial (void);

		/**
		 * CalculateStatic()
		 * This function calculates the static equations of the model.
		 * These equations are only dependent from parameters and constants
		 */
		void CalculateStatic (void);

		/**
		 * CalculateInput()
		 * This function calculates the input equations of the model.
		 * These equations are dynamic equations that must not change
		 * in calls from the integration method (like random and delay).
		 */
		void CalculateInput (void);

		/**
		 * CalculateOutput()
		 * This function calculates the output equations of the model.
		 * These equations are not needed for calculation of the rates
		 * and are kept separate to make the dynamic set of equations smaller.
		 * These dynamic equations are called often more than one time for each
		 * integration step that is taken. This makes model computation much faster.
		 */
		void CalculateOutput (void);

		/**
		 * CalculateFinal()
		 * This function calculates the final equations of the model.
		 * These equations are calculated after all the calculations
		 * are performed
		 */
		void CalculateFinal (void);

		/**
		 * CopyInputsToVariables
		 * This private function copies the input variables from the input vector
		 * @param u	This is the array with all input signals for this submodel
		 */
		void CopyInputsToVariables ();

		/**
		 * CopyVariablesToOutputs
		 * This private function copies the output variables to the output vector
		 * @param y	This is the array with all output signals from this submodel
		 */
		void CopyVariablesToOutputs ();

		/**
		 * initProperty()
		 * This function checkes a generated xml file and defines orocos property.
		 * These properties can then be changed at run time or by using configuration file
		 */
		void initParameters(void);

		%INTEGRATION_METHOD_NAME% myintegmethod;	///< pointer to the integration method for this submodel

		/**
		 * OROCOS Ports for input and ouput
		 */

		RTT::InputPort<double> %VARPREFIX%Input[%NUMBER_INPUTS% + 1];
		RTT::OutputPort<double> %VARPREFIX%Output[%NUMBER_OUTPUTS% + 1];

		/**
		 * Property bag for 20sim parameters.
		 */
		RTT::PropertyBag param_bag;

		/**
		 * Save the 20sim parameters on exit?
		 */
		bool save_properties_on_exit;

};

#endif	// %SUBMODEL_NAME%_H
