#ifndef COMMANDS_H_
#define COMMANDS_H_

typedef enum addressing_types
{
	/** All addressings are valid (0) */
	ALL_BUT_TWO,
	/** No parameter (1) */
	NO_ADDRESSINGS_TYPES,
	/** All addressings except immediate (2) */
	ONE_AND_THREE,
	/** Only immediate and direct addressings are valid */
	ONE_AND_TWO,
	/** Only direct addressing is valid */
	ONLY_ONE,
	/** Only direct and jumping addressings are valid */
	/** Failed/Not detected addressing */
	NONE_ADDR = -1
} addressing_type;

typedef enum params_nums{
	ONE_PARAM,
	TWO_PARAMS,
	LABEL_NON_OR_TWO_PARAMS,
	NO_PARAMS
} param_num;

typedef struct {
	char *cmd;
	char params_num;
	addressing_type source;
	addressing_type dest;
} command_table;

/* The command table, containing all supported commands and their properties. */
extern const command_table table[];

typedef enum registers {
	R0 = 0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	NON_REG = -1
} reg;

#endif /* COMMANDS_H_ */
