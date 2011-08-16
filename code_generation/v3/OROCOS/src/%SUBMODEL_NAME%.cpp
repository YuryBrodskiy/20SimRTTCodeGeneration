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
 **********************************************************/

/* Standard include files */
#include <stdio.h>
#include <math.h>

/* 20-sim include files */
#include "%SUBMODEL_NAME%.h"

/* Orocos include */
#include <boost/algorithm/string.hpp>
#include <rtt/marsh/Marshalling.hpp>

using namespace Orocos;
using namespace RTT;
using namespace std;

const string XXsim_config_xml = "config/%SUBMODEL_NAME%_base_config.xml";
const string Orocos_config_xml = "config/%SUBMODEL_NAME%_overrides_config.cpf";


	/* this PRIVATE function sets the input variables from the input vector */
	//@todo Improve for multiple component inputs to have a synchronized execution.
void %SUBMODEL_NAME%::CopyInputsToVariables ()
{
	/* OROCOS Entry to copy port to input array */
	double val = 0.0;

	for (int i=0;i<%NUMBER_INPUTS%; ++i)
	{
		  if ( %VARPREFIX%Input[i].read(val) != RTT::NoData ) {
		      u[i] = val;
		  }
	}

	/* copy the input vector to the input variables */
	%INPUT_TO_VARIABLE_EQUATIONS%
}

/* this PRIVATE function uses the output variables to fill the output vector */
void %SUBMODEL_NAME%::CopyVariablesToOutputs ()
{
	/* copy the output variables to the output vector */
	%VARIABLE_TO_OUTPUT_EQUATIONS%

	/* OROCOS Entry to copy output to port */
	for (int i=0;i<%NUMBER_OUTPUTS%; ++i)
	{
		  %VARPREFIX%Output[i].write(y[i]);
	}
}

%SUBMODEL_NAME%::%SUBMODEL_NAME%(string name): TaskContext(name, PreOperational), save_properties_on_exit(false)
{
	//------------------ 20-sim ------------------------------
	%VARPREFIX%start_time = %START_TIME%;
	%VARPREFIX%finish_time = %FINISH_TIME%;
	%VARPREFIX%step_size = %TIME_STEP_SIZE%;
	%VARPREFIX%%XX_TIME% = 0;
	%VARPREFIX%major = true;

	%VARPREFIX%number_constants = %NUMBER_CONSTANTS%;
	%VARPREFIX%number_parameters = %NUMBER_PARAMETERS%;
	%VARPREFIX%number_initialvalues = %NUMBER_INITIAL_VALUES%;
	%VARPREFIX%number_variables = %NUMBER_VARIABLES%;
	%VARPREFIX%number_states = %NUMBER_STATES%;
	%VARPREFIX%number_rates = %NUMBER_STATES%;
	%VARPREFIX%number_matrices = %NUMBER_MATRICES%;
	%VARPREFIX%number_unnamed = %NUMBER_UNNAMED%;

	/* the variable arrays */
	%VARPREFIX%%XX_CONSTANT_ARRAY_NAME% = new XXDouble[%NUMBER_CONSTANTS% + 1];		/* constants */
	%VARPREFIX%%XX_PARAMETER_ARRAY_NAME% = new XXDouble[%NUMBER_PARAMETERS% + 1];		/* parameters, currently only one type of parameter exists: double */
	%VARPREFIX%%XX_INITIAL_VALUE_ARRAY_NAME% = new XXDouble[%NUMBER_INITIAL_VALUES% + 1];		/* initial values */
	%VARPREFIX%%XX_VARIABLE_ARRAY_NAME% = new XXDouble[%NUMBER_VARIABLES% + 1];		/* variables */
	%VARPREFIX%%XX_STATE_ARRAY_NAME% = new XXDouble[%NUMBER_STATES% + 1];		/* states */
	%VARPREFIX%%XX_RATE_ARRAY_NAME% = new XXDouble[%NUMBER_STATES% + 1];		/* rates (or new states) */
	%VARPREFIX%%XX_MATRIX_ARRAY_NAME% = new XXMatrix[%NUMBER_MATRICES% + 1];		/* matrices */
	%VARPREFIX%%XX_UNNAMED_ARRAY_NAME% = new XXDouble[%NUMBER_UNNAMED% + 1];		/* unnamed */
	%VARPREFIX%workarray = new XXDouble[%WORK_ARRAY_SIZE% + 1];

	myintegmethod.Initialize(this);

	state = initialrun;

	//------------------orocos------------------------------
	this->addProperty("integration_step_size", %VARPREFIX%step_size ).doc("Integration step size.");

	this->addProperty("save_parameters_on_exit", save_properties_on_exit ).doc("Save the parameters on exit.");

	initParameters();

	string inputstr[%NUMBER_INPUTS%] = {%INPUT_NAMES%};
	string outputstr[%NUMBER_OUTPUTS%] = {%OUTPUT_NAMES%};

	for (int i=0;i<%NUMBER_INPUTS%; ++i )
	{
		  this->ports()->addPort(inputstr[i],%VARPREFIX%Input[i]).doc("Input port");
	}

	for (int i=0;i<%NUMBER_OUTPUTS%; ++i )
	{
		  this->ports()->addPort(outputstr[i],%VARPREFIX%Output[i]).doc("Output port");
	}
}

%SUBMODEL_NAME%::~%SUBMODEL_NAME%(void)
{
	if(save_properties_on_exit)
	{
		this->getProvider<Marshalling>("marshalling")->writeProperties( Orocos_config_xml );
	}

	/* free memory */
	delete[] %VARPREFIX%%XX_CONSTANT_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_PARAMETER_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_INITIAL_VALUE_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_VARIABLE_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_STATE_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_RATE_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_MATRIX_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_UNNAMED_ARRAY_NAME%;
	delete[] %VARPREFIX%workarray;
}

bool %SUBMODEL_NAME%::configureHook()
{
	if(! TaskContext::configureHook())
	{
		return false;
	}

	/* initialization phase (allocating memory) */
	%VARPREFIX%initialize = true;

	/* copy the inputs */
	//%VARPREFIX%%XX_TIME% = t;

	/* set the constants */
	%INITIALIZE_CONSTANTS%

	/* set the parameters */
	%INITIALIZE_PARAMETERS%

	/* set the states */
	%INITIALIZE_STATES%

	/* set the matrices */
	%INITIALIZE_MATRICES%

	/* end of initialization phase */
	%VARPREFIX%initialize = false;

	return true;
}


/* the initialization function for submodel */
bool %SUBMODEL_NAME%::startHook()
{
	if(! TaskContext::startHook())
	{
		return false;
	}

	/* calculate initial and static equations */
	CalculateInitial ();
	CalculateStatic ();
	CopyInputsToVariables ();
	CalculateInput ();
	CalculateDynamic();
	CalculateOutput ();
	CopyVariablesToOutputs ();
	return true;
}

/* the function that calculates the submodel */
void %SUBMODEL_NAME%::updateHook ()
{
	TaskContext::updateHook();

	/* another precessor submodel could determine the parameters of this submodel
	   and therefore the static parameter calculations need to be performed. */
	CalculateStatic ();

	/* main calculation of the model */
	CopyInputsToVariables ();        //get input from port
	CalculateInput ();
	myintegmethod.Step();
	CalculateOutput ();
	CopyVariablesToOutputs ();       //send output to port
}

/* the termination function for submodel */
void %SUBMODEL_NAME%::stopHook()
{
	TaskContext::stopHook();

	/* copy the inputs */
	CopyInputsToVariables ();       //get inputs from port

	/* calculate the final model equations */
	CalculateFinal ();

	/* set the outputs */
	CopyVariablesToOutputs ();     //send output to port
}


/* This function calculates the initial equations of the model.
 * These equations are calculated before anything else
 */
void %SUBMODEL_NAME%::CalculateInitial (void)
{
  %INITIAL_EQUATIONS%
}

/* This function calculates the static equations of the model.
 * These equations are only dependent from parameters and constants
 */
void %SUBMODEL_NAME%::CalculateStatic (void)
{
  %STATIC_EQUATIONS%
}

/* This function calculates the input equations of the model.
 * These equations are dynamic equations that must not change
 * in calls from the integration method (like random and delay).
 */
void %SUBMODEL_NAME%::CalculateInput (void)
{
  %INPUT_EQUATIONS%
}

/* This function calculates the dynamic equations of the model.
 * These equations are called from the integration method
 * to calculate the new model rates (that are then integrated).
 */
void %SUBMODEL_NAME%::CalculateDynamic (void)
{
  %DYNAMIC_EQUATIONS%
}

/* This function calculates the output equations of the model.
 * These equations are not needed for calculation of the rates
 * and are kept separate to make the dynamic set of equations smaller.
 * These dynamic equations are called often more than one time for each
 * integration step that is taken. This makes model computation much faster.
 */
void %SUBMODEL_NAME%::CalculateOutput (void)
{
  %OUTPUT_EQUATIONS%
  %IF%%XX_NR_DELAY_FUNCS%
	/* delay update */
	XXDelayUpdate();
  %ENDIF%
}

/* This function calculates the final equations of the model.
 * These equations are calculated after all the calculations
 * are performed
 */
void %SUBMODEL_NAME%::CalculateFinal (void)
{
  %FINAL_EQUATIONS%
}

/* This function uses tinyxml to parse model configuration xml file.
 * Then paramters are selected and added as a property to the orocos
 * component. These properties can be updated at runtime
 */
void %SUBMODEL_NAME%::initParameters()
{
	// 1) Load the 20Sim parameter config
	TiXmlDocument doc(XXsim_config_xml);
	if(! doc.LoadFile() )
	{
		log(Error) << "File not found: " << XXsim_config_xml << endlog();
		return;
	}

	TiXmlHandle hdoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	int parIndex;
	const char * lbl;
	TiXmlNode* tNode(NULL);

	hRoot=TiXmlHandle(hdoc.FirstChildElement().Element());
	pElem = hRoot.FirstChild("modelVariables").FirstChild().Element();

	if(pElem)
	{
		while(pElem=pElem->NextSiblingElement())
		{
			log(Info) << "Here!" << endlog();

			lbl = (tNode = pElem->FirstChild("kind")) == NULL ? NULL : tNode->ToElement()->GetText();

			if(boost::equals(lbl, "parameter"))
			{
				log(Info) << "Here2!" << endlog();
				const char * name = (tNode = pElem->FirstChild("name")) == NULL ? NULL : tNode->ToElement()->GetText();
				const char * disc = (tNode = pElem->FirstChild("description")) == NULL ? "" : tNode->ToElement()->GetText();
				const char * index = (tNode = pElem->FirstChild("storage")) == NULL ? NULL : tNode->FirstChild("index")->ToElement()->GetText();
				const char * type = (tNode = pElem->FirstChild("type")) == NULL ? NULL : tNode->ToElement()->GetText();

				if(name == NULL || index == NULL || type == NULL)
				{
					log(Error) << "XML file: " << XXsim_config_xml << " incorrect." << endlog();
					continue;
				}

				parIndex = atoi(index);

				log(Info) << "Name: " << name << " Disc: " << disc << " Index: " << parIndex << " Type: " << type << endlog();

				this->addProperty(name,%VARPREFIX%%XX_PARAMETER_ARRAY_NAME%[parIndex]).doc(disc);
			}
		}
	}

	// 2) Update the parameters with the 'Orocos' parameter config.
	// Note: We need to load the 20Sim parameters first, otherwise the parameter location is not know.
	if(! this->getProvider<Marshalling>("marshalling")->loadProperties( Orocos_config_xml ))
	{
		log(Info) << "Did not find: " << Orocos_config_xml << ", therefore using original 20Sim parameters." << endlog();
	}
}

bool %SUBMODEL_NAME%::setPeriod(RTT::Seconds s)
{
	if(TaskContext::setPeriod(s))
	{
		%VARPREFIX%step_size = s;
		return true;
	}
	else
	{
		return false;
	}
}
