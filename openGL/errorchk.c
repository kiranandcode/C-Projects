#include "errorchk.h"
#include "logger.h"

void errorchk_program(GLuint prgram) {
	int params = -1;
	glGetProgramiv(prgram, GL_LINK_STATUS, &params);
	if(GL_TRUE != params) {
		logger_err("Could not log shader program index %u\n", prgram);
		int max_len = 2048;
		int act_len = 0;
		char log[max_len];
		glGetProgramInfoLog(prgram, max_len, &act_len, log);
		logger_err("shader program error msg[%u]: %s\n",
				prgram, log);
	}

}


int errorchk_program_validity(GLuint program) {
	glValidateProgram(program);
	int params = -1;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &params);
	logger_log("program %i GL_VALIDATE_STATUS = %i\n", program, params);
	if(GL_TRUE != params) {
		errorchk_program(program);
		return 0;
	}
	return 1;
}
