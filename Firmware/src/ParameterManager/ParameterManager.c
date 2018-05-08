#include "ParameterManager/ParameterManager.h"
#include "Mavlink/common/mavlink.h"
#include "string.h"

Parameter_t param_list[9];

int numberOfParameters(void){
	return 9;
}

void parameters_init(void){
	strcpy(param_list[0].id, "ROLL_P");
	param_list[0].type = MAV_PARAM_TYPE_REAL32;
	param_list[0].value.floating = 1.0;

	strcpy(param_list[1].id, "ROLL_I");
	param_list[1].type = MAV_PARAM_TYPE_REAL32;
	param_list[1].value.floating = 1.0;

	strcpy(param_list[2].id, "ROLL_D");
	param_list[2].type = MAV_PARAM_TYPE_REAL32;
	param_list[2].value.floating = 1.0;

	strcpy(param_list[3].id, "PITCH_P");
	param_list[3].type = MAV_PARAM_TYPE_REAL32;
	param_list[3].value.floating = 1.0;

	strcpy(param_list[4].id, "PITCH_I");
	param_list[4].type = MAV_PARAM_TYPE_REAL32;
	param_list[4].value.floating = 1.0;

	strcpy(param_list[5].id, "PITCH_D");
	param_list[5].type = MAV_PARAM_TYPE_REAL32;
	param_list[5].value.floating = 1.0;

	strcpy(param_list[6].id, "YAW_P");
	param_list[6].type = MAV_PARAM_TYPE_REAL32;
	param_list[6].value.floating = 1.0;

	strcpy(param_list[7].id, "YAW_I");
	param_list[7].type = MAV_PARAM_TYPE_REAL32;
	param_list[7].value.floating = 1.0;

	strcpy(param_list[8].id, "YAW_D");
	param_list[8].type = MAV_PARAM_TYPE_REAL32;
	param_list[8].value.floating = 1.0;
}

