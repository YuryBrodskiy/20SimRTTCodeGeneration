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
#include <stdexcept>

/* 20-sim include files */
#include "%SUBMODEL_NAME%.h"

/* Orocos include */
#include <boost/algorithm/string.hpp>
#include <ocl/Component.hpp>
#include <rtt/types/SequenceTypeInfo.hpp>
#include <rtt/types/carray.hpp>
#include <ros/package.h>
//#include <rtt/Logger.hpp>

using namespace Orocos;
using namespace RTT;
using namespace std;

namespace %MODEL_NAME%
{


	%SUBMODEL_NAME%::%SUBMODEL_NAME%(string name): TaskContext(name, PreOperational), m_model_properties(this)
	{
		using namespace boost;

		//RTT::Logger* l = RTT::Logger::Instance();
		//l->setLogLevel(RTT::Logger::Debug);

		setupComputation();

		xml_config_file = ros::package::getPath("%SUBMODEL_NAME%");
		xml_config_file = xml_config_file + "/config/%SUBMODEL_NAME%_base_config.xml";

    try
    {
      m_model_properties.load(xml_config_file);
      log(Info) << "Loaded XXModelProperties" << endlog();
    }catch(std::exception& e)
    {
      log(Error) << e.what() << endlog();
			this->error();
      this->exception();
      throw(e);
    }

		this->addProperty("integration_step_size", %VARPREFIX%step_size ).doc("Integration step size.");
		this->addProperty("configuration_file",xml_config_file).doc("Path to configuation xml, relative to run directory");

		setupComponentInterface();

		initializeComputation();
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

		return true;
	}

	/* the initialization function for submodel */
	bool %SUBMODEL_NAME%::startHook()
	{
		if(! TaskContext::startHook())
		{
			return false;
		}

		for(unsigned int i = 0; i < inputPorts.size(); ++i)
		{
		  if(!inputPorts[i].getPort()->connected() )
      {
        log(Warning) << "InputPort (" << inputPorts[i].getFullName() << ") not connected." << endlog();
      }
		}

    for(unsigned int i = 0; i < outputPorts.size(); ++i)
    {
      if(!outputPorts[i].getPort()->connected() )
      {
        log(Warning) << "OutputPort (" << outputPorts[i].getFullName() << ") not connected." << endlog();
      }
      outputPorts[i].getPort()->setDataSample( outputPorts[i].getPortData() );
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
		CopyInputsToVariables (); //get input from port
		CalculateInput ();
		myintegmethod.Step();
		CalculateOutput ();
		CopyVariablesToOutputs (); //send output to port
	}

	/* the termination function for submodel */
	void %SUBMODEL_NAME%::stopHook()
	{
		TaskContext::stopHook();

		/* copy the inputs */
		CopyInputsToVariables (); //get inputs from port

		/* calculate the final model equations */
		CalculateFinal ();

		/* set the outputs */
		CopyVariablesToOutputs (); //send output to port
	}

	/* this PRIVATE function sets the input variables from the input vector */
	//@todo Improve for multiple component inputs to have a synchronized execution.
	void %SUBMODEL_NAME%::CopyInputsToVariables ()
	{
		/* OROCOS Entry to copy port to input array */
    for (vector<Adapter20Sim<RTT::InputPort<flat_matrix_t> > >::iterator it =
				inputPorts.begin(); it != inputPorts.end(); ++it)
	  {
			if(it->getPort()->read(it->getPortData())!=RTT::NoData)
			{
				it->copyPortToVariable();
			}

	  }
	}

	/* this PRIVATE function uses the output variables to fill the output vector */
	void %SUBMODEL_NAME%::CopyVariablesToOutputs ()
	{
		/* OROCOS Entry to copy output to port */
	  for (vector<Adapter20Sim<RTT::OutputPort<flat_matrix_t> > >::iterator it =
				outputPorts.begin(); it != outputPorts.end(); ++it)
	  {
			it->copyVariableToPort();
			it->getPort()->write(it->getPortData());
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

	void %SUBMODEL_NAME%::setupComputation()
  {
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
    %VARPREFIX%%XX_CONSTANT_ARRAY_NAME% = new XXDouble[%NUMBER_CONSTANTS% + 1]; /* constants */
    %VARPREFIX%%XX_PARAMETER_ARRAY_NAME% = new XXDouble[%NUMBER_PARAMETERS% + 1]; /* parameters, currently only one type of parameter exists: double */
    %VARPREFIX%%XX_INITIAL_VALUE_ARRAY_NAME% = new XXDouble[%NUMBER_INITIAL_VALUES% + 1]; /* initial values */
    %VARPREFIX%%XX_VARIABLE_ARRAY_NAME% = new XXDouble[%NUMBER_VARIABLES% + 1]; /* variables */

    %VARPREFIX%%XX_STATE_ARRAY_NAME% = new XXDouble[%NUMBER_STATES% + 1]; /* states */
    %VARPREFIX%%XX_RATE_ARRAY_NAME% = new XXDouble[%NUMBER_STATES% + 1]; /* rates (or new states) */
    %VARPREFIX%%XX_MATRIX_ARRAY_NAME% = new XXMatrix[%NUMBER_MATRICES% + 1]; /* matrices */
    %VARPREFIX%%XX_UNNAMED_ARRAY_NAME% = new XXDouble[%NUMBER_UNNAMED% + 1]; /* unnamed */
    %VARPREFIX%workarray = new XXDouble[%WORK_ARRAY_SIZE% + 1];
  }

  bool %SUBMODEL_NAME%::initializeComputation()
  {
	 myintegmethod.Initialize(this);

	 /* initialization phase (allocating memory) */
	 %VARPREFIX%initialize = true;
    //CONSTANTS
    %INITIALIZE_CONSTANTS%

    //PARAMETERS
    %INITIALIZE_PARAMETERS%

    //INITIAL VALUES
    %INITIALIZE_INITIAL_VALUES%

    //MATRICES
    %INITIALIZE_MATRICES%

    //INITIALIZE_DEPSTATES
    //INITIALIZE_ALGLOOPS
    //INITIALIZE_CONSTRAINTS%

    //INPUTS
    %INITIALIZE_INPUTS%

    //OUTPUTS
    %INITIALIZE_OUTPUTS%

    //INITIALIZE_FAVORITE_PARS
    //INITIALIZE_FAVORITE_VARS

    //INITIALIZE_CONSTANTS%
    /* set the states */
    //INITIALIZE_STATES%

    /* set the matrices */
    //INITIALIZE_MATRICES%

    // overload INITIALIZE_* with values from xml
    std::vector<XVMatrix> pps = m_model_properties.getPortsAndProperties();

    for(unsigned int i = 0; i < pps.size(); ++i)
    {
      if( static_cast<unsigned int>(pps[i].storage.rows * pps[i].storage.columns) != pps[i].values.size())
        throw std::out_of_range("" + pps[i].name);

      // Copy to XXData -> double*
      memcpy(pps[i].storage.mat, pps[i].values.data(), pps[i].values.size()*sizeof(double));
    }

    //STATES - do NOT move this line up!
    %INITIALIZE_STATES%

    /* end of initialization phase */
    %VARPREFIX%initialize = false;
    return %VARPREFIX%initialize;
  }

  void %SUBMODEL_NAME%::setupComponentInterface()
  {
    std::vector<XVMatrix> pps = m_model_properties.getPortsAndProperties();
    log(Info) << "Number of ports and properties in XML: " << pps.size() << endlog();

    for(unsigned int i = 0; i < pps.size(); ++i)
    {
      log(Debug) << "Name: " << pps[i].name << " CEType: " << pps[i].type << " Storage: " << pps[i].storage.mat << " Rows: " << pps[i].storage.rows << " Columns: " << pps[i].storage.columns << endlog();

      switch(pps[i].type)
      {
        case(INPUT):
        {
          RTT::InputPort<flat_matrix_t> * rtt = new RTT::InputPort<flat_matrix_t>;
          Adapter20Sim<RTT::InputPort<flat_matrix_t> > xxsim(pps[i], rtt);
          this->addPort(xxsim.getFullName(), *rtt).doc(xxsim.getDescription());
          inputPorts.push_back(xxsim);
          break;
        }
        case(OUTPUT):
        {
          RTT::OutputPort<flat_matrix_t> * rtt = new RTT::OutputPort<flat_matrix_t>;
          Adapter20Sim<RTT::OutputPort<flat_matrix_t> > xxsim(pps[i], rtt);
          this->addPort(xxsim.getFullName(), *rtt).doc(xxsim.getDescription());
          outputPorts.push_back(xxsim);
          break;
        }
        case(PARAMETER):
        {
          RTT::PropertyBag* p_bag = createPropertyBags(pps[i].name, NULL); // Create the sub-model hierarchy
          Property<RTT::types::carray<double> >* prop = new Property<RTT::types::carray<double> >(makeShortName(pps[i].name), pps[i].description,
              RTT::types::carray<double>(pps[i].storage.mat, static_cast<std::size_t>(pps[i].storage.rows * pps[i].storage.columns)));
          Adapter20Sim<RTT::Property<RTT::types::carray<double> > > xxsim(pps[i], prop);
          p_bag->addProperty(*prop);
          propertyPorts.push_back(xxsim);
          break;
        }
        case(INTERNAL):
            break;
        default:
        {
          log(Info) << "Unknown type modelVariable (" << pps[i].name << ")." << endlog();
          break;
        }
      }
    }
	log(Info) << "Total input ports: " << inputPorts.size() << endlog();
	log(Info) << "Total output ports: " << outputPorts.size() << endlog();
	log(Info) << "Total properties: " << propertyPorts.size() << endlog();
  }

  RTT::PropertyBag* %SUBMODEL_NAME%::createPropertyBags(std::string name, RTT::PropertyBag* head)
  {
    size_t found;
    found=name.find_first_of("\\");
    RTT::PropertyBag* p_bag(NULL);

    if(found != string::npos)
    {
      std::string sub_name(name, 0, found);
      sub_name = replaceIllegalCharacter(sub_name);

      if(head == NULL)
      {
        RTT::Property<PropertyBag>* ppb = dynamic_cast<RTT::Property<PropertyBag>*>(this->getProperty(sub_name));
        if(ppb == NULL)
        {
          p_bag = new RTT::PropertyBag;
          this->addProperty(sub_name, *p_bag).doc("Submodel parameters");
        }
        else
        {
          p_bag = &(ppb->value());
        }
      }
      else
      {
        RTT::Property<PropertyBag>* ppb = dynamic_cast<RTT::Property<PropertyBag>*>(head->getProperty(sub_name));
        RTT::PropertyBag* ptmp(NULL);
        if(ppb == NULL)
        {
          ptmp = new RTT::PropertyBag;
          head->addProperty(sub_name, *ptmp).doc("Submodel parameters");
        }
        else
        {
          ptmp = &(ppb->value());
        }
        p_bag = ptmp;
      }
      assert((name[found+1]) != NULL); // \ shouldn't be last character.
      return createPropertyBags(name.substr(found+1), p_bag);
    }
    else
    {
      if (head != NULL)
      {
        return head;
      }
      else
      {
        return this->properties();
      }
    }
  }

  void %SUBMODEL_NAME%::cleanupPropertyBags(RTT::PropertyBag* p)
  {
    RTT::Property<PropertyBag>* ppb(NULL);

    for(RTT::PropertyBag::iterator it = p->begin(); it != p->end(); ++it)
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
}

/* Macro to generate component library
 * Can be modified if the component is part of a big project with other components 
 */
ORO_CREATE_COMPONENT(%MODEL_NAME%::%SUBMODEL_NAME%)

