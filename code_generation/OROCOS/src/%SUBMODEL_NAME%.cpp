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
#include <ocl/Component.hpp>
#include <rtt/types/SequenceTypeInfo.hpp>
#include <rtt/types/carray.hpp>

using namespace Orocos;
using namespace RTT;
using namespace std;

namespace %MODEL_NAME%
{
	const string XXsim_config_xml = "config/%SUBMODEL_NAME%_base_config.xml";

	%SUBMODEL_NAME%::%SUBMODEL_NAME%(string name): TaskContext(name, PreOperational)
	{
		using namespace boost;

		RTT::types::Types()->addType(new
				RTT::types::CArrayTypeInfo<RTT::types::carray<double> >("double[]"));

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

//		state = initialrun;

		//------------------orocos------------------------------
		this->addProperty("integration_step_size", %VARPREFIX%step_size ).doc("Integration step size.");

		setupParametersAndStates();

	}

	%SUBMODEL_NAME%::~%SUBMODEL_NAME%(void)
	{
		cleanupPropertyBags(this->properties());

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

		/*set intial values*/
		%INITIALIZE_INITIAL_VALUES%
		/*set favorite if any*/
			%INITIALIZE_FAVORITE_PARS%
		%INITIALIZE_FAVORITE_VARS%

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

		for (vector<Adapter20Sim<RTT::InputPort<flat_matrix_t> > >::iterator it = inputPorts.begin(); it != inputPorts.end(); ++it) {
			flat_matrix_t temp ;
			if(! it->getPort()->connected() ){
				log(Warning) << "InputPort (" << it->getFullName() << ") not connected." << endlog();
//				return false;
			}
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

	/* this PRIVATE function sets the input variables from the input vector */
	//@todo Improve for multiple component inputs to have a synchronized execution.
	void %SUBMODEL_NAME%::CopyInputsToVariables ()
	{
		/* OROCOS Entry to copy port to input array */
		for (vector<Adapter20Sim<RTT::InputPort<flat_matrix_t> > >::iterator it =
				inputPorts.begin(); it != inputPorts.end(); ++it) {
			flat_matrix_t temp ;
			if(it->getPort()->read(temp)!=RTT::NoData){
				log(Debug)<<"CopyInputsToVariables"<<endlog();
				it->setValue(temp);
			}
		}

	}

	/* this PRIVATE function uses the output variables to fill the output vector */
	void %SUBMODEL_NAME%::CopyVariablesToOutputs ()
	{

		/* OROCOS Entry to copy output to port */
		for (vector<Adapter20Sim<RTT::OutputPort<flat_matrix_t> > >::iterator it =
				outputPorts.begin(); it != outputPorts.end(); ++it) {
		//	log(Debug)<<"writing out ="<<it->getValue().address()[1]<<endlog();

			//temp=it->getValue();

			//log(Debug)<<temp.address()[0]<<temp.address()[1]<<temp.address()[2]<<endlog();
			//log(Debug)<<it->getPort().getName()<<endlog();
			flat_matrix_t temp;
			temp=it->getValue();
			it->getPort()->write(temp);
		}
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

	void %SUBMODEL_NAME%::setupParametersAndStates()
	{
		using namespace boost;

			TiXmlDocument doc(XXsim_config_xml);
			if (!doc.LoadFile()) {
				log(Error) << "File not found: " << XXsim_config_xml << endlog();
				return;
			}

			TiXmlHandle hdoc(&doc);
			TiXmlElement* pElem;
			TiXmlHandle hRoot(0);
			TiXmlNode* tNode(NULL);

			hRoot = TiXmlHandle(hdoc.FirstChildElement().Element());
			pElem = hRoot.FirstChild("modelVariables").FirstChild().Element();

			if (pElem) {

				do {
					log(Debug) << "Begin of processing the tag" << endlog();
					log(Debug) << "Read all relevant fields" << endlog();
					const char * kind =
							(tNode = pElem->FirstChild("kind")) == NULL ?
									NULL : tNode->ToElement()->GetText();
					const char * name =
							(tNode = pElem->FirstChild("name")) == NULL ?
									NULL : tNode->ToElement()->GetText();
					const char * description =
							(tNode = pElem->FirstChild("description")) == NULL ?
									" " : tNode->ToElement()->GetText();
					description =
							description == NULL || description == "" ?
									" " : description;
					const char * container =
							(tNode = pElem->FirstChild("storage")) == NULL ?
									NULL :
									tNode->FirstChild("name")->ToElement()->GetText();
					const char * strIndex =
							(tNode = pElem->FirstChild("storage")) == NULL ?
									NULL :
									tNode->FirstChild("index")->ToElement()->GetText();
					const char * strRows =
							(tNode = pElem->FirstChild("size")) == NULL ?
									NULL :
									tNode->FirstChild("rows")->ToElement()->GetText();
					const char * strColumns =
							(tNode = pElem->FirstChild("size")) == NULL ?
									NULL :
									tNode->FirstChild("columns")->ToElement()->GetText();
					const char * type =
							(tNode = pElem->FirstChild("type")) == NULL ?
									NULL : tNode->ToElement()->GetText();

					log(Debug) << " strings to numbers" << endlog();
					int index, rows, columns;
					if (strIndex == NULL)
					{
						log(Debug)
								<< "Processing : "
								<< XXsim_config_xml
								<< " Link to 20 sim variables incorrectly defined: Index =null \n"
								<< " The node will be not processed" << endlog();
						continue;
					}
					index = atoi(strIndex);
					rows = strRows == NULL ? 1 : atoi(strRows);
					columns = strColumns == NULL ? 1 : atoi(strColumns);

					log(Debug) << "Name: " << name << " Disc: " << description
							<< " Index: " << index << " Kind: " << kind << " Type: "
							<< type << endlog();

					common20sim::XVMatrix* tempXVMatrix;
					log(Debug) << " Selecting source of data for this node" << endlog();
					if (boost::equals(container, "V")) {
						tempXVMatrix=new common20sim::XVMatrix(V+index, rows, columns);
					} else if (boost::equals(container, "C")) {
						tempXVMatrix=new common20sim::XVMatrix(C+index, rows, columns);
					} else if (boost::equals(container, "P")) {
						tempXVMatrix=new common20sim::XVMatrix(P+index, rows, columns);
					} else if (boost::equals(container, "I")) {
						tempXVMatrix=new common20sim::XVMatrix(I+index, rows, columns);
					} else if (boost::equals(container, "s")) {
						tempXVMatrix=new common20sim::XVMatrix(s+index, rows, columns);
					} else if (boost::equals(container, "R")) {
						tempXVMatrix=new common20sim::XVMatrix(R+index, rows, columns);
					} else if (boost::equals(container, "M")) {
						tempXVMatrix=new common20sim::XVMatrix(M[index]);
					} else if (boost::equals(container, "U")) {
						tempXVMatrix=new common20sim::XVMatrix(U+index, rows, columns);
					} else if (boost::equals(container, "workarray")) {
						tempXVMatrix=new common20sim::XVMatrix(workarray+index, rows, columns);
					} else {
						log(Debug) << "Processing : " << XXsim_config_xml
								<< " Link to 20 sim variables incorrectly defined: "
								<< container << " The node will be not processed"
								<< endlog();
						continue;
					}


					log(Debug)<<"select the action based o kind of parameter"<<endlog();

					if (boost::equals(kind, "parameter")) {
						// create port decorator
						//RTT::Property<RTT::types::carray<double> >* p_rttPort; //TODO: Fix me!

						Adapter20Sim<RTT::Property<RTT::types::carray<double> > > p_20simport(
								string(name), string(description), tempXVMatrix, NULL);

						// check if the property should be hierarchical folded
						RTT::PropertyBag* p_bag(NULL);
						p_bag = createHierarchicalPropertyBags(name);

						p_bag->addProperty(p_20simport.getShortName(), p_20simport.getLink()->getCArray());
						// save the link for updates
						propertyPorts.push_back(p_20simport);

					} else if (boost::equals(kind, "state")) {


					} else if (boost::equals(kind, "variable")) {
						//not implemented yet :)
						log(Debug)<<"Variables are not exhibited to out side of the component"<<endlog();
						// Recommend update is to show Interesting variables
					} else if (boost::equals(kind, "input")) {
						// create port decorator
						RTT::InputPort<flat_matrix_t> * p_rttPort = new RTT::InputPort<flat_matrix_t>;

						Adapter20Sim<RTT::InputPort<flat_matrix_t> > p_20simport(
								string(name), string(description), tempXVMatrix, p_rttPort);

						// create a port
						this->addPort(p_20simport.getFullName(), *p_rttPort).doc(
								p_20simport.getDescription());
						//save link for updates

						inputPorts.push_back(p_20simport);

					} else if (boost::equals(kind, "output")) {
						// create port decorator
						RTT::OutputPort<flat_matrix_t> * p_rttPort = new RTT::OutputPort<flat_matrix_t>;

						Adapter20Sim<RTT::OutputPort<flat_matrix_t> > p_20simport(
								string(name), string(description), tempXVMatrix, p_rttPort);

						// create a port
						this->addPort(p_20simport.getFullName(), *p_rttPort).doc(
								p_20simport.getDescription());

						outputPorts.push_back(p_20simport);

					} else {
						log(Debug) << XXsim_config_xml << " token kind not recognized("
								<< kind << ")" << endlog();
						continue;
					}
					log(Debug) << "End of processing the tag" << endlog();
				} while ( (pElem = pElem->NextSiblingElement()) != NULL);
				log(Debug) << "End of processing the xml" << endlog();
			}
	}


	RTT::PropertyBag* %SUBMODEL_NAME%::createHierarchicalPropertyBags(const char * name) {
		using namespace boost;
		RTT::PropertyBag* p_bag(NULL);
		string cleaned_name;

		typedef split_iterator<const char*> ssi;
		ssi end;
		for (ssi it = make_split_iterator(name, first_finder("\\", is_iequal()));
				it != end;) {
			cleaned_name = replaceIllegalCharacter(
					copy_range<std::string>(*it));

			// Are we at the end already?
			if (++it == end) {
				break; // last name of the string is the name of the property.
			}

			if (p_bag == NULL)
			{
				RTT::Property<PropertyBag>* ppb = dynamic_cast<RTT::Property<
						PropertyBag>*>(this->getProperty(cleaned_name));
				if(ppb == NULL)
				{
					p_bag = new RTT::PropertyBag;
					this->addProperty(cleaned_name, *p_bag).doc("Submodel parameters");
				}
				else
				{
					p_bag = &(ppb->value());
				}
			}
				else
				{
					RTT::Property<PropertyBag>* ppb = dynamic_cast<RTT::Property<PropertyBag>*>(p_bag->getProperty(cleaned_name));
					RTT::PropertyBag* ptmp(NULL);
					if(ppb == NULL)
					{
						ptmp = new RTT::PropertyBag;
						p_bag->addProperty(cleaned_name, *ptmp).doc("Submodel parameters");
					}
					else
					{
						ptmp = &(ppb->value());
					}
					p_bag = ptmp;
				}
			}
		if (p_bag != NULL)
		{
			return p_bag;
		} else {
			return this->properties();

		}
	}
	void %SUBMODEL_NAME%::cleanupPropertyBags(RTT::PropertyBag* p)
	{
		RTT::PropertyBag::iterator it(p->begin());
		RTT::PropertyBag::iterator end(p->end());

		RTT::Property<PropertyBag>* ppb(NULL);

		for(;it != end; ++it)
		{
			ppb = dynamic_cast<RTT::Property<PropertyBag>*>(*it);

			if(ppb != NULL)
			{
				PropertyBag* ptmp = &(ppb->value());
				cleanupPropertyBags(ptmp);
				delete ptmp;
			}
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
}

/* Macro to generate component library
 * Can be modified if the component is part of a big project with other components 
 */
ORO_CREATE_COMPONENT(%MODEL_NAME%::%SUBMODEL_NAME%)

