#include "llvmcode.hpp"
extern Symtab symtable;


std::string LlvmCodeHandler::freshVar(){
    return "%var_" + std::to_string(this->RegNum++);
}


void LlvmCodeHandler::handle_binop(Node* res_exp, Node* L_exp, Node* R_exp, std::string op){ 
    res_exp->var = freshVar();
    string llvm_op;
    if (op == "*") {
        llvm_op = "mul";
    }
    else if (op == "/") {
        if (res_exp->type == "INT") {
            llvm_op = "sdiv";
        }
        else { //res_exp->type = "byte"
            llvm_op = "udiv";
        }
        // Check devision by zero
        codeBuffer.emit("call void @check_division(i32 " + R_exp->var + ")");
    }
    else if (op == "+") {
        llvm_op = "add";
    }
    else if (op == "-") {
        llvm_op = "sub";
    }
    codeBuffer.emit(res_exp->var + " = " + llvm_op + " i32 " + L_exp->var + ", " + R_exp->var); //var3 add i32 var1, var2

    
    /* Check overflow */
    /* No need for Int_t type since already we use i32 in llvm */
    if (res_exp->type == "BYTE") {
        string new_new_var = freshVar();
        codeBuffer.emit(new_new_var + " = " + "and " + "i32 " + res_exp->var + ", " + "255");
        res_exp->var = new_new_var;
    }


}


void LlvmCodeHandler::handle_relop(BoolVarNode* res_exp, Node* L_exp, Node* R_exp, string op){ 
    string llvm_relop = "";
    bool sign;
    if (op == "=="){
        llvm_relop = "eq";
    }
    else if (op == "!=") {
        llvm_relop = "ne";
    }
    else if (op == "<"){
        llvm_relop = "lt";
    }
    else if (op == ">") {
        llvm_relop = "gt";
    }
    else if (op == "<=" ) {
        llvm_relop = "le";
    }
    else if (op == ">=") {
        llvm_relop = "ge";
    }

    if (L_exp->type == "INT" || R_exp->type == "INT")
        sign = true;
    else
        sign = false;
    
    if (llvm_relop != "eq" && llvm_relop != "ne") {
        if (sign)
            llvm_relop = "s" + llvm_relop;
        else
            llvm_relop = "u" + llvm_relop;
    }

    // std::string relop_start_label = codeBuffer.freshLabel();
    /* nextlist of exp2 is the start of these operations */
    res_exp->var = this->freshVar();
    this->codeBuffer.emit(res_exp->var + " = " + "icmp " + llvm_relop + " i32 " + L_exp->var + ", " + R_exp->var);

    //int next_instr = this->codeBuffer.emit("br i1 " + var + ", label @, label @");

    /* Entry is exp1 */
    //res_exp.start_label = L_exp.start_label;
    /* nextlist of exp1 is exp2 entry*/

}

string LlvmCodeHandler::allocate_var(string type, string id, string var, int offset, string assigningVar) {

    string llvmtype = getLlvmType(type);

    codeBuffer.emit(var + " = add i32 0, 0");

    string ptrvar = freshVar();
    codeBuffer.emit(ptrvar + " = getelementptre i32, i32* " + symtable.rbpvar + ", i32 " + to_string(offset));
    codeBuffer.emit("store i32 " + var + ", i32* " + ptrvar); //MAKE CASES FOR BYTE

    if(llvmtype == "INT" || "BYTE")
        codeBuffer.emit("store " + llvmtype + " 0, " + llvmtype + "* " + var);
    else if(llvmtype == "BOOL")
        codeBuffer.emit("store i1 false, i1* " + var);

    if(assigningVar != ""){
        codeBuffer.emit("store " + llvmtype + " " + assigningVar + ", " + llvmtype + "* " + var);
    }

    return var;
}

string LlvmCodeHandler::getLlvmType(string type){
    string llvmtype;
    if(type == "INT")
        llvmtype = "i32";
    else if(type == "BYTE")
        llvmtype = "i8";
    else if(type == "BOOL")
        llvmtype = "i1";
    else //string
        llvmtype = "i8*";

    return llvmtype;
}