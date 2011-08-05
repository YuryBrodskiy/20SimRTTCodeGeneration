/**********************************************************
 * This file is generated by 20-sim ANSI-C Code Generator
 *
 *  file:  %FILE_NAME%
 *  model: %MODEL_NAME%
 *  expmt: %EXPERIMENT_NAME%
 *  date:  %GENERATION_DATE%
 *  time:  %GENERATION_TIME%
 *  user:  %USER_NAME%
 *  from:  %COMPANY_NAME%
 *  build: %GENERATION_BUILD%
 **********************************************************/

/* This file defines the functions necessary for motion profiles
   functions
*/
%IF%%NUMBEROF_DLL_MotionProfiles%

#ifndef __MOTION_PROFILES_H
#define __MOTION_PROFILES_H

void MotionProfiles_Initialize ();
void MotionProfiles_Terminate ();



XXInteger MotionProfiles_ProfileFlat(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfileRamp(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfileSine(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfileCubic(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfilePartialCubic(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfileTrapezoidal(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfilePartialTrapezoidal(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfileModifiedTrapezoidal(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfileCycloidal(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfileStandardModifiedSine(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfileModifiedSineWithConstantVelocity(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfileGeneralModifiedSineWithConstantVelocity(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfilePolynomial345(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfilePolynomial13579(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfileCrankRod(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);
XXInteger MotionProfiles_ProfileGenevaMechanism(XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major);

#endif  /* __MOTION_PROFILES_H */

%ENDIF%