/* Diffusivity for mean age of air is solved using a user-defined scalar. 
This calculation can be performed after the normal problem has already been solved ... just turn off
all of the previous equations being solved for and activate solution of the user-defined scalar-0
equation. Make sure to select all fluid regions for the following.

- mean_age_diff as the uds diffusivity in the materials panel

- mass flow rate as the uds flux function

- mean_age_source as the uds volumetric source term in


Use a journal like this to do that in F63 :

def bc fluid fluid-1 n y 0 0 0 0 0 0 0 1 n y "mean_age_source" n y 0 0 0 0 0 0 n n n
def bc fluid fluid-2 n y 0 0 0 0 0 0 0 1 n y "mean_age_source" n y 0 0 0 0 0 0 n n n
def bc fluid fluid-3 n y 0 0 0 0 0 0 0 1 n y "mean_age_source" n y 0 0 0 0 0 0 n n n

etc.

- set uds-0 scalar values = 0.0 at all inlets and outlets

mean age of air calculations do not require any energy, radiation, or species transport
calculations to have been performed

*/

/************************************************** *****/

#include "udf.h"

#include "prop.h"

DEFINE_DIFFUSIVITY(mean_age_diff, c, t, i)

{
	return C_R(c,t)*2.88e-05 + C_MU_EFF(c,t)/0.7;
}

DEFINE_SOURCE(mean_age_source, c, t, dS, eqn)

{
	dS[eqn]=0;

	return C_R(c,t);
}
