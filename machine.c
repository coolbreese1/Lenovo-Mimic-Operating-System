#include <stdio.h>
#include "machine.h"

/*
 * Colin Farrell
 * UID: 117139884
 * Directory ID: cfarrel3 
 * Section Number: 0212
 * 
 * This file contains several functions that mimick the function of a Nelovo 
 * system. The first method print_instruction prints the machine word and its
 * components based off the opcode. Disassemble prints out the instructions  
 * and memory address if they are valid from the machine words in Nelovo memory.  
 * Is_valid and checks to see if the instructions given in the parameter are 
 * valid within the constraints of the Nelovo CPU rules. Check_branches checks 
 * a specific opcode, TEST, to see if the parameters are valid for it. 
 * Load_program takes an array of machine words and copies it to the Nelovo
 * memory. Get_register and get_memory will get the appropriate register and 
 * memory from the Nelovo machine. Run_program simulates the Nelovo machine
 * by carrying out the functions of each opcode that is in the given 
 * instructions. 
 */


/*
 * This function takes the instructions given in the parameter and prints it out
 * if the instructions are valid. It checks this by calling the is_valid 
 * function. Five variables are declared and given values based off the 
 * instructions provided. The opcode of the instructions is compared to a series
 * of cases and whichever one matches, that is what will print. This function
 * will return one if the instructions are valid and zero if invalid. 
 *
 * @param instr
 * @return unsigned short
 */
unsigned short print_instruction(Mach_word instr) {
    unsigned int opcode = 0, reg1 = 0, reg2 = 0, reg3 = 0, address = 0;
    /* The instructions are shifted so only the first 6 bits are available */
    opcode = instr >> 26;
  
    /*
     * The instruction is masked using a decimal number which grabs the bits 
     * associated with each of the different registers in the instruction and 
     * then that masked number is bit shifted so those bits take the leftmost
     * bits so they can be evaluated. 
     */
    reg1 = (instr & 62914560) >> 22;
    reg2 = (instr & 3932160) >> 18;
    reg3 = (instr & 245768) >> 14;
  
    /* 
     * The instructions are masked so that only the associated address bits are
     * copied to the variable address. 
     */
    address = (instr & 16383);
  
    if (is_valid(instr) == 1) {
        switch(opcode) {
        case HALT:
            printf("halt");
            break;
        case ADD:
            printf("add R%d R%d R%d", reg1, reg2, reg3);
            break;
        case SUB:
            printf("sub R%d R%d R%d", reg1, reg2, reg3);
            break;
        case MUL: 
            printf("mul R%d R%d R%d", reg1, reg2, reg3);
            break;
        case DIV:
            printf("div R%d R%d R%d", reg1, reg2, reg3);
            break;
        case REM:
            printf("rem R%d R%d R%d", reg1, reg2, reg3);
            break;
        case NEG:
            printf("neg R%d R%d",  reg1, reg2);
            break;
        case AND:
            printf("and R%d R%d R%d", reg1, reg2, reg3);
            break;
        case OR:
            printf("or R%d R%d R%d",  reg1, reg2, reg3);
            break;
        case NOT: 
            printf("not R%d R%d", reg1, reg2);
            break;
        case NEQ:
            printf("neq R%d R%d R%d", reg1, reg2, reg3);
            break;
        case LT:
            printf("lt R%d R%d R%d", reg1, reg2, reg3);
            break;
        case GT:
            printf("gt R%d R%d R%d", reg1, reg2, reg3);
            break;
        case LE:
            printf("le R%d R%d R%d", reg1, reg2, reg3);
            break;
        case GE: 
            printf("ge R%d R%d R%d", reg1, reg2, reg3);
            break;
        case TEST:
            printf("test R%d %05d", reg1, address);
            break;
        case MOVE:
            printf("move R%d R%d", reg1, reg2);
            break;
        case LI:
            printf("li R%d %d", reg1, address);
            break;
        case LW: 
            printf("lw R%d %05d", reg1, address);
            break;
        case SW: 
            printf("sw R%d %05d", reg1, address);
            break;
        case READ: 
            printf("read R%d", reg1);
            break;
        case PRT:
            printf("prt R%d",  reg1);
            break;
        }
    }
    else 
        return 0;

    return 1;
}
/*
 * This function takes the parameter program[] and prints all of the 
 * instructions that are in the array. If the max_addr is greater than 16384 
 * the program will not print anything out because 16384 is the maximum amount
 * of bytes in a Nelovo system. The while loop is set so that the program will 
 * stop printing out instructions before the count goes past max_addr. If one of
 * the instructions is invalid zero will be returned and the functio will stop
 * printing out the instructions. If count is less than max_addr and the 
 * instructions are valid then the instructions will print like in the function
 * print_instructions except with memory address added in front for all 
 * instructions in the array parameter program. 
 *
 * @param machine, max_addr 
 * @param unsigned short 
 */
unsigned short disassemble(Nelovo machine, unsigned short max_addr) {
    int i = 0, count = 0, result = 0;

    /*first checks that the max address provided is not larger than the address
    of the last word in nelovo. Then iterates through each instruction in the
    program and prints of the memory address in hexdecimal followed by the
    instruction in the print_instruction function*/
    if (max_addr < NUM_BYTES)
    {
        while (count <= max_addr)
        {
            if (!is_valid(machine.mem_location[i * BYTES_PER_WORD]))
                return 0;

            printf("%04x: ", count);
            print_instruction(machine.mem_location[i * BYTES_PER_WORD]);
            printf("\n");
            i++;
            count += BYTES_PER_WORD;
            result = 1;
        }
    }
    else
        result = 0;
    return result;
}

/*
 * This function checks to see if all the different parts of the instructions
 * given in the parameter word are valid along the rules of the Nelovo system. 
 * It instatntiates five unsigned shorts to represent the five different parts
 * of the instructions. After the variables are given values their are a series 
 * of if statements to check to see if the opcode is valid. Some opcodes have
 * different rules that it has to follow regarding registers and addresses and
 * therefore there are different if statements for those rules. After that
 * it checks to see if the register/s are valid and then the addresses for the
 * opcodes that need valid addresses. If the instructions are valid the function
 * retuns a one otherwise the function returns a zero. 
 *
 * Opcode numbers range from 0 to 22. Registers can range from 0 to 13. 
 * Addresses have to be divisble by 4 because all addresses are stored with 
 * mulitples of 4 bytes in memory. 
 *
 * @param word
 * @return unsigned short 
 */
unsigned short is_valid(Mach_word word) {
    /* 
     * The instructions are shifted so only the first 6 bits are available for 
     * the opcode variable. 
     */
    unsigned int opcode = word >> 26, reg1 = 0, reg2 = 0, reg3 = 0, address = 0;
    unsigned short result = 0;  
 
    /*
     * The instruction is masked using a decimal number which grabs the bits 
     * associated with each of the different registers in the instruction and 
     * then that masked number is bit shifted so those bits take the leftmost
     * bits so they can be evaluated. 
     */
    reg1 = (word & 62914560) >> 22;
    reg2 = (word & 3932160) >> 18;
    reg3 = (word & 245768) >> 14;

    /* 
     * The instructions are masked so that only the associated address bits are
     * copied to the variable address. 
     */
    address = (word & 16383);

    /* Halt opcode only needs a valid opcode so only the opcode is checked */ 
    if (opcode == HALT) 
        result = 1;

    /* 
     * Opcode is checked to see if it fits the first conditional if statement 
     * and then registers one, two, and three are checked. Addresses do not 
     * matter for these opcodes.    
     */
    if ((opcode >= ADD && opcode <= REM) || opcode == AND ||
                            opcode == OR || (opcode >= EQL && opcode <= GE)) {
        if ((reg1 >= R0 && reg1 <= R12) && (reg2 >= R0 && reg2 <= R12)
	                                            && (reg3 >= R0 && reg3 <= R12))
            result = 1;
    }
    /* 
     * Opcode is checked to see if it fits the first conditional if statement 
     * and then registers one and two are checked. Register three and the 
     * address does not matter for these opcodes.    
     */

    else if (opcode == NEG || opcode == NOT || opcode == MOVE) {
        if ((reg1 >= R0 && reg1 <= R12) && (reg2 >= R0 && reg2 <= R12)) 
            result = 1;
    }

    /* 
     * Opcode is checked to see if it fits the first conditional if statement 
     * and register one and the address is checked. The other registers do not 
     * matter for this opcode. 
     */
    else if (opcode == TEST || opcode == SW) {
        if (reg1 >= R0 && reg1 <= R13) {
            if (address % 4 == 0) 
      	        result = 1;
        }
    }

    /* 
     * Opcode is checked to see if it fits the first conditional if statement 
     * and register one is checked. The other information does not matter for 
     * this opcode. 
     */
    else if (opcode == LI) {
        if (reg1 >= R0 && reg1 <= R12) 
            result = 1;
    }
  
    /* 
     * Opcode is checked to see if it fits the first conditional if statement 
     * and register one and the address is checked. The other registers do 
     * not matter for this opcode. 
     */

    else if (opcode == LW) {
        if (reg1 >= R0 && reg1 <= R12) {
            if (address % 4 == 0) 
	            result = 1;
        }
    }

    /* 
     * Opcode is checked to see if it fits the first conditional if statement 
     * and register one is checked. The other information does not matter for 
     * this opcode. 
     */
    else if (opcode == READ) {
        if (reg1 >= R0 && reg1 <= R12) 
            result = 1;
    }

    /* 
     * Opcode is checked to see if it fits the first conditional if statement 
     * and register one is checked. The other information does not matter for 
     * this opcode. 
     */
    else if (opcode == PRT) {
        if (reg1 >= R0 && reg1 <= R13) 
            result = 1;
    }
  
    return result;
}
/*
 * This funciton checks all mach_words in the machine array of memory locations
 * to see if any if the instructions have TEST opcode and if any have invalid
 * addresses. If the instruction is invalid then the memory address of that 
 * instruction will be returned. Otherwise one will be returned. 
 * 
 * @param machine, max_addr, invalid_instr
 * @return short 
 */
short check_branches(Nelovo *const machine, unsigned short max_addr,
                     unsigned short *const invalid_instr) {
    unsigned int opcode = 0, address = 0, i = 0, count = 0;

    short result = 1;

    if (max_addr >= NUM_BYTES || machine == NULL || invalid_instr == NULL) 
        result = -1;
    else {
        while (count <= max_addr && result == 1){
            opcode = machine -> mem_location[i * BYTES_PER_WORD] >> 26;
            address = machine -> mem_location[i * BYTES_PER_WORD] & 16383;
        /* 
         * If the opcode equals test and either the address is not divisble by 4
         * or the current memory address has hit the max, max_addr, then the 
         * current memory address is saved in result. 
         */
        if((opcode == TEST) && ((address % 4 != 0) || (address > max_addr))) 
      	    result = 0;
            *invalid_instr = count;

         i++;
        count += BYTES_PER_WORD;
        }
    }

    return result;                         
}
/* 
 * This function takes the parameter program[] that is filled with Machine
 * words and copies it into the Nelovo machine, allocating the correct amount 
 * of memory for each word.
 * 
 * @param machine, program[], num_instrs
 * @return num_instrs 
 */
unsigned short load_program(Nelovo *const machine, const Mach_word program[],
                            unsigned short num_instrs) {
    int i = 0, result = 1;

    /* 
     * Sets result equal to zero if machine or program are equal to zero. Will
     * also set result equal to zero if num_instrs is greater than 
     * NUM_BYTES. Otherwise the instructions are copied over to machine. 
     */
    if (machine == NULL || program == NULL || num_instrs >= NUM_BYTES)
        result = 0;
    else
    {
        for (i = 0; i < num_instrs; i++)
            machine->mem_location[i * BYTES_PER_WORD] = program[i];   
    }

    return result;                            
}

/* 
 * This function gets the value in the register specified by the parameter 
 * which_register in the Nelovo machine. 
 * 
 * @param machine, which_register, result
 * @return unsigned short 
 */
unsigned short get_register(Nelovo machine, unsigned short which_register,
                            Mach_word *const result) {
    int rtrn = 1;

    if (result == NULL || which_register >= 14)
        rtrn = 0;
    else
        *result = machine.reg[which_register];

    return rtrn;
}

/* 
 * This function gets the value in the memory_location in the Nelovo machine 
 * specified by the parameter which_memory_addr. 
 * 
 * @param machine, which_memory_addr, result
 * @return unsigned short 
 */

unsigned short get_memory(Nelovo machine, unsigned short which_memory_addr,
                          Mach_word *const result) {
    int rtrn = 1;

    if (result == NULL || which_memory_addr >= 4096 || 
                                            which_memory_addr % 4 != 0)
        rtrn = 0;
    else
        *result = machine.mem_location[which_memory_addr];

    return rtrn;
}

/*
 * This function models the Nelovo system. First checks to see if the 
 * instruciton is valid then it finds the different components of the 
 * current instruction. It then finds the matching opcode and carries out 
 * whatever function that opcode is supposed to do. 
 * 
 * @param machine, max_instrs, num_instrs_executed, invalid_instr, trace_flag
 * @return RUNNING
 */

Status run_program(Nelovo *const machine, unsigned int max_instrs,
                   unsigned short *const num_instrs_executed,
                   unsigned short *const invalid_instr,
                   unsigned short trace_flag) {
    unsigned int i = 0, j = 0, opcode = 0, reg1 = 0, reg2 = 0, reg3 = 0, 
            address = 0, input = 0;

    *num_instrs_executed = 0;

    for (j = 0; i < 14; i++) {
        machine->reg[j] = 0;
    }

    for (i = 0; i < max_instrs; i++) {
        if (!is_valid(machine->mem_location[machine->reg[13]])) {
            *invalid_instr = i;
            return ERROR;
        }

        /* Bitmasked to get the first register and then bitshifted 26 spaces */
        opcode = machine->mem_location[machine->reg[13]] >> 26;

        /* 
         * Bitmasked to get the first register and then bitshifted 
         * 22 spaces 
         */
        reg1 = (machine->mem_location[machine->reg[13]] & 62914560) >> 22;

        /* 
         * Bitmasked to get the second register and then bitshifted 18
         * spaces 
         */
        reg2 = (machine->mem_location[machine->reg[13]] & 3932160) >> 18;

        /* 
         * Bitmasked to get the third register and then bitshifted 14 
         * spaces 
         */
        reg3 = (machine->mem_location[machine->reg[13]] & 245760) >> 14;

        /* bitmasked to find the address */
        address = (machine->mem_location[machine->reg[13]] & 16383);

        /* 
         * This witch statement changes the function of each instruction based 
         * on its opcode. 
         */
        switch (opcode) {
            case HALT:
                return HALTED;
                break;
            case ADD:
                machine->reg[reg1] = machine->reg[reg2] + machine->reg[reg3];
                break;
            case SUB:
                machine->reg[reg1] = machine->reg[reg2] - machine->reg[reg3];
                break;
            case MUL:
                machine->reg[reg1] = machine->reg[reg3] * machine->reg[reg2];
                break;
            case DIV:
                if(machine->reg[reg2] == 0) {
                    *invalid_instr = i;
                    return 0;
                }
                machine->reg[reg1] = machine->reg[reg2] / machine->reg[reg3];
                break;
            case REM:
                if(machine->reg[reg2] == 0) {
                    *invalid_instr = i;
                    return 0;
                }
                machine->reg[reg1] = machine->reg[reg2] % machine->reg[reg3];
                break;
            case NEG:
                machine->reg[reg1] = -(machine->reg[reg2]);
                break;
            case AND:
                machine->reg[reg1] = machine->reg[reg3] && machine->reg[reg2];
                break;
            case OR:
                machine->reg[reg1] = machine->reg[reg3] || machine->reg[reg2];
                break;
            case NOT:
                machine->reg[reg1] = !(machine->reg[reg2]);
                break;
            case EQL:
                machine->reg[reg1] = machine->reg[reg3] == machine->reg[reg2];
                break;
            case NEQ:
                machine->reg[reg1] = machine->reg[reg3] != machine->reg[reg2];
                break;
            case LT:
                machine->reg[reg1] = machine->reg[reg2] < machine->reg[reg3];
                break;
            case GT:
                machine->reg[reg1] = machine->reg[reg2] > machine->reg[reg3];
                break;
            case LE:
                machine->reg[reg1] = machine->reg[reg2] <= machine->reg[reg3];
                break;
            case GE:
                machine->reg[reg1] = machine->reg[reg2] >= machine->reg[reg3];
                break;
            case TEST:
                if(machine->reg[reg1])
                    machine->reg[13] = address - BYTES_PER_WORD;
                break;
            case MOVE:
                machine->reg[reg1] = machine->reg[reg2];
                break;
            case LI:
                machine->reg[reg1] = address;
                break;
            case LW:
                machine->reg[reg1] = machine->mem_location[address];
                break;
            case SW:
                machine->mem_location[address] = machine->reg[reg1];
                break;
            case READ:
                scanf("%u", &input);
                machine->reg[reg1] = input;
                break;
            case PRT:
                printf("\n%d", machine->reg[reg1]);
                break;
        }

        (*num_instrs_executed)++;

        machine->reg[13] += BYTES_PER_WORD;
    }

    return RUNNING;
}