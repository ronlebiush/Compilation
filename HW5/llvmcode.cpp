#include "llvmcode.hpp"

std::string LlvmCodeHandler::freshVar(){
    return "%var_" + std::to_string(this->RegNum++);
}


void LlvmCodeHandler::handle_binop(const Node& L_exp, const Node& R_exp, Node& new_exp, const std::string op){ 
    //new_exp.var = freshVar();
    std::string llvm_op;
    if (op == "*") {
        llvm_op = "mul";
    }
    else if (op == "/") {
        if (res.type == "int") {
            llvm_op = "sdiv";
        }
        else { //res->type = "byte"
            llvm_op = "udiv";
        }
        // Check devision by zero
        codeBuffer.emit("call void @check_division(i32 " + R_exp.type + ")");
    }
    else if (op == "+") {
        llvm_op = "add";
    }
    else if (op == "-") {
        llvm_op = "sub";
    }
    
    // Emit the instruction
    buffer.emit(freshVar() + " = " + llvm_op + " i32 " + L_exp.type + ", " + R_exp.type);

    

    std::string binop_start_label = this->cb->genLabel();
    /* nextlist of exp2 is the start of these operations */

    this->cb->emit(new_exp.var + " = " + binop_instr + "i32 " + exp1.var + ", " + exp2.var);
    /* Check overflow */
    /* No need for Int_t type since already we use i32 in llvm */
    if (new_exp.type == Byte_t) {
        E_var new_new_var = freshVar();
        codeBuffer->emit(new_new_var + " = " + "and " + "i32 " + new_exp.var + ", " + "255");
        new_exp.var = new_new_var;
    }


}
